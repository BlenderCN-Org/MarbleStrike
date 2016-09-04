#ifndef RASTERIZER_STATE_HPP
#define RASTERIZER_STATE_HPP

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

static const char* gRasterizerStatePath = "effects\\states\\rasterizer\\";

#define FILLMODE_TUPLE \
	FILLMODE_ENTRY(FillMode_Wireframe, "FillMode_Wireframe" ) \
	FILLMODE_ENTRY(FillMode_Solid, "FillMode_Solid" ) \

enum FillMode
{
#define FILLMODE_ENTRY(ENUM, NAME) ENUM,
	FILLMODE_TUPLE
#undef FILLMODE_ENTRY
	FILLMODE_MAX
};

#define CULLMODE_TUPLE \
	CULLMODE_ENTRY(CullMode_None, "CullMode_None" ) \
	CULLMODE_ENTRY(CullMode_Front, "CullMode_Front" ) \
	CULLMODE_ENTRY(CullMode_Back, "CullMode_Back" ) \

enum CullMode
{
#define CULLMODE_ENTRY(ENUM, NAME) ENUM,
	CULLMODE_TUPLE
#undef CULLMODE_ENTRY
	CULLMODE_MAX
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct RasterizerStateStruct
{
	RasterizerStateStruct()
		:	mFillMode( FillMode_Solid )
		,	mCullMode( CullMode_None )
		,	mFrontCounterClockwise( false )
		,	mDepthBias( 0 )
		,	mDepthBiasClamp( 0 )
		,	mSlopeScaledDepthBias( 0 )
		,	mDepthClipEnable( false )
		,	mScissorEnable( false )
		,	mMultisampleEnable( false )
		,	mAntialiasedLineEnable( false )
	{
	}

	FillMode mFillMode;
	CullMode mCullMode;
	bool mFrontCounterClockwise;
	int mDepthBias;
	float mDepthBiasClamp;
	float mSlopeScaledDepthBias;
	bool mDepthClipEnable;
	bool mScissorEnable;
	bool mMultisampleEnable;
	bool mAntialiasedLineEnable;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RasterizerState
{
public:

	RasterizerState();
	virtual ~RasterizerState();

	virtual void Dispatch( 
		RenderStateShadowing* renderStateShadowing, 
		void* context,
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

	const RasterizerStateStruct* GetData() const
	{
		return &mData;
	}

	void UpdateData( const RasterizerStateStruct* newData );

	static const char* GetPath()
	{
		return gRasterizerStatePath;
	}

	RasterizerState& operator=( const RasterizerState &otherState )
	{
		if ( this != &otherState )
		{
			System::StringCopy( this->mName, 32, otherState.mName );
			memcpy( &mData, &( otherState.mData ), sizeof( RasterizerStateStruct ) );
		}

		return *this;
	}

private:

	virtual void Create()
	{
	}

	static int GetFillModeIndex( const char* name );
	static int GetCullModeIndex( const char* name );

protected:

	void LoadXML( tinyxml2::XMLNode* node );	

	char mName[32];
	RasterizerStateStruct mData;
};

#endif
