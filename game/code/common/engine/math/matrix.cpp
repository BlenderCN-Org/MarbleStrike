////////////////////////////////////////////////////////
//// INCLUDES
////////////////////////////////////////////////////////
//
//#include "common/engine/math/matrix.hpp"
//#include "common/engine/system/assert.hpp"
//#include "common/engine/system/memory.hpp"
//#include "common/engine/system/utilities.hpp"
//#include "common/engine/math/gaussjordan.hpp"
//
////////////////////////////////////////////////////////
//// CLASS METHODS
////////////////////////////////////////////////////////
//
//namespace Math 
//{
//	void TransposeMatrix( int row, int col, float** matrix, float** transpose )
//	{
//		int transposeRow = col;
//		int transposeCol = row;
//		for ( int i = 0; i < transposeRow; i++ )
//		{
//			for ( int j = 0; j < transposeCol; j++ )
//			{
//				transpose[i][j] = matrix[j][i];
//			}
//		}
//	}
//
//	//===========================================================================
//
//	void MultiplyMatrix( int row1, int col1, float** matrix1, int row2, int col2, float** matrix2, float** result )
//	{
//		if ( col1 == row2 )
//		{
//			for ( int r1 = 0; r1 < row1; r1++ )
//			{
//				for ( int c2 = 0; c2 < col2; c2++ )
//				{
//					result[r1][c2] = 0;
//					float temp = result[r1][c2];
//					for ( int n = 0; n < col1; n++ )
//					{
//						result[r1][c2] += matrix1[r1][n] * matrix2[n][c2];
//						temp += matrix1[r1][n] * matrix2[n][c2];
//					}        
//				}
//			}
//		}
//	}
//
//	//===========================================================================
//
//	void SubtractMatrix( int row1, int col1, float** matrix1, int row2, int col2, float** matrix2, float** result )
//	{
//		if ( col1 == col2 && row1 == row2 )
//		{
//			for ( int r = 0; r < row1; r++ )
//			{
//				for ( int c = 0; c < col1; c++ )
//				{
//					result[r][c] = matrix1[r][c] - matrix2[r][c];          
//				}
//			}
//		}
//	}
//
//	//===========================================================================
//
//	void VectorMultiply( const Math::Matrix& matrix, int vectorSize, float* vector, int resultSize, float* result )
//	{
//		if ( matrix.GetNumCols() == vectorSize && matrix.GetNumRows() == resultSize )
//		{
//			for ( int r = 0; r < matrix.GetNumRows(); r++ )
//			{      
//				result[r] = 0;
//				for ( int c = 0; c < matrix.GetNumCols(); c++ )
//				{
//					result[r] += matrix[r][c] * vector[c];
//				}
//			}
//		}
//		else
//		{
//			Assert( false, "" );
//		}
//	}
//
//	//////////////////////////////////////////////////////
//	// Matrix Row x Column Class Implementation
//	//////////////////////////////////////////////////////
//
//	Matrix::Matrix()     
//		: mNumRows( 0 ),
//		mNumCols( 0 ),
//		mData( NULL )
//	{
//	}
//
//	//===========================================================================
//
//	Matrix::Matrix( int rows, int cols )
//		: mNumRows( 0 ),
//		mNumCols( 0 ),
//		mData( NULL )
//	{  
//		Set( rows, cols );
//	}
//
//	//===========================================================================
//
//	Matrix::Matrix( const Matrix &otherMatrix )
//		: mNumRows( 0 ),
//		mNumCols( 0 ),
//		mData( NULL )
//	{
//		Copy( otherMatrix );
//	}
//
//	//===========================================================================
//
//	Matrix::~Matrix()
//	{
//		Clean();
//	}
//
//	//===========================================================================
//
//	void Matrix::Set( int rows, int cols )
//	{
//		Clean();
//
//		mNumRows = rows;
//		mNumCols = cols;
//
//		mData = NEW_PTR( "Matrix Data 1" ) float*[mNumRows];
//		for ( int r = 0; r < mNumRows; r++ )
//		{
//			mData[r] = NEW_PTR( "Matrix Data 2" ) float[mNumCols];
//
//			for ( int c = 0; c < mNumCols; c++ )
//			{
//				mData[r][c] = 0.0f;
//			}
//		}
//	}
//
//	//===========================================================================
//
//	void Matrix::SetIdentity()
//	{
//		if ( mNumRows == mNumCols )
//		{
//			for ( int r = 0; r < mNumRows; r++ )
//			{
//				for ( int c = 0; c < mNumCols; c++ )
//				{
//					if ( r == c )
//					{
//						mData[r][c] = 1.0f;
//					}
//					else
//					{
//						mData[r][c] = 0.0f;
//					}
//				}
//			}
//		}
//		else
//		{
//			Assert( false, "Cannot set identity for non-square matrix.\n" );
//		}
//	}
//
//	//===========================================================================
//
//	Matrix& Matrix::operator=( const Matrix& otherMatrix )
//	{
//		//
//		// Check for self assignment
//		//
//		if ( this != &otherMatrix )
//		{
//			Copy( otherMatrix );
//		}
//
//		return *this;
//	}
//
//	//===========================================================================
//
//	Matrix Matrix::operator* ( const Matrix& m ) const
//	{     
//		Matrix result( mNumRows, m.GetNumCols() );
//		if ( mNumCols == m.GetNumRows() )
//		{
//			for ( int r1 = 0; r1 < mNumRows; r1++ )
//			{
//				for ( int c2 = 0; c2 < m.GetNumCols(); c2++ )
//				{
//					result[r1][c2] = 0;          
//					for ( int n = 0; n < mNumCols; n++ )
//					{
//						result[r1][c2] += mData[r1][n] * m[n][c2];
//					}        
//				}
//			}
//		}
//		else
//		{
//			Assert( false, "Cannot mutiply matrices\n" );
//		}
//
//		return result;
//	}
//
//	//===========================================================================
//
//	void Matrix::PrintMatrix() const
//	{
//		System::PrintToConsole( "\n" );
//
//		for ( int r = 0; r < mNumRows; r++ )
//		{
//			for ( int c = 0; c < mNumCols; c++ )
//			{
//				System::PrintToConsole( "%f ", mData[r][c] );
//			}
//			System::PrintToConsole( "\n" );
//		}    
//		System::PrintToConsole( "\n" );
//	}
//
//	//===========================================================================
//
//	Matrix Matrix::Transpose()
//	{
//		Matrix transpose( mNumCols, mNumRows );
//		for ( int r = 0; r < transpose.GetNumRows(); r++ )
//		{
//			for ( int c = 0; c < transpose.GetNumCols(); c++ )
//			{
//				transpose[r][c] = mData[c][r];
//			}
//		}
//
//		return transpose;
//	}
//
//	//===========================================================================
//
//	Matrix Matrix::Inverse()
//	{
//		Matrix inverse( *this );
//		if ( mNumRows == mNumCols )
//		{      
//			Matrix identity( mNumCols, mNumRows );
//			identity.SetIdentity();    
//
//			GaussJordan( inverse, mNumRows, identity, mNumRows );
//		}
//		else
//		{
//			Assert( false, "Cannot invert a non-square matrix.\n" );
//		}
//		return inverse;
//	}
//
//	//===========================================================================
//
//	void Matrix::Copy( const Matrix &otherMatrix )
//	{
//		Clean();
//
//		mNumRows = otherMatrix.GetNumRows();
//		mNumCols = otherMatrix.GetNumCols();
//
//		mData = NEW_PTR( "Matrix Data 1" ) float*[mNumRows];
//		for ( int r = 0; r < mNumRows; r++ )
//		{
//			mData[r] = NEW_PTR( "Matrix Data 2" ) float[mNumCols];      
//		}    
//
//		for ( int r = 0; r < mNumRows; r++ )
//		{
//			for ( int c = 0; c < mNumCols; c++ )
//			{
//				mData[r][c] = otherMatrix[r][c];
//			}
//		}
//	}
//
//	//===========================================================================
//
//	void Matrix::Clean()
//	{
//		for ( int r = 0; r < mNumRows; r++ )
//		{
//			DELETE_PTR_ARRAY( mData[r] );
//		}
//		DELETE_PTR_ARRAY( mData );
//
//		mNumRows = 0;
//		mNumCols = 0;
//	}
//
//}