#ifndef MATRIX44_HPP
#define MATRIX44_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <math.h>
#include "vector4.hpp"
#include "vector3.hpp"

#ifdef _ARM_ARCH_7
#include <arm_neon.h>
#endif

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

namespace Math
{
	class Quaternion;
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math 
{
#ifdef _ARM_ARCH_7    
    class Matrix44Neon
    {    
    public:
        Matrix44Neon()
        {          
        }
        
        ~Matrix44Neon()
        {        
        }
        
    private:
        float32x4_t mData[4];
    };
#endif    

	class Matrix44
	{
	public:

		Matrix44() 
		{
			SetIdentity();
		}

		Matrix44( const Matrix44& m ) 
		{
			for( int i=0; i < 16; i++ )	
			{
				mData[i] = m.mData[i];			
			}
		}

		Matrix44( 
			float m00, float m04, float m08, float m12,
			float m01, float m05, float m09, float m13,
			float m02, float m06, float m10, float m14,
			float m03, float m07, float m11, float m15 
			)
		{
			Set( 
				m00, m04, m08, m12,
				m01, m05, m09, m13,
				m02, m06, m10, m14,
				m03, m07, m11, m15 
				);
		}

		~Matrix44();
		Matrix44 operator* ( const Matrix44& m ) const;

		Matrix44 InverseAffine();
		void Inverse();

		Matrix44& operator=( const Matrix44& m ) 
		{
			if ( this != &m ) 
			{
				for ( int i = 0; i < 16; i++ ) 
				{
					mData[i] = m[i];
				}
			}
			return *this;
		}

		void Set( 
			float m00, float m04, float m08, float m12,
			float m01, float m05, float m09, float m13,
			float m02, float m06, float m10, float m14,
			float m03, float m07, float m11, float m15 
			)
		{
			mData[0] = m00; mData[1] = m01; mData[2] = m02; mData[3] = m03;
			mData[4] = m04; mData[5] = m05; mData[6] = m06; mData[7] = m07;
			mData[8] = m08; mData[9] = m09; mData[10] = m10; mData[11] = m11;
			mData[12] = m12; mData[13] = m13; mData[14] = m14; mData[15] = m15;
		}

		void Set( float input[] )
		{
			for ( int i = 0; i < 16; i++ )	
			{
				mData[i] = input[i];
			}
		}

		void SetIdentity() 
		{
			mData[0] = 1; mData[4] = 0; mData[8]  = 0; mData[12] = 0;
			mData[1] = 0; mData[5] = 1; mData[9]  = 0; mData[13] = 0;
			mData[2] = 0; mData[6] = 0; mData[10] = 1; mData[14] = 0;
			mData[3] = 0; mData[7] = 0; mData[11] = 0; mData[15] = 1;
		}
				
		void SetViewport( float width, float height );

		Matrix44 Transpose() const
		{
			Matrix44 temp;
			temp[0] = mData[0];  temp[4] = mData[1];  temp[8]  = mData[2];  temp[12] = mData[3];
			temp[1] = mData[4];  temp[5] = mData[5];  temp[9]  = mData[6];  temp[13] = mData[7];
			temp[2] = mData[8];  temp[6] = mData[9];  temp[10] = mData[10]; temp[14] = mData[11];
			temp[3] = mData[12]; temp[7] = mData[13]; temp[11] = mData[14]; temp[15] = mData[15];

			return temp;
		}
        
        bool operator==( const Matrix44 &m )
        {
            if ( mData[0] == m.mData[0] &&
                mData[1] == m.mData[1] &&
                mData[2] == m.mData[2] &&
                mData[3] == m.mData[3] &&
                mData[4] == m.mData[4] &&
                mData[5] == m.mData[5] &&
                mData[6] == m.mData[6] &&
                mData[7] == m.mData[7] &&
                mData[8] == m.mData[8] &&
                mData[9] == m.mData[9] &&
                mData[10] == m.mData[10] &&
                mData[11] == m.mData[11] &&
                mData[12] == m.mData[12] &&
                mData[13] == m.mData[13] &&
                mData[14] == m.mData[14] &&
                mData[15] == m.mData[15]
                )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        bool operator!=( const Matrix44 &m )
        {
            return !( *this == m );
        }

		float& operator[]( int i ) 
		{
			return mData[i];
		}

		const float& operator[]( int i ) const 
		{
			return mData[i];
		}

		operator float*()
		{
			return mData;
		}		

		const float* GetRAWData() const
		{
			return mData;
		}

		Matrix44 operator+=( const float& s ) 
		{
			for ( int i = 0; i < 16; i++ ) 
			{
				mData[i] += s;
			}
			return *this;
		}

		Matrix44 operator-=( const float& s )
		{
			for ( int i=0; i < 16; i++ ) 
			{
				mData[i] -= s;			
			}
			return *this;
		}

		Matrix44 operator*=( const float& s ) 
		{
			for ( int i = 0; i < 16; i++ ) 
			{
				mData[i] *= s;			
			}
			return *this;
		}

		Matrix44 operator/=( const float& s ) 
		{
			for ( int i=0; i < 16; i++ ) 
			{
				mData[i] /= s;			
			}
			return *this;
		}

		Matrix44 operator+( const float& s ) const 
		{
			return Matrix44(*this) += s;
		}

		Matrix44 operator-( const float& s ) const 
		{
			return Matrix44(*this) -= s;
		}

		Matrix44 operator*( const float& s ) const 
		{
			return Matrix44(*this) *= s;
		}

		Matrix44 operator/( const float& s ) const 
		{
			return Matrix44(*this) /= s;
		}
        
#ifdef _ARM_ARCH_7 
        void NEON_Matrix4Vector4Mul(const float* m, const float* v, float* output) const
        {
            __asm volatile 
            (
             // Store m & v - avoiding q4-q7 which need to be preserved - q0 = result
             "vldmia %1, { q8-q11 } \n\t"   // q8-q11 = m
             "vldmia %2, { q1 }             \n\t"   // q1     = v
             
             // result = first column of A x V.x
             "vmul.f32 q0, q8, d2[0]\n\t"
             
             // result += second column of A x V.y
             "vmla.f32 q0, q9, d2[1]\n\t"
             
             // result += third column of A x V.z
             "vmla.f32 q0, q10, d3[0]\n\t"
             
             // result += last column of A x V.w
             "vmla.f32 q0, q11, d3[1]\n\t"
             
             // output = result registers
             "vstmia %0, { q0 }"
             
             : // no output
             : "r" (output), "r" (m), "r" (v)       // input - note *value* of pointer doesn't change
             : "memory", "q0", "q1", "q8", "q9", "q10", "q11" //clobber
             );
        }
#endif

		Vector4 operator*( const Vector4& v ) const 
		{
#ifdef _ARM_ARCH_7
            Vector4 output;
            NEON_Matrix4Vector4Mul( mData, &v[0], &output[0] );
            return output;
#else
			return Vector4(
				mData[0]*v[0] + mData[4]*v[1] + mData[8]*v[2] + mData[12]*v[3],
				mData[1]*v[0] + mData[5]*v[1] + mData[9]*v[2] + mData[13]*v[3],
				mData[2]*v[0] + mData[6]*v[1] + mData[10]*v[2] + mData[14]*v[3],
				mData[3]*v[0] + mData[7]*v[1] + mData[11]*v[2] + mData[15]*v[3]
			);
#endif
		}

		Vector3 operator*( const Vector3& v ) const 
		{
#ifdef _ARM_ARCH_7
            Vector4 output;
            Vector4 input( v[0], v[1], v[2], 0.0f );
            NEON_Matrix4Vector4Mul( mData, &input[0], &output[0] );
            Vector3 result( output[0], output[1], output[3] );
            return result;
#else
			return Vector3(
				mData[0]*v[0] + mData[4]*v[1] + mData[8]*v[2] + mData[12],
				mData[1]*v[0] + mData[5]*v[1] + mData[9]*v[2] + mData[13],
				mData[2]*v[0] + mData[6]*v[1] + mData[10]*v[2] + mData[14]
			);
#endif
		}

		Matrix44 operator+=( const Matrix44& m ) 
		{
			for ( int i=0; i < 16; i++ ) 
			{
				mData[i] += m[i];
			}
			return *this;
		}	

		Matrix44 operator-=( const Matrix44& m ) 
		{
			for ( int i=0; i < 16; i++ ) 
			{
				mData[i] -= m[i];
			}
			return *this;
		}	

		Matrix44 operator*=( const Matrix44& m ) 
		{			
			return *this = *this * m;
		}	

		Matrix44 operator+( const Matrix44& m ) const 
		{
			Matrix44 tempMat;
			for ( int i = 0; i< 16;i++) 
			{
				tempMat[i] = mData[i] + m[i];
			}
			return tempMat;
		}	

		Matrix44 operator-( const Matrix44& m ) const 
		{
			Matrix44 tempMat;
			for ( int i=0; i < 16; i++ ) 
			{
				tempMat[i] = mData[i] - m[i];
			}
			return tempMat;
		}       

		Math::Vector3 GetXAxis() const 
		{ 
			return Math::Vector3( mData[0], mData[1], mData[2] );
		}

		Math::Vector3 GetYAxis() const 
		{ 
			return Math::Vector3( mData[4], mData[5], mData[6] );
		}

		Math::Vector3 GetZAxis() const 
		{ 
			return Math::Vector3( mData[8], mData[9], mData[10] );
		}

		void Translate( float x, float y, float z );
		void Translate( const Vector3& pos );
		void SetTranslation( const Vector3& pos );
		Vector3 GetTranslation() const;
		void Rotate( float degree, float x, float y, float z );
		void SetRotation( const Math::Quaternion &quat );
		void Scale( float x, float y, float z );
		void Scale( float scale );

		void SetReflectPlanar( const Vector4 &normal, const Vector4 &point );
		void PrintMatrix() const;

		void ConvertSingleToDouble( float outMat[4][4], float* inMat );
		void ConvertDoubleToSingle( float* outMat, float inMat[4][4] );

		void GetEulerAngle( float &xAngle, float &yAngle, float &zAngle );

	private:
		// 00 04 08 12
		// 01 05 09 13
		// 02 06 10 14
		// 03 07 11 15		  
		float mData[16];
	};
}

#endif
