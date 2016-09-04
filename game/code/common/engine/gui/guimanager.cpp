//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guimanager.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/renderutilities.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/gui/guiinput.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

GuiManager::GuiManager()
    :	mNumOpenedScreens( 0 )
{
    for ( int i = 0; i < MAX_OPENED_SCREENS; ++i )
    {
        mOpenedScreens[i] = NULL;
    }

	mGuiInput = NEW_PTR( "Gui Input" ) GuiInput;
}

//===========================================================================

GuiManager::~GuiManager()
{
	DELETE_PTR( mGuiInput );
}

//===========================================================================

void GuiManager::Update( float elapsedTime )
{
    if ( mNumOpenedScreens > 0 && mOpenedScreens[mNumOpenedScreens - 1] )
    {
        mOpenedScreens[mNumOpenedScreens - 1]->UpdateInput( mGuiInput );
    }

    if ( mNumOpenedScreens > 0 && mOpenedScreens[mNumOpenedScreens - 1] )
    {		
		mOpenedScreens[mNumOpenedScreens - 1]->Update( elapsedTime );
    }
}

//===========================================================================

void GuiManager::RenderUpdate()
{
	float screenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() );
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );

	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection( 
		0,
		0,
		screenWidthSize,
		screenHeightSize,
		-1,
		1
		);    
   orthoMatrix = Render::ProjectionOrientation( orthoMatrix );

    for ( int i = 0; i < mNumOpenedScreens; ++i )
    {
        mOpenedScreens[i]->Render( orthoMatrix );
	} 
}