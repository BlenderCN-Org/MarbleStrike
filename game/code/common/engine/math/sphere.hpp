#ifndef SPHERE_HPP
#define SPHERE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 
    class Sphere
    {
    public:
  
		Sphere();
        Sphere( const float &radius, const Math::Vector3 &position );
        ~Sphere();

        float GetRadius() const 
        { 
            return mRadius; 
        }

        void SetRadius( const float& radius ) 
        { 
            mRadius = radius; 
        }
       
        Math::Vector3 GetPosition() const 
        { 
            return mPosition; 
        }
		
		void SetPosition( const Math::Vector3 &pos ) 
        { 
            mPosition = pos; 
        }

        Math::Vector3 GetVelocity() const 
        { 
            return mVelocity; 
        }

        void SetVelocity( const Math::Vector3 &vel ) 
        { 
            mVelocity = vel; 
        }

        Math::Vector3 GetOldPosition() const 
        { 
            return mOldPosition; 
        }

        void SetOldPosition( const Math::Vector3 &oldPosition ) 
        { 
            mOldPosition = oldPosition; 
        }

    protected:        
        float mRadius;
        Math::Vector3 mPosition;
        Math::Vector3 mVelocity;
        Math::Vector3 mOldPosition;
    };

}

#endif
