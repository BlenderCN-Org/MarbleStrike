//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/math/quaternion.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{
	Vector3 Rotate( const Vector3& v, const float& angleDegree, const Vector3& axis )
	{
		Quaternion rotation;
		Quaternion rotationConj;			
		Quaternion newVector;
		Quaternion tempVector;

		rotation.Set( axis, angleDegree );
		rotation = rotation.Normalize();
		rotationConj = rotation;
		rotationConj = rotationConj.Conjugate();

		tempVector.Set( v[0], v[1], v[2], 0 );
		newVector = rotation * tempVector * rotationConj;

		Vector3 finalVector( newVector[0], newVector[1], newVector[2] );

		return finalVector;
	}

	//===========================================================================

	Vector3 MakeNormal( const Vector3& p0, const Vector3& p1,const Vector3& p2 )
	{
		Vector3 v1( p1 - p0 );
		Vector3 v2( p2 - p0 );			
		Vector3 normal = Math::Vector3::Cross( v1, v2 );
		normal = Math::Vector3::Normalize( normal );

		return Vector3( normal );
	}		

	//===========================================================================

	Vector3 Reflect( const Math::Vector3 &v, const Math::Vector3 &normal )
	{
		return ( v - normal * Math::Vector3::Dot( normal, v ) * 2.0f );
	}

	//===========================================================================

	Vector3 Refract( const Math::Vector3 &v, const Math::Vector3 &normal, float etaRatio )
	{
		Math::Vector3 tempVector = -v;
		float cosI = Math::Vector3::Dot( v, normal );
		float costT2 = 1.0f - etaRatio * etaRatio * ( 1.0f - cosI * cosI );    
		if ( costT2 >= 0 )
		{
			Vector3 T = normal * ( etaRatio * cosI - sqrtf( costT2 ) ) - v * etaRatio;
			return T;
		}
		else
		{
			return Reflect( v, normal );
		}
	}
}
