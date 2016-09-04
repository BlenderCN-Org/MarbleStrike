#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/ogles2/oglesrenderer.hpp"
#include "common/engine/render/ogles2/oglesrenderfactory.hpp"
#include "common/engine/render/ogles2/oglesshaderprogram.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/system/memory.hpp"

#if defined PLAT_IOS
#include "ios/EAGLContext.h"
#endif

#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
#include "common/engine/render/ogles2/oglescontext.hpp"
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

unsigned int OGLESRenderer::mTextureID = 0;

namespace OGLES
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

	static int PrimitiveTypeOGL[PRIMITIVE_TYPE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) OGLTYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};	
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESRenderer::OGLESRenderer()
#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
	:   mOGLESContext( NULL )
#endif
{
	mCurrentSamplerUnit = -1;
	mCurrentBindBufferArrayIndex = -1;
	mCurrentBindBufferElementIndex = -1;
}

//===========================================================================

OGLESRenderer::~OGLESRenderer()
{
	std::list<OGLESTargetData>::iterator i;
	for ( i = mColorTargetDataList.begin(); i != mColorTargetDataList.end(); ++i )
	{
		OGLESTargetData data = *i;

		DELETE_PTR( data.mTexture );

		if ( data.mRenderBuffer > 0 )
		{
			OGLES::oglDeleteRenderbuffers( 1, &data.mRenderBuffer );
		}
	}

	for ( i = mDepthTargetDataList.begin(); i != mDepthTargetDataList.end(); ++i )
	{
		OGLESTargetData data = *i;

		DELETE_PTR( data.mTexture );

		if ( data.mRenderBuffer > 0 )
		{
			OGLES::oglDeleteRenderbuffers( 1, &data.mRenderBuffer );
		}
	}

	DELETE_PTR( mRenderFactory );

#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
	DELETE_PTR( mOGLESContext );
#endif
}

//===========================================================================

#if defined( PLAT_PC )
void OGLESRenderer::Initialize( HWND hWnd )
#endif

#if defined( PLAT_IOS )
	void OGLESRenderer::Initialize()
#endif

#if defined( PLAT_ANDROID )
	void OGLESRenderer::Initialize( ANativeWindow* window )
#endif
{

#if defined( PLAT_PC )
	mOGLESContext = NEW_PTR( "OGL ES Context" ) OGLESContext;
	mOGLESContext->Initialize( hWnd );
#endif

#if defined( PLAT_ANDROID )
	mOGLESContext = NEW_PTR( "OGL ES Context" ) OGLESContext;
	mOGLESContext->Initialize( window );
#endif

	mRenderFactory = NEW_PTR( "Render Factory" ) OGLESRenderFactory;

	GLint maxTextureSize;
	OGLES::oglGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTextureSize );

	OGLES::oglGenFramebuffers( 1, &mFramebuffer );
	mDefaultFramebuffer = 0;
}

//===========================================================================

#if defined( PLAT_ANDROID )
void OGLESRenderer::DeInitialize()
{
	if ( mOGLESContext )
	{
		mOGLESContext->DeInitialize();
	}
}
#endif

//===========================================================================

#if defined( PLAT_ANDROID )
void OGLESRenderer::ReInitialize( ANativeWindow* window )
{
	if ( mOGLESContext )
	{
		mOGLESContext->ReInitialize( window );
	}
}
#endif

//===========================================================================

void OGLESRenderer::SwapFrameBuffers()
{
#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
	mOGLESContext->SwapFrameBuffers();
#endif
}

//===========================================================================

void OGLESRenderer::SetBackgroundColor( float red, float green, float blue, float alpha )
{
	OGLES::oglClearDepthf( 1.0f );
	OGLES::oglClearColor( red, green, blue, alpha );
}

//===========================================================================

void OGLESRenderer::RenderBuffers( 
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

void OGLESRenderer::Render( 
	SimpleMesh* simpleMesh, 
	VertexDeclaration* vertexDeclaration,
	EffectPass* effectPass,
	int /*threadID*/,
	void* context
	)
{
	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( effectPass->GetShaderProgram() );
	OGLESShaderProgram* oglesShaderProgram = reinterpret_cast<OGLESShaderProgram*>( shaderProgram );

	OGLES::oglBindBuffer( GL_ARRAY_BUFFER, 0 );
	OGLES::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	int offset[MAX_NUM_STREAMS];
	memset( &offset, 0, sizeof( int ) * vertexDeclaration->GetNumStreams() );

	vertexDeclaration->Dispatch( context );

	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		switch ( vertexElement.GetUsageType() )
		{
		case USAGE_POSITION:
			{
				int index = oglesShaderProgram->GetInVertex();	
				if ( index >= 0 )
				{
					OGLES::oglEnableVertexAttribArray( index );
					OGLES::oglVertexAttribPointer( 
						index, 
						vertexElement.GetNumCoordinates(), 
						OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
						GL_FALSE, 
						vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
						( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
					);
				}
			}
			break;
		case USAGE_TEXCOORD:
			{
				int index = oglesShaderProgram->GetInTexCoord();	
				if ( index >= 0 )
				{
					OGLES::oglEnableVertexAttribArray( index );
					OGLES::oglVertexAttribPointer( 
						index, 
						vertexElement.GetNumCoordinates(), 
						OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
						GL_FALSE, 
						vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
						( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
					);
				}
			}
			break;
		case USAGE_COLOR:
			{
				int index = oglesShaderProgram->GetInColor();	
				if ( index >= 0 )
				{
					OGLES::oglEnableVertexAttribArray( index );
					OGLES::oglVertexAttribPointer( 
						index, 
						vertexElement.GetNumCoordinates(), 
						OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
						GL_FALSE, 
						vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
						( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
					);
				}
			}
			break;
		case USAGE_NORMAL:
			{
				int index = oglesShaderProgram->GetInNormal();	
				if ( index >= 0 )
				{
					OGLES::oglEnableVertexAttribArray( index );
					OGLES::oglVertexAttribPointer( 
						index, 
						3, 
						OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
						GL_FALSE, 
						vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
						( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
					);
				}
			}
			break;
		default:
			break;
		}

		offset[vertexElement.GetStream()] += OGLES::VertexAttribFormatSize[vertexElement.GetFormatType()];
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

	OGLES::oglDrawArrays(
		OGLES::PrimitiveTypeOGL[simpleMesh->mPrimitiveType],
		0,
		countIndices
		);

	int index = oglesShaderProgram->GetInVertex();
	if ( index >= 0 )
	{
		OGLES::oglDisableVertexAttribArray( index );
	}

	index = oglesShaderProgram->GetInTexCoord();
	if ( index >= 0 )
	{
		OGLES::oglDisableVertexAttribArray( index );
	}

	index = oglesShaderProgram->GetInColor();
	if ( index >= 0 )
	{
		OGLES::oglDisableVertexAttribArray( index );
	}

	index = oglesShaderProgram->GetInNormal();
	if ( index >= 0 )
	{
		OGLES::oglDisableVertexAttribArray( index );
	}	
}

//===========================================================================

void OGLESRenderer::SetRenderTarget( RenderTarget* renderTarget, void* context )
{
	if ( renderTarget == NULL )
	{
		OGLES::oglBindFramebuffer( GL_FRAMEBUFFER, mDefaultFramebuffer );
		SetViewport( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), context );
		SetScissorRectangle( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), renderTarget, context );
	}
	else
	{
		OGLES::oglBindFramebuffer( GL_FRAMEBUFFER, mFramebuffer );
		SetViewport( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), context );
		SetScissorRectangle( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), renderTarget, context );

		Texture* colorTexture = renderTarget->GetColorTexture();	
		if ( colorTexture )
		{
			OGLESTexture* oglESTexture = reinterpret_cast<OGLESTexture*>( colorTexture );
			OGLES::oglFramebufferTexture2D( 
				GL_FRAMEBUFFER, 
				GL_COLOR_ATTACHMENT0, 
				GL_TEXTURE_2D, 
				oglESTexture->GetHandle(), 
				0 
				);		
		}
		else
		{
			OGLESRenderTarget* oglRenderTarget = reinterpret_cast<OGLESRenderTarget*>( renderTarget );
			unsigned int renderBuffer = oglRenderTarget->GetColorRenderBuffer();
			OGLES::oglFramebufferRenderbuffer( 
				GL_FRAMEBUFFER, 
				GL_COLOR_ATTACHMENT0, 
				GL_RENDERBUFFER, 
				renderBuffer
				);
		}

		Texture* depthTexture = renderTarget->GetDepthTexture();	
		if ( depthTexture )
		{
			OGLESTexture* oglesDepthTexture = reinterpret_cast<OGLESTexture*>( depthTexture );
			OGLES::oglFramebufferTexture2D( 
				GL_FRAMEBUFFER, 
				GL_DEPTH_ATTACHMENT, 
				GL_TEXTURE_2D, 
				oglesDepthTexture->GetHandle(), 
				0 
				);
		}
		else
		{
			OGLESRenderTarget* oglRenderTarget = reinterpret_cast<OGLESRenderTarget*>( renderTarget );
			unsigned int renderBuffer = oglRenderTarget->GetDepthRenderBuffer();
			OGLES::oglFramebufferRenderbuffer( 
				GL_FRAMEBUFFER, 
				GL_DEPTH_ATTACHMENT, 
				GL_RENDERBUFFER, 
				renderBuffer
				);
		}
	}
}

//===========================================================================

void OGLESRenderer::ClearRenderer( void* /*context*/ ) const
{    
	OGLES::oglDepthMask( true );
	OGLES::oglColorMask( true, true, true, true );
	OGLES::oglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );    
}

//===========================================================================

void OGLESRenderer::ClearColor( void* /*context*/ ) const
{  
	OGLES::oglClear( GL_COLOR_BUFFER_BIT );
}

//===========================================================================

void OGLESRenderer::ClearDepth( void* /*context*/ ) const
{  
	OGLES::oglClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

//===========================================================================

void OGLESRenderer::SetScissorRectangle( int x, int y, int width, int height, RenderTarget* /*renderTarget*/, void* /*context*/ )
{
	OGLES::oglScissor( x, y, width, height );
}

//===========================================================================

void OGLESRenderer::SetViewport( int x, int y, int width, int height, void* /*context*/ )
{	
	int scale = 1;

	if ( Database::Get()->GetIsLandscape() )
	{		
		int newX = y * scale;
		int newY = ( Database::Get()->GetBackBufferWidth() - x - width ) * scale;
		OGLES::oglViewport( newX, newY, height * scale, width * scale );
	}	
	else
	{
		OGLES::oglViewport( x, y, width * scale, height * scale );	
	}
}

//===========================================================================

void OGLESRenderer::AddColorTarget( OGLESTargetData data )
{
	mColorTargetDataList.push_back( data );
}

//===========================================================================

void OGLESRenderer::AddDepthTarget( OGLESTargetData data )
{
	mDepthTargetDataList.push_back( data );
}

//===========================================================================

Math::Matrix44 OGLESRenderer::GetPerspectiveProjection(
	float radians, 
	float aspect, 
	float zNear, 
	float zFar
	)
{
	Math::Matrix44 data;
	float f = 1.0f / tan( radians/2.0f );
	data[0] = f;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[4] = 0;
	data[5] = f / aspect;
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

Math::Matrix44 OGLESRenderer::GetOrthoProjection( 
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

//===========================================================================

void OGLESRenderer::SetMainContext()
{
#if defined( PLAT_PC ) || defined( PLAT_ANDRIOD )
    mOGLESContext->SetMainContext();
#endif
}

//===========================================================================

void OGLESRenderer::SetThreadContext()
{
#if defined PLAT_IOS
    IPhoneOGLContext::SetThreadContext();
#endif
#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
    mOGLESContext->SetThreadContext();
#endif
}

//===========================================================================

void OGLESRenderer::FlushContext()
{   
    OGLES::oglFlush();
#if defined( PLAT_PC )
    mOGLESContext->SetEmptyContext();
#endif
}

#endif