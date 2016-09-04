//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowlerstartgui.hpp"
#include "common/engine/database/database.hpp"
#include <string>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerStartGui::BowlerStartGui()
	:	GuiScreen( BOWLER_GUI_START )
{	
}

//===========================================================================

BowlerStartGui::~BowlerStartGui()
{
}

//===========================================================================

void BowlerStartGui::Initialize()
{
    const char* resolutionString = GameApp::GetResolutionString();
    std::string filename = "game\\bowler\\gui\\bowlerstartgui";  
    filename += resolutionString;
    filename += ".xml";
    Load( filename.c_str() );
	
	PlayAnimation();
	LoopAnimation();
	
}

//===========================================================================

void BowlerStartGui::OnEvent( EventType /*event*/, int /*controlID*/, GuiControl* /*control*/ )
{
	
}