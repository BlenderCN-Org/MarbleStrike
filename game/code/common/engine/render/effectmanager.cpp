//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/profiler.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gVertexAttribFormatType[VERTEX_ATTRIB_FORMAT_MAX] =
{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) TYPENAME,
	VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
};

static const char* gVertexAttribUsageType[VERTEX_ATTRIB_USAGE_MAX] =
{
#define VERTEX_ATTRIB_USAGE_ENTRY( ENUM, USAGENAME, D3DTYPE, PS3TYPE, OGLTYPE ) USAGENAME,
	VERTEX_ATTRIB_USAGE_TUPLE
#undef VERTEX_ATTRIB_USAGE_ENTRY
};

static int gVertexAttribFormatCoordinate[VERTEX_ATTRIB_FORMAT_MAX] =
{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) COORDINATE,
	VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

EffectManager::EffectManager() 
{	
}

//============================================================================

EffectManager::~EffectManager()
{
}

//============================================================================

Effect* EffectManager::CreateEffect()
{
	Effect* newEffect = NEW_PTR( "Effect" ) Effect;
	mEffects.push_back( newEffect );
	return newEffect;
}

//============================================================================

void EffectManager::Clear()
{
	std::list<Effect*>::iterator iter = mEffects.begin();
	while ( iter != mEffects.end() )
	{
		Effect* effect = *iter;
		effect->Clear();
		DELETE_PTR( effect );

		iter++;
	}
}

//============================================================================

Effect* EffectManager::GetEffect( const char* name )
{
	std::list<Effect*>::iterator iter = mEffects.begin();
	while ( iter != mEffects.end() )
	{
		Effect* effect = *iter;
		if ( strcmp( name, effect->GetName() ) == 0 )
		{
			return effect;
		}

		iter++;
	}

	char fullfilename[256];
	System::StringCopy( fullfilename, 256, System::Path::SystemSlash( "effects\\" ) );
	System::StringConcat( fullfilename, 256, name );
	System::StringConcat( fullfilename, 256, ".xml" );

	Effect* newEffect = Load( fullfilename, name );	

	return newEffect;
}

//============================================================================

Effect* EffectManager::Load( const char* filename, const char* name, bool loadXML )
{
	char filenameFullPath[256];
	const char* resourcePath = Database::Get()->GetResourcePath();
	System::StringCopy( filenameFullPath, 256, resourcePath );
	System::StringConcat( filenameFullPath, 256, filename );

	Effect* newEffect = NULL;
	newEffect = CreateEffect();
	newEffect->SetName( name );

	if ( loadXML )
	{
		FILE* file = System::OpenFile( filenameFullPath, "rb" );
		tinyxml2::XMLDocument doc;
        tinyxml2::XMLError loadOkay = doc.LoadFile( file );
		fclose( file );
		file = NULL;

        Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
		if ( loadOkay == tinyxml2::XML_SUCCESS )
		{	
			tinyxml2::XMLNode* node = &doc;
			LoadXML( node, newEffect );
		}	
	}

	return newEffect;
}

//===========================================================================

void EffectManager::Reload()
{
	std::list<Effect*>::iterator iter = mEffects.begin();
	while ( iter != mEffects.end() )
	{
		Effect* effect = *iter;
		effect->Reload();

		iter++;
	}
}

//===========================================================================

void EffectManager::LoadXML( tinyxml2::XMLNode* node, Effect* newEffect )
{   
    tinyxml2::XMLElement* element = node->ToElement();
	if ( element )
	{		
		const char* elementValue = element->Value();
		if ( strcmp( elementValue, "effect" ) == 0 )
		{		
		}
		else if ( strcmp( elementValue, "usage" ) == 0 )
		{	
			VertexDeclaration* vertexDeclaration = NULL;
			vertexDeclaration = newEffect->CreateVertexDeclaration();

			LoadUsageXML( node, vertexDeclaration );

			//vertexDeclaration->CreateVertexDeclaration();
		}
		else if ( strcmp( elementValue, "technique" ) == 0 )
		{				
			char techniqueName[256];
			System::StringCopy( techniqueName, 256, element->Attribute( "name" ) );

			EffectTechnique* newTechnique = NULL;
			newTechnique = newEffect->CreateTechnique();
			newTechnique->SetName( techniqueName );

			LoadTechniqueXML( node, newTechnique, newEffect );
		}
	}		

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadXML( item, newEffect );
	}
}

//===========================================================================

void EffectManager::LoadUsageXML( tinyxml2::XMLNode* node, VertexDeclaration* vertexDeclaration )
{
    tinyxml2::XMLElement* element = node->ToElement();
	if ( element )
	{		
		const char* elementValue = element->Value();
		if ( strcmp( elementValue, "stream" ) == 0 )
		{	
            int streamNum = element->IntAttribute( "num" );

			char dataType[256];					
			System::StringCopy( dataType, 256, element->Attribute( "datatype" ) );

			char streamUsage[256];
			System::StringCopy( streamUsage, 256, element->Attribute( "shaderinput" ) );

			char streamFormat[256];
			System::StringCopy( streamFormat, 256, element->Attribute( "format" ) );

			char usageName[256];
			int usageIndex = 0;					
			VertexElement::ExtractNameAndIndex( streamUsage, usageName, usageIndex );

			char dataName[256];
			int dataIndex = 0;
			VertexElement::ExtractNameAndIndex( dataType, dataName, dataIndex );

			int usageTypeIndex = FindIndex( gVertexAttribUsageType, VERTEX_ATTRIB_USAGE_MAX, usageName );
			int dataTypeIndex = FindIndex( gVertexAttribUsageType, VERTEX_ATTRIB_USAGE_MAX, dataName );
			int formatIndex = FindIndex( gVertexAttribFormatType, VERTEX_ATTRIB_FORMAT_MAX, streamFormat );

			VertexElement vertexElement;
			vertexElement.SetStream( streamNum );
			vertexElement.SetDataType( static_cast<UsageType>( dataTypeIndex ) );
			vertexElement.SetDataIndex( dataIndex );
			vertexElement.SetUsageType( static_cast<UsageType>( usageTypeIndex ) );
			vertexElement.SetUsageIndex( usageIndex );
			vertexElement.SetNumCoordinates( gVertexAttribFormatCoordinate[formatIndex] );
			vertexElement.SetFormatType( static_cast<FormatType>( formatIndex ) );

			vertexDeclaration->AddVertexElement( vertexElement );
		}
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadUsageXML( item, vertexDeclaration );
	}
}

//===========================================================================

void EffectManager::LoadTechniqueXML( 
	tinyxml2::XMLNode* node,
	EffectTechnique* newTechnique, 
	Effect* newEffect 
	)
{
    tinyxml2::XMLElement* element = node->ToElement();
	if ( element )
	{	
		const char* elementValue = element->Value();
		if ( strcmp( elementValue, "pass" ) == 0 )
		{	
			char passName[256];
			System::StringCopy( passName, 256, element->Attribute( "name" ) );

			EffectPass* newPass = NULL;
			newPass = newTechnique->CreatePass();
			newPass->SetName( passName );

			ResourceHandle<VertexShader> newVertexShader;
			ResourceHandle<PixelShader> newPixelShader;

			LoadPassXML( node, newPass, newVertexShader, newPixelShader );
			
			ResourceHandle<ShaderProgram> program = GameApp::Get()->GetShaderManager()->GetShaderProgram( newVertexShader, newPixelShader );
			newPass->SetShaderProgram( program );
		}	
	}

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadTechniqueXML( item, newTechnique, newEffect );
	}
}

//===========================================================================

void EffectManager::LoadPassXML( 
	tinyxml2::XMLNode* node, 
	EffectPass* newPass, 
	ResourceHandle<VertexShader> &newVertexShader, 
	ResourceHandle<PixelShader> &newPixelShader 
	)
{
    tinyxml2::XMLElement* element = node->ToElement();
	if ( element )
	{	
		const char* elementValue = element->Value();
		if ( strcmp( elementValue, "depthstencil" ) == 0 )
		{				
			char state[256];
			System::StringCopy( state, 256, element->Attribute( "name" ) );

			ResourceHandle<DepthStencilState> depthStencilState = GameApp::Get()->GetRenderStateManager()->GetDepthStencilState( state );
			newPass->SetDepthStencilState( depthStencilState );
		}
		else if ( strcmp( elementValue, "blend" ) == 0 )
		{
			char state[256];
			System::StringCopy( state, 256, element->Attribute( "name" ) );

			ResourceHandle<BlendState> blendState = GameApp::Get()->GetRenderStateManager()->GetBlendState( state );
			newPass->SetBlendState( blendState );
		}
		else if ( strcmp( elementValue, "rasterizer" ) == 0 )
		{
			char state[256];
			System::StringCopy( state, 256, element->Attribute( "name" ) );

			ResourceHandle<RasterizerState> rasterizerState = GameApp::Get()->GetRenderStateManager()->GetRasterizerState( state );
			newPass->SetRasterizerState( rasterizerState );
		}
		else if ( strcmp( elementValue, "sampler" ) == 0 )
		{
			char name[256];
			System::StringCopy( name, 256, element->Attribute( "name" ) );

			char state[256];
			System::StringCopy( state, 256, element->Attribute( "state" ) );

			ResourceHandle<SamplerState> samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerState( state );
            newPass->SetSamplerState( System::StringHash( name ), samplerState );
		}
		else if ( strcmp( elementValue, "vertexshader" ) == 0 )
		{
			char shaderName[256];
			System::StringCopy( shaderName, 256, element->Attribute( "name" ) );

			newVertexShader = GameApp::Get()->GetShaderManager()->GetVertexShader( shaderName );
			newPass->SetVertexShaderName( shaderName );
		}
		else if ( strcmp( elementValue, "pixelshader" ) == 0 )
		{					
			char shaderName[256];
			System::StringCopy( shaderName, 256, element->Attribute( "name" ) );

			newPixelShader = GameApp::Get()->GetShaderManager()->GetPixelShader( shaderName );					
			newPass->SetPixelShaderName( shaderName );
		}

	}	

	for ( tinyxml2::XMLNode* item = node->FirstChild(); item != NULL; item = item->NextSibling() )
	{
		LoadPassXML( item, newPass, newVertexShader, newPixelShader );
	}
}

//===========================================================================

int EffectManager::FindIndex( const char* nameArray[], int maxNum, const char* value )
{ 
	bool isFound = false;
	for ( int i = 0; i < maxNum; ++i )
	{
		if ( strcmp( nameArray[i], value ) == 0 )
		{
			isFound = true;
			return i;
		}
	}

	Assert( isFound == true, "" );
	return -1;
}