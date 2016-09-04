#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////


#include "common/engine/render/texture.hpp"
#include <string>
#include <map>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Texture;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class TextureManager
{
public:

	TextureManager();
	~TextureManager();

	Texture* GetTexture( const char* textureName, TextureType texturetype = TEXTURE_TYPE_2D, bool returnNULL = false );
	void RemoveTexture( const char* name );
	
private:

	struct TextureHandle
	{
		int mReference;
		Texture* mTexture;
	};

	void AddTexture( const char* name, TextureHandle textureHandle );

	typedef std::map< std::string, TextureHandle > TextureHandleMap;
	TextureHandleMap mTextures;
	
};

#endif
