//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/boundingvolume.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/quaternion.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BoundingVolume::BoundingVolume() 
	:	RenderObjectData( RENDER_OBJECT_DATA_TYPE_BOUNDING_VOLUME )
{
    mIsMoveableX = false;
    mIsMoveableZ = false;
}

//===========================================================================

BoundingVolume::~BoundingVolume()
{
}

//===========================================================================

void BoundingVolume::Load( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{			
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "bounding_volume" ) == 0 )
			{			
				const char* isMoveableX = element->Attribute( "is_moveable_x" );
                if ( isMoveableX && System::StringICmp( isMoveableX, "true" ) == 0 )
                {
                    mIsMoveableX = true;
                }

                const char* isMoveableZ = element->Attribute( "is_moveable_z" );
                if ( isMoveableZ && System::StringICmp( isMoveableZ, "true" ) == 0 )
                {
                    mIsMoveableZ = true;
                }
			}
		}
	}
}