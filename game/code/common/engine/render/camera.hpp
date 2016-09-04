#ifndef CAMERA_HPP
#define CAMERA_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum CameraViewType
{
	CAMERA_VIEW_TYPE_PERSPECTIVE,
	CAMERA_VIEW_TYPE_ORTHO
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Camera : public RenderObjectData
{
public:

	static Math::Matrix44 CreateTransform( 
		const Math::Vector3& position, 
		const Math::Vector3& up, 
		const Math::Vector3& look 
		);
	
	Camera();
	~Camera();

	void Update( float elapsedTime );
	void Load( tinyxml2::XMLNode* node );
	
	void SetName( const char* name )
	{
		System::StringCopy( mName, 64, name );
	}

	const char* GetName()
	{
		return mName;
	}

	int GetScreenWidth()
	{
		return mScreenWidth;
	}

	void SetScreenWidth( int width )
	{
		mScreenWidth = width;
	}
	
	int GetScreenHeight()
	{
		return mScreenHeight;
	}

	void SetScreenHeight( int height )
	{
		mScreenHeight = height;
	}

	float GetFieldOfView() const
	{
		return mFieldOfViewAngleRadians;
	}	

	void SetFieldOfView( float radians )
	{
		mFieldOfViewAngleRadians = radians;
	}

	float GetZNear() const
	{
		return mZNear;
	}	

	float GetZFar() const
	{
		return mZFar;
	}	

	void SetZNearFar( float zNear, float zFar )
	{
		mZNear = zNear;
		mZFar = zFar;
	}

	float GetAspectRatio() const
	{
		float width = static_cast<float>( mScreenWidth );
		float height = static_cast<float>( mScreenHeight );
		return height / width;
	}

	const char* GetTrackToName()
	{
		return mTrackToName;
	}
	
private:
	
	char mName[64];
	char mTrackToName[64];
				
	float mFieldOfViewAngleRadians;
	int mScreenWidth;
	int mScreenHeight;
	float mZNear;
	float mZFar;

	CameraViewType mCameraType;
};

#endif
