//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "quaternion.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math 
{
    Quaternion& Quaternion::operator*=( const Quaternion& q ) 
    {    
        float x =   mData[3]*q[0] - mData[2]*q[1] + mData[1]*q[2] + mData[0]*q[3]; 
        float y = - mData[0]*q[2] + mData[1]*q[3] + mData[3]*q[1] + mData[2]*q[0]; 
        float z = - mData[1]*q[0] + mData[2]*q[3] + mData[3]*q[2] + mData[0]*q[1]; 
        float w =   mData[3]*q[3] - mData[0]*q[0] - mData[1]*q[1] - mData[2]*q[2];

        mData[0] = x;
        mData[1] = y;
        mData[2] = z;
        mData[3] = w;

        return *this;
    }

    //===========================================================================

    Math::Quaternion Quaternion::Slerp( const Math::Quaternion& q2, float t )
    {    
        float costheta = Dot( q2 );
        Math::Quaternion q = ( *this );
        if ( costheta >= -1.0 && costheta <= 1.0 )
        {
            float theta = static_cast<float>( acos( costheta ) );
            if ( theta != 0 )
            {        
                q = 
                    ( 
                    ( *this ) * static_cast<float>( sin( ( 1.0 - t ) * theta ) ) + 
                    ( q2 * static_cast<float>( sin( t * theta ) ) ) 
                    ) 
                    / static_cast<float>( sin( theta ) );
            }
        }

        return q;
    }

}
