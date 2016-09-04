#ifndef DEPTH_STENCIL_STATE_HPP
#define DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <string.h>
#include "common/engine/system/utilities.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderStateShadowing;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

static const char* gDepthStencilStatePath = "effects\\states\\depthstencil\\";

#define COMPARE_FUNC_TUPLE \
	COMPARE_FUNC_ENTRY(CompareFunc_Never, "CompareFunc_Never" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Less, "CompareFunc_Less" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Equal, "CompareFunc_Equal" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Less_Equal, "CompareFunc_Less_Equal" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Greater, "CompareFunc_Greater" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Not_Equal, "CompareFunc_Not_Equal" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Greater_Equal, "CompareFunc_Greater_Equal" ) \
	COMPARE_FUNC_ENTRY(CompareFunc_Always, "CompareFunc_Always" ) \

enum CompareFunc
{
#define COMPARE_FUNC_ENTRY(ENUM, NAME) ENUM,
	COMPARE_FUNC_TUPLE
#undef COMPARE_FUNC_ENTRY
	COMPARE_FUNC_MAX
};

#define STENCIL_OP_TUPLE \
	STENCIL_OP_ENTRY(StencilOp_Keep, "StencilOp_Keep" ) \
	STENCIL_OP_ENTRY(StencilOp_Zero, "StencilOp_Zero" ) \
	STENCIL_OP_ENTRY(StencilOp_Replace, "StencilOp_Replace" ) \
	STENCIL_OP_ENTRY(StencilOp_Increase_Saturate, "StencilOp_Increase_Saturate" ) \
	STENCIL_OP_ENTRY(StencilOp_Decrease_Saturate, "StencilOp_Decrease_Saturate" ) \
	STENCIL_OP_ENTRY(StencilOp_Invert, "StencilOp_Invert" ) \
	STENCIL_OP_ENTRY(StencilOp_Increase, "StencilOp_Increase" ) \
	STENCIL_OP_ENTRY(StencilOp_Decrease, "StencilOp_Decrease" ) \

enum StencilOp
{
#define STENCIL_OP_ENTRY(ENUM, NAME) ENUM,
	STENCIL_OP_TUPLE
#undef STENCIL_OP_ENTRY
	STENCIL_OP_MAX
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct StencilOpStruct
{
	StencilOpStruct()
		:	mStencilFailOp( StencilOp_Keep )
		,	mStencilDepthFailOp( StencilOp_Keep )
		,	mStencilPassOp( StencilOp_Keep )
		,	mStencilFunc( CompareFunc_Always )
	{
	}

	StencilOp mStencilFailOp;
	StencilOp mStencilDepthFailOp;
	StencilOp mStencilPassOp;
	CompareFunc mStencilFunc;
};

struct DepthStencilStateStruct 
{
	DepthStencilStateStruct() 
		:	mDepthEnable( true )
		,	mDepthWriteMask( true )
		,	mDepthFunc( CompareFunc_Less_Equal )
		,	mStencilEnable( false )
		,	mStencilReadMask( 0xFFFFFFFF )
		,	mStencilWriteMask( 0xFFFFFFFF )	
		,	mStencilRef( 1 )
	{
	}

	bool mDepthEnable;
	bool mDepthWriteMask;
	CompareFunc mDepthFunc;
	bool mStencilEnable;
	int mStencilReadMask;
	int mStencilWriteMask;
	StencilOpStruct mFrontFace;
	StencilOpStruct mBackFace;
	int mStencilRef;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class DepthStencilState
{
public:

	DepthStencilState();
	virtual ~DepthStencilState();

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

	const DepthStencilStateStruct* GetData() const
	{
		return &mData;
	}

	void UpdateData( const DepthStencilStateStruct* newData );

	static const char* GetPath()
	{
		return gDepthStencilStatePath;
	}

	DepthStencilState& operator=( const DepthStencilState &otherState )
	{
		if ( this != &otherState )
		{
			System::StringCopy( this->mName, 32, otherState.mName );
			memcpy( &mData, &( otherState.mData ), sizeof( DepthStencilStateStruct ) );
		}

		return *this;
	}

private:

	virtual void Create()
	{

	}

	static int GetCompareFuncIndex( const char* name );
	static int GetStencilOpIndex( const char* name );

protected:
	
	void LoadXML( tinyxml2::XMLNode* node );
	void LoadStencilFace( tinyxml2::XMLNode* node, StencilOpStruct &data );

	char mName[32];
	DepthStencilStateStruct mData;
};

#endif
