#ifndef GAME_HPP
#define GAME_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Game
{
public:
	
	Game();
	virtual ~Game();
	
	virtual void Initialize() = 0;
	virtual void Update( float elapsedTime ) = 0;
	
	virtual void UpdateVariable( float /*elapsedTime*/ )
	{
	}

	virtual void RenderUpdate() = 0;
    virtual void Show()
    {
    }

    virtual void Hide()
    {
    }

	virtual void SetVolume()
	{
	}

	virtual void OnBackButton()
	{
	}

	virtual bool IsExitable()
	{
		return false;
	}

private:
	
};

#endif

