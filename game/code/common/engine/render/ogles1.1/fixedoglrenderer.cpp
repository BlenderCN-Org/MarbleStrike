#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/effect.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderfactory.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/render/vertexbuffer.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/memory.hpp"

#if defined PLAT_PC
#include "iphone/render/ogles1.1/fixedoglcontext.hpp"
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

unsigned int FixedOGLRenderer::mTextureID = 0;
const SamplerState* FixedOGLRenderer::mCurrentSamplerState[2];

namespace FixedOGL
{	
	static int VertexAttribFormatSize[VERTEX_ATTRIB_FORMAT_MAX] =
	{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) SIZE,
		VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
	};

	static int VertexAttribFormatType[VERTEX_ATTRIB_FORMAT_MAX] =
	{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) OGL_TYPE,
		VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
	};

	static int VertexAttribUsageType[VERTEX_ATTRIB_USAGE_MAX] =
	{
#define VERTEX_ATTRIB_USAGE_ENTRY( ENUM, USAGENAME, D3DTYPE, PS3TYPE, OGLTYPE ) OGLTYPE,
		VERTEX_ATTRIB_USAGE_TUPLE
#undef VERTEX_ATTRIB_USAGE_ENTRY
	};

	static int PrimitiveTypeOGL[PRIMITIVE_TYPE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D1TYPE ) OGLTYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};	
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLRenderer::FixedOGLRenderer()
{
#if defined PLAT_PC
	mOGLContext = NULL;
#endif
}

//===========================================================================

FixedOGLRenderer::~FixedOGLRenderer()
{
	std::list<FixedOGLTargetData>::iterator i;
	for ( i = mColorTargetDataList.begin(); i != mColorTargetDataList.end(); ++i )
	{
		FixedOGLTargetData data = *i;

		DELETE_PTR( data.mTexture );

		if ( data.mRenderBuffer > 0 )
		{
			FixedOGL::oglDeleteRenderbuffers( 1, &data.mRenderBuffer );
		}
	}

	for ( i = mDepthTargetDataList.begin(); i != mDepthTargetDataList.end(); ++i )
	{
		FixedOGLTargetData data = *i;

		DELETE_PTR( data.mTexture );

		if ( data.mRenderBuffer > 0 )
		{
			FixedOGL::oglDeleteRenderbuffers( 1, &data.mRenderBuffer );
		}
	}

	FixedOGL::oglDeleteFramebuffers( 1, &mFramebuffer );
	DELETE_PTR( mRenderFactory );

	mDefaultFramebuffer = 0;

#if defined PLAT_PC
	DELETE_PTR( mOGLContext );
#endif
}

//===========================================================================

#if defined PLAT_PC
void FixedOGLRenderer::Initialize( HWND hWnd )
#endif

#if defined PLAT_IOS
	void FixedOGLRenderer::Initialize()
#endif

{
#if defined PLAT_PC
	mOGLContext = NEW_PTR( "OGL Context" ) FixedOGLContext;
	mOGLContext->Initialize( hWnd );
#endif

	const GLubyte* oglVendor = FixedOGL::oglGetString( GL_VENDOR );
	const GLubyte* oglVersion = FixedOGL::oglGetString( GL_VERSION );

	System::PrintToConsole( "Vendor: %s\n", oglVendor );
	System::PrintToConsole( "Version: %s\n", oglVersion );

	FixedOGL::oglEnable( GL_NORMALIZE );

	FixedOGL::oglGetIntegerv( GL_MAX_TEXTURE_UNITS, &mNumTextureUnits );

	mRenderFactory = NEW_PTR( "Render Factory" ) FixedOGLRenderFactory;

	FixedOGL::oglPointSize( 5.0f );

	FixedOGL::InitializeExt();

	//
	// Create frame buffer object
	//
	FixedOGL::oglGenFramebuffers( 1, &mFramebuffer );
	mDefaultFramebuffer = 0;
}

//===========================================================================

void FixedOGLRenderer::SwapFrameBuffers()
{
#if defined PLAT_PC
	mOGLContext->SwapFrameBuffers();
#endif
}

//===========================================================================

void FixedOGLRenderer::SetBackgroundColor( float red, float green, float blue, float alpha )
{
	FixedOGL::oglClearDepth( 1.0f );
	FixedOGL::oglClearColor( red, green, blue, alpha );
}

//===========================================================================

void FixedOGLRenderer::SetRenderTarget( RenderTarget* renderTarget, void* context )
{
	if ( renderTarget == NULL )
	{
		FixedOGL::oglBindFramebuffer( GL_FRAMEBUFFER_OES, mDefaultFramebuffer );
		SetViewport( 0, 0, Database::Get()->GetAppScreenWidth(), Database::Get()->GetAppScreenHeight(), context );
	}
	else
	{


		FixedOGL::oglBindFramebuffer( GL_FRAMEBUFFER_OES, mFramebuffer );

		Texture* texture = renderTarget->GetColorTexture();	
		if ( texture )
		{
			FixedOGLTexture* fixedOGLTexture = reinterpret_cast<FixedOGLTexture*>( texture );
			FixedOGL::oglFramebufferTexture2D( 
				GL_FRAMEBUFFER_OES, 
				GL_COLOR_ATTACHMENT0_OES, 
				GL_TEXTURE_2D, 
				fixedOGLTexture->GetHandle(), 
				0 
				);
		}
		else
		{
			FixedOGLRenderTarget* oglRenderTarget = reinterpret_cast<FixedOGLRenderTarget*>( renderTarget );
			unsigned int renderBuffer = oglRenderTarget->GetColorRenderBuffer();
			FixedOGL::oglFramebufferRenderbuffer( 
				GL_FRAMEBUFFER_OES, 
				GL_COLOR_ATTACHMENT0_OES, 
				GL_RENDERBUFFER_OES, 
				renderBuffer
				);
		}

		FixedOGLRenderTarget* oglRenderTarget = reinterpret_cast<FixedOGLRenderTarget*>( renderTarget );
		unsigned int renderBuffer = oglRenderTarget->GetDepthRenderBuffer();
		FixedOGL::oglFramebufferRenderbuffer( 
			GL_FRAMEBUFFER_OES, 
			GL_DEPTH_ATTACHMENT_OES, 
			GL_RENDERBUFFER_OES, 
			renderBuffer
			);

		FixedOGL::oglViewport( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight() );
	}
}

//===========================================================================

void FixedOGLRenderer::ClearRenderer( void* /*context*/ ) const
{    
	FixedOGL::oglDepthMask( true );
	FixedOGL::oglColorMask( true, true, true, true );
	FixedOGL::oglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );    

	FixedOGL::oglMatrixMode( GL_TEXTURE );
	FixedOGL::oglLoadIdentity();

	FixedOGL::oglMatrixMode( GL_PROJECTION );
	FixedOGL::oglLoadIdentity();

	FixedOGL::oglMatrixMode( GL_MODELVIEW );
	FixedOGL::oglLoadIdentity();
}

//===========================================================================

void FixedOGLRenderer::ClearColor( void* context ) const
{    
	UNUSED_ALWAYS( context );

	FixedOGL::oglClear( GL_COLOR_BUFFER_BIT );    
}

//===========================================================================

void FixedOGLRenderer::ClearDepth( void* context ) const
{    
	UNUSED_ALWAYS( context );

	FixedOGL::oglClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );    
}

//===========================================================================

void FixedOGLRenderer::RenderBuffers( 
	Mesh* mesh, 
	VertexDeclaration* vertexDeclaration,
	EffectPass* effectPass,
	void* context
	)
{
	VertexBuffer* vertexBufferFound = NULL;
	for ( int i = 0; i < mesh->GetNumVertexBuffers(); ++i )
	{
		VertexBuffer* vertexBuffer = mesh->GetVertexBuffer( i );

		if ( vertexDeclaration == vertexBuffer->GetVertexDeclaration() )
		{
			vertexBufferFound = vertexBuffer;
			break;
		}
	}

	if ( vertexBufferFound == NULL )
	{
		vertexBufferFound = mesh->CreateVertexBuffer();
		vertexBufferFound->Create( mesh, vertexDeclaration );
	}

	vertexDeclaration->Dispatch( context );
	vertexBufferFound->Render( vertexDeclaration, effectPass, context );
}

//===========================================================================

void FixedOGLRenderer::Render( 
	SimpleMesh* simpleMesh, 
	VertexDeclaration* vertexDeclaration,
	EffectPass* effectPass,
	int threadID,
	void* context
	)
{
	UNUSED_ALWAYS( effectPass );
	UNUSED_ALWAYS( threadID );

	FixedOGL::oglBindBuffer( GL_ARRAY_BUFFER, 0 );
	FixedOGL::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	int offset[MAX_NUM_STREAMS];
	memset( &offset, 0, sizeof( int ) * MAX_NUM_STREAMS );

	vertexDeclaration->Dispatch( context );

	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		if ( vertexElement.GetUsageType() == USAGE_TEXCOORD )
		{
			FixedOGL::oglClientActiveTexture( GL_TEXTURE0 + vertexElement.GetUsageIndex() );
		}

		FixedOGL::oglEnableClientState( FixedOGL::VertexAttribUsageType[vertexElement.GetUsageType()] );
		switch ( vertexElement.GetUsageType() )
		{
		case USAGE_POSITION:
			FixedOGL::oglVertexPointer( 
				vertexElement.GetNumCoordinates(), 
				FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
				vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
				( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()] );
			break;
		case USAGE_TEXCOORD:
			FixedOGL::oglTexCoordPointer( 
				vertexElement.GetNumCoordinates(), 
				FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()],  
				vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
				( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()] );
			break;
		case USAGE_NORMAL:
			FixedOGL::oglNormalPointer( 
				FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
				vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
				( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()] );
			break;
		case USAGE_COLOR:
			FixedOGL::oglColorPointer(
				vertexElement.GetNumCoordinates(), 
				FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
				vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
				( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()] );
			break;
		default:
			break;
		}

		offset[vertexElement.GetStream()] += FixedOGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}

	int countIndices = 0;
	switch ( simpleMesh->mPrimitiveType )
	{
	case PRIMITIVE_TRIANGLES:
		countIndices = simpleMesh->mNumPrimitives * simpleMesh->mVertexPerPrimitive;
		break;
	case PRIMITIVE_TRIANGLE_FAN:
		countIndices = simpleMesh->mNumPrimitives + 2;
		break;
	case PRIMITIVE_LINES:
		countIndices = simpleMesh->mNumPrimitives * simpleMesh->mVertexPerPrimitive;
		break;
	case PRIMITIVE_LINE_STRIP:
		countIndices = simpleMesh->mNumPrimitives + 1;
		break;
    default:
        break;
	}

	FixedOGL::oglDrawArrays(
		FixedOGL::PrimitiveTypeOGL[simpleMesh->mPrimitiveType],
		0,
		countIndices
		);

	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		FixedOGL::oglBindBuffer( GL_ARRAY_BUFFER, 0 );
		if ( 
			vertexElement.GetUsageType() == USAGE_POSITION || 
			vertexElement.GetUsageType() == USAGE_TEXCOORD || 
			vertexElement.GetUsageType() == USAGE_NORMAL ||
			vertexElement.GetUsageType() == USAGE_COLOR
			)
		{			
			if ( vertexElement.GetUsageType() == USAGE_TEXCOORD )
			{
				FixedOGL::oglClientActiveTexture( GL_TEXTURE0 + vertexElement.GetUsageIndex() );
			}
			FixedOGL::oglDisableClientState( FixedOGL::VertexAttribUsageType[vertexElement.GetUsageType()] );		
		}
	}	
}

//===========================================================================

void FixedOGLRenderer::SetScissorRectangle( int x, int y, int width, int height, void* context )
{	
	UNUSED_ALWAYS( context );

	int scale = 1;
	if ( Database::Get()->GetIsRetinaDisplay() )
	{
		scale = 2;
	}

	if ( Database::Get()->GetIsLandscape() )
	{			
		int newX = y * scale;
		int newY = ( 480 - x - width ) * scale;
		FixedOGL::oglScissor( newX, newY, height * scale, width * scale );
	}
	else
	{
		FixedOGL::oglScissor( x, y, width, height );
	}
}

//===========================================================================

void FixedOGLRenderer::SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context )
{
	UNUSED_ALWAYS( context );
	UNUSED_ALWAYS( renderTarget );

	FixedOGL::oglScissor( x, y, width, height );
}

//===========================================================================

void FixedOGLRenderer::SetViewport( int x, int y, int width, int height, void* context )
{	
	UNUSED_ALWAYS( context );

	int scale = 1;
	if ( Database::Get()->GetIsRetinaDisplay() )
	{
		scale = 2;
	}

	if ( Database::Get()->GetIsLandscape() )
	{		
		int newX = y * scale;
		int newY = ( 480 - x - width ) * scale;
		FixedOGL::oglViewport( newX, newY, height * scale, width * scale );
		//FixedOGL::oglViewport( x, y, height * scale, width * scale );	
	}	
	else
	{
		FixedOGL::oglViewport( x, y, width * scale, height * scale );	
	}	
}

//===========================================================================

void FixedOGLRenderer::AddColorTarget( FixedOGLTargetData data )
{
	mColorTargetDataList.push_back( data );
}

//===========================================================================

void FixedOGLRenderer::AddDepthTarget( FixedOGLTargetData data )
{
	mDepthTargetDataList.push_back( data );
}

//===========================================================================

Math::Matrix44 FixedOGLRenderer::GetPerspectiveProjection(
	float radians, 
	float aspect, 
	float zNear, 
	float zFar
	)
{
	Math::Matrix44 data;
	float f = 1.0f / tan( radians/2.0f );
	data[0] = f / aspect;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[4] = 0;
	data[5] = f;
	data[6] = 0;
	data[7] = 0;

	data[8] = 0;
	data[9] = 0;
	data[10] = ( zFar + zNear ) / ( zNear - zFar );
	data[11] = -1;

	data[12] = 0;
	data[13] = 0;
	data[14] = 2.0f*zFar*zNear / ( zNear - zFar );
	data[15] = 0;

	return data;
}


//===========================================================================

Math::Matrix44 FixedOGLRenderer::GetOrthoProjection( 
	float left, 
	float bottom, 
	float right, 
	float top, 
	float znear, 
	float zfar 
	)
{
	Math::Matrix44 data;

	float tx = -( right + left ) / ( right - left );
	float ty = -( top + bottom ) / ( top - bottom );
	float tz = -( zfar + znear ) / ( zfar - znear );

	data[0] = 2.0f / ( right - left );
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[4] = 0;
	data[5] = 2.0f / ( top - bottom );
	data[6] = 0;
	data[7] = 0;

	data[8] = 0;
	data[9] = 0;
	data[10] = -2.0f / ( zfar - znear );
	data[11] = 0;

	data[12] = tx;
	data[13] = ty;
	data[14] = tz;
	data[15] = 1;

	return data;
}

#endif