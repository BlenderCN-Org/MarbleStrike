//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/math/gaussjordan.hpp"
#include "common/engine/math/quaternion.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math 
{
	Matrix44::~Matrix44()
	{
	}

	//===========================================================================

	void Matrix44::SetViewport( float width, float height )
	{
		mData[0] = width / 2.0f;
		mData[1] = 0;
		mData[2] = 0;
		mData[3] = 0;

		mData[4] = 0;
		mData[5] = height / 2.0f;
		mData[6] = 0;
		mData[7] = 0;

		mData[8] = 0;
		mData[9] = 0;
		mData[10] = 1.0f;
		mData[11] = 0;

		mData[12] = width / 2.0f;
		mData[13] = height / 2.0f;
		mData[14] = 0;
		mData[15] = 1.0f;
	}
    
#if defined( _ARM_ARCH_7 ) && defined( PLAT_IOS )
    void NEON_Matrix4Mul(const float* a, const float* b, float* output )
    {
        __asm volatile 
        (
         // Store A & B leaving room for q4-q7, which should be preserved
         "vldmia %1, { q0-q3 } \n\t"
         "vldmia %2, { q8-q11 }\n\t"
         
         // result = first column of B x first row of A
         "vmul.f32 q12, q8, d0[0]\n\t"
         "vmul.f32 q13, q8, d2[0]\n\t"
         "vmul.f32 q14, q8, d4[0]\n\t"
         "vmul.f32 q15, q8, d6[0]\n\t"
         
         // result += second column of B x second row of A
         "vmla.f32 q12, q9, d0[1]\n\t"
         "vmla.f32 q13, q9, d2[1]\n\t"
         "vmla.f32 q14, q9, d4[1]\n\t"
         "vmla.f32 q15, q9, d6[1]\n\t"
         
         // result += third column of B x third row of A
         "vmla.f32 q12, q10, d1[0]\n\t"
         "vmla.f32 q13, q10, d3[0]\n\t"
         "vmla.f32 q14, q10, d5[0]\n\t"
         "vmla.f32 q15, q10, d7[0]\n\t"
         
         // result += last column of B x last row of A
         "vmla.f32 q12, q11, d1[1]\n\t"
         "vmla.f32 q13, q11, d3[1]\n\t"
         "vmla.f32 q14, q11, d5[1]\n\t"
         "vmla.f32 q15, q11, d7[1]\n\t"
         
         // output = result registers
         "vstmia %0, { q12-q15 }"
         : // no output
         : "r" (output), "r" (a), "r" (b)       // input - note *value* of pointer doesn't change
         : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15" //clobber
         );
    }
#endif    

	//===========================================================================

	Matrix44 Matrix44::operator* ( const Matrix44& m ) const 
	{
		Matrix44 tempMat;
#if defined( _ARM_ARCH_7 ) && defined( PLAT_IOS )
        float output[16];
        NEON_Matrix4Mul( m.GetRAWData(), mData,output );
        tempMat.Set(output);
#else        
		tempMat[0] = mData[0]*m[0] + mData[4]*m[1] + mData[8]*m[2] + mData[12]*m[3];
		tempMat[1] = mData[1]*m[0] + mData[5]*m[1] + mData[9]*m[2] + mData[13]*m[3];
		tempMat[2] = mData[2]*m[0] + mData[6]*m[1] + mData[10]*m[2] + mData[14]*m[3];
		tempMat[3] = mData[3]*m[0] + mData[7]*m[1] + mData[11]*m[2] + mData[15]*m[3];

		tempMat[4] = mData[0]*m[4] + mData[4]*m[5] + mData[8]*m[6] + mData[12]*m[7];
		tempMat[5] = mData[1]*m[4] + mData[5]*m[5] + mData[9]*m[6] + mData[13]*m[7];
		tempMat[6] = mData[2]*m[4] + mData[6]*m[5] + mData[10]*m[6] + mData[14]*m[7];
		tempMat[7] = mData[3]*m[4] + mData[7]*m[5] + mData[11]*m[6] + mData[15]*m[7];

		tempMat[8] = mData[0]*m[8] + mData[4]*m[9] + mData[8]*m[10] + mData[12]*m[11];
		tempMat[9] = mData[1]*m[8] + mData[5]*m[9] + mData[9]*m[10] + mData[13]*m[11];
		tempMat[10] = mData[2]*m[8] + mData[6]*m[9] + mData[10]*m[10] + mData[14]*m[11];
		tempMat[11] = mData[3]*m[8] + mData[7]*m[9] + mData[11]*m[10] + mData[15]*m[11];

		tempMat[12] = mData[0]*m[12] + mData[4]*m[13] + mData[8]*m[14] + mData[12]*m[15];
		tempMat[13] = mData[1]*m[12] + mData[5]*m[13] + mData[9]*m[14] + mData[13]*m[15];
		tempMat[14] = mData[2]*m[12] + mData[6]*m[13] + mData[10]*m[14] + mData[14]*m[15];
		tempMat[15] = mData[3]*m[12] + mData[7]*m[13] + mData[11]*m[14] + mData[15]*m[15];
#endif            

		return tempMat;
	}

	//===========================================================================

	Matrix44 Matrix44::InverseAffine()
	{
		Matrix44 result;

		result[0] = mData[0];
		result[1] = mData[4];
		result[2] = mData[8];

		result[4] = mData[1];
		result[5] = mData[5];
		result[6] = mData[9];

		result[8] = mData[2];
		result[9] = mData[6];
		result[10] = mData[10];

		result[3] = 0;
		result[7] = 0;
		result[11] = 0;
		result[15] = 1;

		float Tx = mData[12];
		float Ty = mData[13];
		float Tz = mData[14];

		result[12] = - ( Tx * mData[0] + Ty * mData[1] + Tz * mData[2] );
		result[13] = - ( Tx * mData[4] + Ty * mData[5] + Tz * mData[6] );
		result[14] = - ( Tx * mData[8] + Ty * mData[9] + Tz * mData[10] );

		Set( result );

		return result;
	}

	//===========================================================================

	void Matrix44::Inverse()
	{		
		Matrix44 identityMatrix;
		float identity[4][4];
		float inverse[4][4];

		ConvertSingleToDouble( identity, identityMatrix );
		ConvertSingleToDouble( inverse, mData );
		GaussJordan( inverse, 4, identity, 4 );
		ConvertDoubleToSingle( mData, inverse );
	}

	//===========================================================================

	void Matrix44::Translate( float x, float y, float z )
	{
		Math::Matrix44 mat;
		mat.SetIdentity();

		mat[12] = x;
		mat[13] = y;
		mat[14] = z;

		*this = *this * mat;
	}

	//===========================================================================

	void Matrix44::Translate( const Vector3& pos )
	{
		Translate( pos[0], pos[1], pos[2] );
	}

	//===========================================================================

	void Matrix44::SetTranslation( const Vector3& pos )
	{
		mData[12] = pos[0];
		mData[13] = pos[1];
		mData[14] = pos[2];
	}

	//===========================================================================

	Vector3 Matrix44::GetTranslation() const
	{
		return Vector3( mData[12], mData[13], mData[14] );
	}

	//===========================================================================

	void Matrix44::Rotate( float degree, float x, float y, float z )
	{
		Math::Quaternion rotation;        
		rotation.Set( Vector3( x, y, z ), degree );
		rotation = rotation.Normalize();

		*this = *this * rotation.GetMatrix();
	}

	//===========================================================================

	void Matrix44::SetRotation( const Math::Quaternion &quat )
	{
		const Math::Matrix44 rotationMatrix = quat.GetMatrix();

		mData[0] = rotationMatrix[0];
		mData[1] = rotationMatrix[1];
		mData[2] = rotationMatrix[2];

		mData[4] = rotationMatrix[4];
		mData[5] = rotationMatrix[5];
		mData[6] = rotationMatrix[6];

		mData[8] = rotationMatrix[8];
		mData[9] = rotationMatrix[9];
		mData[10] = rotationMatrix[10];
	}

	//===========================================================================

	void Matrix44::Scale( float x, float y, float z )
	{
		Math::Matrix44 mat;
		mat.SetIdentity();

		mat[0] *= x;
		mat[5] *= y;
		mat[10] *= z;

		*this = *this * mat;
	}

	//===========================================================================

	void Matrix44::Scale( float scale )
	{
		Math::Matrix44 mat;
		mat.SetIdentity();

		mat[0] *= scale;
		mat[5] *= scale;
		mat[10] *= scale;

		*this = *this * mat;
	}

	//===========================================================================

	void Matrix44::SetReflectPlanar( const Vector4 &normal, const Vector4 &point )
	{
		float k = Vector4::Dot( normal, point );

		mData[0] = 1.0f - 2.0f * normal[0] * normal[0];
		mData[1] = -2.0f * normal[1] * normal[0];
		mData[2] = -2.0f * normal[2] * normal[0];
		mData[3] = 0.0f;

		mData[4] = -2.0f * normal[0] * normal[1];
		mData[5] = 1.0f - 2.0f * normal[1] * normal[1];
		mData[6] = -2.0f * normal[2] * normal[1];
		mData[7] = 0.0f;

		mData[8] = -2.0f * normal[0] * normal[2];
		mData[9] = -2.0f * normal[1] * normal[2];
		mData[10] = 1.0f - 2.0f * normal[2] * normal[2];
		mData[11] = 0.0f;

		mData[12] = 2.0f * normal[0] * k;
		mData[13] = 2.0f * normal[1] * k;
		mData[14] = 2.0f * normal[2] * k;
		mData[15] = 1.0f;
	}

	//===========================================================================

	void Matrix44::PrintMatrix() const
	{
		System::PrintToConsole( "\n" );
		System::PrintToConsole( "%f %f %f %f", mData[0], mData[4], mData[8] , mData[12] );
		System::PrintToConsole( "%f %f %f %f", mData[1], mData[5], mData[9] , mData[13] );
		System::PrintToConsole( "%f %f %f %f", mData[2], mData[6], mData[10], mData[14] );
		System::PrintToConsole( "%f %f %f %f", mData[3], mData[7], mData[11], mData[15] );
		System::PrintToConsole( "\n" );
	}

	//===========================================================================

	void Matrix44::ConvertSingleToDouble( float outMat[4][4], float* inMat )
	{
		outMat[0][0] = inMat[0];
		outMat[1][0] = inMat[1];
		outMat[2][0] = inMat[2];
		outMat[3][0] = inMat[3];

		outMat[0][1] = inMat[4];
		outMat[1][1] = inMat[5];
		outMat[2][1] = inMat[6];
		outMat[3][1] = inMat[7];

		outMat[0][2] = inMat[8];
		outMat[1][2] = inMat[9];
		outMat[2][2] = inMat[10];
		outMat[3][2] = inMat[11];

		outMat[0][3] = inMat[12];
		outMat[1][3] = inMat[13];
		outMat[2][3] = inMat[14];
		outMat[3][3] = inMat[15];
	}

	//===========================================================================

	void Matrix44::ConvertDoubleToSingle( float* outMat, float inMat[4][4] )
	{
		outMat[0] = inMat[0][0];
		outMat[1] = inMat[1][0];
		outMat[2] = inMat[2][0];
		outMat[3] = inMat[3][0];

		outMat[4] = inMat[0][1];
		outMat[5] = inMat[1][1];
		outMat[6] = inMat[2][1];
		outMat[7] = inMat[3][1];

		outMat[8] = inMat[0][2];
		outMat[9] = inMat[1][2];
		outMat[10] = inMat[2][2];
		outMat[11] = inMat[3][2];

		outMat[12] = inMat[0][3];
		outMat[13] = inMat[1][3];
		outMat[14] = inMat[2][3];
		outMat[15] = inMat[3][3];
	}

	//===========================================================================

	void Matrix44::GetEulerAngle( float &xAngle, float &yAngle, float &zAngle )
	{
		float angleX = atan2f( mData[9], mData[10] );
		float c2 = sqrtf( mData[0] * mData[0] + mData[4] * mData[4] );
		float angleY = atan2f( -mData[8], c2 );
		float s1 = sinf( angleX );
		float c1 = cosf( angleX );
		float angleZ = atan2f( s1 * mData[2] - c1 * mData[1], c1 * mData[5] - s1 * mData[6] );

		xAngle = angleX;
		yAngle = angleY;
		zAngle = angleZ;
	}
}
