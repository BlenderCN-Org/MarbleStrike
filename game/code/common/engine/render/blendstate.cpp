//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/blendstate.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/pathutilities.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gBlendNames[] = {
#define BLEND_FUNC_ENTRY(ENUM, NAME) NAME,
	BLEND_FUNC_TUPLE
#undef BLEND_FUNC_ENTRY
};

static const char* gBlendOpNames[] = {
#define BLENDOP_FUNC_ENTRY(ENUM, NAME) NAME,
	BLENDOP_FUNC_TUPLE
#undef BLENDOP_FUNC_ENTRY
};

static const char* gAlphaCompareFuncNames[] = {
#define ALPHA_FUNC_COMPARE_ENTRY(ENUM, NAME) NAME,
	ALPHA_FUNC_COMPARE_TUPLE
#undef ALPHA_FUNC_COMPARE_ENTRY
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BlendState::BlendState() 
{    
	System::StringCopy( mName, 32, "" );
}

//============================================================================

BlendState::~BlendState()
{
}

//===========================================================================

void BlendState::Load( const char* filename )
{
	char filenameFullPath[256];	
	const char* resourcePath = Database::Get()->GetResourcePath();
	System::StringCopy( filenameFullPath, 256, resourcePath );
	System::StringConcat( filenameFullPath, 256, filename );

	FILE* file = System::OpenFile( filenameFullPath, "rb" );
	tinyxml2::XMLDocument doc;
    tinyxml2::XMLError loadOkay = doc.LoadFile( file );
	fclose( file );
	file = NULL;

	Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
	if ( loadOkay == tinyxml2::XML_SUCCESS )
	{	
		tinyxml2::XMLNode* node = &doc;
		LoadXML( node );
	}

	Create();
}

//===========================================================================

void BlendState::UpdateData( const BlendStateStruct* newData )
{
	memcpy( &mData, newData, sizeof( BlendStateStruct ) );
}

//===========================================================================

int BlendState::GetBlendIndex( const char* name )
{
	for ( int i = 0; i < BLEND_FUNC_MAX; ++i )
	{
		if ( strcmp( name, gBlendNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find blend name." );
	return -1;
}

//===========================================================================

int BlendState::GetBlendOpIndex( const char* name )
{
	for ( int i = 0; i < BLENDOP_FUNC_MAX; ++i )
	{
		if ( strcmp( name, gBlendOpNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find blend op name." );
	return -1;
}

//===========================================================================

int BlendState::GetAlphaCompareFunc( const char* name )
{
	for ( int i = 0; i < ALPHA_FUNC_COMPARE_MAX; ++i )
	{
		if ( strcmp( name, gAlphaCompareFuncNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find alpha compare func name." );
	return -1;
}

//===========================================================================

void BlendState::LoadXML( tinyxml2::XMLNode* node )
{
    tinyxml2::XMLElement* element = node->ToElement();
	if ( element )
	{		
		const char* elementValue = element->Value();
		if ( strcmp( elementValue, "Name" ) == 0 )
		{
			const char* name = element->Attribute( "value" );
			System::StringCopy( mName, 32, name );
		}
		else if ( strcmp( elementValue, "AlphaToCoverageEnable" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mAlphaToCoverageEnable = boolValue;					
		}
		else if ( strstr( elementValue, "BlendEnable" ) )
		{
			int target = element->IntAttribute( "target" );

			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mBlendEnable[target] = boolValue;	
		}
		else if ( strcmp( elementValue, "SrcBlend" ) == 0 )
		{
			const char* blendName = element->Attribute( "value" );
			int blendIndex = GetBlendIndex( blendName );
			mData.mSrcBlend = static_cast<Blend>( blendIndex );
		}
		else if ( strcmp( elementValue, "DestBlend" ) == 0 )
		{
			const char* blendName = element->Attribute( "value" );
			int blendIndex = GetBlendIndex( blendName );
			mData.mDestBlend = static_cast<Blend>( blendIndex );
		}
		else if ( strcmp( elementValue, "BlendOp" ) == 0 )
		{
			const char* blendOpName = element->Attribute( "value" );
			int blendOpIndex = GetBlendOpIndex( blendOpName );
			mData.mBlendOp = static_cast<BlendOp>( blendOpIndex );
		}
		else if ( strcmp( elementValue, "SrcBlendAlpha" ) == 0 )
		{
			const char* blendName = element->Attribute( "value" );
			int blendIndex = GetBlendIndex( blendName );
			mData.mSrcBlendAlpha = static_cast<Blend>( blendIndex );
		}
		else if ( strcmp( elementValue, "DestBlendAlpha" ) == 0 )
		{
			const char* blendName = element->Attribute( "value" );
			int blendIndex = GetBlendIndex( blendName );
			mData.mDestBlendAlpha = static_cast<Blend>( blendIndex );
		}
		else if ( strcmp( elementValue, "BlendOpAlpha" ) == 0 )
		{
			const char* blendOpName = element->Attribute( "value" );
			int blendOpIndex = GetBlendOpIndex( blendOpName );
			mData.mBlendOpAlpha = static_cast<BlendOp>( blendOpIndex );
		}
		else if ( strcmp( elementValue, "RenderTargetWriteMask" ) == 0 )
		{
			int r = element->IntAttribute( "r" );
			int g = element->IntAttribute( "g" );
			int b = element->IntAttribute( "b" );
			int a = element->IntAttribute( "a" );

			int target = element->IntAttribute( "target" );

			unsigned int mask = ( ( r * 255 ) << 24 ) | ( ( g * 255 ) << 16 ) | ( ( b * 255 ) << 8 ) | ( ( a * 255 ) );
			mData.mRenderTargetWriteMask[target] = mask;
		}
		else if ( strstr( elementValue, "AlphaTestEnable" ) )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mAlphaTestEnable = boolValue;	
		}
		else if ( strcmp( elementValue, "AlphaFunc" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			mData.mAlphaFunc = static_cast<AlphaFuncComparison>( value );
		}
		else if ( strcmp( elementValue, "AlphaRef" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			mData.mAlphaRef = value;
		}
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadXML( item );
	}
}