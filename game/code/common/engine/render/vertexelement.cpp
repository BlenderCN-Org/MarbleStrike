//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/shader.hpp"
#include "common/engine/system/assert.hpp"
#include <string.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

VertexElement::VertexElement()
:	mStream( 0 )
,	mUsage( USAGE_POSITION )
,	mUsageIndex( 0 )
,	mDataType( USAGE_POSITION )
,	mDataIndex( 0 )
,	mNumCoordinates( 0 )
,	mFormat( FORMAT_TYPE_NONE )
{

}

//============================================================================

VertexElement::~VertexElement()
{

}

//============================================================================

void VertexElement::SetStream( int i )
{
	Assert( i < MAX_NUM_STREAMS, "Exceed maximum number of streams." );
	mStream = i;
}

//============================================================================

void VertexElement::ExtractNameAndIndex( const char* usageType, char* usageName, int &index )
{
	int length = static_cast<int>( strlen( usageType ) );

	int stringIndex = 0;
	for ( int i = 0; i < length; ++i )
	{
		if ( usageType[i] < '0' || usageType[i] > '9' )
		{
			usageName[stringIndex] = usageType[i];
			stringIndex++;
		}
		else
		{
			index = static_cast<int>( usageType[i] - '0' );
		}
	}

	usageName[stringIndex] = '\0';
}