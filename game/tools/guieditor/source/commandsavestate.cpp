//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "commandsavestate.hpp"
#include "common/game/guieditor/guieditorapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guistatic.hpp"
#include "common/engine/gui/guislider.hpp"
#include "common/engine/gui/guisprite.hpp"
#include "guieditor.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

CommandSaveState::CommandSaveState( GuiEditorApp* guiEditorApp )
	: Command( guiEditorApp, COMMAND_SAVE_STATE )
{
	mControl = NULL;
}

//===========================================================================

CommandSaveState::~CommandSaveState()
{
	DELETE_PTR( mControl );
}

//===========================================================================

void CommandSaveState::Execute()
{

}

//===========================================================================

void CommandSaveState::Undo()
{
	GuiControl* currentControl = mGuiEditorApp->GetGuiControl( mControl->GetID() );
	if ( currentControl )
	{
		currentControl->Copy( mControl );
		DELETE_PTR( mControl );
	}
}

//===========================================================================

void CommandSaveState::SaveState( GuiControl* control )
{
	switch ( control->GetType() )
	{
	case CONTROL_TYPE_STATIC:
		mControl = NEW_PTR("Static Control") GuiStatic( control->GetParent() );
		break;
	case CONTROL_TYPE_BUTTON:
		mControl = NEW_PTR("Button Control") GuiButton( control->GetParent() );
		break;
	case CONTROL_TYPE_SLIDER:
		mControl = NEW_PTR("Slider Control") GuiSlider( control->GetParent() );
		break;
	case CONTROL_TYPE_SPRITE:
		mControl = NEW_PTR("Sprite Control") GuiSprite( control->GetParent() );
		break;
	}

	mControl->Copy( control );
}