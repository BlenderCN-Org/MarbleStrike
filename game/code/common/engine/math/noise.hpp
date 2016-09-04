#ifndef NOISE_HPP
#define NOISE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{
	class Noise
	{
	public:  

		Noise();
		~Noise();
		float Noise3( float x, float y, float z );

	private:      		

		float Fade( float t );
		float Lerp( float t, float a, float b );
		float Grad( int hash, float x, float y, float z );

		int mPermutation[512];
		Math::Vector3 mGradients[16];
	};
}

#endif