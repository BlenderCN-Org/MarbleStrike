#ifndef QUATERNION_HPP
#define QUATERNION_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math 
{
	class Quaternion
	{
	public:

		Quaternion() 
		{
			Set( 0, 0, 0, 1 );
		}

		Quaternion( const float& x, const float& y, const float& z, const float& w )
		{
			Set( x, y, z, w );
		}

		Quaternion( const Vector3& axis, const float& angleDegree )
		{
			Set( axis, angleDegree );
		}

		Quaternion( const Quaternion& q ) 
		{
			Set( q );
		}

		~Quaternion()
		{

		}

		Quaternion& operator=( const Quaternion& q )
		{
			mData[0] = q[0]; 
			mData[1] = q[1]; 
			mData[2] = q[2]; 
			mData[3] = q[3];

			return *this;
		}

		void Set( const Vector3& axis, const float& angleDegree ) 
		{
			Math::Vector3 normalizedAxis = Math::Vector3::Normalize( axis );
			float angleRadians = Math::DegreeToRadians( angleDegree );
			float s = static_cast<float>( sin( angleRadians/2.0f ) );
			mData[0] = normalizedAxis[0] * s;
			mData[1] = normalizedAxis[1] * s;
			mData[2] = normalizedAxis[2] * s;
			mData[3] = static_cast<float>( cos( angleRadians/2.0f ) );
		}

		void Set( const float& angleDegreeX, const float& angleDegreeY, const float& angleDegreeZ )
		{ 
			Math::Vector3 xaxis( 1, 0, 0 );
			Math::Vector3 yaxis( 0, 1, 0 );
			Math::Vector3 zaxis( 0, 0, 1 );

			Math::Quaternion qx( xaxis, angleDegreeX );
			Math::Quaternion qy( yaxis, angleDegreeY );
			Math::Quaternion qz( zaxis, angleDegreeZ );

			*this = qx * qy * qz;
		}

		void SetIdentity()
		{
			mData[0] = 0;
			mData[1] = 0;
			mData[2] = 0;
			mData[3] = 1.0f;
		}

		void Set( const float& x, const float& y, const float& z, const float& w ) 
		{
			mData[0] = x; 
			mData[1] = y; 
			mData[2] = z; 
			mData[3] = w;
		}

		void Set( const Quaternion& q ) 
		{
			mData[0] = q[0]; 
			mData[1] = q[1]; 
			mData[2] = q[2]; 
			mData[3] = q[3];
		}

		void Set( const Math::Matrix44& mat )
		{
			float trace;
			trace = mat[0] + mat[5] + mat[10] + 1.0f;
			if ( trace > 0 )
			{
				float s = 0.5f / static_cast<float>( sqrt( trace ) );
				mData[3] = 0.25f / s;            
				mData[0] = ( mat[9] - mat[6] ) * s;
				mData[1] = ( mat[2] - mat[8] ) * s;
				mData[2] = ( mat[4] - mat[1] ) * s;

				/*
				0 4 8 12
				1 5 9 13
				2 6 10 14
				3 7 11 15
				*/

				mData[3] = static_cast<float>( sqrt(1.0 + mat[0] + mat[5] + mat[10] ) / 2.0f );
				float w4 = (4.0f * mData[3]);
				mData[0] = (mat[9] - mat[6]) / w4 ;
				mData[1] = (mat[2] - mat[8]) / w4 ;
				mData[2] = (mat[4] - mat[1]) / w4 ;
			}
			else
			{
				float s = 0;
				if ( ( mat[0] > mat[5] ) && ( mat[0] > mat[5] ) )
				{
					s = static_cast<float>( 2.0f * sqrt( 1.0f + mat[0] - mat[5] - mat[10] ) );
					mData[0] = 0.5f / s;                
					mData[1] = ( mat[1] + mat[4] ) / s;
					mData[2] = ( mat[2] + mat[8] ) / s;
					mData[3] = ( mat[6] + mat[9] ) / s;
				}
				else if ( mat[5] > mat[10] )
				{
					s = static_cast<float>( 2.0f * sqrt( 1.0f + mat[5] - mat[0] - mat[10] ) );                               
					mData[0] = ( mat[1] + mat[4] ) / s;
					mData[1] = 0.5f / s;
					mData[2] = ( mat[6] + mat[9] ) / s;
					mData[3] = ( mat[2] + mat[8] ) / s;             
				}
				else
				{
					s = static_cast<float>( 2.0f * sqrt( 1.0f + mat[10] - mat[0] - mat[5] ) );                
					mData[0] = ( mat[2] + mat[8] ) / s;
					mData[1] = ( mat[6] + mat[9] ) / s;
					mData[2] = 0.5f / s;
					mData[3] = ( mat[4] + mat[1] ) / s;
				}
			}
		}

		float& operator[]( int i ) 
		{
			return mData[i];
		}

		const float& operator[]( int i ) const 
		{
			return mData[i];
		}

		Quaternion& operator+=( const float& s ) 
		{
			mData[0] += s; 
			mData[1] += s; 
			mData[2] += s; 
			mData[3] += s;

			return *this;
		}

		Quaternion& operator-=( const float& s ) 
		{
			mData[0] -= s; 
			mData[1] -= s; 
			mData[2] -= s; 
			mData[3] -= s;

			return *this;
		}

		Quaternion& operator*=( const float& s ) 
		{
			mData[0] *= s; 
			mData[1] *= s; 
			mData[2] *= s; 
			mData[3] *= s;

			return *this;
		}

		Quaternion& operator/=( const float& s ) 
		{
			mData[0] /= s; 
			mData[1] /= s; 
			mData[2] /= s; 
			mData[3] /= s;

			return *this;
		}

		Quaternion operator+( const float& s ) const 
		{
			return Quaternion( *this ) += s;
		}

		Quaternion operator-( const float& s ) const 
		{
			return Quaternion( *this ) +- s;
		}

		Quaternion operator*( const float& s ) const 
		{
			return Quaternion( *this ) *= s;
		}

		Quaternion operator/( const float& s ) const 
		{
			return Quaternion( *this ) /= s;
		}

		Quaternion& operator+=( const Quaternion& q ) 
		{
			mData[0] += q[0]; 
			mData[1] += q[1]; 
			mData[2] += q[2]; 
			mData[3] += q[3];

			return *this;
		}

		Quaternion& operator-=( const Quaternion& q ) 
		{
			mData[0] -= q[0]; 
			mData[1] -= q[1]; 
			mData[2] -= q[2]; 
			mData[3] -= q[3];

			return *this;
		}

		Quaternion& operator*=( const Quaternion& q );

		Quaternion operator+( const Quaternion& q ) const 
		{
			return Quaternion( mData[0] + q[0], mData[1] + q[1], mData[2] + q[2], mData[3] + q[3] );
		}

		Quaternion operator-( const Quaternion& q ) const
		{
			return Quaternion( mData[0] - q[0], mData[1] - q[1], mData[2] - q[2], mData[3] - q[3] );
		}

		Quaternion operator*( const Quaternion& q ) const 
		{
			return Quaternion( *this ) *= q;
		}		

		float Dot( const Math::Quaternion& q ) const
		{
			return ( mData[0] * q[0] + mData[1] * q[1] + mData[2] * q[2] + mData[3] * q[3] );
		}

		float Magnitude() const
		{
			return static_cast<float>( sqrt( 
				mData[0] * mData[0] + 
				mData[1] * mData[1] + 
				mData[2] * mData[2] + 
				mData[3] * mData[3] ) );
		}

		Quaternion Normalize() const
		{
			float length = Magnitude();      
			if ( length == 0.0 )
			{
				length = 1.0;
			}
			return Quaternion( mData[0]/length, mData[1]/length, mData[2]/length, mData[3]/length );
		}

		Quaternion Conjugate() 
		{
			return Quaternion( -mData[0], -mData[1], -mData[2], mData[3] );
		}

		float GetAngleRadians() 
		{
			return static_cast<float>( acos( mData[3] ) * 2.0f );
		}

		Math::Vector3 GetAxis() 
		{
			float angleRadians = GetAngleRadians();
			float s = static_cast<float>( sin( angleRadians/2.0f ) );

			if ( s == 0 )
			{
				return Math::Vector3( 0, 0, 1 );
			}

			Math::Vector3 axis;
			axis[0] = mData[0] / s;
			axis[1] = mData[1] / s;
			axis[2] = mData[2] / s;

			return axis;      
		}

		Math::Matrix44 GetMatrix() const
		{
			Math::Matrix44 mat;

			// Matrix form of a Quaternion
			// 1-2yy-2zz , 2xy - 2wz , 2xz+2wy
			// 2xy + 2wz , 1-2xx-2zz , 2yz-2wx
			// 2xz - 2wy , 2yz + 2wx , 1-2xx-2yy

			mat[0] = 1 - 2*mData[1]*mData[1] - 2*mData[2]*mData[2];
			mat[1] = 2*mData[0]*mData[1] + 2*mData[3]*mData[2];
			mat[2] = 2*mData[0]*mData[2] - 2*mData[3]*mData[1];
			mat[3] = 0;

			mat[4] = 2*mData[0]*mData[1] - 2*mData[3]*mData[2];
			mat[5] = 1 - 2*mData[0]*mData[0] - 2*mData[2]*mData[2];
			mat[6] = 2*mData[1]*mData[2] + 2*mData[3]*mData[0];
			mat[7] = 0;

			mat[8]  = 2*mData[0]*mData[2] + 2*mData[3]*mData[1];
			mat[9]  = 2*mData[1]*mData[2] - 2*mData[3]*mData[0];
			mat[10] = 1 - 2*mData[0]*mData[0] - 2*mData[1]*mData[1];
			mat[11] = 0;

			mat[12] = 0;
			mat[13] = 0;
			mat[14] = 0;
			mat[15] = 1;

			return mat;
		}

		Math::Quaternion Slerp( const Math::Quaternion& q2, float t );

	private:

		float mData[4];

	};
}

#endif
