//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/triangle.hpp"

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace Math
{
    float RandomFloat1()
    {
        const long IA = 16807;
        const long IM = 2147483647;
        const float AM = (1.0f/IM);
        const long IQ = 127773;
        const long IR = 2836;
        const long MASK = 123459876;

        long k;
        float ans;
        Seed ^= MASK; 
        k = Seed/IQ;
        Seed = IA * (Seed - k * IQ ) - IR * k;
        if ( Seed < 0 ) 
        {
            Seed += IM;
        }
        ans = AM * Seed; 
        Seed ^= MASK; 

        return ans;
    }

    //===========================================================================

    Math::Vector3 UnProject( 
        const Math::Matrix44 &view, 
        const Math::Matrix44 &projection, 
        const Math::Matrix44 &viewport,
        const Math::Vector3 &screenPos )
    {
        Math::Matrix44 VPMInvert = viewport * projection * view;	
        VPMInvert.Inverse();

        Math::Vector4 objectPoint;
        objectPoint = VPMInvert * Math::Vector4( screenPos, 1 );
        objectPoint /= objectPoint[3];

        return Math::Vector3( objectPoint[0], objectPoint[1], objectPoint[2] );
    }

    //===========================================================================

    float PointToTriangleDistance( const Math::Triangle &tri, const Math::Vector3 &point )
    {
        Math::Vector3 B = tri[0];
        Math::Vector3 E0 = tri[1] - tri[0];
        Math::Vector3 E1 = tri[2] - tri[0];
        Math::Vector3 D = B - point;
        float a = Math::Vector3::Dot( E0, E0 );
        float b = Math::Vector3::Dot( E0,E1 );
        float c = Math::Vector3::Dot( E1, E1 );
        float d = Math::Vector3::Dot( E0, D );
        float e = Math::Vector3::Dot( E1, D );	
        float det = a * c - b * b; 
        float s = b * e - c * d;
        float t = b * d - a * e;

        if ( s + t <= det )
        {
            if ( s < 0 ) 
            { 
                if ( t < 0 ) 
                { 
                    float tmp0 = b + d;
                    float tmp1 = c + e;
                    if ( tmp1 > tmp0 ) // minimum on edge s+t=1
                    {
                        float numer = tmp1 - tmp0;
                        float denom = a - 2.0f * b + c;
                        s = ( numer >= denom ? 1.0f : numer/denom );
                        t = 1.0f - s;
                    }
                    else // minimum on edge s=0
                    {
                        s = 0.0f;
                        t = ( tmp1 <= 0.0f ? 1.0f : ( e >= 0.0f ? 0 : -e/c ) );
                    }
                } 
                else 
                { 
                    s = 0;
                    t = ( e >= 0 ? 0 : ( -e >= c ? 1 : -e/c ) );
                } 
            }
            else if ( t < 0 ) 
            { 
                s = 0;
                t = ( e >= 0 ? 0 : ( -e >= c ? 1 : -e/c ) );
            }
            else 
            { 
                float invDet = 1.0f / det;
                s *= invDet;
                t *= invDet;
            }
        }
        else
        {
            if ( s < 0 ) 
            { 
                float tmp0 = b + d;
                float tmp1 = c + e;
                if ( tmp1 > tmp0 ) // minimum on edge s+t=1
                {
                    float numer = tmp1 - tmp0;
                    float denom = a - 2.0f * b + c;
                    s = ( numer >= denom ? 1.0f : numer/denom );
                    t = 1.0f - s;
                }
                else // minimum on edge s=0
                {
                    s = 0.0f;
                    t = ( tmp1 <= 0.0f ? 1.0f : ( e >= 0.0f ? 0 : -e/c ) );
                }
            }
            else if ( t < 0 ) 
            { 
                float tmp0 = b + d;
                float tmp1 = c + e;
                if ( tmp1 > tmp0 ) // minimum on edge s+t=1
                {
                    float numer = tmp1 - tmp0;
                    float denom = a - 2.0f * b + c;
                    s = ( numer >= denom ? 1.0f : numer/denom );
                    t = 1.0f - s;
                }
                else // minimum on edge s=0
                {
                    s = 0.0f;
                    t = ( tmp1 <= 0.0f ? 1.0f : ( e >= 0.0f ? 0 : -e/c ) );
                }
            }
            else 
            { 
                float numer = c + e - b - d;
                if ( numer <= 0 )
                {
                    s = 0;
                }
                else
                {
                    float denom = a-2*b+c; // positive quantity
                    s = ( numer >= denom ? 1 : numer/denom );
                }
                t = 1.0f - s;
            }
        }

        Math::Vector3 triPoint = B + E0 * s + E1 * t;
        float dist = Math::Vector3::Magnitude( ( point - triPoint ) );

        return dist;
    }    
}