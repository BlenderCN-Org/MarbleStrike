#ifndef BLEND_STATE_HPP
#define BLEND_STATE_HPP

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

static const char* gBlendStatePath = "effects\\states\\blend\\";

#define BLEND_FUNC_TUPLE \
	BLEND_FUNC_ENTRY(Blend_Zero, "Blend_Zero" ) \
	BLEND_FUNC_ENTRY(Blend_One, "Blend_One" ) \
	BLEND_FUNC_ENTRY(Blend_Src_Color, "Blend_Src_Color" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Src_Color, "Blend_Inv_Src_Color" ) \
	BLEND_FUNC_ENTRY(Blend_Src_Alpha, "Blend_Src_Alpha" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Src_Alpha, "Blend_Inv_Src_Alpha" ) \
	BLEND_FUNC_ENTRY(Blend_Dest_Alpha, "Blend_Dest_Alpha" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Dest_Alpha, "Blend_Inv_Dest_Alpha" ) \
	BLEND_FUNC_ENTRY(Blend_Dest_Color, "Blend_Dest_Color" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Dest_Color, "Blend_Inv_Dest_Color" ) \
	BLEND_FUNC_ENTRY(Blend_Src_Alpha_Sat, "Blend_Src_Alpha_Sat" ) \
	BLEND_FUNC_ENTRY(Blend_Blend_Factor, "Blend_Blend_Factor" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Blend_Factor, "Blend_Inv_Blend_Factor" ) \
	BLEND_FUNC_ENTRY(Blend_Src1_Color, "Blend_Src1_Color" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Src1_Color, "Blend_Inv_Src1_Color" ) \
	BLEND_FUNC_ENTRY(Blend_Src1_Alpha, "Blend_Src1_Alpha" ) \
	BLEND_FUNC_ENTRY(Blend_Inv_Src1_Alpha, "Blend_Inv_Src1_Alpha" ) \

enum Blend
{
#define BLEND_FUNC_ENTRY(ENUM, NAME) ENUM,
	BLEND_FUNC_TUPLE
#undef BLEND_FUNC_ENTRY
	BLEND_FUNC_MAX
};

#define BLENDOP_FUNC_TUPLE \
	BLENDOP_FUNC_ENTRY(BlendOp_Add, "BlendOp_Add" ) \
	BLENDOP_FUNC_ENTRY(BlendOp_Subtract, "BlendOp_Subtract" ) \
	BLENDOP_FUNC_ENTRY(BlendOp_Rev_Subtract, "BlendOp_Rev_Subtract" ) \
	BLENDOP_FUNC_ENTRY(BlendOp_Min, "BlendOp_Min" ) \
	BLENDOP_FUNC_ENTRY(BlendOp_Max, "BlendOp_Max" ) \

enum BlendOp
{
#define BLENDOP_FUNC_ENTRY(ENUM, NAME) ENUM,
	BLENDOP_FUNC_TUPLE
#undef BLENDOP_FUNC_ENTRY
	BLENDOP_FUNC_MAX
};

#define ALPHA_FUNC_COMPARE_TUPLE \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Never, "AlphaFuncCompare_Never" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Less, "AlphaFuncCompare_Less" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Equal, "AlphaFuncCompare_Equal" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Less_Equal, "AlphaFuncCompare_Less_Equal" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Greater, "AlphaFuncCompare_Greater" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Not_Equal, "AlphaFuncCompare_Not_Equal" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Greater_Equal, "AlphaFuncCompare_Greater_Equal" ) \
	ALPHA_FUNC_COMPARE_ENTRY(AlphaFuncCompare_Always, "AlphaFuncCompare_Always" ) \

enum AlphaFuncComparison
{
#define ALPHA_FUNC_COMPARE_ENTRY(ENUM, NAME) ENUM,
	ALPHA_FUNC_COMPARE_TUPLE
#undef ALPHA_FUNC_COMPARE_ENTRY
	ALPHA_FUNC_COMPARE_MAX
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

static const int MAX_BLEND_RENDER_TARGET = 8;

struct BlendStateStruct 
{
	BlendStateStruct()
		:	mAlphaToCoverageEnable( false )
		,	mSrcBlend( Blend_Zero )
		,	mDestBlend( Blend_Zero )
		,	mBlendOp( BlendOp_Add )
		,	mSrcBlendAlpha( Blend_Zero )
		,	mDestBlendAlpha( Blend_Zero )
		,	mBlendOpAlpha( BlendOp_Add )
		,	mAlphaTestEnable( false )
		,	mAlphaFunc( AlphaFuncCompare_Never )
		,	mAlphaRef( 0 )
	{
		memset( mBlendEnable, false, sizeof( bool ) * MAX_BLEND_RENDER_TARGET );
		memset( mRenderTargetWriteMask, 0, sizeof( unsigned int ) * MAX_BLEND_RENDER_TARGET );
	}

	bool mAlphaToCoverageEnable;
	bool mBlendEnable[MAX_BLEND_RENDER_TARGET];
	Blend mSrcBlend;
	Blend mDestBlend;
	BlendOp mBlendOp;
	Blend mSrcBlendAlpha;
	Blend mDestBlendAlpha;
	BlendOp mBlendOpAlpha;
	unsigned int mRenderTargetWriteMask[MAX_BLEND_RENDER_TARGET];
	bool mAlphaTestEnable;
	AlphaFuncComparison mAlphaFunc;
	int mAlphaRef;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BlendState
{
public:

	BlendState();
	virtual ~BlendState();

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

	const BlendStateStruct* GetData() const
	{
		return &mData;
	}

	void UpdateData( const BlendStateStruct* newData );

	static const char* GetPath()
	{
		return gBlendStatePath;
	}

	BlendState& operator=( const BlendState &otherState )
	{
		if ( this != &otherState )
		{
			System::StringCopy( this->mName, 32, otherState.mName );
			memcpy( &mData, &( otherState.mData ), sizeof( BlendStateStruct ) );
		}

		return *this;
	}

private:

	virtual void Create()
	{

	}

	static int GetBlendIndex( const char* name );
	static int GetBlendOpIndex( const char* name );
	static int GetAlphaCompareFunc( const char* name );
		
protected:

	void LoadXML( tinyxml2::XMLNode* node );

	char mName[32];
	BlendStateStruct mData;
};

#endif
