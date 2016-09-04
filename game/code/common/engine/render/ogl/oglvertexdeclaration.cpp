#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogl/oglrenderer.hpp"
#include "common/engine/render/ogl/oglvertexdeclaration.hpp"
#include "common/engine/system/memory.hpp"

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
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLVertexDeclaration::OGLVertexDeclaration()
{

}

//============================================================================

OGLVertexDeclaration::~OGLVertexDeclaration()
{
}

//============================================================================


void OGLVertexDeclaration::CreateVertexDeclaration( EffectPass* effectPass )
{  
    UNUSED_ALWAYS( effectPass );
    if ( mInitialized == false )
    {
        std::list<VertexElement>::iterator iter = mVertexElements.begin();
        for ( iter; iter != mVertexElements.end(); ++iter )
        {
            VertexElement vertexElement = *iter;
            if ( vertexElement.GetStream() > mNumStreams )
            {
                mNumStreams = vertexElement.GetStream();
            }
        }
        mNumStreams += 1;

        memset( mStreamSize, 0, sizeof( int ) * MAX_NUM_STREAMS );

        iter = mVertexElements.begin();
        for ( iter; iter != mVertexElements.end(); ++iter )
        {
            VertexElement vertexElement = *iter;	
            mStreamSize[vertexElement.GetStream()] += OGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
        }
        mInitialized = true;
    }
}

//============================================================================

void OGLVertexDeclaration::Dispatch( void* context )
{
    UNUSED_ALWAYS( context );
}

#endif