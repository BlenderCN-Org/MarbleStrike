#ifndef VECTOR_2_HPP
#define	VECTOR_2_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <assert.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{
	template<class Type> class Vector2Base
	{
	public:

		static float Magnitude( const Vector2Base& v ) 
		{
			return sqrtf( v[0]*v[0] + v[1]*v[1] );
		}

		static Vector2Base Normalize( const Vector2Base& v ) 
		{
			float length = Magnitude( v );			
			if ( length == 0.0 ) 
			{
				length = 1.0;
			}
			return Vector2Base( v[0]/length, v[1]/length );		
		}

		static float Dot( const Vector2Base& v0, const Vector2Base& v1 ) 
		{
			return v0[0]*v1[0] + v0[1]*v1[1];
		}

		Vector2Base() 
		{
			mData[0] = 0;
			mData[1] = 0;
		}

		Vector2Base( const Type& x, const Type& y ) 
		{
			Set( x, y );
		}

		Vector2Base( const Vector2Base& otherV ) 
		{
			Set( otherV );
		}

		~Vector2Base()
		{
		}

		void Set( const Type& x, const Type& y )	
		{
			mData[0] = x;
			mData[1] = y;
		}

		void Set( const Vector2Base& otherV ) 
		{
			mData[0] = otherV.mData[0]; 
			mData[1] = otherV.mData[1];
		}

		Vector2Base& operator=( const Vector2Base& otherV )
		{
			if ( this != &otherV )
			{
				mData[0] = otherV[0];
				mData[1] = otherV[1];
			}
			return *this;
		}

		Type& operator[]( int i )	
		{
			assert( i >= 0 && i < 2 );
			return mData[i];
		}

		const Type& operator[]( int i ) const	
		{
			assert( i >= 0 && i < 2 );
			return mData[i];
		}

		operator Type*() 
		{
			return mData;
		}

		bool operator ==( const Vector2Base& v ) const 
		{
			return mData[0] == v[0] && mData[1] == v[1];
		}

		bool operator !=( const Vector2Base& v ) const 
		{
			return !( *this == v );
		}

		Vector2Base& operator+=( const Type& s ) 
		{
			mData[0] += s; 
			mData[1] += s; 

			return *this;
		}

		Vector2Base& operator-=( const Type& s ) 
		{
			mData[0] -= s; 
			mData[1] -= s; 

			return *this;
		}

		Vector2Base& operator*=( const Type& s ) 
		{
			mData[0] *= s; 
			mData[1] *= s; 

			return *this;	
		}

		Vector2Base& operator/=( const Type& s ) 
		{		
			mData[0] /= s; 
			mData[1] /= s; 

			return *this;
		} 

		Vector2Base operator+( const Type& s ) const 
		{
			return Vector2Base( mData[0] + s, mData[1] + s );
		}

		Vector2Base operator-( const Type& s ) const 
		{
			return Vector2Base( mData[0] - s, mData[1] - s );
		}

		Vector2Base operator/( const Type& s ) const 
		{
			return Vector2Base( mData[0] / s, mData[1] / s );
		}

		Vector2Base operator*( const Type& s ) const 
		{
			return Vector2Base( mData[0] * s, mData[1] * s );
		}

		Vector2Base& operator+=( const Vector2Base& v ) 
		{
			mData[0] += v[0]; 
			mData[1] += v[1]; 

			return *this;
		}

		Vector2Base& operator-=( const Vector2Base& v ) 
		{
			mData[0] -= v[0]; 
			mData[1] -= v[1];

			return *this;
		}

		Vector2Base operator+( const Vector2Base& v ) const 
		{
			return Vector2Base( mData[0] + v[0], mData[1] + v[1] );
		}

		Vector2Base operator-( const Vector2Base& v ) const 
		{
			return Vector2Base( mData[0] - v[0], mData[1] - v[1] );
		}

		Vector2Base operator-() const 
		{
			return Vector2Base( -mData[0], -mData[1] );
		}

	private:    

		Type mData[2];
	};
}

//////////////////////////////////////////////////////
// TYPE DEFINITIONS
//////////////////////////////////////////////////////

namespace Math
{
	typedef Vector2Base<float> Vector2;
}

#endif
