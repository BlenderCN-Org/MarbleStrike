#ifndef BOWLER_OPTIONS_GUI_HPP
#define BOWLER_OPTIONS_GUI_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/animation/animationcontroller.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
	class AudioManager;
}

class BowlerDatabase;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerOptionsGui : public GuiScreen
{
public:

	enum GuiButtonID
	{
		GUI_BUTTON_BACK,
		GUI_BUTTON_RESET_DATA,
		GUI_BUTTON_SWAP_CONTROL,
		GUI_BUTTON_CREDITS,
		GUI_BUTTON_CREDITS_BACK,
		GUI_BUTTON_RESET_YES,
		GUI_BUTTON_RESET_NO,
		GUI_BUTTON_SWAP_LEFT,
		GUI_BUTTON_SWAP_RIGHT
	};

	enum OptionGuiState
	{
		OPTION_GUI_STATE_MAIN,
		OPTION_GUI_STATE_RESET,
		OPTION_GUI_STATE_CONTROLS,
		OPTION_GUI_STATE_CREDITS
	};

	BowlerOptionsGui(Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase);
	~BowlerOptionsGui();

	void OnMessageCallback( void* userData );
	void Initialize();
	void Update( float elapsedTime );
	void OnEvent( EventType event, int controlID, GuiControl* control );

protected:

	void ResetDialogDialog( bool show );
	void SwapControlDialog( bool show );
	void CreditsControlDialog( bool show );
	void OptionsGui( bool show );
	void UpdateControlVisuals();
	void CloseResetDialog();
	void CloseControlDialog();

	OptionGuiState mOptionGuiState;

	Audio::AudioManager* mAudioManager;
	BowlerDatabase* mBowlerDatabase;

	GuiButton* mBackButton;
	GuiStatic* mOptionsTitle;
	GuiButton* mResetDataButton;
	GuiButton* mSwapControlButton;
	GuiButton* mCreditsButton;

	GuiStatic* mResetTitle;
	GuiButton* mYesButton;
	GuiButton* mNoButton;

	GuiStatic* mControlTitle;
	GuiButton* mLeftButton;
	GuiButton* mRightButton;
	Math::Vector4 mLeftButtonColor;
	Math::Vector4 mRightButtonColor;

	AnimationController<float> mAlphaAnimationController;
	GuiStatic* mCreditTitle;
	std::vector<GuiStatic*> mCredits[2];
	GuiButton* mCreditBackButton;
	int mCreditIndex;

};

#endif
