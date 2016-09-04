#ifndef GUI_EDITOR_HPP
#define GUI_EDITOR_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "guieditorgui.h"
#include "common/engine/gui/guicontrol.hpp"
#include "common/game/guieditor/guieditorapp.hpp"
#include <vector>
#include <string>
#include <stack>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class GuiEditorApp;
class GuiControl;
class Command;
class Texture;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum PanelProperties
{
	PANEL_PROPERTIES_STATIC,
	PANEL_PROPERTIES_BUTTON,
	PANEL_PROPERTIES_SLIDER,
	PANEL_PROPERTIES_SPRITE,
	PANEL_PROPERTIES_NONE,
	PANEL_PROPERTIES_MAX
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class SceneGraphNode : public wxTreeItemData
{
public:

	SceneGraphNode( ControlType type, int id )
		:	mNodeType( type )
		,	mID( id )
	{

	}

	int mID;
	ControlType mNodeType;
};

//===========================================================================

class GuiEditor : public GuiEditorGui
{
public:
	GuiEditor( const wxString& title );
	~GuiEditor();

	void OnQuit( wxCommandEvent &event );

	void OnMenuSelectionRenderer( wxCommandEvent& event );
	void OnMenuSelectionEditor( wxCommandEvent& event );
	void OnMenuSelectionNew( wxCommandEvent& event );
	void OnMenuSelectionOpen( wxCommandEvent& event );
	void OnMenuSelectionSaveAs( wxCommandEvent& event );
	void OnMenuSelectionSave( wxCommandEvent& event );
	void OnMenuSelectionPortrait( wxCommandEvent& event );
	void OnMenuSelectionLandscape( wxCommandEvent& event );	
	void OnMenuSelectionEdit( wxCommandEvent& event );	
	void OnMenuSelectionAnimation( wxCommandEvent& event );	

	void OnTextEnter( wxCommandEvent& event );
	void OnKillFocus( wxFocusEvent& event );
	void OnScroll( wxScrollEvent& event );
	void OnScrollChanged(  wxScrollEvent& event );
	void OnChoice( wxCommandEvent& event );
	void OnColorChanged( wxColourPickerEvent& event );
	void OnButtonClick(  wxCommandEvent& event );
	void OnFileChanged( wxFileDirPickerEvent& event );

	void OnKeyDownSceneGraph( wxKeyEvent& event );
	void OnLeftDownTree( wxMouseEvent& event );
	void OnTreeItemMenu( wxTreeEvent& event );
	void OnRightDownTree( wxMouseEvent& event );	
	void OnSizeGraphicsPanel( wxSizeEvent& event );
	void OnLeftDownGraphicsPanel( wxMouseEvent& event );
	void OnLeftUpGraphicsPanel( wxMouseEvent& event );
    void OnRightDownGraphicsPanel( wxMouseEvent& event );
	void OnRightUpGraphicsPanel( wxMouseEvent& event );
	void OnMotionGraphicsPanel( wxMouseEvent& event );
	void OnKeyDownGraphicsPanel( wxKeyEvent& event );
	void OnKeyUpGraphicsPanel( wxKeyEvent& event );
	void OnRadioBoxTransform( wxCommandEvent& event );	
	void OnButtonClickLayout( wxCommandEvent& event );
	void OnTextEnterAnimationFrame( wxCommandEvent& event );
	void OnCharAnimationFrame( wxKeyEvent& event );
	void UpdateAnimationFrame( int frame );	
	void OnButtonClickAnimation( wxCommandEvent& event );
	void OnColourChangedBoundingBox( wxColourPickerEvent& event );
	void OnColourChangedBackground( wxColourPickerEvent& event );
	void OnButtonClickZoom( wxCommandEvent& event );

	GuiEditorApp* GetGuiEditorApp()
	{
		return mGuiEditorApp;
	}

private:

	void SelectItem( wxTreeItemId &item );

	enum TransformSelection
	{
		TRANSFORM_TRANSLATE_X,
		TRANSFORM_TRANSLATE_Y,
		TRANSFORM_TRANSLATE_XY
	};

	GuiControl* GetCurrentControl();
	void UpdateAnimationPanel( GuiControl* guiControl );
	void AddKeyFrame();
	void DeleteKeyFrame();
	void PlayAnimation();
	void StopAnimation();
	void UpdateScenegraphTree();
	void UpdateProperties( GuiControl* guiControl );
	void TogglePanels( GuiControl* guiControl );

	void OnTextEnterStatic( wxCommandEvent& event );
	void OnKillFocusStatic( wxFocusEvent& event );
	void OnScrollStatic( wxScrollEvent& event );
	void OnScrollChangedStatic( wxScrollEvent& event );
	void OnChoiceStatic( wxCommandEvent& event );
	void OnColorChangedStatic( wxColourPickerEvent& event );
	void OnButtonClickStatic( wxCommandEvent& event );
	void OnFileChangedStatic( wxFileDirPickerEvent& event );
	void UpdatePropertiesStatic( GuiStatic* guiStatic );

	void OnTextEnterButton( wxCommandEvent& event );
	void OnKillFocusButton();
	void OnScrollButton( wxScrollEvent& event );
	void OnScrollChangedButton( wxScrollEvent& event );
	void OnChoiceButton( wxCommandEvent& event );
	void OnColorChangedButton( wxColourPickerEvent& event );
	void OnButtonClickButton( wxCommandEvent& event );
	void OnFileChangedButton( wxFileDirPickerEvent& event );
	void UpdatePropertiesButton( GuiButton* guiButton );

	void OnTextEnterSlider( wxCommandEvent& event );
	void OnKillFocusSlider();	
	void OnButtonClickSlider( wxCommandEvent& event );
	void OnFileChangedSlider( wxFileDirPickerEvent& event );
	void UpdatePropertiesSlider( GuiSlider* guiSlider );

	void OnTextEnterSprite( wxCommandEvent& event );
	void OnKillFocusSprite();
	void OnScrollSprite( wxScrollEvent& event );
	void OnScrollChangedSprite( wxScrollEvent& event );
	void OnColorChangedSprite( wxColourPickerEvent& event );
	void OnButtonClickSprite( wxCommandEvent& event );
	void OnFileChangedSprite( wxFileDirPickerEvent& event );
	void UpdatePropertiesSprite( GuiSprite* guiSprite );

    void OnChoiceAnimationSequence( wxCommandEvent& event );
    void OnTextEnterAnimationSequence( wxCommandEvent& event );
    void OnButtonAnimationSequence( wxCommandEvent& event );

	void TogglePanel( PanelProperties panelType );	
	void SaveData( bool saveAs );

    Texture* GetFilePickTexture( wxFileDirPickerEvent& event, GuiControl* guiControl );
	wxString GetFilePickPath( wxFileDirPickerEvent& event );
    wxTreeItemId GetTreeCurrentSelection();

	GuiEditorApp* mGuiEditorApp;
	wxString mCurrentSavePath;	
	TransformConstraint mTransformType;
	std::stack<Command*> mCommandList;
	wxScrolledWindow* mPropertyPanels[PANEL_PROPERTIES_MAX];
	bool mIsFiledOpened;
	int mCurrentCopyControlID;
	bool mIsScrollingSlider;

    int mCurrentScrollX;
    int mCurrentScrollY;
    bool mLeftButtonDown;
    bool mRightButtonDown;

	float mScreenWidth;
	float mScreenHeight;
	float mZoomScale;
};

#endif
