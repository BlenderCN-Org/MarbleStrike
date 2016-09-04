#ifndef VECTOR_4_HPP
#define	VECTOR_4_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <assert.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Math
{
	template<class Type> class Vector3Base;
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{
	template<class Type> class Vector4Base
	{
	public:

		static float Magnitude( const Vector4Base& v ) 
		{
			return sqrtf( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3] );
		}

		static Vector4Base Normalize( const Vector4Base& v ) 
		{
			float length = Magnitude( v );			
			if ( length == 0.0 ) 
			{
				length = 1.0;
			}
			return Vector4Base( v[0]/length, v[1]/length, v[2]/length, v[3]/length );		
		}

		static float Dot( const Vector4Base& v0, const Vector4Base& v1 ) 
		{
			return v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2] + v0[3]*v1[3];
		}		
		
		Vector4Base() 
		{
			mData[0] = 0;
			mData[1] = 0;
			mData[2] = 0;
			mData[3] = 0;
		}

		Vector4Base( const Type& x, const Type& y, const Type& z, const Type& w ) 
		{
			Set( x, y, z, w );
		}

		Vector4Base( const Vector4Base& otherV ) 
		{
			Set( otherV );
		}

		Vector4Base( const Vector3Base<Type>& otherV, const Type& w ) 
		{
			Set( otherV[0], otherV[1], otherV[2], w );
		}

		~Vector4Base()
		{
		}

		void Set( const Type& x, const Type& y, const Type& z, const Type& w )	
		{
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
			mData[3] = w;
		}

		void Set( const Vector4Base& otherV ) 
		{
			mData[0] = otherV.mData[0]; 
			mData[1] = otherV.mData[1];
			mData[2] = otherV.mData[2];
			mData[3] = otherV.mData[3];
		}

		Vector4Base& operator=( const Vector4Base& otherV )
		{
			if ( this != &otherV )
			{
				mData[0] = otherV[0];
				mData[1] = otherV[1];
				mData[2] = otherV[2];
				mData[3] = otherV[3];
			}
			return *this;
		}

		Type& operator[]( int i )	
		{
			assert( i >= 0 && i < 4 );
			return mData[i];
		}

		const Type& operator[]( int i ) const	
		{
			assert( i >= 0 && i < 4 );
			return mData[i];
		}

		operator Type*() 
		{
			return mData;
		}

		bool operator ==( const Vector4Base& v ) const 
		{
			return mData[0] == v[0] && mData[1] == v[1] && mData[2] == v[2] && mData[3] == v[3];
		}

		bool operator !=( const Vector4Base& v ) const 
		{
			return !( *this == v );
		}

		Vector4Base& operator+=( const Type& s ) 
		{
			mData[0] += s; 
			mData[1] += s; 
			mData[2] += s;
			mData[3] += s;

			return *this;
		}

		Vector4Base& operator-=( const Type& s ) 
		{
			mData[0] -= s; 
			mData[1] -= s; 
			mData[2] -= s;
			mData[3] -= s;

			return *this;
		}

		Vector4Base& operator*=( const Type& s ) 
		{
			mData[0] *= s; 
			mData[1] *= s; 
			mData[2] *= s;
			mData[3] *= s;

			return *this;	
		}

		Vector4Base& operator/=( const Type& s ) 
		{		
			mData[0] /= s; 
			mData[1] /= s; 
			mData[2] /= s;
			mData[3] /= s;

			return *this;
		} 

		Vector4Base operator+( const Type& s ) const 
		{
			return Vector4Base( mData[0] + s, mData[1] + s, mData[2] + s, mData[3] + s );
		}

		Vector4Base operator-( const Type& s ) const 
		{
			return Vector4Base( mData[0] - s, mData[1] - s, mData[2] - s, mData[3] - s );
		}

		Vector4Base operator/( const Type& s ) const 
		{
			return Vector4Base( mData[0] / s, mData[1] / s, mData[2] / s, mData[3] / s );
		}

		Vector4Base operator*( const Type& s ) const 
		{
			return Vector4Base( mData[0] * s, mData[1] * s, mData[2] * s, mData[3] * s );
		}

		Vector4Base& operator+=( const Vector4Base& v ) 
		{
			mData[0] += v[0]; 
			mData[1] += v[1]; 
			mData[2] += v[2];
			mData[3] += v[3];

			return *this;
		}

		Vector4Base& operator-=( const Vector4Base& v ) 
		{
			mData[0] -= v[0]; 
			mData[1] -= v[1]; 
			mData[2] -= v[2];
			mData[3] -= v[3];

			return *this;
		}

		Vector4Base operator+( const Vector4Base& v ) const 
		{
			return Vector4Base( mData[0] + v[0], mData[1] + v[1], mData[2] + v[2], mData[3] + v[3] );
		}

		Vector4Base operator-( const Vector4Base& v ) const 
		{
			return Vector4Base( mData[0] - v[0], mData[1] - v[1], mData[2] - v[2], mData[3] - v[3] );
		}

		Vector4Base operator-() const 
		{
			return Vector4Base( -mData[0], -mData[1], -mData[2], -mData[3] );
		}

		const float* GetRAWData() const
		{
			return mData;
		}

	private:    

		Type mData[4];
	};	
}

//////////////////////////////////////////////////////
// TYPE DEFINITIONS
//////////////////////////////////////////////////////

namespace Math
{
	typedef Vector4Base<float> Vector4;
}

#endif
