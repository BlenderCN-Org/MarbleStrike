//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/animation/interpolators.hpp"
#include "common/engine/math/quaternion.hpp"
#include "common/engine/math/vector4.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////////////

namespace Animation
{
	Math::Vector3 LinearInterpolation( const Math::Vector3 &data0, const Math::Vector3& data1, float t )
	{
		Math::Vector3 result = data0 * ( 1.0f - t ) + data1 * t;
		return result;
	}

	//===========================================================================

	Math::Quaternion Interpolation( const Math::Quaternion &data0, const Math::Quaternion& data1, float t )
	{
		float dot = data0.Dot( data1 );

		const float DOT_THRESHOLD = 0.9995f;
		if ( dot > DOT_THRESHOLD ) 
		{		
			Math::Quaternion result = data0 + ( data1 - data0 ) * t;
			result.Normalize();
			return result;
		}

		if ( dot < -1 ) 
		{
			dot = -1;
		}

		if ( dot > 1 ) 
		{
			dot = 1;
		}

		float theta_0 = acosf( dot ); 
		float theta = theta_0 * t;

		Math::Quaternion v2 = data1 - data0 * dot;
		v2.Normalize();

		Math::Quaternion result = data0 * cosf( theta ) + v2 * sinf( theta );
		return result;
	}

	//===========================================================================

	float LinearInterpolation( const float &data0, const float &data1, float t )
	{
		float result = data0 * ( 1.0f - t ) + data1 * t;
		return result;
	}

	//===========================================================================

	int LinearInterpolation( const int &data0, const int &data1, float t )
	{
		float floatData0 = static_cast<float>( data0 );
		float floatData1 = static_cast<float>( data1 );

		float result = floatData0 * ( 1.0f - t ) + floatData1 * t;

		int intResult = static_cast<int>( result );
		return intResult;
	}
}
