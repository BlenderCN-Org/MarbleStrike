#ifndef CAMERA_VOLUME_HPP
#define CAMERA_VOLUME_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// FORWARD DECLARTIONS
//////////////////////////////////////////////////////

class Camera;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum CameraType
{
    CAMERA_TYPE_FOLLOW_TRACK,
    CAMERA_TYPE_FOLLOW
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class CameraVolume : public RenderObjectData
{
public:
	
	CameraVolume();
	~CameraVolume();

	void Update( float elapsedTime );
	void Load( tinyxml2::XMLNode* node );

    Math::Vector3 GetAngleOffset()
    {
        return mAngleOffset;
    }

    Math::Vector3 GetPositionOffset()
    {
        return mPositionOffset;
    }

    CameraType GetCameraType()
    {
        return mCameraType;
    }

	const char* GetCameraName()
	{
		return mCameraName;
	}
	
private:

    Math::Vector3 mAngleOffset;
    Math::Vector3 mPositionOffset;
    CameraType mCameraType;
	char mCameraName[32];
};

#endif
