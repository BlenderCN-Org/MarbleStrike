//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "disc.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math
{
    Disc::Disc() 
        :	mRadius( 0 )
    {
    }

    //===========================================================================

    Disc::Disc( const Math::Vector3 &axis, float radius, const Math::Vector3 &position ) 
		:	mAxis( Math::Vector3::Normalize( axis ) )
		,	mPosition( position )
		,	mRadius( radius )
    {
    }

    //===========================================================================

    Disc::~Disc()
    {
    }

}
