#ifndef JACOBI_HPP
#define JACOBI_HPP

/////////////////////////
// Includes
/////////////////////////

#include <math.h>
#include <stdio.h>

/////////////////////////
// Defines
/////////////////////////

#define ROTATE(a,i,j,k,l) g=a[i][j]; h=a[k][l]; a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau);

/////////////////////////
// Public Functions
/////////////////////////

///
/// \brief Jacobi Transform
///
void jacobi( float** a, int n, float d[], float** v, int* nrot ) 
{
	int j, iq, ip, i;
	float tresh, theta, tau, t, sm, s, h, g, c, *b, *z;

	b = new float[n+1];
	z = new float[n+1];

	for ( ip = 1; ip <= n; ip++ ) 
	{
		for ( iq = 1; iq <= n; iq++ ) v[ip][iq]=0.0;
		v[ip][ip] = 1.0;
	}

	for ( ip = 1; ip <= n; ip++ ) 
	{
		b[ip] = d[ip] = a[ip][ip];
		z[ip] = 0.0;
	}

	*nrot = 0;
	for ( i = 1; i <= 50; i++ ) 
	{
		sm = 0.0;
		for ( ip = 1; ip <= n - 1; ip++ ) 
		{
			for ( iq = ip + 1; iq <= n; iq++ ) sm += fabs( a[ip][iq] );
		}

		if ( sm == 0.0 ) 
		{
			if ( b != NULL ) delete [] b;
			if ( z != NULL ) delete [] z;
			return;
		}

		if ( i < 4 )
		{
			tresh = 0.2f * sm/( n * n );
		}
		else
		{
			tresh = 0.0;
		}

		for ( ip = 1; ip <= n - 1; ip++ ) 
		{
			for ( iq = ip + 1; iq <= n; iq++ ) 
			{
				g = 100.0f * fabs( a[ip][iq] );
				if ( i > 4 && (float)( fabs( d[ip] ) + g ) == (float)fabs( d[ip] ) && (float)( fabs( d[iq] ) + g ) == (float)fabs( d[iq] ) )
				{
					a[ip][iq] = 0.0;
				}
				else if ( fabs( a[ip][iq] ) > tresh ) 
				{
					h = d[iq] - d[ip];
					if ( (float)( fabs( h ) + g ) == (float)fabs( h ) )
					{
						t = ( a[ip][iq] )/h;
					}
					else 
					{
						theta = 0.5f*h/( a[ip][iq] );
						t = 1.0f/( fabs( theta ) + sqrt( 1.0f + theta*theta ) );
						if ( theta < 0.0 ) t = -t;
					}
					c = 1.0f/sqrt( 1 + t * t );
					s = t * c;
					tau = s/( 1.0f + c );
					h = t * a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq] = 0.0;
					for ( j = 1; j <= ip - 1; j++ ) 
					{
						ROTATE( a, j, ip, j, iq );
					}
					for ( j = ip + 1; j <= iq - 1; j++ ) 
					{
						ROTATE( a, ip, j, j, iq );
					}
					for ( j = iq + 1; j <= n; j++ ) 
					{
						ROTATE( a, ip, j, iq, j );
					}
					for ( j = 1; j <= n; j++ ) 
					{
						ROTATE( v, j, ip, j, iq );
					}
					++(*nrot);
				}
			}
		}
		for ( ip = 1; ip <= n; ip++ ) 
		{
			b[ip] += z[ip];
			d[ip] = b[ip];
			z[ip] = 0.0;
		}
	}

	printf( "ERROR: Too many iterations in routine jacobi" );

	if ( b != NULL ) delete [] b;
	if ( z != NULL ) delete [] z;
}

#endif