#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/list.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/matrix44.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class GuiInput;
class GuiScreen;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_OPENED_SCREENS = 3;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GuiManager
{
public:

	GuiManager();
	virtual ~GuiManager();
	
	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;
	bool InputUpdate()
	{
		return false;
	}
	void Update( float elapsedTime );
	void RenderUpdate();
		
	GuiInput* GetGuiInput()
	{
		return mGuiInput;
	}

protected:	

	GuiInput* mGuiInput;
	GuiScreen* mOpenedScreens[MAX_OPENED_SCREENS];	
	int mNumOpenedScreens;
};

#endif

