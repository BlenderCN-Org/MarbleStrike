#ifndef EFFECT_HPP
#define EFFECT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedarray.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class EffectTechnique;
class VertexDeclaration;

///////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Effect
{
public:

	Effect();
	~Effect();

	EffectTechnique* CreateTechnique();
	VertexDeclaration* CreateVertexDeclaration();
	void Clear();

	int GetNumTechniques()
	{
		return mTechniques.GetSize();
	}

	EffectTechnique* GetTechnique( int index );
	EffectTechnique* GetTechnique( const char* name );
	int GetTechniqueIndex( const char* name );

	VertexDeclaration* GetVertexDeclaration();

	const char* GetName()
	{
		return mName;
	}

	void SetName( const char* name );

	void Reload();
	
private:

	char mName[32];
	FixedArray<EffectTechnique*> mTechniques;
	VertexDeclaration* mVertexDeclaration;
};

#endif
