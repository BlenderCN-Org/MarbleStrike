//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/render/image.hpp"

//////////////////////////////////////////////////////
// GLOBAL
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

TextureManager::TextureManager()
{
	const int imageDataSize = 1 * 1 * 4;
	unsigned char imageData[imageDataSize];	
	imageData[0] = 255;
	imageData[1] = 255;
	imageData[2] = 255;
	imageData[3] = 255;
	Image emptyImage;
	emptyImage.CreateImage( imageData, Image::PF_RGBA, 1, 1, imageDataSize );
	Texture* texture = Renderer::Get()->GetRenderFactory()->CreateTexture();
	texture->CreateTextureWithImage( emptyImage );	
	TextureHandle textureHandle;
	textureHandle.mReference = 0;
	textureHandle.mTexture = texture;
	AddTexture( "emptytexture", textureHandle );

	const int randomWidth = 4;
	const int randomHeight = 4;
	const int randomSize = randomWidth * randomHeight;
	const int randomImageDataSize = randomWidth * randomHeight * 4;
	unsigned char randomImageData[randomImageDataSize];	
	for ( int i = 0; i < randomSize; ++i )
	{
		randomImageData[i*4] = static_cast<unsigned char>( Math::RandomFloat1() * 255.0f );
		randomImageData[i*4+1] = static_cast<unsigned char>( Math::RandomFloat1() * 255.0f );
		randomImageData[i*4+2] = 0;
		randomImageData[i*4+3] = 255;
	}	
	Image randomImage;
	randomImage.CreateImage( randomImageData, Image::PF_RGBA, randomWidth, randomHeight, randomImageDataSize );
	Texture* randomTexture = Renderer::Get()->GetRenderFactory()->CreateTexture();
	randomTexture->CreateTextureWithImage( randomImage );	
	TextureHandle randomTextureHandle;
	randomTextureHandle.mReference = 0;
	randomTextureHandle.mTexture = randomTexture;
	AddTexture( "randomtexture", randomTextureHandle );
}

//============================================================================

TextureManager::~TextureManager()
{
	for( TextureHandleMap::iterator iter = mTextures.begin(); iter != mTextures.end(); ++iter )
	{
		TextureHandle* textureHandle = &( ( *iter ).second );
		DELETE_PTR( textureHandle->mTexture );
	}
}

//===========================================================================

Texture* TextureManager::GetTexture( const char* textureName, TextureType texturetype, bool returnNULL )
{
	TextureHandleMap::iterator findIter = mTextures.find( textureName );
	Texture* item = NULL;

	if ( returnNULL && findIter == mTextures.end() )
	{
		return NULL;
	}

	if ( findIter == mTextures.end() )
	{
		item = Renderer::Get()->GetRenderFactory()->CreateTexture();

		switch ( texturetype )
		{
		case TEXTURE_TYPE_2D:
			item->LoadTextureImage( textureName );
			break;
		case TEXTURE_TYPE_CUBEMAP:
			item->LoadTextureCubeImage( textureName );
			break;
		}

		TextureHandle textureHandle;
		textureHandle.mReference = 1;
		textureHandle.mTexture = item;
		AddTexture( textureName, textureHandle );
	}
	else
	{
		TextureHandle* textureHandle = &( ( *findIter ).second );
		textureHandle->mReference++;
		item = textureHandle->mTexture;
	}	

	return item;	
}

//===========================================================================

void TextureManager::AddTexture( const char* name, TextureHandle textureHandle )
{
	mTextures[name] = textureHandle;	
}

//===========================================================================

void TextureManager::RemoveTexture( const char* name )
{
	TextureHandleMap::iterator findIter = mTextures.find( name );
	if ( findIter != mTextures.end() )
	{
		TextureHandle* textureHandle = &( ( *findIter ).second );
		textureHandle->mReference--;
		if ( textureHandle->mReference == 0 )
		{
			DELETE_PTR( textureHandle->mTexture );
			mTextures.erase( findIter );
		}
	}
}