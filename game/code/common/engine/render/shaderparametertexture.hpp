#ifndef SHADER_PARAMETER_TEXTURE_HPP
#define SHADER_PARAMETER_TEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/render/shaderparameter.hpp"
#include "common/engine/render/textureparameter.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class ShaderProgram;
class Texture;
class EffectPass;
class RenderStateShadowing;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ShaderParameterTexture : public ShaderParameter
{
public:
	ShaderParameterTexture( int id );
	~ShaderParameterTexture();
	
	void SetData( Texture** data )
	{
		mData = data;
	}

	Texture** GetData()
	{
		return mData;
	}

	void SetFilename( const char* filename )
	{
		System::StringCopy( mFilename, 32, filename );
	}

	const char* GetFilename()
	{
		return mFilename;
	}

	void SetHandle( TextureParameter handle )
	{
		mHandle = handle;
	}

	TextureParameter GetHandle()
	{
		return mHandle;
	}

	void SetParameter( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		);
	int UpdateParameter( ResourceHandle<ShaderProgram> shaderProgram );
	bool operator != ( const ShaderParameter &other );

private:

	char mFilename[32];
	Texture** mData;
	TextureParameter mHandle;
};
#endif
