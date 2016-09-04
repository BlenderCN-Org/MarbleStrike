#ifndef GUI_STATIC_HPP
#define GUI_STATIC_HPP

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

#define MAX_STATIC_TEXT 256

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Texture;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiStatic : public GuiControl
{
public:

	GuiStatic( GuiScreen* parentScreen );
	virtual ~GuiStatic();
	
	virtual void Update( float elapsedTime );
	virtual void Copy( GuiControl* control );

	virtual void Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox = false );

	void SetText( const char* text );
	const char* GetText();

	void SetUVSize( Math::Vector2 size )
	{
		mUVSize = size;
	}

	Math::Vector2 GetUVSize()
	{
		return mUVSize;
	}

	void SetUVPosition( Math::Vector2 position )
	{
		mUVPosition = position;
	}

	Math::Vector2 GetUVPosition()
	{
		return mUVPosition;
	}

	int GetFontIndex()
	{
		return mFontIndex;
	}

	void SetFontIndex( int index )
	{
		mFontIndex = index;
	}

	void SetTexture( Texture* texture )
	{
		mTexture = texture;
	}

	Texture* GetTexture()
	{
		return mTexture;
	}

	Math::Vector4 GetColor()
	{
		return mColor;
	}

	void SetColor( Math::Vector4 color )
	{
		mColor = color;
		mCurrentColor = color;
	}

	Math::Vector4 GetFontColor()
	{
		return mFontColor;
	}

	void SetFontColor( Math::Vector4 color )
	{
		mFontColor = color;
		mCurrentFontColor = color;
	}

	float GetFontSize()
	{
		return mFontSize;
	}

	void SetFontSize( float size )
	{
		mFontSize = size;
	}

	TextHorizontalAlignment GetTextHorizontalAlignment()
	{
		return mTextHorizontalAlignment;
	}

	void SetTextHorizontalAlignment( TextHorizontalAlignment alignment )
	{
		mTextHorizontalAlignment = alignment;
	}

	TextVerticalAlignment GetTextVerticalAlignment()
	{
		return mTextVerticalAlignment;
	}

	void SetTextVerticalAlignment( TextVerticalAlignment alignment )
	{
		mTextVerticalAlignment = alignment;
	}	

    void SetDisabledAlpha( float alpha )
    {
        mDisabledAlpha = alpha;
    }

protected:
	
	char mText[MAX_STATIC_TEXT];

	Math::Vector2 mUVSize;
	Math::Vector2 mUVPosition;
	int mFontIndex;
	Texture* mTexture;
	Math::Vector4 mColor;
	Math::Vector4 mCurrentColor;
	Math::Vector4 mFontColor;
	Math::Vector4 mCurrentFontColor;
	float mFontSize;
	TextHorizontalAlignment mTextHorizontalAlignment;
	TextVerticalAlignment mTextVerticalAlignment;
    float mDisabledAlpha;
};

#endif
