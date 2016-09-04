#ifndef GUI_BUTTON_HPP
#define GUI_BUTTON_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guistatic.hpp"
#include "common/engine/math/vector4.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class GuiScreen;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiButton : public GuiStatic
{
public:

    GuiButton( GuiScreen* parentScreen );
    virtual ~GuiButton();

    void Copy( GuiControl* control );	
    virtual void Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox = false );	

    virtual bool HandleMouse( GuiInput* input );
    virtual bool ContainsPoint( int x, int y );

    virtual bool CanHaveFocus()
    {
        return ( mVisible && mEnabled );
    }

    void SetOnPressUVSize( Math::Vector2 size )
    {
        mOnPressUVSize = size;
    }

    Math::Vector2 GetOnPressUVSize()
    {
        return mOnPressUVSize;
    }

    void SetOnPressUVPosition( Math::Vector2 position )
    {
        mOnPressUVPosition = position;
    }

    Math::Vector2 GetOnPressUVPosition()
    {
        return mOnPressUVPosition;
    }

    void SetOnPressTexture( Texture* texture )
    {
        mOnPressTexture = texture;
    }

    Texture* GetOnPressTexture()
    {
        return mOnPressTexture;
    }

    void SetDirty( bool value )
    {
        mIsDirty = value;
    }

protected:

    bool mPressed;	 
    int mTouchID;
    bool mIsDirty;

    Math::Vector2 mOnPressUVSize;
    Math::Vector2 mOnPressUVPosition;	
    Texture* mOnPressTexture;
};

#endif
