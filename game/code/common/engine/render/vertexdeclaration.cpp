//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

VertexDeclaration::VertexDeclaration()
:	mNumStreams( 0 )
{
    mInitialized = false;
}

//============================================================================

VertexDeclaration::~VertexDeclaration()
{
}