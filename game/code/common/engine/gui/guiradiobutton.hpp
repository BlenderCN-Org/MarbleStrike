#ifndef GUI_RADIOBUTTON_HPP
#define GUI_RADIOBUTTON_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guicheckbox.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class GuiScreen;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiRadioButton : public GuiCheckBox
{
public:
	GuiRadioButton( GuiScreen* parentScreen );    
	virtual ~GuiRadioButton();

	void SetButtonGroup( int buttonGroup )
	{
		mButtonGroup = buttonGroup;
	}

	int GetButtonGroup()
	{
		return mButtonGroup;
	}

private:

	int mButtonGroup;

};

#endif