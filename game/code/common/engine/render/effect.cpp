//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/effect.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/assert.hpp"
#include <string.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Effect::Effect() 
{
	System::StringCopy( mName, 32, "" );
	mTechniques.SetSize( 5 );
}

//============================================================================

Effect::~Effect()
{	
}

//============================================================================

EffectTechnique* Effect::CreateTechnique()
{
	EffectTechnique* newTechnique = NEW_PTR( "Effect Technique" ) EffectTechnique;
	mTechniques.PushBack( newTechnique );
	return newTechnique;
}

//============================================================================

VertexDeclaration* Effect::CreateVertexDeclaration()
{
	mVertexDeclaration = Renderer::Get()->GetRenderFactory()->CreateVertexDeclaration();
	return mVertexDeclaration;
}

//============================================================================

void Effect::Clear()
{
	int numTechniques = mTechniques.GetSize();
	for ( int i = 0; i < numTechniques; ++i )
	{
		mTechniques[i]->Clear();
		DELETE_PTR( mTechniques[i] );
	}

	DELETE_PTR( mVertexDeclaration );
}

//============================================================================

EffectTechnique* Effect::GetTechnique( int index )
{
	return mTechniques[index];
}

//============================================================================

EffectTechnique* Effect::GetTechnique( const char* name )
{
	int numTechniques = mTechniques.GetSize();
	for ( int i = 0; i < numTechniques; ++i )
	{
		const char* techniqueName = mTechniques[i]->GetName();
		if ( strcmp( techniqueName, name ) == 0 )
		{
			return mTechniques[i];
		}
	}
	
	Assert( false, "" );
	return NULL;
}

//============================================================================

int Effect::GetTechniqueIndex( const char* name )
{
	int numTechniques = mTechniques.GetSize();
	for ( int i = 0; i < numTechniques; ++i )
	{
		const char* techniqueName = mTechniques[i]->GetName();
		if ( strcmp( techniqueName, name ) == 0 )
		{
			return i;
		}
	}

	Assert( false, "" );
	return 0;
}

//============================================================================

VertexDeclaration* Effect::GetVertexDeclaration()
{
	Assert( mVertexDeclaration != NULL, "" );
	return mVertexDeclaration;
}

//============================================================================

void Effect::SetName( const char* name )
{
	System::StringCopy( mName, 32, name );
}

//============================================================================

void Effect::Reload()
{
	int numTechniques = GetNumTechniques();
	for ( int i = 0; i < numTechniques; ++i )
	{
		int numPasses = mTechniques[i]->GetNumPasses();
		for ( int pass = 0; pass < numPasses; ++pass )
		{
			EffectPass* effectPass = mTechniques[i]->GetPass( pass );
			effectPass->Reload();
		}
	}
}