//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "commandmove.hpp"
#include "common/game/guieditor/guieditorapp.hpp"
#include "guieditor.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

CommandMove::CommandMove( GuiEditorApp* guiEditorApp )
	: Command( guiEditorApp, COMMAND_MOVE )
{
}

//===========================================================================

CommandMove::~CommandMove()
{

}

//===========================================================================

void CommandMove::Execute()
{

}

//===========================================================================

void CommandMove::Undo()
{
	GuiControl* control = mGuiEditorApp->GetGuiControl( mID );
	if ( control )
	{
		control->GoToKeyFrame( mFrame );
		control->SetX( mOldPosition[0] );
		control->SetY( mOldPosition[1] );
	}
}