#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglvertexdeclaration.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace FixedOGL
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

FixedOGLVertexDeclaration::FixedOGLVertexDeclaration()
{

}

//============================================================================

FixedOGLVertexDeclaration::~FixedOGLVertexDeclaration()
{
}

//============================================================================


void FixedOGLVertexDeclaration::CreateVertexDeclaration( EffectPass* effectPass )
{  
    if ( mInitialized == false )
	{
        
        for ( std::list<VertexElement>::iterator iter = mVertexElements.begin(); iter != mVertexElements.end(); ++iter )
        {
            VertexElement vertexElement = *iter;		
            if ( vertexElement.GetStream() > mNumStreams )
            {
                mNumStreams = vertexElement.GetStream();
            }
        }

        mNumStreams += 1;
		
        memset( mStreamSize, 0, sizeof( int ) * MAX_NUM_STREAMS );

        for ( std::list<VertexElement>::iterator iter = mVertexElements.begin(); iter != mVertexElements.end(); ++iter )
        {
            VertexElement vertexElement = *iter;				
            mStreamSize[vertexElement.GetStream()] += FixedOGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
        }
    }
}

//============================================================================

void FixedOGLVertexDeclaration::Dispatch( void* context )
{
	UNUSED_ALWAYS( context );
}

#endif