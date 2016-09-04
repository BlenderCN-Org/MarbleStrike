//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/cameravolume.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/quaternion.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

CameraVolume::CameraVolume() 
	:	RenderObjectData( RENDER_OBJECT_DATA_TYPE_CAMERA_VOLUME )
{
    mCameraType = CAMERA_TYPE_FOLLOW;
}

//===========================================================================

CameraVolume::~CameraVolume()
{
}

//===========================================================================

void CameraVolume::Update( float /*elapsedTime*/ )
{	
}

//===========================================================================

void CameraVolume::Load( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{			
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "camera_volume" ) == 0 )
			{			
				const char* cameraType = element->Attribute( "camera_type" );
				if ( strcmp( cameraType, "camera_follow_track" ) == 0 )
                {
                    mCameraType = CAMERA_TYPE_FOLLOW_TRACK;
                }
                else if ( strcmp( cameraType, "camera_follow" ) == 0 )
                {
                    mCameraType = CAMERA_TYPE_FOLLOW;
                }

				const char* cameraName = element->Attribute( "camera_name" );
				if ( cameraName )
				{
					System::StringCopy( mCameraName, 32, cameraName );
				}

                const char* xAngleOffset = element->Attribute( "x_angle_offset" );
                mAngleOffset[0] = static_cast<float>( atof( xAngleOffset ) );
                const char* yAngleOffset = element->Attribute( "y_angle_offset" );
                mAngleOffset[1] = static_cast<float>( atof( yAngleOffset ) );
                const char* zAngleOffset = element->Attribute( "z_angle_offset" );
                mAngleOffset[2] = static_cast<float>( atof( zAngleOffset ) );

                const char* xPositionOffset = element->Attribute( "x_position_offset" );
                mPositionOffset[0] = static_cast<float>( atof( xPositionOffset ) );
                const char* yPositionOffset = element->Attribute( "y_position_offset" );
                mPositionOffset[1] = static_cast<float>( atof( yPositionOffset ) );
                const char* zPositionOffset = element->Attribute( "z_position_offset" );
                mPositionOffset[2] = static_cast<float>( atof( zPositionOffset ) );
			}
		}
	}
}