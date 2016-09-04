//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/plane.hpp"
#include "common/engine/math/matrix44.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math
{
	Plane::Plane()
	{
	}

	//===========================================================================

	Plane::Plane( const Math::Vector3 &position, const Math::Vector3 &normal ) 
		:	mCoeff( normal[0], normal[1], normal[2], 0 )
		,   mPosition( position )
	{
		ComputeCoeff( normal, position );
	}

	//===========================================================================

	Plane::~Plane()
	{
	}

	//===========================================================================

	float Plane::DistanceToPlane( const Math::Vector3 &point ) const
	{
		Math::Vector3 normal( mCoeff[0], mCoeff[1], mCoeff[2] );        
		return Math::Vector3::Dot( normal, point ) + mCoeff[3];
	}

	//===========================================================================

	void Plane::ComputeCoeff( Math::Vector3 normal, Math::Vector3 position )
	{
		mCoeff[0] = normal[0];
		mCoeff[1] = normal[1];
		mCoeff[2] = normal[2];
		mCoeff[3] = -( Math::Vector3::Dot( normal, position ) );
	}

	//===========================================================================

	void Plane::Transform( const Math::Matrix44 &transform )
	{		
		mPosition = transform * mPosition;
		Math::Vector3 normal( mCoeff[0], mCoeff[1], mCoeff[2] );
		normal = transform * normal;
		normal = Math::Vector3::Normalize( normal );
		ComputeCoeff( normal, mPosition );
	}
}