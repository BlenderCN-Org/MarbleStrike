//#ifndef MATRIX_HPP
//#define MATRIX_HPP
//
////////////////////////////////////////////////////////
//// INCLUDES
////////////////////////////////////////////////////////
//
//#include <math.h>
//#include "vector4.hpp"
//#include "vector3.hpp"
//
////////////////////////////////////////////////////////
//// CLASSES
////////////////////////////////////////////////////////
//
//namespace Math
//{
//	class Matrix;
//}
//
//namespace Math 
//{ 
//	void TransposeMatrix( int row, int col, float** matrix, float** transpose );
//	void MultiplyMatrix( int row1, int col1, float** matrix1, int row2, int col2, float** matrix2, float** result );
//	void SubtractMatrix( int row1, int col1, float** matrix1, int row2, int col2, float** matrix2, float** result );
//	void VectorMultiply( const Math::Matrix& matrix, int vectorSize, float* vector, int resultSize, float* result );
//
//	//////////////////////////////////////////////////////
//	/// Matrix Row x Column Class.
//	//////////////////////////////////////////////////////
//	class Matrix
//	{
//	public:
//
//		//
//		// Default constructor
//		//
//		Matrix();
//
//		//
//		// Default constructor
//		//
//		Matrix( int rows, int cols );
//
//		//
//		// Copy constructor
//		//
//		Matrix( const Matrix& otherMatrix );
//
//		//
//		// Destructor
//		//
//		~Matrix();  
//
//		void Set( int rows, int cols );
//
//		void SetIdentity();
//
//		Matrix& operator=( const Matrix& otherMatrix );
//
//		//
//		// Cast operator to array
//		//
//		inline operator float**() 
//		{
//			return mData;
//		}
//
//		//
//		// Read-Write
//		//
//		inline float*& operator[]( int i ) 
//		{
//			return mData[i];
//		}
//
//		//
//		// Read-Only
//		//
//		inline float*& operator[]( int i ) const
//		{			
//			return mData[i];
//		}
//
//		Matrix operator* ( const Matrix& m ) const;
//
//		int GetNumRows() const
//		{
//			return mNumRows;
//		}
//
//		int GetNumCols() const
//		{
//			return mNumCols;
//		}
//
//		Matrix Transpose();
//		Matrix Inverse();
//
//		void PrintMatrix() const;
//
//	private:
//		//
//		// Copy a matrix into another
//		//
//		void Copy( const Matrix& otherMatrix );
//
//		//
//		// Free allocated memory
//		//
//		void Clean();
//
//		int     mNumRows;
//		int     mNumCols;
//		float** mData;
//
//	};
//
//}
//
//#endif