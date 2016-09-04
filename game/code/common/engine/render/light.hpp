#ifndef LIGHT_HPP
#define LIGHT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "tinyxml2.h"
#include <vector>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Light : public RenderObjectData
{
public:

	enum LightType
	{
		LIGHT_TYPE_POINT,
		LIGHT_TYPE_DIRECTIONAL,
		LIGHT_TYPE_SPOT
	};
	
	Light();
	~Light();
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

	void SetAmbientColor( Math::Vector4 color )
	{
		mAmbientColor = color;
	}

	Math::Vector4 GetAmbientColor()
	{
		return mAmbientColor;
	}

	void SetColor( Math::Vector4 color )
	{
		mColor = color;
	}

	Math::Vector4 GetColor()
	{
		return mColor;
	}

	void SetRadius( float radius )
	{
		mRadius = radius;
	}

	float GetRadius()
	{
		return mRadius;
	}

	float GetFoVAngle()
	{
		return mFoVAngle;
	}

	float GetZNear()
	{
		return mZNear;
	}

	float GetZFar()
	{
		return mZFar;
	}

private:

	void LoadLight( tinyxml2::XMLNode* node );

	char mName[64];

	LightType mLightType;
	float mZNear;
	float mZFar;
	float mFoVAngle;
	Math::Vector4 mColor;
	Math::Vector4 mAmbientColor;
	float mRadius;
};

#endif
