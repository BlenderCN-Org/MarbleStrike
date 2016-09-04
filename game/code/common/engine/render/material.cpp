//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/material.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/effectmanager.hpp"
#include <vector>
#include <string>
#include <algorithm>

//////////////////////////////////////////////////////
// GLOBAL
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Material::Material()
{
	mMaterialName[0] = '\0';
	mColorTexture = NULL;
	mNormalTexture = NULL;
	mEffect = NULL;
	mReflectivity = 0;
	mFriction = 1.0f;
	mUseTransparency = false;
}

//============================================================================

Material::~Material()
{
}

//===========================================================================

void Material::Load( tinyxml2::XMLNode* node )
{	
	tinyxml2::XMLElement* element = node->ToElement();
	
	const char* materialName = element->Attribute( "name" );
	System::StringCopy( mMaterialName, 32, materialName );

	const char* useTransparency = element->Attribute( "use_transparency" );
	mUseTransparency = false;
	if ( useTransparency && strcmp( useTransparency, "True" ) == 0 )
	{
		mUseTransparency = true;
	}

	const char* effectName = element->Attribute( "effect" );
	if ( effectName != NULL )
	{
		mEffect = GameApp::Get()->GetEffectManager()->GetEffect( effectName );
	}
	
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* currentElement = currentNode->ToElement();
		if ( currentElement )
		{			
			const char* currentElementValue = currentElement->Value();
			if ( strcmp( currentElementValue, "diffusecolor" ) == 0 )
			{
				const char* text = currentElement->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

				std::vector<std::string> tokens;
				tokens.reserve( 4 );
				System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

				mDiffuseColor[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				mDiffuseColor[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				mDiffuseColor[2] = static_cast<float>( atof( tokens[2].c_str() ) );
				mDiffuseColor[3] = static_cast<float>( atof( tokens[3].c_str() ) );
			}
			else if ( strcmp( currentElementValue, "specularcolor" ) == 0 )
			{
				const char* text = currentElement->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

				std::vector<std::string> tokens;
				tokens.reserve( 4 );
				System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

				mSpecularColor[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				mSpecularColor[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				mSpecularColor[2] = static_cast<float>( atof( tokens[2].c_str() ) );
				mSpecularColor[3] = static_cast<float>( atof( tokens[3].c_str() ) );
			}
			else if ( strcmp( currentElementValue, "reflectivity" ) == 0 )
			{
				const char* text = currentElement->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

				std::vector<std::string> tokens;
				tokens.reserve( 1 );
				System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

				mReflectivity = static_cast<float>( atof( tokens[0].c_str() ) );					
			}
			else if ( strcmp( currentElementValue, "friction" ) == 0 )
			{
				const char* text = currentElement->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

				std::vector<std::string> tokens;
				tokens.reserve( 1 );
				System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

				mFriction = static_cast<float>( atof( tokens[0].c_str() ) );					
			}
			else if ( strcmp( currentElementValue, "textureimage" ) == 0 )
			{
				const char* mapTo = currentElement->Attribute( "mapto" );                
				
                std::string str = currentElement->GetText();
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                const char* textureImageName = str.c_str();
                
				if ( mapTo )					
				{
					if ( strcmp( mapTo, "color" ) == 0 )
					{						
						mColorTexture = GameApp::Get()->GetTextureManager()->GetTexture( textureImageName );
					}
					else if ( strcmp( mapTo, "normal" ) == 0 )
					{							
						mNormalTexture = GameApp::Get()->GetTextureManager()->GetTexture( textureImageName );
					}
				}
			}
		}
	}
}