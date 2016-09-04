//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/depthstencilstate.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/pathutilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gCompareFuncNames[] = {
#define COMPARE_FUNC_ENTRY(ENUM, NAME) NAME,
	COMPARE_FUNC_TUPLE
#undef COMPARE_FUNC_ENTRY
};

static const char* gStencilOpNames[] = {
#define STENCIL_OP_ENTRY(ENUM, NAME) NAME,
	STENCIL_OP_TUPLE
#undef STENCIL_OP_ENTRY
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

DepthStencilState::DepthStencilState() 
{    
	System::StringCopy( mName, 32, "" );
}

//============================================================================

DepthStencilState::~DepthStencilState()
{
}

//===========================================================================

void DepthStencilState::Load( const char* filename )
{
	char filenameFullPath[256];
	System::StringCopy( filenameFullPath, 256, "" );
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

void DepthStencilState::UpdateData( const DepthStencilStateStruct* newData )
{
	memcpy( &mData, newData, sizeof( DepthStencilStateStruct ) );
}

//===========================================================================

int DepthStencilState::GetCompareFuncIndex( const char* name )
{
	for ( int i = 0; i < COMPARE_FUNC_MAX; ++i )
	{
		if ( strcmp( name, gCompareFuncNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find compare func name." );
	return -1;
}

//===========================================================================

int DepthStencilState::GetStencilOpIndex( const char* name )
{
	for ( int i = 0; i < STENCIL_OP_MAX; ++i )
	{
		if ( strcmp( name, gStencilOpNames[i] ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "Cannot find stencil op name." );
	return -1;
}

//===========================================================================

void DepthStencilState::LoadXML( tinyxml2::XMLNode* node )
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
		else if ( strcmp( elementValue, "DepthEnable" ) == 0 )
		{
            int value = element->IntAttribute( "value" );

			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}

			mData.mDepthEnable = boolValue;					
		}
		else if ( strcmp( elementValue, "DepthWriteMask" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mDepthWriteMask = boolValue;	
		}
		else if ( strcmp( elementValue, "DepthFunc" ) == 0 )
		{
			const char* depthFuncName = element->Attribute( "value" );
			int depthFuncIndex = GetCompareFuncIndex( depthFuncName );
			mData.mDepthFunc = static_cast<CompareFunc>( depthFuncIndex );
		}
		else if ( strcmp( elementValue, "StencilEnable" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			bool boolValue = true;
			if ( value == 0 )
			{
				boolValue = false;
			}
			mData.mStencilEnable = boolValue;
		}
		else if ( strcmp( elementValue, "StencilRef" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			mData.mStencilRef = value;
		}
		else if ( strcmp( elementValue, "StencilReadMask" ) == 0 )
		{
			int value = element->IntAttribute( "value" );
			mData.mStencilReadMask = value;
		}
		else if ( strcmp( elementValue, "StencilWriteMask" ) == 0 )
		{
			int value = element->IntAttribute( "value" );	
			mData.mStencilWriteMask = value;
		}
		else if ( strcmp( elementValue, "FrontFace" ) == 0 )
		{
			LoadStencilFace( node, mData.mFrontFace );
		}
		else if ( strcmp( elementValue, "BackFace" ) == 0 )
		{
			LoadStencilFace( node, mData.mBackFace );
		}
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadXML( item );
	}
}

//===========================================================================

void DepthStencilState::LoadStencilFace( tinyxml2::XMLNode* node, StencilOpStruct &data )
{
    tinyxml2::XMLElement* element = node->ToElement();
	if ( element )
	{	
		const char* elementValue = element->Value();

		if ( strcmp( elementValue, "StencilFailOp" ) == 0 )
		{		
			const char* stencilOpName = element->Attribute( "value" );
			int stencilOpIndex = GetStencilOpIndex( stencilOpName );
			data.mStencilFailOp = static_cast<StencilOp>( stencilOpIndex );
		}
		else if ( strcmp( elementValue, "StencilDepthFailOp" ) == 0 )
		{					
			const char* stencilOpName = element->Attribute( "value" );
			int stencilOpIndex = GetStencilOpIndex( stencilOpName );
			data.mStencilDepthFailOp = static_cast<StencilOp>( stencilOpIndex );
		}
		else if ( strcmp( elementValue, "StencilPassOp" ) == 0 )
		{					
			const char* stencilOpName = element->Attribute( "value" );
			int stencilOpIndex = GetStencilOpIndex( stencilOpName );
			data.mStencilPassOp = static_cast<StencilOp>( stencilOpIndex );
		}
		else if ( strcmp( elementValue, "StencilFunc" ) == 0 )
		{					
			const char* funcName = element->Attribute( "value" );
			int funcIndex = GetCompareFuncIndex( funcName );
			data.mStencilFunc = static_cast<CompareFunc>( funcIndex );
		}
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadStencilFace( item, data );
	}
}
