#ifndef PLANE_HPP
#define PLANE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/mathfunctions.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 
	class Plane
	{
	public:

		Plane();
		Plane( const Math::Vector3 &position, const Math::Vector3 &normal );
		~Plane();

		float DistanceToPlane( const Math::Vector3 &point ) const;

		Math::Vector3 GetNormal() const 
		{ 
			return Math::Vector3( mCoeff[0], mCoeff[1], mCoeff[2] ); 
		}

		void SetNormal( const Math::Vector3 &normal ) 
		{ 
			Math::Vector3 n = Math::Vector3::Normalize( normal );
			mCoeff[0] = n[0]; 
			mCoeff[1] = n[1]; 
			mCoeff[2] = n[2];
		}

		Math::Vector3 GetPosition() const 
		{ 
			return mPosition; 
		}

		void SetPosition( const Math::Vector3 &p ) 
		{ 
			mPosition = p; 
		}

		void SetCoeff( const Math::Vector4 &coeff ) 
		{ 
			mCoeff = coeff; 
		}

		Math::Vector4 GetCoeff() const
		{ 
			return mCoeff; 
		}

		void ComputeCoeff( Math::Vector3 normal, Math::Vector3 position );
		void Transform( const Math::Matrix44 &transform );

	private:	

		Math::Vector4 mCoeff;    
		Math::Vector3 mPosition;
	};
}

#endif
