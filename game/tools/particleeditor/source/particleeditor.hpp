#ifndef PARTICLE_EDITOR_HPP
#define PARTICLE_EDITOR_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/resolutionconfigs.hpp"
#include "common/game/particleeditor/particleeditorapp.hpp"
#include "particleeditorgui.h"
#include <stack>
#include <string>
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class ParticleEditorApp;
class Command;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ParticleEditor : public ParticleEditorGui
{
public:
	ParticleEditor( const wxString& title );
	~ParticleEditor();

	void OnQuit( wxCommandEvent &event );

	void OnMenuSelectionRenderer( wxCommandEvent& event );
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
	void OnColorChanged( wxColourPickerEvent& event );
	void OnButtonClick(  wxCommandEvent& event );
	void OnFileChanged( wxFileDirPickerEvent& event );
	void OnListBox( wxCommandEvent& event );
	void OnSliderScroll( wxScrollEvent& event );

	void OnKeyDownSceneGraph( wxKeyEvent& event );
	void OnSizeGraphicsPanel( wxSizeEvent& event );
	void OnLeftDownGraphicsPanel( wxMouseEvent& event );
	void OnLeftUpGraphicsPanel( wxMouseEvent& event );
	void OnMotionGraphicsPanel( wxMouseEvent& event );
	void OnKeyDownGraphicsPanel( wxKeyEvent& event );
	void OnKeyUpGraphicsPanel( wxKeyEvent& event );
	void OnButtonClickAnimation( wxCommandEvent& event );
	
	ParticleEditorApp* GetParticleEditorApp()
	{
		return mParticleEditorApp;
	}

private:

	void UpdateMaxParticles( int value );
	void UpdateMaxParticlesUI( int value );

	void PlayAnimation();
	void StopAnimation();
	void UpdateProperties();
	void SaveData( bool saveAs );

	ParticleEditorApp* mParticleEditorApp;
	wxString mCurrentSavePath;	
	std::stack<Command*> mCommandList;
	bool mIsFiledOpened;
	int mCurrentForceDataIndex;

    ResolutionConfig mResolutionConfig;
};

#endif
