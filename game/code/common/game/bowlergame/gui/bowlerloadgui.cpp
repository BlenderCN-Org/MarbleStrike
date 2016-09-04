//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guiinput.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/system/timer.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowlerloadgui.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"
#include <string>

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerLoadGui::BowlerLoadGui()     
    :   GuiScreen( BOWLER_GUI_LOAD )
{
}

//===========================================================================

BowlerLoadGui::~BowlerLoadGui()
{
}

//===========================================================================

void BowlerLoadGui::Initialize()
{
    const char* resolutionString = GameApp::GetResolutionString();
    std::string filename = "game\\bowler\\gui\\bowlerloadgui";  
    filename += resolutionString;
    filename += ".xml";
    Load( filename.c_str() );
}