//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/quaternion.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Math::Matrix44 Camera::CreateTransform( 
	const Math::Vector3& position, 
	const Math::Vector3& up, 
	const Math::Vector3& look 
	)
{
	Math::Vector3 z = position - look;
	Math::Vector3 x = Math::Vector3::Cross( up, z );
	Math::Vector3 y = Math::Vector3::Cross( z, x );

	y = Math::Vector3::Normalize( y );
	x = Math::Vector3::Normalize( x );
	z = Math::Vector3::Normalize( z );

	Math::Matrix44 transform;
	transform[0] = x[0];
	transform[1] = x[1];
	transform[2] = x[2];
	transform[3] = 0;
	transform[4] = y[0];
	transform[5] = y[1];
	transform[6] = y[2];
	transform[7] = 0;
	transform[8] = z[0];
	transform[9] = z[1];
	transform[10] = z[2];
	transform[11] = 0;
	transform[12] = position[0];
	transform[13] = position[1];
	transform[14] = position[2];
	transform[15] = 1;

	return transform;
}

//===========================================================================

Camera::Camera() 
	:	RenderObjectData( RENDER_OBJECT_DATA_TYPE_CAMERA )
{	
	mScreenWidth = 1;
	mScreenHeight = 1;
	mFieldOfViewAngleRadians = 0;
	mZNear = 0;
	mZFar = 0;	
	System::StringCopy( mName, 64, "" );	
	System::StringCopy( mTrackToName, 64, "" );	
	mCameraType = CAMERA_VIEW_TYPE_PERSPECTIVE;
}

//===========================================================================

Camera::~Camera()
{
}

//===========================================================================

void Camera::Update( float /*elapsedTime*/ )
{	
}

//===========================================================================

void Camera::Load( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "camera" ) == 0 )
			{			
				const char* name = element->Attribute( "name" );
				SetName( name );

                double value = element->DoubleAttribute( "angle" );
				mFieldOfViewAngleRadians = static_cast<float>( value );
                value = element->DoubleAttribute( "near" );
				mZNear = static_cast<float>( value );
                value = element->DoubleAttribute( "far" );
				mZFar = static_cast<float>( value );

				const char* trackToName = element->Attribute( "trackto" );
				if ( trackToName )
				{
					System::StringCopy( mTrackToName, 64, trackToName );
				}
			}
		}
	}
}