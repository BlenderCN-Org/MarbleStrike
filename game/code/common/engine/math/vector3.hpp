#ifndef VECTOR_3_HPP
#define	VECTOR_3_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <assert.h>
#include <math.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Math
{
	template<class Type> class Vector2Base;
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{
	template<class Type> class Vector3Base
	{
	public:

		static float Magnitude( const Vector3Base& v ) 
		{
			return sqrtf( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
		}

		static Vector3Base Normalize( const Vector3Base& v ) 
		{
			float length = Magnitude( v );			
			if ( length == 0.0 ) 
			{
				length = 1.0;
			}
			return Vector3Base( v[0]/length, v[1]/length, v[2]/length );		
		}

		static float Dot( const Vector3Base& v0, const Vector3Base& v1 ) 
		{
			return v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2];
		}

		static Vector3Base Cross( const Vector3Base& v0, const Vector3Base& v1 ) 
		{
			return Vector3Base( 
				v0[1]*v1[2] - v0[2]*v1[1],
				v0[2]*v1[0] - v0[0]*v1[2],
				v0[0]*v1[1] - v0[1]*v1[0] 
			);
		}

		Vector3Base() 
		{
			mData[0] = 0;
			mData[1] = 0;
			mData[2] = 0;			
		}

		Vector3Base( const Type& x, const Type& y, const Type& z ) 
		{
			Set( x, y, z );
		}

		Vector3Base( const Vector3Base& otherV ) 
		{
			Set( otherV );
		}

		Vector3Base( const Vector2Base<Type> &otherV, const Type &z )
		{
			Set( otherV[0], otherV[1], z );
		}

		~Vector3Base()
		{
		}

		void Set( const Type& x, const Type& y, const Type& z )	
		{
			mData[0] = x;
			mData[1] = y;
			mData[2] = z;
		}

		void Set( const Vector3Base& otherV ) 
		{
			mData[0] = otherV.mData[0]; 
			mData[1] = otherV.mData[1];
			mData[2] = otherV.mData[2];
		}

		Vector3Base& operator=( const Vector3Base& otherV )
		{
			if ( this != &otherV )
			{
				mData[0] = otherV[0];
				mData[1] = otherV[1];
				mData[2] = otherV[2];
			}
			return *this;
		}

		Type& operator[]( int i )	
		{
			assert( i >= 0 && i < 3 );
			return mData[i];
		}

		const Type& operator[]( int i ) const	
		{
			assert( i >= 0 && i < 3 );
			return mData[i];
		}

		operator Type*() 
		{
			return mData;
		}

		bool operator ==( const Vector3Base& v ) const 
		{
			return mData[0] == v[0] && mData[1] == v[1] && mData[2] == v[2];
		}

		bool operator !=( const Vector3Base& v ) const 
		{
			return !( *this == v );
		}

		Vector3Base& operator+=( const Type& s ) 
		{
			mData[0] += s; 
			mData[1] += s; 
			mData[2] += s;

			return *this;
		}

		Vector3Base& operator-=( const Type& s ) 
		{
			mData[0] -= s; 
			mData[1] -= s; 
			mData[2] -= s;

			return *this;
		}

		Vector3Base& operator*=( const Type& s ) 
		{
			mData[0] *= s; 
			mData[1] *= s; 
			mData[2] *= s;

			return *this;	
		}

		Vector3Base& operator/=( const Type& s ) 
		{		
			mData[0] /= s; 
			mData[1] /= s; 
			mData[2] /= s;

			return *this;
		} 

		Vector3Base operator+( const Type& s ) const 
		{
			return Vector3Base( mData[0] + s, mData[1] + s, mData[2] + s );
		}

		Vector3Base operator-( const Type& s ) const 
		{
			return Vector3Base( mData[0] - s, mData[1] - s, mData[2] - s );
		}

		Vector3Base operator/( const Type& s ) const 
		{
			return Vector3Base( mData[0] / s, mData[1] / s, mData[2] / s );
		}

		Vector3Base operator*( const Type& s ) const 
		{
			return Vector3Base( mData[0] * s, mData[1] * s, mData[2] * s );
		}

		Vector3Base& operator+=( const Vector3Base& v ) 
		{
			mData[0] += v[0]; 
			mData[1] += v[1]; 
			mData[2] += v[2];

			return *this;
		}

		Vector3Base& operator-=( const Vector3Base& v ) 
		{
			mData[0] -= v[0]; 
			mData[1] -= v[1]; 
			mData[2] -= v[2];

			return *this;
		}

		Vector3Base operator+( const Vector3Base& v ) const 
		{
			return Vector3Base( mData[0] + v[0], mData[1] + v[1], mData[2] + v[2] );
		}

		Vector3Base operator-( const Vector3Base& v ) const 
		{
			return Vector3Base( mData[0] - v[0], mData[1] - v[1], mData[2] - v[2] );
		}

		Vector3Base operator-() const 
		{
			return Vector3Base( -mData[0], -mData[1], -mData[2] );
		}

	private:    

		Type mData[3];
	};
}

//////////////////////////////////////////////////////
// TYPE DEFINITIONS
//////////////////////////////////////////////////////

namespace Math
{
	typedef Vector3Base<float> Vector3;
	typedef Vector3Base<unsigned char> Vector3Byte;
	typedef Vector3Base<unsigned int> Vector3UInt;
}

//////////////////////////////////////////////////////
// FUNCTIONS
//////////////////////////////////////////////////////

namespace Math
{
	Vector3 Rotate( const Vector3& v, const float& angleDegree, const Vector3& axis );
	Vector3 MakeNormal( const Vector3& p0, const Vector3& p1,const Vector3& p2 );
	Vector3 Reflect( const Math::Vector3 &v, const Math::Vector3 &normal );
	Vector3 Refract( const Math::Vector3 &v, const Math::Vector3 &normal, float etaRatio );
}

#endif
