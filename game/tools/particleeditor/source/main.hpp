#ifndef MAIN_HPP
#define MAIN_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"
#include <wx/wx.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class MemoryControl
{
public:
	MemoryControl()
	{
		System::Memory::Create();
	}

	~MemoryControl()
	{
		System::Memory::CheckMemoryLog();
		System::Memory::Destroy();
	}
};

static MemoryControl gMemoryControl;

//===========================================================================

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

#endif