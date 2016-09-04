#ifndef AABB_HPP
#define AABB_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 

	class AABB
	{
	public:

		AABB();
		AABB( const Math::Vector3 &halfSides, const Math::Vector3 &position );    
		~AABB();
		
		Math::Vector3 GetNormal( const Math::Vector3 &p );

		Math::Vector3 GetMin() const 
		{ 
			Math::Vector3 min;
			min = mPosition - mHalfSides;
			return min; 
		}

		inline Math::Vector3 GetMax() const 
		{ 
			Math::Vector3 max;
			max = mPosition + mHalfSides;
			return max; 
		}

		inline void SetHalfSides( const Math::Vector3 &halfSides )
		{
			mHalfSides = halfSides;
		}

		inline Math::Vector3 GetHalfSides() const
		{
			return mHalfSides;
		}

		inline void SetPosition( const Math::Vector3 &position )
		{
			mPosition = position;
		}

		Math::Vector3 GetPosition() const
		{
			return mPosition;
		}

		void SetVelocity( const Math::Vector3 &velocity )
		{
			mVelocity = velocity;
		}

		Math::Vector3 GetVelocity() const
		{
			return mVelocity;
		}

		void SetOldPosition( const Math::Vector3 &oldPosition )
		{
			mOldPosition = oldPosition;
		}

		Math::Vector3 GetOldPosition() const
		{
			return mOldPosition;
		}

	private:    
		Math::Vector3 mHalfSides;

		Math::Vector3 mPosition;
		Math::Vector3 mVelocity;

		Math::Vector3 mOldPosition;
	};

}

#endif
