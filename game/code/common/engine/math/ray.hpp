#ifndef RAY_HPP
#define RAY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 

	class Ray
	{
	public:

		Ray();
		Ray( const Math::Vector3& o, const Math::Vector3& d );
		~Ray();

		Math::Vector3 GetDirection() const 
		{ 
			return mDirection;
		}

		void SetDirection( const Math::Vector3& direction ) 
		{ 
			mDirection = Math::Vector3::Normalize( direction ); 
		}

		Math::Vector3 GetOrigin() const 
		{ 
			return mOrigin; 
		}

		void SetOrigin( const Math::Vector3& origin ) 
		{ 
			mOrigin = origin; 
		}

	private:

		Math::Vector3 mDirection;
		Math::Vector3 mOrigin;
	};
}

#endif
