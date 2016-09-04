//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "aabb.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math
{

	AABB::AABB()
	{
	}

	//===========================================================================

	AABB::AABB( const Math::Vector3& halfSides, const Math::Vector3& position ) 
		:	mHalfSides( halfSides )
		,	mPosition( position ) 
	{
	}

	//===========================================================================

	AABB::~AABB()
	{
	}

	//===========================================================================

	Math::Vector3 AABB::GetNormal( const Math::Vector3& p )
	{
		Math::Vector3 normal;

		normal.Set( 1, 0, 0 );
		if ( fabs( Math::Vector3::Dot( normal, p ) - ( GetMax()[0] + GetPosition()[0] ) ) < 0.01f )
		{
			return normal;
		}

		normal.Set( -1, 0, 0 );    
		if ( fabs( Math::Vector3::Dot( normal, p ) + ( GetMin()[0] + GetPosition()[0] ) ) < 0.01f )
		{
			return normal;
		}

		normal.Set( 0, 1, 0 );
		if ( fabs( Math::Vector3::Dot( normal, p ) - ( GetMax()[1] + GetPosition()[1] ) ) < 0.01f )
		{
			return normal;
		}

		normal.Set( 0, -1, 0 );
		if ( fabs( Math::Vector3::Dot( normal, p ) + ( GetMin()[1] + GetPosition()[1] ) ) < 0.01f )
		{
			return normal;
		}

		normal.Set( 0, 0, 1 );
		if ( fabs( Math::Vector3::Dot( normal, p ) - ( GetMax()[2] + GetPosition()[2] ) ) < 0.01f )
		{
			return normal;
		}

		normal.Set( 0, 0, -1 );
		if ( fabs( Math::Vector3::Dot( normal, p ) + ( GetMin()[2] + GetPosition()[2] ) ) < 0.01f )
		{
			return normal;
		}
		return normal;
	}

}
