#ifndef MATERIAL_HPP
#define MATERIAL_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Texture;
class Effect;

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Material
{
public:

	Material();
	~Material();
		
	void Load( tinyxml2::XMLNode* node );
	
	void SetName( const char* name )
	{
		System::StringCopy( mMaterialName, 32, name );
	}
	
	const char* GetMaterialName() const
	{
		return mMaterialName;
	}

	Math::Vector4 GetDiffuseColor() const
	{
		return mDiffuseColor;
	}

	void SetDiffuseColor( const Math::Vector4 &color )
	{
		mDiffuseColor = color;
	}

	Math::Vector4 GetSpecularColor() const
	{
		return mSpecularColor;
	}

	Texture* GetColorTexture() const
	{
		return mColorTexture;
	}

	void SetColorTexture( Texture* texture )
	{
		mColorTexture = texture;
	}

	Texture* GetNormalTexture() const
	{
		return mNormalTexture;
	}

	Effect* GetEffect() const
	{
		return mEffect;
	}

	float GetReflectivity()
	{
		return mReflectivity;
	}

	void SetReflectivity( float value )
	{
		mReflectivity = value;
	}

	float GetFriction()
	{
		return mFriction;
	}

	bool GetUseTransparency()
	{
		return mUseTransparency;
	}

	void SetmUseTransparency( bool value )
	{
		mUseTransparency = value;
	}
	
private:
	
	char mMaterialName[32];	
	Math::Vector4 mDiffuseColor;
	Math::Vector4 mSpecularColor;
	float mReflectivity;
	Texture* mColorTexture;
	Texture* mNormalTexture;
	Effect* mEffect;
	float mFriction;
	bool mUseTransparency;
};

#endif
