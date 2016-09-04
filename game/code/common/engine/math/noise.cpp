//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "noise.hpp"
#include "common/engine/system/memory.hpp"
#include <math.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static int permutation[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math
{  
	Noise::Noise() 
	{
		for ( int i = 0; i < 256 ; i++ ) 
		{
			mPermutation[256+i] = mPermutation[i] = permutation[i];
		}

		mGradients[0].Set( 1, 1, 0 );
		mGradients[1].Set( -1, 1, 0 );
		mGradients[2].Set( 1, -1, 0 );
		mGradients[3].Set( -1, -1, 0 );
		mGradients[4].Set( 1, 0, 1 );
		mGradients[5].Set( -1, 0, 1 );
		mGradients[6].Set( 1, 0, -1 );
		mGradients[7].Set( -1, 0, -1 );
		mGradients[8].Set( 0, 1, 1 );
		mGradients[9].Set( 0, -1, 1 );
		mGradients[10].Set( 0, 1, -1 );
		mGradients[11].Set( 0, -1, -1 );
		mGradients[12].Set( 1, 1, 0 );
		mGradients[13].Set( 0, -1, 1 );
		mGradients[14].Set( -1, 1, 0 );
		mGradients[15].Set( 0, -1, -1 );
	}

	//===========================================================================

	Noise::~Noise()
	{
	}

	//===========================================================================

	float Noise::Noise3( float x, float y, float z) 
	{
		int X = (int)floorf(x) & 255;
		int	Y = (int)floorf(y) & 255;
		int	Z = (int)floorf(z) & 255;

		x -= floorf( x );
		y -= floorf( y );
		z -= floorf( z );

		float u = Fade( x );
		float v = Fade( y );
		float w = Fade( z );

		int A = mPermutation[X] + Y;
		int AA = mPermutation[A] + Z;
		int AB = mPermutation[A + 1] + Z;
		int B = mPermutation[X + 1] + Y;
		int BA = mPermutation[B] + Z;
		int BB = mPermutation[B + 1] + Z;

		float grad1 = Grad( mPermutation[AA]  , x   ,  y    , z     );
		float grad2 = Grad( mPermutation[AB]  , x   ,  y - 1, z     );
		float grad3 = Grad( mPermutation[AA+1], x   ,  y    , z - 1 );
		float grad4 = Grad( mPermutation[AB+1], x   ,  y - 1, z - 1 );
		float grad5 = Grad( mPermutation[BA]  , x - 1, y    , z     );
		float grad6 = Grad( mPermutation[BB]  , x - 1, y - 1, z     );
		float grad7 = Grad( mPermutation[BA+1], x - 1, y    , z - 1 );
		float grad8 = Grad( mPermutation[BB+1], x - 1, y - 1, z - 1 ); 

		float lerp1 = Lerp( u, grad1, grad5 );
		float lerp2 = Lerp( u, grad2, grad6 );
		float lerp3 = Lerp( u, grad3, grad7 );
		float lerp4 = Lerp( u, grad4, grad8 );
		float lerp5 = Lerp( v, lerp1, lerp2 );
		float lerp6 = Lerp( v, lerp3, lerp4 );
		float lerp7 = Lerp( w, lerp5, lerp6 );

		return lerp7;
	}

	//===========================================================================

	float Noise::Fade( float t ) 
	{ 
		return t * t * t * (t * (t * 6 - 15) + 10); 
	}

	//===========================================================================

	float Noise::Lerp( float t, float a, float b ) 
	{ 
		return a + t *  ( b - a ); 
	}

	//===========================================================================

	float Noise::Grad( int hash, float x, float y, float z ) 
	{
		Math::Vector3 gradient = mGradients[ hash % 16];
		Math::Vector3 position( x, y, z );

		float dot = Math::Vector3::Dot( gradient, position );

		return dot;
	}
}