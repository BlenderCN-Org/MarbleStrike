//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/frustum.hpp"

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

namespace Math
{
    Frustum::Frustum()
    {
    }

    //===========================================================================

    Frustum::~Frustum()
    {
    }

    //===========================================================================

    void Frustum::ExtractPlanes( const Math::Matrix44 &projView )
    {
        //
        // Extract right plane
        //    
        Math::Vector4 coeffs;
        float t;
        coeffs[0] = projView[3] - projView[0];
        coeffs[1] = projView[7] - projView[4];
        coeffs[2] = projView[11] - projView[8];
        coeffs[3] = projView[15] - projView[12];    
        t = static_cast<float>( sqrt( coeffs[0] * coeffs[0] + coeffs[1]* coeffs[1] + coeffs[2] * coeffs[2] ) );
        coeffs /= t;   
        mPlanes[0].SetCoeff( coeffs );    

        //
        // Extract left plane
        //    
        coeffs[0] = projView[3] + projView[0];
        coeffs[1] = projView[7] + projView[4];
        coeffs[2] = projView[11] + projView[8];
        coeffs[3] = projView[15] + projView[12];
        t = static_cast<float>( sqrt( coeffs[0] * coeffs[0] + coeffs[1]* coeffs[1] + coeffs[2] * coeffs[2] ) );
        coeffs /= t;
        mPlanes[1].SetCoeff( coeffs );

        //
        //Extract bottom plane
        //
        coeffs[0] = projView[3] + projView[1];
        coeffs[1] = projView[7] + projView[5];
        coeffs[2] = projView[11] + projView[9];
        coeffs[3] = projView[15] + projView[13];
        t = static_cast<float>( sqrt( coeffs[0] * coeffs[0] + coeffs[1]* coeffs[1] + coeffs[2] * coeffs[2] ) );
        coeffs /= t;
        mPlanes[2].SetCoeff( coeffs );

        //
        // Extract top plane
        //
        coeffs[0] = projView[3] - projView[1];
        coeffs[1] = projView[7] - projView[5];
        coeffs[2] = projView[11] - projView[9];
        coeffs[3] = projView[15] - projView[13];
        t = static_cast<float>( sqrt( coeffs[0] * coeffs[0] + coeffs[1]* coeffs[1] + coeffs[2] * coeffs[2] ) );
        coeffs /= t;
        mPlanes[3].SetCoeff( coeffs );

        //
        // Extract far plane 
        //
        coeffs[0] = projView[3] - projView[2];
        coeffs[1] = projView[7] - projView[6];
        coeffs[2] = projView[11] - projView[10];
        coeffs[3] = projView[15] - projView[14];
        t = static_cast<float>( sqrt( coeffs[0] * coeffs[0] + coeffs[1]* coeffs[1] + coeffs[2] * coeffs[2] ) );
        coeffs /= t;
        mPlanes[4].SetCoeff( coeffs );

        //
        // Extract near plane 
        //
        coeffs[0] = projView[ 3] + projView[ 2];
        coeffs[1] = projView[ 7] + projView[ 6];
        coeffs[2] = projView[11] + projView[10];
        coeffs[3] = projView[15] + projView[14];
        t = static_cast<float>( sqrt( coeffs[0] * coeffs[0] + coeffs[1]* coeffs[1] + coeffs[2] * coeffs[2] ) );
        coeffs /= t;
        mPlanes[5].SetCoeff( coeffs );
    }
}