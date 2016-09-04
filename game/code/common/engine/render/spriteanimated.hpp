#ifndef SPRITE_ANIMATED_HPP
#define SPRITE_ANIMATED_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <vector>
#include "common/engine/math/vector2.hpp"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Texture;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class SpriteAnimated
{
public:

	struct FrameTexCoord
	{
		Math::Vector2 mTexCoords[4];
	};

public:	

	SpriteAnimated();
	~SpriteAnimated();

	void Copy( const SpriteAnimated &other );
	void Load( const char* filename );
	void Update( float elapsedTime );
	void SetFramesPerSecond( float fps );
	float GetFramesPerSecond();

	Texture* GetTexture();
	FrameTexCoord GetCurrentFrameTexCoord();
	
private:

	SpriteAnimated& operator=( const SpriteAnimated &other )
	{
		UNUSED_ALWAYS( other );
		return *this;
	}

	std::vector< FrameTexCoord > mFrameTexCoords;

	Texture* mTextureAtlas;
	float mTimeCount;
	int mCurrentFrame;	
	int mNumFrames;
	float mFramesPerSecond;
};

#endif
