//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/gaussjordan.hpp"
#include "common/engine/system/memory.hpp"
#include <math.h>
#include <stdio.h>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace Math
{
	void GaussJordan( float** a, int n, float** b, int m ) 
	{
		int* indxc;
		int* indxr;
		int* ipiv;
		int i;
		int icol = 0;
		int irow = 0;
		int j;
		int k;
		int l;
		int ll;
		float big;
		float dum;
		float pivinv;
		float temp;

		indxc = NEW_PTR( "indxc" ) int[n]; 
		indxr = NEW_PTR( "indxr" ) int[n];
		ipiv = NEW_PTR( "ipiv" ) int[n];

		for ( j = 0; j < n; j++ ) 
		{
			ipiv[j] = 0;
		}

		for ( i = 0; i < n; i++ ) 
		{ 
			big = 0.0f;

			for ( j = 0; j < n; j++ )
			{
				if ( ipiv[j] != 1 )
				{
					for ( k = 0; k < n; k++ ) 
					{
						if ( ipiv[k] == 0 ) 
						{
							if ( fabs( a[j][k] ) >= big ) 
							{
								big = static_cast<float>( fabs( a[j][k] ) );
								irow = j;
								icol = k;
							}
						} 
						else if ( ipiv[k] > 1 ) 
						{ 
							printf( "ERROR: gaussj: Singular Matrix-1" );
						}
					}
				}
			}
			++( ipiv[icol] );

			if ( irow != icol ) 
			{
				for ( l = 0; l < n; l++ ) 
				{
					SWAP( a[irow][l], a[icol][l] )
				}

				for ( l = 0; l < m; l++ ) 
				{
					SWAP( b[irow][l], b[icol][l] )
				}
			}

			indxr[i] = irow; 
			indxc[i] = icol;

			if ( a[icol][icol] == 0.0f ) 
			{
				printf( "ERROR: gaussj: Singular Matrix-2" );
			}

			pivinv = 1.0f/a[icol][icol];
			a[icol][icol] = 1.0f;
			for ( l = 0; l < n; l++ ) 
			{
				a[icol][l] *= pivinv;
			}

			for ( l = 0; l < m; l++ ) 
			{
				b[icol][l] *= pivinv;
			}

			for ( ll = 0; ll < n; ll++ )
			{
				if ( ll != icol ) 
				{ 
					dum = a[ll][icol];
					a[ll][icol] = 0.0f;
					for ( l = 0; l < n; l++ ) 
					{
						a[ll][l] -= a[icol][l] * dum;
					}

					for ( l = 0; l < m; l++ ) 
					{
						b[ll][l] -= b[icol][l] * dum;
					}
				}
			}
		}

		for ( l = n - 1; l >= 0; l-- ) 
		{
			if ( indxr[l] != indxc[l] )
			{
				for ( k = 0; k < n; k++ )
				{
					SWAP( a[k][indxr[l]], a[k][indxc[l]] );
				}
			}
		}

		DELETE_PTR_ARRAY( ipiv );
		DELETE_PTR_ARRAY( indxr );
		DELETE_PTR_ARRAY( indxc );
	}

	void GaussJordan( float a[4][4], int n, float b[4][4], int m ) 
	{
		int* indxc;
		int* indxr;
		int* ipiv;
		int i;
		int icol = 0;
		int irow = 0;
		int j;
		int k;
		int l;
		int ll;
		float big;
		float dum;
		float pivinv;
		float temp;

		indxc = NEW_PTR( "indxc" ) int[n]; 
		indxr = NEW_PTR( "indxr" ) int[n];
		ipiv = NEW_PTR( "ipiv" ) int[n];

		for ( j = 0; j < n; j++ ) 
		{
			ipiv[j] = 0;
		}

		for ( i = 0; i < n; i++ ) 
		{ 
			big = 0.0f;

			for ( j = 0; j < n; j++ )
			{
				if ( ipiv[j] != 1 )
				{
					for ( k = 0; k < n; k++ ) 
					{
						if ( ipiv[k] == 0 ) 
						{
							if ( fabs( a[j][k] ) >= big ) 
							{
								big = static_cast<float>( fabs( a[j][k] ) );
								irow = j;
								icol = k;
							}
						} 
						else if ( ipiv[k] > 1 ) 
						{ 
							printf( "ERROR: gaussj: Singular Matrix-1" );
						}
					}
				}
			}
			++( ipiv[icol] );

			if ( irow != icol ) 
			{
				for ( l = 0; l < n; l++ ) 
				{
					SWAP( a[irow][l], a[icol][l] )
				}

				for ( l = 0; l < m; l++ ) 
				{
					SWAP( b[irow][l], b[icol][l] )
				}
			}

			indxr[i] = irow; 
			indxc[i] = icol;

			if ( a[icol][icol] == 0.0f ) 
			{
				printf( "ERROR: gaussj: Singular Matrix-2" );
			}

			pivinv = 1.0f/a[icol][icol];
			a[icol][icol] = 1.0f;
			for ( l = 0; l < n; l++ ) 
			{
				a[icol][l] *= pivinv;
			}

			for ( l = 0; l < m; l++ ) 
			{
				b[icol][l] *= pivinv;
			}

			for ( ll = 0; ll < n; ll++ )
			{
				if ( ll != icol ) 
				{ 
					dum = a[ll][icol];
					a[ll][icol] = 0.0f;
					for ( l = 0; l < n; l++ ) 
					{
						a[ll][l] -= a[icol][l] * dum;
					}

					for ( l = 0; l < m; l++ ) 
					{
						b[ll][l] -= b[icol][l] * dum;
					}
				}
			}
		}

		for ( l = n - 1; l >= 0; l-- ) 
		{
			if ( indxr[l] != indxc[l] )
			{
				for ( k = 0; k < n; k++ )
				{
					SWAP( a[k][indxr[l]], a[k][indxc[l]] );
				}
			}
		}

		DELETE_PTR_ARRAY( ipiv );
		DELETE_PTR_ARRAY( indxr );
		DELETE_PTR_ARRAY( indxc );
	}

}
