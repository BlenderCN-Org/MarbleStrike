//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/shaderparametertexture.hpp"
#include "common/engine/render/shaderprogram.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ShaderParameterTexture::ShaderParameterTexture( int id )
	:	ShaderParameter( id )
{
	mData = NULL;
	System::StringCopy( mFilename, 32, "" );
}

//============================================================================

ShaderParameterTexture::~ShaderParameterTexture()
{

}

//============================================================================

void ShaderParameterTexture::SetParameter( 
	RenderStateShadowing* /*renderStateShadowing*/, 
	EffectPass* /*effectPass*/,
	void* /*context*/
	)
{
}

//============================================================================

int ShaderParameterTexture::UpdateParameter( ResourceHandle<ShaderProgram> /*shaderProgram*/ )
{
	//TextureParameter handle = shaderProgram->GetTextureParamHandle( GetName() );
	//SetHandle( handle );

	//return handle.mHandle;
	return 0;
}

//============================================================================

bool ShaderParameterTexture::operator != ( const ShaderParameter &other )
{
	const ShaderParameterTexture* otherData = reinterpret_cast< const ShaderParameterTexture*>( &other );
	if ( mData != otherData->mData )
	{
		return true;
	}
	else
	{
		return false;
	}
}