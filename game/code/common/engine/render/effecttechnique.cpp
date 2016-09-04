//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

EffectTechnique::EffectTechnique() 
{    
	System::StringCopy( mName, 32, "" );
	mPasses.SetSize( 3 );
}

//============================================================================

EffectTechnique::~EffectTechnique()
{
}

//============================================================================

EffectPass* EffectTechnique::CreatePass()
{
	EffectPass* newPass = NEW_PTR( "Effect Technique" ) EffectPass;
	mPasses.PushBack( newPass );
	return newPass;
}

//============================================================================

void EffectTechnique::Clear()
{
	int numPasses = mPasses.GetSize();
	for ( int i = 0; i < numPasses; ++i )
	{
		DELETE_PTR( mPasses[i] );
	}
}

//============================================================================

EffectPass* EffectTechnique::GetPass( int index )
{
	return mPasses[index];
}

//============================================================================

EffectPass* EffectTechnique::GetPass( const char* name )
{
	int numPasses = mPasses.GetSize();
	for ( int i = 0; i < numPasses; ++i )
	{
		const char* passName = mPasses[i]->GetName();
		if ( strcmp( passName, name ) == 0 )
		{
			return mPasses[i];
		}
	}
	
	Assert( false, "" );
	return NULL;
}

//============================================================================

void EffectTechnique::SetName( const char* name )
{
	System::StringCopy( mName, 32, name );
}