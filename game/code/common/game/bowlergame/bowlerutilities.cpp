//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/bowlerutilities.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace BowlerUtilities
{
	void ExtractTime( float time, int &minutes, int &seconds, int &fraction )
	{
		minutes = static_cast<int>( time / 60.0f );
		seconds = static_cast<int>( time - ( static_cast<float>( minutes ) * 60.0f ) );
		fraction = static_cast<int>( ( time - static_cast<int>( time ) ) * 100.0f );

		if ( minutes > 99 )
		{
			minutes = 99;
		}

		if ( seconds > 99 )
		{
			seconds = 99;
		}
	}
}