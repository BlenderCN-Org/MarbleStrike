//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "commandsavestate.hpp"
#include "common/game/particleeditor/particleeditorapp.hpp"
#include "particleeditor.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

CommandSaveState::CommandSaveState( ParticleEditorApp* particleEditorApp )
	: Command( particleEditorApp, COMMAND_SAVE_STATE )
{
}

//===========================================================================

CommandSaveState::~CommandSaveState()
{
}

//===========================================================================

void CommandSaveState::Execute()
{

}

//===========================================================================

void CommandSaveState::Undo()
{
}