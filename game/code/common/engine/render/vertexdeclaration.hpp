#ifndef VERTEXDECLARATION_HPP
#define VERTEXDECLARATION_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexelement.hpp"
#include <list>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class EffectPass;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class VertexDeclaration
{
public:

    VertexDeclaration();
	virtual ~VertexDeclaration();

	virtual void Dispatch( void* context ) = 0;
	virtual void CreateVertexDeclaration( EffectPass* effectPass ) = 0;

	void AddVertexElement( VertexElement vertexElement )
	{
		mVertexElements.push_back( vertexElement );
	}	

	std::list<VertexElement>& GetVertexElements()
	{
		return mVertexElements;
	}

	int GetNumStreams()
	{
		return mNumStreams;
	}

	int GetStreamSize( int i )
	{
		return mStreamSize[i];
	}

protected:

	int mStreamSize[MAX_NUM_STREAMS];
	int mNumStreams;
	std::list<VertexElement> mVertexElements;

    bool mInitialized; 
};

#endif
