#if defined( RENDER_PLAT_OPENGL )

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
#include "common/engine/render/ogl/oglcontext.hpp"
#include "common/engine/render/ogl/oglrenderer.hpp"
#include "common/engine/render/ogl/oglrenderfactory.hpp"
#include "common/engine/render/ogl/oglshaderprogram.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/system/memory.hpp"
#include <gl/glew.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace OGL
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

OGLRenderer::OGLRenderer()
    :   mOGLContextMain( NULL )
    ,   mOGLContextThread( NULL )
    ,	mMaxColorAttachments( 0 )
{

}

//===========================================================================

OGLRenderer::~OGLRenderer()
{	
    std::list<OGLTargetData>::iterator i;
    for ( i = mColorTargetDataList.begin(); i != mColorTargetDataList.end(); ++i )
    {
        OGLTargetData data = *i;

        DELETE_PTR( data.mTexture );

        if ( data.mRenderBuffer > 0 )
        {
            OGL::oglDeleteRenderbuffersEXT( 1, &data.mRenderBuffer );
        }
    }

    for ( i = mDepthTargetDataList.begin(); i != mDepthTargetDataList.end(); ++i )
    {
        OGLTargetData data = *i;

        DELETE_PTR( data.mTexture );

        if ( data.mRenderBuffer > 0 )
        {
            OGL::oglDeleteRenderbuffersEXT( 1, &data.mRenderBuffer );
        }
    }

    OGL::oglDeleteFramebuffersEXT( 1, &mFrameBuffer );
    DELETE_PTR( mRenderFactory );
    DELETE_PTR( mOGLContextMain );
    DELETE_PTR( mOGLContextThread );
}

//===========================================================================

void OGLRenderer::Initialize( HWND hWnd )
{
    mOGLContextMain = NEW_PTR( "OGL Context Main" ) OGLContext;
    mOGLContextMain->Initialize( hWnd );    
    mOGLContextThread = NEW_PTR( "OGL Context Thread" ) OGLContext;
    mOGLContextThread->Initialize( hWnd );

    BOOL error = false;
	error = wglShareLists( mOGLContextMain->GetContext(), mOGLContextThread->GetContext() );	
    Assert( error == TRUE, "" );

    mOGLContextMain->SetCurrent();

    GLenum err = glewInit();
    if ( GLEW_OK != err )
    {
        System::PrintToConsole( "Error: %s\n", glewGetErrorString( err ) );		
    }
    System::PrintToConsole( "Status: Using GLEW %s\n", glewGetString( GLEW_VERSION ) );

    const GLubyte* oglVendor = OGL::oglGetString( GL_VENDOR );
    const GLubyte* oglVersion = OGL::oglGetString( GL_VERSION );

    System::PrintToConsole( "Vendor: %s\n", oglVendor );
    System::PrintToConsole( "Version: %s\n", oglVersion );

    int majorNumber = 0;
    int minorNumber = 0;
    int releaseNumber = 0;
    sscanf( reinterpret_cast<const char*>( oglVersion ), "%i.%i.%i", &majorNumber, &minorNumber, &releaseNumber );

    UNUSED_ALWAYS( minorNumber );
    UNUSED_ALWAYS( releaseNumber );
    Assert( majorNumber >= 2, "" );

    OGL::oglGetIntegerv( GL_MAX_TEXTURE_UNITS, &mNumTextureUnits );

    mRenderFactory = NEW_PTR( "Render Factory" ) OGLRenderFactory;

    //OGL::oglPointSize( 1.0f );
    //OGL::oglFrontFace( GL_CCW );
    //OGL::oglColorMask( true, true, true, true );
    //OGL::oglClearDepth( 1.0f );

    OGL::oglGetIntegerv(GL_MAX_DRAW_BUFFERS, &mMaxColorAttachments);

    OGL::oglGenFramebuffersEXT( 1, &mFrameBuffer );	
}

//===========================================================================

void OGLRenderer::SwapFrameBuffers()
{
    mOGLContextMain->SwapFrameBuffers();
}

//===========================================================================

void OGLRenderer::SetBackgroundColor( float red, float green, float blue, float alpha )
{	
    OGL::oglClearColor( red, green, blue, alpha );
}

//===========================================================================

void OGLRenderer::RenderBuffers( 
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

void OGLRenderer::Render(
    SimpleMesh* simpleMesh, 
    VertexDeclaration* vertexDeclaration,
    EffectPass* effectPass,
    int threadID,
    void* context
    )
{
    UNUSED_ALWAYS( threadID );

    ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( effectPass->GetShaderProgram() );

	OGLShaderProgram* oglShaderProgram = 
		reinterpret_cast<OGLShaderProgram*>( shaderProgram );

    OGL::oglBindBuffer( GL_ARRAY_BUFFER, 0 );
    OGL::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    int offset[MAX_NUM_STREAMS];
    memset( &offset, 0, sizeof( int ) * MAX_NUM_STREAMS );

    vertexDeclaration->Dispatch( context );

    std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
    std::list<VertexElement>::iterator iter = vertexElementList.begin();
    for ( iter; iter != vertexElementList.end(); ++iter )
    {
        VertexElement vertexElement = *iter;

        switch ( vertexElement.GetUsageType() )
        {
        case USAGE_POSITION:
            {
                int index = oglShaderProgram->GetInVertex();	
                if ( index >= 0 )
                {
                    OGL::oglEnableVertexAttribArray( index );
                    OGL::oglVertexAttribPointer( 
                        index, 
                        vertexElement.GetNumCoordinates(), 
                        OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
                        GL_FALSE, 
                        vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
                        ( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
                    );
                }
            }
            break;
        case USAGE_TEXCOORD:
            {
                int index = oglShaderProgram->GetInTexCoord( vertexElement.GetUsageIndex() );	
                if ( index >= 0 )
                {
                    OGL::oglEnableVertexAttribArray( index );
                    OGL::oglVertexAttribPointer( 
                        index, 
                        vertexElement.GetNumCoordinates(), 
                        OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
                        GL_FALSE, 
                        vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
                        ( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
                    );
                }
            }
            break;
        case USAGE_NORMAL:
            {
                int index = oglShaderProgram->GetInNormal();	
                if ( index >= 0 )
                {
                    OGL::oglEnableVertexAttribArray( index );
                    OGL::oglVertexAttribPointer( 
                        index, 
                        3, 
                        OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
                        GL_FALSE, 
                        vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
                        ( char* )( simpleMesh->mVertexData ) + offset[vertexElement.GetStream()]
                    );
                }
            }
            break;
        case USAGE_COLOR:
            {
                int index = oglShaderProgram->GetInColor();	
                if ( index >= 0 )
                {
                    OGL::oglEnableVertexAttribArray( index );
                    OGL::oglVertexAttribPointer( 
                        index, 
                        vertexElement.GetNumCoordinates(), 
                        OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
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

        offset[vertexElement.GetStream()] += OGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
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
    }

    OGL::oglDrawArrays(
        OGL::PrimitiveTypeOGL[simpleMesh->mPrimitiveType],
        0,
        countIndices
        );

    iter = vertexElementList.begin();
    for ( iter; iter != vertexElementList.end(); ++iter )
    {
        VertexElement vertexElement = *iter;
        int index = -1;

        switch ( vertexElement.GetUsageType() )
        {
        case USAGE_POSITION:
            index = oglShaderProgram->GetInVertex();
            break;
        case USAGE_TEXCOORD:
            index = oglShaderProgram->GetInTexCoord( vertexElement.GetUsageIndex() );	
            break;
        case USAGE_NORMAL:
            index = oglShaderProgram->GetInNormal();
            break;
        case USAGE_COLOR:
            index = oglShaderProgram->GetInColor();
            break;
        default:
            Assert( false, "" );
            break;
        }

        if ( index >= 0 )
        {
            OGL::oglDisableVertexAttribArray( index );
        }
    }
}

//===========================================================================

void OGLRenderer::SetRenderTarget( RenderTarget* renderTarget, void* context )
{
    if ( renderTarget == NULL )
    {
        OGL::oglBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
        SetViewport( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), context );
        SetScissorRectangle( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), renderTarget, context );
    }
    else
    {	
        OGL::oglBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFrameBuffer );
        SetViewport( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), context );
        SetScissorRectangle( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), renderTarget, context );

        Texture* colorTexture = renderTarget->GetColorTexture();	
        if ( colorTexture )
        {
            OGLTexture* oglTexture = reinterpret_cast<OGLTexture*>( colorTexture );
            OGL::oglFramebufferTexture2DEXT( 
                GL_FRAMEBUFFER_EXT, 
                GL_COLOR_ATTACHMENT0_EXT, 
                GL_TEXTURE_2D, 
                oglTexture->GetHandle(),
                0 
                );
        }
        else
        {
            OGLRenderTarget* oglRenderTarget = reinterpret_cast<OGLRenderTarget*>( renderTarget );
            unsigned int renderBuffer = oglRenderTarget->GetColorRenderBuffer();
            OGL::oglFramebufferRenderbufferEXT( 
                GL_FRAMEBUFFER_EXT, 
                GL_COLOR_ATTACHMENT0_EXT, 
                GL_RENDERBUFFER_EXT, 
                renderBuffer
                );
        }

        int numTextures = 1;//mTextures.GetSize();
        GLenum buffers[] = { 
            GL_COLOR_ATTACHMENT0_EXT, 
            GL_COLOR_ATTACHMENT1_EXT,
            GL_COLOR_ATTACHMENT2_EXT,
            GL_COLOR_ATTACHMENT3_EXT
        };
        OGL::oglDrawBuffers( numTextures, buffers );

        Texture* depthTexture = renderTarget->GetDepthTexture();
        if ( depthTexture )
        {
            OGLTexture* oglTexture = reinterpret_cast<OGLTexture*>( depthTexture );
            OGL::oglFramebufferTexture2DEXT( 
                GL_FRAMEBUFFER_EXT, 
                GL_DEPTH_ATTACHMENT_EXT, 
                GL_TEXTURE_2D, 
                oglTexture->GetHandle(), 
                0 
                );
        }
        else
        {
            OGLRenderTarget* oglRenderTarget = reinterpret_cast<OGLRenderTarget*>( renderTarget );
            unsigned int renderBuffer = oglRenderTarget->GetDepthRenderBuffer();
            OGL::oglFramebufferRenderbufferEXT( 
                GL_FRAMEBUFFER_EXT, 
                GL_DEPTH_ATTACHMENT_EXT, 
                GL_RENDERBUFFER_EXT, 
                renderBuffer
                );
        }
    }
}

//===========================================================================

void OGLRenderer::ClearRenderer( void* context ) const
{    
    UNUSED_ALWAYS( context );

    OGL::oglDepthMask( true );	
    OGL::oglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );    
}

//===========================================================================

void OGLRenderer::ClearColor( void* context ) const
{
    UNUSED_ALWAYS( context );

    OGL::oglClear( GL_COLOR_BUFFER_BIT );    
}

//===========================================================================

void OGLRenderer::ClearDepth( void* context ) const
{    
    UNUSED_ALWAYS( context );

    OGL::oglClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );    
}

//===========================================================================

void OGLRenderer::SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context )
{
    UNUSED_ALWAYS( context );
    UNUSED_ALWAYS( renderTarget );

    OGL::oglScissor( x, y, width, height );
}

//===========================================================================

void OGLRenderer::SetViewport( int x, int y, int width, int height, void* context )
{
    UNUSED_ALWAYS( context );

    OGL::oglViewport( x, y, width, height );
}

//===========================================================================

void OGLRenderer::AddColorTarget( OGLTargetData data )
{
    mColorTargetDataList.push_back( data );
}

//===========================================================================

void OGLRenderer::AddDepthTarget( OGLTargetData data )
{
    mDepthTargetDataList.push_back( data );
}

//===========================================================================

Math::Matrix44 OGLRenderer::GetPerspectiveProjection(
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

Math::Matrix44 OGLRenderer::GetOrthoProjection( 
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

void OGLRenderer::SetThreadContext()
{
    mOGLContextThread->SetCurrent();
}

//===========================================================================

void OGLRenderer::FlushContext()
{
    OGL::oglFlush();
}

#endif