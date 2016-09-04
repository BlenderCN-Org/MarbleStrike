//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/light.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/math/quaternion.hpp"
#include <string>
#include <vector>
#include <algorithm>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Light::Light()
	:	RenderObjectData( RENDER_OBJECT_DATA_TYPE_LIGHT )
{
	mLightType = LIGHT_TYPE_POINT;
	mZNear = 0;
	mZFar = 0;
	mFoVAngle = 0;
	mRadius = 0;	
	mColor.Set( 1, 1, 1, 1 );
}

//===========================================================================

Light::~Light()
{
}

//===========================================================================

void Light::Update( float /*elapsedTime*/ )
{
}

//===========================================================================

void Light::Load( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "light" ) == 0 )
			{			
				const char* name = element->Attribute( "name" );
				SetName( name );

				const char* type = element->Attribute( "type" );

				if ( strcmp( type, "POINT" ) == 0 )
				{
					mLightType = LIGHT_TYPE_POINT;
				}
				else if ( strcmp( type, "SPOT" ) == 0 )
				{
					mLightType = LIGHT_TYPE_SPOT;

                    double value = element->DoubleAttribute( "angle" );
					mFoVAngle = static_cast<float>( value );
                    value = element->DoubleAttribute( "znear" );
					mZNear = static_cast<float>( value );
                    value = element->DoubleAttribute( "zfar" );
					mZFar = static_cast<float>( value );
				}
				else if ( strcmp( type, "SUN" ) == 0 )
				{
					mLightType = LIGHT_TYPE_DIRECTIONAL;
				}

				LoadLight( currentNode->FirstChild() );
			}		
		}
	}
}

//===========================================================================

void Light::LoadLight( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node; currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "color" ) == 0 )
			{
				const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

				std::vector<std::string> tokens;
				tokens.reserve( 3 );
				System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

				mColor[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				mColor[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				mColor[2] = static_cast<float>( atof( tokens[2].c_str() ) );
				mColor[3] = 1.0f;
			}			
		}
	}
}