#ifndef EFFECT_MANAGER_HPP
#define EFFECT_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/array.hpp"
#include "common/engine/render/effectpass.hpp"
#include "tinyxml2.h"
#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Effect;
class VertexDeclaration;
class EffectTechnique;
class EffectPass;
class VertexShader;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class EffectManager
{
public:

	EffectManager();
	~EffectManager();	

	Effect* CreateEffect();
	void Clear();
	Effect* GetEffect( const char* name );
	Effect* Load( const char* filename, const char* name, bool loadXML = true );
	void Reload();

private:

	void LoadXML( 
		tinyxml2::XMLNode* node, 
		Effect* newEffect
		);

	void LoadUsageXML( 
		tinyxml2::XMLNode* node, 
		VertexDeclaration* vertexDeclaration 
		);

	void LoadTechniqueXML( 
		tinyxml2::XMLNode* node, 
		EffectTechnique* newTechnique, 
		Effect* newEffect 
		);

	void LoadPassXML( 
		tinyxml2::XMLNode* node, 
		EffectPass* newPass, 
		ResourceHandle<VertexShader> &vertexShader, 
		ResourceHandle<PixelShader> &pixelShader 
		);

	void WriteInteger( FILE* file, int myValue, bool isBinary = true );
	void WriteString( FILE* file, const char* myString, bool isBinary = true );	
	int FindIndex( const char* nameArray[], int maxNum, const char* value );

	std::list<Effect*> mEffects;
};

#endif
