#ifndef DISC_HPP
#define DISC_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 
	class Disc
	{
	public:

		Disc();
		Disc( const Math::Vector3 &axis, float radius, const Math::Vector3 &position );    
		~Disc();

		Math::Vector3 GetPosition() const
		{
			return mPosition;
		}

		void SetPosition( const Math::Vector3 &position )
		{
			mPosition = position;
		}

		Math::Vector3 GetAxis() const
		{
			return mAxis;
		}

		void SetAxis( const Math::Vector3 &axis )
		{
			mAxis = axis;
		}

		float GetRadius() const
		{
			return mRadius;
		}

		void SetRadius( float radius )
		{
			mRadius = radius;
		}

	private:

		Math::Vector3 mAxis;
		Math::Vector3 mPosition;
		float mRadius;
	};

}

#endif
