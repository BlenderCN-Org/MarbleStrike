//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/samplerstate.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/pathutilities.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gFilterModeNames[] = {
#define FILTER_MODE_ENTRY(ENUM, NAME) NAME,
	FILTER_MODE_TUPLE
#undef FILTER_MODE_ENTRY
};

static const char* gTextureAddressNames[] = {
#define TEXTURE_ADDRESS_ENTRY(ENUM, NAME) NAME,
	TEXTURE_ADDRESS_TUPLE
#undef TEXTURE_ADDRESS_ENTRY
};


//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

SamplerState::SamplerState() 
{    
	System::StringCopy( mName, 32, "" );
}

//============================================================================

SamplerState::~SamplerState()
{
}

//===========================================================================

void SamplerState::Load( const char* filename )
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

void SamplerState::UpdateData( const SamplerStateStruct* newData )
{
	memcpy( &mData, newData, sizeof( SamplerStateStruct ) );
}

//===========================================================================

int SamplerState::GetFilterModeIndex( const char* name )
{
	for ( int i = 0; i < FILTER_MODE_MAX; ++i )
	{
		if ( strcmp( name, gFilterModeNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find filter name." );
	return -1;
}

//===========================================================================

int SamplerState::GetTextureAddressModeIndex( const char* name )
{
	for ( int i = 0; i < TEXTURE_ADDRESS_MAX; ++i )
	{
		if ( strcmp( name, gTextureAddressNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find texture address name." );
	return -1;
}

//===========================================================================

void SamplerState::LoadXML( tinyxml2::XMLNode* node )
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
		else if ( strcmp( elementValue, "MinFilter" ) == 0 )
		{					
			const char* name = element->Attribute( "value" );
			int filterIndex = GetFilterModeIndex( name );
			mData.mMinFilter = static_cast<FilterMode>( filterIndex );
		}
		else if ( strcmp( elementValue, "MagFilter" ) == 0 )
		{
			const char* name = element->Attribute( "value" );
			int filterIndex = GetFilterModeIndex( name );
			mData.mMagFilter = static_cast<FilterMode>( filterIndex );
		}
		else if ( strcmp( elementValue, "MipFilter" ) == 0 )
		{
			const char* name = element->Attribute( "value" );
			int filterIndex = GetFilterModeIndex( name );
			mData.mMipFilter = static_cast<FilterMode>( filterIndex );
		}
		else if ( strcmp( elementValue, "AddressU" ) == 0 )
		{
			const char* name = element->Attribute( "value" );
			int addressModeIndex = GetTextureAddressModeIndex( name );
			mData.mAddressU = static_cast<TextureAddressMode>( addressModeIndex );
		}				
		else if ( strcmp( elementValue, "AddressV" ) == 0 )
		{
			const char* name = element->Attribute( "value" );
			int addressModeIndex = GetTextureAddressModeIndex( name );
			mData.mAddressV = static_cast<TextureAddressMode>( addressModeIndex );
		}
		else if ( strcmp( elementValue, "AddressW" ) == 0 )
		{
			const char* name = element->Attribute( "value" );
			int addressModeIndex = GetTextureAddressModeIndex( name );
			mData.mAddressW = static_cast<TextureAddressMode>( addressModeIndex );
		}
		else if ( strcmp( elementValue, "BorderColor" ) == 0 )
		{
			double value = 0;
			value = element->DoubleAttribute( "r" );
			mData.mBorderColor[0] = static_cast<float>( value );					
            value = element->DoubleAttribute( "g" );
			mData.mBorderColor[1] = static_cast<float>( value );
            value = element->DoubleAttribute( "b" );
			mData.mBorderColor[2] = static_cast<float>( value );
            value = element->DoubleAttribute( "a" );
			mData.mBorderColor[3] = static_cast<float>( value );
		}
		else if ( strcmp( elementValue, "MipLODBias" ) == 0 )
		{
			double value = element->DoubleAttribute( "value" );
			mData.mMipLODBias = static_cast<float>( value );
		}
		else if ( strcmp( elementValue, "MaxAnisotropy" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			mData.mMaxAnisotropy = static_cast<unsigned int>( value );
		}
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadXML( item );
	}
}