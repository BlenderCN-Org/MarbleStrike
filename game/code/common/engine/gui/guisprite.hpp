#ifndef GUI_SPRITE_HPP
#define GUI_SPRITE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guicontrol.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/engine/math/vector4.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class SpriteAnimated;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiSprite : public GuiControl
{
public:

	GuiSprite( GuiScreen* parentScreen );
	virtual ~GuiSprite();
	
	virtual void Update( float elapsedTime );
	virtual void Copy( GuiControl* control );

	virtual void Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox = false );

	void LoadSprite( const char* filename );
	void Clear();
	Texture* GetTexture();
	void SetFramesPerSecond( float fps );
	float GetFramesPerSecond();

	Math::Vector4 GetColor()
	{
		return mColor;
	}

	void SetColor( Math::Vector4 color )
	{
		mColor = color;
		mCurrentColor = color;
	}

    void SetDisabledAlpha( float alpha )
    {
        mDisabledAlpha = alpha;
    }

protected:
	
	SpriteAnimated* mSpriteAnimated;
	Math::Vector4 mColor;
	Math::Vector4 mCurrentColor;
	float mDisabledAlpha;
};

#endif
