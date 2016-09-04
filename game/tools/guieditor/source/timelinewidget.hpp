#ifndef TIMELINE_WIDGET_HPP
#define TIMELINE_WIDGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <wx/wx.h>
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class GuiEditor;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class TimeLineWidget : public wxPanel
{
public:
	TimeLineWidget(
		wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr
		);

	TimeLineWidget( wxPanel* parent, int id );

	void SetGuiEditor( GuiEditor* guiEditor )
	{
		mGuiEditor = guiEditor;
	}

	void AddKeyFrame( int keyFrame );
	void ClearKeyFrames();
	void SetAnimationLength( int length );

	void OnSize( wxSizeEvent &event );
	void OnPaint( wxPaintEvent &event );
	void OnLeftDown( wxMouseEvent &event );
	void OnLeftUp( wxMouseEvent &event );
	void OnMove( wxMouseEvent &event );
	void OnRightDown( wxMouseEvent &event );	

	int GetCurrentFrame() const;
	void SetCurrentFrame( int frame );

private:

	void SetCurrentFrameByPos( float xPos );

	int FindClosestKeyFrame( float value );

	std::vector<float> mKeyFrames;	
	int mSelectedIndex;
	int mAnimationLength;
	int mCurrentFrame;
	bool mLeftMouseButtonDown;

	GuiEditor* mGuiEditor;
};

#endif
