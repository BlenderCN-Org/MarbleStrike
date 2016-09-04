#ifndef TEXTURE_UV_DIALOG_HPP
#define TEXTURE_UV_DIALOG_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "textureuvdialoggui.h"
#include "common/engine/math/vector2.hpp"
#include <wx/bitmap.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum SelectMode
{
	SELECT_CORNER1,
	SELECT_CORNER2,
	SELECT_CORNER3,
	SELECT_CORNER4,
	SELECT_MOVE,
	SELECT_NONE
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class TextureUVDialog : public TextureUVDialogGui
{
public:
	TextureUVDialog( wxWindow* parent );
	~TextureUVDialog();

	void SetTextureImage( wxString path );
	Math::Vector2 GetUVSize();
	Math::Vector2 GetUVPosition();

	void OnInitDialog( wxInitDialogEvent& event );
	void OnEraseBackground( wxEraseEvent& event );
	void OnPaint( wxPaintEvent& event );

	void OnButtonClickOK( wxCommandEvent& event );
	void OnButtonClickCancel( wxCommandEvent& event );	
	void OnTextEnter( wxCommandEvent& event );

	void OnLeftDownImagePanel( wxMouseEvent& event );
	void OnLeftUpImagePanel( wxMouseEvent& event );
	void OnMotionImagePanel( wxMouseEvent& event );

	void OnKeyDownImagePanel( wxKeyEvent& event );
	void OnKeyUpImagePanel( wxKeyEvent& event );

	void OnButtonClickMagnified( wxCommandEvent& event );
	void OnButtonClickMinified( wxCommandEvent& event );

	void SetCurrentUVSize( Math::Vector2 size )
	{
		mCurrentUVSize = size;
	}

	void SetCurrentUVPosition( Math::Vector2 position )
	{
		mCurrentUVPosition = position;
	}

private:

	void UpdateFields();

	wxBitmap mImage;
		
	SelectMode mSelectMode;

	Math::Vector2 mCurrentUVPosition;
	Math::Vector2 mCurrentUVSize;

	Math::Vector2 mOnDownPosition;
	Math::Vector2 mMotionData;
	bool mOnDown;
	bool mSelected;
	bool mOnPanelDown;
	Math::Vector2 mPanelPosition;
	int mScale;

	float mScaleHandleSize;
};

#endif
