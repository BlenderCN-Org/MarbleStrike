//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rasterizerstate.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/pathutilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gFillModeNames[] = {
#define FILLMODE_ENTRY(ENUM, NAME) NAME,
	FILLMODE_TUPLE
#undef FILLMODE_ENTRY
};

static const char* gCullModeNames[] = {
#define CULLMODE_ENTRY(ENUM, NAME) NAME,
	CULLMODE_TUPLE
#undef CULLMODE_ENTRY
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RasterizerState::RasterizerState() 
{    
	System::StringCopy( mName, 32, "" );
}

//============================================================================

RasterizerState::~RasterizerState()
{
}

//===========================================================================

void RasterizerState::Load( const char* filename )
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

void RasterizerState::UpdateData( const RasterizerStateStruct* newData )
{
	memcpy( &mData, newData, sizeof( RasterizerStateStruct ) );
}

//===========================================================================

int RasterizerState::GetFillModeIndex( const char* name )
{
	for ( int i = 0; i < FILLMODE_MAX; ++i )
	{
		if ( strcmp( name, gFillModeNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find fill mode name." );
	return -1;
}

//===========================================================================

int RasterizerState::GetCullModeIndex( const char* name )
{
	for ( int i = 0; i < CULLMODE_MAX; ++i )
	{
		if ( strcmp( name, gCullModeNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find cull mode name." );
	return -1;
}

//===========================================================================

void RasterizerState::LoadXML( tinyxml2::XMLNode* node )
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
		else if ( strcmp( elementValue, "FillMode" ) == 0 )
		{
			const char* fillModeName = element->Attribute( "value" );
			int fillModeIndex = GetFillModeIndex( fillModeName );
			mData.mFillMode = static_cast<FillMode>( fillModeIndex );
		}
		else if ( strcmp( elementValue, "CullMode" ) == 0 )
		{
			const char* cullModeName = element->Attribute( "value" );
			int cullModeIndex = GetCullModeIndex( cullModeName );
			mData.mCullMode = static_cast<CullMode>( cullModeIndex );
		}
		else if ( strcmp( elementValue, "FrontCounterClockwise" ) == 0 )
		{
            int value = element->IntAttribute( "value" );			
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mFrontCounterClockwise = boolValue;
		}
		else if ( strcmp( elementValue, "DepthBias" ) == 0 )
		{
			int value = element->IntAttribute( "value" );			
			mData.mDepthBias = value;
		}
		else if ( strcmp( elementValue, "DepthBiasClamp" ) == 0 )
		{
			double value = element->IntAttribute( "value" );		
			mData.mDepthBiasClamp = static_cast<float>( value );
		}
		else if ( strcmp( elementValue, "SlopeScaledDepthBias" ) == 0 )
		{
			double value = element->IntAttribute( "value" );		
			mData.mSlopeScaledDepthBias = static_cast<float>( value );
		}
		else if ( strcmp( elementValue, "DepthClipEnable" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mDepthClipEnable = boolValue;
		}
		else if ( strcmp( elementValue, "ScissorEnable" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mScissorEnable = boolValue;
		}
		else if ( strcmp( elementValue, "MultisampleEnable" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mMultisampleEnable = boolValue;
		}
		else if ( strcmp( elementValue, "AntialiasedLineEnable" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mAntialiasedLineEnable = boolValue;
		}
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadXML( item );
	}
}
