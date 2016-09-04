//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/pixelshader.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include <string.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PixelShader::PixelShader() 
{    
}

//============================================================================

PixelShader::~PixelShader()
{
	int size = mSamplerIndexPairs.GetSize();
	for ( int i = 0; i < size; ++i )
	{
		DELETE_PTR( mSamplerIndexPairs[i] );
	}
}

//============================================================================

int PixelShader::GetSamplerIndex( const char* name )
{
	int size = mSamplerIndexPairs.GetSize();
	for ( int i = 0; i < size; ++i )
	{
		if ( strcmp( mSamplerIndexPairs[i]->mSamplerName, name ) == 0 )
		{
			return mSamplerIndexPairs[i]->mSamplerIndex;
		}
	}

	return SHADER_HANDLE_ERROR;
}

//============================================================================

void PixelShader::AddSamplerIndexPair( const char* name, int index )
{
	SamplerIndexPair* newPair = NEW_PTR( "Sampler Index Pair" ) SamplerIndexPair;
	System::StringCopy( newPair->mSamplerName, 32, name );
	newPair->mSamplerIndex = index;

	mSamplerIndexPairs.PushBack( newPair );
}