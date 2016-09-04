#ifndef GUI_SLIDER_HPP
#define GUI_SLIDER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guicontrol.hpp"
#include "common/engine/math/vector2.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class GuiScreen;
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

class GuiSlider : public GuiControl
{
public:
	GuiSlider( GuiScreen* parentScreen );
	virtual ~GuiSlider();

	virtual void Copy( GuiControl* control );

	virtual void Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox = false );	

	virtual bool HandleMouse( GuiInput* input );
	virtual bool ContainsPoint( int x, int y );

	virtual bool CanHaveFocus()
	{
		return ( mVisible && mEnabled );
	}

	void SetValue( int value )
	{
		mValue = value;
	}

	void SetRange( int minValue, int maxValue )
	{
		mMin = minValue;
		mMax = maxValue;
	}

	void SetMin( int minValue )
	{
		mMin = minValue;		
	}

	void SetMax( int maxValue )
	{
		mMax = maxValue;
	}

	int GetMin()
	{
		return mMin;
	}

	int GetMax()
	{
		return mMax;
	}

	int GetValue()
	{
		return mValue;
	}

	void SetUVSizeBar( Math::Vector2 size )
	{
		mUVSizeBar = size;
	}

	void SetUVPositionBar( Math::Vector2 position )
	{
		mUVPositionBar = position;
	}

	void SetTextureBar( Texture* texture )
	{
		mTextureBar = texture;
	}

	void SetUVSizeButton( Math::Vector2 size )
	{
		mUVSizeButton = size;
	}

	void SetUVPositionButton( Math::Vector2 position )
	{
		mUVPositionButton = position;
	}

	void SetTextureButton( Texture* texture )
	{
		mTextureButton = texture;
	}

	Math::Vector2 GetUVSizeBar()
	{
		return mUVSizeBar;
	}

	Math::Vector2 GetUVPositionBar()
	{
		return mUVPositionBar;
	}

	Texture* GetTextureBar()
	{
		return mTextureBar;
	}

	Math::Vector2 GetUVSizeButton()
	{
		return mUVSizeButton;
	}

	Math::Vector2 GetUVPositionButton()
	{
		return mUVPositionButton;
	}

	Texture* GetTextureButton()
	{
		return mTextureButton;
	}

private:

	int mValue;
	int mMin;
	int mMax;
	int mStartDragX;
	int mOffsetDrag; // Drag offset from the center of the button	
	bool mPressed;
	Texture* mTextureBar;
	Texture* mTextureButton;
	Math::Vector2 mUVPositionBar;
	Math::Vector2 mUVSizeBar;
	Math::Vector2 mUVPositionButton;
	Math::Vector2 mUVSizeButton;
};

#endif
