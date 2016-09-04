#ifndef GUI_EDITOR_APP_HPP
#define GUI_EDITOR_APP_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/game.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/vector4.hpp"
#include <string>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Camera;
class RenderObject;
class GuiScreen;
class GuiStatic;
class GuiButton;
class GuiSlider;
class GuiControl;
class GuiSprite;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum TransformConstraint
{
	MOVEMENT_XY,
	MOVEMENT_X,
	MOVEMENT_Y,
	TRANSFORM_ROTATE
};

enum TransformMode
{
	TRANSFORM_SELECT_CORNER1,
	TRANSFORM_SELECT_CORNER2,
	TRANSFORM_SELECT_CORNER3,
	TRANSFORM_SELECT_CORNER4,
	TRANSFORM_SELECT_MOVE,
	TRANSFORM_SELECT_NONE
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiEditorApp : public Game
{
public:

	GuiEditorApp();
	~GuiEditorApp();

	void Initialize();
	void Update( float elapsedTime );
	void RenderUpdate();
	
	void CopyGuiControl( GuiControl* guiControl );
	GuiStatic* AddGuiStatic();
	GuiButton* AddGuiButton();
	GuiSlider* AddGuiSlider();
	GuiSprite* AddGuiSprite();
	GuiStatic* GetGuiStatic( int id );
	GuiButton* GetGuiButton( int id );
	GuiSlider* GetGuiSlider( int id );
	GuiSprite* GetGuiSprite( int id );
	GuiControl* GetGuiControl( int id );
	int GetNumControls();
	GuiControl* GetGuiControlByIndex( int index );
	void MoveControlBack( int id );
	void MoveControlFront( int id );
	
	void DeleteGuiControl( int id );	

	void SetSelected( GuiControl* selectedItem );	
	GuiControl* GetSelected()
	{
		return mGuiControlSelected;
	}

	void OnLeftDown( int x, int y );
	void OnLeftUp( int x, int y );
	void OnMotion( int x, int y, TransformConstraint moveType );	
	
	void Open( const char* filename );
	void Save( const char* filename );	
	void Clear();
		
	void PlayAnimation();
	void StopAnimation();

	Math::Vector4 GetBoundingBoxColor()
	{
		return mBoundingBoxColor;
	}

	Math::Vector4 GetBoundingBoxColorSelect()
	{
		return mBoundingBoxColorSelect;
	}

	void SetBoundingBoxColor( Math::Vector4 color )
	{
		mBoundingBoxColor = color;
	}

	void SetBoundingBoxColorSelect( Math::Vector4 color )
	{
		mBoundingBoxColorSelect = color;
	}

	void SetBackgroundColor( Math::Vector4 color );
	int GetGuiWidth();
	int GetGuiHeight();

	void SetZoomScale( float scale )
	{
		mZoomScale = scale;
	}

private:	

	GuiScreen* mGuiScreen;
	RenderObject* mRenderObjectCamera;
	Camera* mViewCamera;	
	Math::Vector2Base<int> mOnDownPosition;
	Math::Vector2Base<int> mMotionData;	
	bool mOnDown;
	Math::Vector4 mBoundingBoxColor;
	Math::Vector4 mBoundingBoxColorSelect;
	TransformMode mTransformMode;
	GuiControl* mGuiControlSelected;
	float mZoomScale;
};

#endif
