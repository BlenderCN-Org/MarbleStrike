#ifndef MATH_FUNCTIONS_HPP
#define MATH_FUNCTIONS_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include <math.h>
#include <stdlib.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

namespace Math
{
    class Matrix44;
    class Triangle;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

namespace Math
{
    const float PI = 3.14159265358979323846f;
    const float PIMUL2 = PI * 2.0f;
    const float PIDIV2 = PI / 2.0f;
    const float INFINITY_NUM = 9999999;
}

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace Math 
{  
    //
    // Generate a random floating point number between -1 and 1.
    //
    inline float RandomFloat0()
    {
        float randomNumber = static_cast<float>( rand() % 100 );

        randomNumber /= 100.0f;

        int sign = rand() % 2;
        if ( sign == 0 ) 
        {
            randomNumber *= -1;
        }    

        return randomNumber;
    }

    //
    // Generate a random floating point number between 0 and 1.
    //
    // Minimal random number generator of Park and Miller. Returns a uniform random deviate
    // between 0.0 and 1.0. Set or reset idum to any integer value (except the unlikely value MASK)
    // to initialize the sequence; idum must not be altered between calls for successive deviates in
    // a sequence.
    //
    float RandomFloat1();

    static long Seed = 50;

    //
    // Set random seed.
    //
    inline void SetRandomSeed( long seed )  
    {
        Seed = seed;
    }

    //
    // Convert from degrees to radians.
    //
    inline float DegreeToRadians( const float& degree ) 
    { 
        return degree * PI / 180.0f; 
    }

    //
    // Convert from radians to degree.
    //
    inline float RadiansToDegree( const float& rad ) 
    { 
        return rad * 180.0f /PI; 
    }  

    //
    // Round the number.
    //
    inline int Round( float x ) 
    { 
        return static_cast<int>( floor( x + 0.5f) ); 
    }

    //
    // Get the maximum value between two values.
    //
    template <typename Type>
    inline Type Max( Type value1, Type value2 ) 
    {
        if ( value1 > value2 )
        {
            return value1;
        }
        else
        {
            return value2;
        }
    }

    //
    // Get the minimum value between two values.
    //
    template <typename Type>
    inline Type Min( Type value1, Type value2 ) 
    {
        if ( value1 < value2 )
        {
            return value1;
        }
        else
        {
            return value2;
        }
    }

    //
    // Calculate the factorial of a given number.
    //
    inline int Factorial( int n )
    {
        int r = 1;
        for ( int i = 1; i <= n; i++ )
        {
            r = r * i;
        }
        return r;
    }

    //
    // Find the next power of 2.
    //
    inline int NextPowerOf2( int value )
    {
        int returnValue = 1;
        while ( returnValue < value ) 
        {
            returnValue <<= 1;
        }
        return returnValue;
    }

    //
    // Generate Gaussian weights
    //
    inline float GaussianWeights( float x, float mean, float stdDeviation )
    {
        return ( 1.0f / sqrt( 2.0f * PI * stdDeviation * stdDeviation ) )
            * expf( ( -( ( x - mean ) * ( x - mean ) ) ) / ( 2.0f * stdDeviation * stdDeviation ) );
    }

    //
    // UnProject from Screen Space to 3D Space
    //
    Math::Vector3 UnProject( 
        const Math::Matrix44 &view, 
        const Math::Matrix44 &projection, 
        const Math::Matrix44 &viewport,
        const Math::Vector3 &screenPos );

    //
    // Point to Triangle Distance
    //
    float PointToTriangleDistance( const Math::Triangle& tri, const Math::Vector3& point );

    inline float EaseInExpo( float currentTime, float startValue, float changeValue, float duration ) 
    {
        float t = currentTime;
        float b = startValue;
        float c = changeValue;
        float d = duration;

	    return c * powf( 2.0f, 10.0f * (t/d - 1.0f) ) + b;
    };

    //
    // Quadratic Ease In-Out
    //
    inline float EaseInOutQuad( float currentTime, float startValue, float changeValue, float duration ) 
    {
        float t = currentTime;
        float b = startValue;
        float c = changeValue;
        float d = duration;

        t /= d/2.0f;
        if ( t < 1.0f ) 
        {
            return c/2.0f * t * t + b;
        }
        t--;
        return -c/2.0f * ( t * ( t - 2.0f ) - 1.0f ) + b;
    }

    //
    // Cubic Ease In-Out
    //
    inline float EaseInOutCubic( float currentTime, float startValue, float changeValue, float duration ) 
    {
        float t = currentTime;
        float b = startValue;
        float c = changeValue;
        float d = duration;

        t /= d/2.0f;
        if ( t < 1.0f ) 
        {
            return c/2.0f * t * t * t + b;
        }
        t -= 2.0f;
        return c/2.0f * ( t * t * t + 2.0f ) + b;
    }

    //
    // Exponential Ease In-Out
    //
    inline float EaseInOutExpo( float currentTime, float startValue, float changeValue, float duration ) 
    {
        float t = currentTime;
        float b = startValue;
        float c = changeValue;
        float d = duration;

        t /= d/2.0f;
        if (t < 1.0f) 
        {
            return c/2.0f * powf( 2.0f, 10.0f * (t - 1.0f) ) + b;
        }
        t--;
        return c/2.0f * ( -powf( 2.0f, -10.0f * t) + 2.0f ) + b;
    }
}

#endif
