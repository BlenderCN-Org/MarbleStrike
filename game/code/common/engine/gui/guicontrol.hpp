#ifndef GUI_CONTROL_HPP
#define GUI_CONTROL_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/animation/animationcontroller.hpp"
#include "common/engine/system/systemdefines.hpp"
#include "common/engine/math/vector2.hpp"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class GuiScreen;
class GuiInput;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum ControlType
{
	CONTROL_TYPE_STATIC,
	CONTROL_TYPE_BUTTON,
	CONTROL_TYPE_CHECKBOX,
	CONTROL_TYPE_SLIDER,
	CONTROL_TYPE_RADIOBUTTON,
	CONTROL_TYPE_SPRITE
};

enum ControlState
{
	CONTROL_STATE_NORMAL = 0,
	CONTROL_STATE_DISABLED,
	CONTROL_STATE_HIDDEN,
	CONTROL_STATE_FOCUS,
	CONTROL_STATE_MOUSEOVER,
	CONTROL_STATE_PRESSED,
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct ControlKeyFrameData
{	
	ControlKeyFrameData()
	{
		mAlpha = 1.0f;
	}

	Math::Vector2Base<int> mPosition;
	Math::Vector2Base<int> mSize;
	float mAlpha;
};

struct AnimationSequence
{
    char mAnimationSequenceName[64];
    AnimationController<ControlKeyFrameData> mAnimationController;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiControl
{
public:
	GuiControl( GuiScreen* parentScreen );
	virtual ~GuiControl();
	
	virtual void Copy( GuiControl* control );
	void SetName( const char* name );

	const char* GetName()
	{
		return mName;
	}

	virtual void Render( const Math::Matrix44& /*orthoMatrix*/, bool /*drawBoundingBox = false*/ )
	{		
	}

	void SetFramesPerSecond( float fps );
	static ControlKeyFrameData LinearInterpolation( const ControlKeyFrameData &data0, const ControlKeyFrameData &data1, float t );
	virtual void Update( float elapsedTime );
	void PlayAnimation();
	void LoopAnimation();

	virtual bool HandleMouse( GuiInput* /*input*/ )
	{
		return false;
	}

	virtual bool ContainsPoint( int x, int y );

	void OnMouseEnter()
	{
		mMouseOver = true;
	}
	
	void OnMouseLeave()
	{
		mMouseOver = false;
	}

	bool GetVisible()
	{
		return mVisible;
	}

	void SetVisible( bool value )
	{
		mVisible = value;
	}

	void OnFocusIn()
	{
		mHasFocus = true;
	}

	void OnFocusOut()
	{
		mHasFocus = false;
	}

	ControlType GetType()
	{
		return mControlType;
	}

	void SetType( ControlType type )
	{
		mControlType = type;
	}

	int GetX();
	int GetY();
	void SetX( int x );
	void SetY( int y );
	void SetPosition( int x, int y );
	int GetWidth();
	int GetHeight();	
	void SetWidth( int width );
	void SetHeight( int height );
	void SetSize( int width, int height );
	float GetAlpha();	
	void SetAlpha( float alpha );

	int GetID()
	{
		return mID;
	}

	void SetID( int id )
	{
		mID = id;
	}

	bool GetEnabled()
	{
		return mEnabled;
	}

	void SetEnabled( bool enable )
	{
		mEnabled = enable;
	}

	GuiScreen* GetParent()
	{
		return mParentScreen;
	}

	virtual bool CanHaveFocus()
	{
		return false;
	}

	void ClearKeyFrames();
	void InsertKeyFrame( ControlKeyFrameData data, int frame );
	void DeleteKeyFrame( int frame );
	void GoToKeyFrame( int frame );
	int GetPreviousFrame( int frame );
	int GetNextFrame( int frame );
	int GetNumKeyFrames();	
	AnimationController<ControlKeyFrameData>::KeyFrameData* GetKeyFrameDataByIndex( int index );

    void SetCurrentAnimationSequence( int index );
    void AddAnimationSequence( const char* name );
    void DeleteAnimationSequence( int index );
    int FindAnimationSequenceIndexByName( const char* name );        
    int GetNumAnimationSequences();
    AnimationSequence* GetAnimationSquence( int index );    
    
    const ControlKeyFrameData* GetControlKeyFrameData() const
    {
        return &mCurrentControlKeyFrameData;
    }

    AnimationState GetAnimationState();
	
protected:

	char mName[64];
	bool mVisible;
	bool mEnabled;
	bool mMouseOver;
	bool mHasFocus;
	int mCurrentFrame;
    int mCurrentAnimationSequenceIndex;
    std::vector<AnimationSequence*> mAnimationSequences;
    ControlKeyFrameData mCurrentControlKeyFrameData;
	int mID;
	ControlType mControlType;
	GuiScreen* mParentScreen;
};

#endif
