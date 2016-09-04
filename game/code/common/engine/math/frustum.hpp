#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/plane.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{
    class Frustum
    {
    public:
        Frustum();
        ~Frustum();

        void ExtractPlanes( const Math::Matrix44 &projView );

        Math::Plane GetPlane( unsigned int i ) const
        {
            Assert( i >= 0 && i < 6, "Out of bounds" );
            return mPlanes[i];
        }

        void SetPlaneCoeff( unsigned int i, Math::Vector4 coeff ) 
        {
            mPlanes[i].SetCoeff( coeff );
        }

    private:
        // 0 - right
        // 1 - left
        // 2 - bottom
        // 3 - top
        // 4 - far
        // 5 - near
        Math::Plane mPlanes[6];

    };
}

#endif
