#ifndef SAMPLER_STATE_HPP
#define SAMPLER_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <string.h>
#include "tinyxml2.h"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderStateShadowing;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

static const char* gSamplerStatePath = "effects\\states\\sampler\\";

#define FILTER_MODE_TUPLE \
	FILTER_MODE_ENTRY(Filter_None, "Filter_None" ) \
	FILTER_MODE_ENTRY(Filter_Point, "Filter_Point" ) \
	FILTER_MODE_ENTRY(Filter_Linear, "Filter_Linear" ) \
	FILTER_MODE_ENTRY(Filter_Anisotropic, "Filter_Anisotropic" )

enum FilterMode
{
#define FILTER_MODE_ENTRY(ENUM, NAME) ENUM,
	FILTER_MODE_TUPLE
#undef FILTER_MODE_ENTRY
	FILTER_MODE_MAX
};

#define TEXTURE_ADDRESS_TUPLE \
	TEXTURE_ADDRESS_ENTRY(TextureAddressMode_Wrap, "TextureAddressMode_Wrap" ) \
	TEXTURE_ADDRESS_ENTRY(TextureAddressMode_Mirror, "TextureAddressMode_Mirror" ) \
	TEXTURE_ADDRESS_ENTRY(TextureAddressMode_Clamp, "TextureAddressMode_Clamp" ) \
	TEXTURE_ADDRESS_ENTRY(TextureAddressMode_Border, "TextureAddressMode_Border" ) \
	TEXTURE_ADDRESS_ENTRY(TextureAddressMode_Mirror_once, "TextureAddressMode_Mirror_once" ) \

enum TextureAddressMode
{
#define TEXTURE_ADDRESS_ENTRY(ENUM, NAME) ENUM,
	TEXTURE_ADDRESS_TUPLE
#undef TEXTURE_ADDRESS_ENTRY
	TEXTURE_ADDRESS_MAX
};

///////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct SamplerStateStruct 
{
	SamplerStateStruct()
		:	mMinFilter( Filter_Point )
		,	mMagFilter( Filter_Point )
		,	mMipFilter( Filter_Point )
		,	mAddressU( TextureAddressMode_Wrap )
		,	mAddressV( TextureAddressMode_Wrap )
		,	mAddressW( TextureAddressMode_Wrap )
		,	mMipLODBias( 0 )
		,	mMaxAnisotropy( 1 )
	{
		memset( mBorderColor, 0, sizeof( float ) * 4 );
	}

	FilterMode mMinFilter;
	FilterMode mMagFilter;
	FilterMode mMipFilter;
	TextureAddressMode mAddressU;
	TextureAddressMode mAddressV;
	TextureAddressMode mAddressW;
	float mBorderColor[4];
	float mMipLODBias;
	unsigned int mMaxAnisotropy;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class SamplerState
{
public:

	SamplerState();
	virtual ~SamplerState();

	virtual void Dispatch( 
		RenderStateShadowing* renderStateShadowing, 
		void* context,
		int samplerIndex, 
		bool forceUpload = false 
		) = 0;
	void Load( const char* filename );

	const char* GetName()
	{
		return mName;
	}

	void SetName( const char* name )
	{
		System::StringCopy( mName, 32, name );
	}

	const SamplerStateStruct* GetData() const
	{
		return &mData;
	}

	void UpdateData( const SamplerStateStruct* newData );

	static const char* GetPath()
	{
		return gSamplerStatePath;
	}

	SamplerState& operator=( const SamplerState &otherState )
	{
		if ( this != &otherState )
		{
			System::StringCopy( this->mName, 32, otherState.mName );
			memcpy( &mData, &( otherState.mData ), sizeof( SamplerStateStruct ) );
		}

		return *this;
	}

private:

	virtual void Create()
	{

	}

	static int GetFilterModeIndex( const char* name );	
	static int GetTextureAddressModeIndex( const char* name );	
		
protected:

	void LoadXML( tinyxml2::XMLNode* node );

	char mName[32];
	SamplerStateStruct mData;
};

#endif
