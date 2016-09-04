#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogles2/oglesrenderer.hpp"
#include "common/engine/render/ogles2/oglesvertexdeclaration.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace OGLES
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

OGLESVertexDeclaration::OGLESVertexDeclaration()
{

}

//============================================================================

OGLESVertexDeclaration::~OGLESVertexDeclaration()
{
}

//============================================================================

void OGLESVertexDeclaration::CreateVertexDeclaration( EffectPass* effectPass )
{  	
    UNUSED_ALWAYS( effectPass );

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
		    mStreamSize[vertexElement.GetStream()] += OGLES::VertexAttribFormatSize[vertexElement.GetFormatType()];
	    }

        mInitialized = true;
    }
}

//============================================================================

void OGLESVertexDeclaration::Dispatch( void* context )
{
	UNUSED_ALWAYS( context );
}

#endif