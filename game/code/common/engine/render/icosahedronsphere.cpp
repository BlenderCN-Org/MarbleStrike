//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/icosahedronsphere.h"
#include "common/engine/system/memory.hpp"
#include <math.h>
#include <stdio.h>
#include <string.h>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CVARS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

IcosahedronSphere::IcosahedronSphere()
{	
}

//===========================================================================

IcosahedronSphere::~IcosahedronSphere()
{	
	DELETE_PTR_ARRAY( mVertices );
	DELETE_PTR_ARRAY( mFaces );
}

//===========================================================================

void IcosahedronSphere::Initialize( float radius, int subdivision ) 
{ 
	InitializeBase( radius );

	for ( int i = 0; i < subdivision; ++i )
	{
		Subdivide();
	}
} 

//===========================================================================

void IcosahedronSphere::InitializeBase( float radius ) 
{ 
	float t = ( 1.0f + sqrt( 5.0f ) ) / 2.0f;
	float tau = t / sqrt( 1.0f + t * t ) * radius;
	float one = 1.0f / sqrt( 1.0f + t * t ) * radius;

	float icosahedron_vertices[] = {
		tau, one, 0.0f,
		-tau, one, 0.0f,
		-tau, -one, 0.0f,
		tau, -one, 0.0f,
		one, 0.0f,  tau,
		one, 0.0f, -tau,
		-one, 0.0f, -tau,
		-one, 0.0f, tau,
		0.0f, tau, one,
		0.0f, -tau, one,
		0.0f, -tau, -one,
		0.0f, tau, -one
	};

	int icosahedron_faces[] = {
		4, 8, 7,
		4, 7, 9,
		5, 6, 11,
		5, 10, 6,
		0, 4, 3,
		0, 3, 5,
		2, 7, 1,
		2, 1, 6,
		8, 0, 11,
		8, 11, 1,
		9, 10, 3,
		9, 2, 10,
		8, 4, 0,
		11, 0, 5,
		4, 9, 3,
		5, 3, 10,
		7, 8, 1,
		6, 1, 11,
		7, 2, 9,
		6, 10, 2
	};

	mNumVertices = 12; 
	mNumFaces = 20;
	mNumEdges = 30;
	mVertices = NEW_PTR( "IcosahedronSphere::mVertices" ) float[ 3 * mNumVertices ]; 
	mFaces = NEW_PTR( "IcosahedronSphere::mFaces" ) int[ 3 * mNumFaces ];
	memcpy( (void*)mVertices, (void*)icosahedron_vertices, 3 * mNumVertices * sizeof( float ) ); 
	memcpy( (void*)mFaces, (void*)icosahedron_faces, 3 * mNumFaces * sizeof( int ) ); 
} 

//===========================================================================

void IcosahedronSphere::Subdivide() 
{ 
	int n_vertices_new = mNumVertices + 2 * mNumEdges; 
	int n_faces_new = 4 * mNumFaces; 
	int i; 

	mEdgeWalk = 0; 
	mNumEdges = 2 * mNumVertices + 3 * mNumFaces; 
	mStart = NEW_PTR( "IcosahedronSphere::mStart" ) int[mNumEdges];
	mEnd = NEW_PTR( "IcosahedronSphere::mEnd" ) int[mNumEdges];
	mMidpoint = NEW_PTR( "IcosahedronSphere::mMidpoint" ) int[mNumEdges];

	int* faces_old = NEW_PTR( "IcosahedronSphere::faces_old" ) int[3 * mNumFaces];				
	faces_old = ( int* )memcpy( ( void* )faces_old, ( void* )mFaces, 3 * mNumFaces * sizeof( int ) );

	float* newVertices = NEW_PTR( "IcosahedronSphere::newVertices" ) float[ 3 * n_vertices_new]; 
	memcpy( (void*)newVertices, (void*)mVertices, 3 * mNumVertices * sizeof( float ) );
	DELETE_PTR_ARRAY( mVertices );
	mVertices = newVertices;

	int* newFaces = NEW_PTR( "IcosahedronSphere::newFaces" ) int[ 3 * n_faces_new]; 
	memcpy( (void*)newFaces, (void*)mFaces, 3 * mNumFaces * sizeof( int ) );
	DELETE_PTR_ARRAY( mFaces );
	mFaces = newFaces;

	n_faces_new = 0; 

	for ( i = 0; i < mNumFaces; i++ ) 
	{ 
		int a = faces_old[3*i]; 
		int b = faces_old[3*i+1]; 
		int c = faces_old[3*i+2]; 

		int ab_midpoint = SearchMidpoint( b, a ); 
		int bc_midpoint = SearchMidpoint( c, b ); 
		int ca_midpoint = SearchMidpoint( a, c ); 

		mFaces[3*n_faces_new] = a; 
		mFaces[3*n_faces_new+1] = ab_midpoint; 
		mFaces[3*n_faces_new+2] = ca_midpoint; 
		n_faces_new++; 
		mFaces[3*n_faces_new] = ca_midpoint; 
		mFaces[3*n_faces_new+1] = ab_midpoint; 
		mFaces[3*n_faces_new+2] = bc_midpoint; 
		n_faces_new++; 
		mFaces[3*n_faces_new] = ca_midpoint; 
		mFaces[3*n_faces_new+1] = bc_midpoint; 
		mFaces[3*n_faces_new+2] = c; 
		n_faces_new++; 
		mFaces[3*n_faces_new] = ab_midpoint; 
		mFaces[3*n_faces_new+1] = b; 
		mFaces[3*n_faces_new+2] = bc_midpoint; 
		n_faces_new++; 
	} 

	mNumFaces = n_faces_new; 

	DELETE_PTR_ARRAY( mStart ); 
	DELETE_PTR_ARRAY( mEnd ); 
	DELETE_PTR_ARRAY( mMidpoint ); 

	DELETE_PTR_ARRAY( faces_old ); 
} 

//===========================================================================

int	IcosahedronSphere::SearchMidpoint( int indexStart, int indexEnd ) 
{ 
	int i;
	for ( i = 0; i < mEdgeWalk; i++ ) 
	{
		if (
			( mStart[i] == indexStart && mEnd[i] == indexEnd ) || 
			( mStart[i] == indexEnd && mEnd[i] == indexStart )
			) 
		{
			int res = mMidpoint[i];

			//
			// update the arrays
			//
			mStart[i] = mStart[mEdgeWalk-1];
			mEnd[i] = mEnd[mEdgeWalk-1];
			mMidpoint[i] = mMidpoint[mEdgeWalk-1];
			mEdgeWalk--;

			return res; 
		}
	}

	//
	// vertex not in the list, so we add it
	//
	mStart[mEdgeWalk] = indexStart;
	mEnd[mEdgeWalk] = indexEnd; 
	mMidpoint[mEdgeWalk] = mNumVertices; 

	//
	// create new vertex
	//
	mVertices[3 * mNumVertices] = (mVertices[3 * indexStart] + mVertices[3 * indexEnd]) / 2.0f;
	mVertices[3 * mNumVertices+1] = (mVertices[3 * indexStart + 1] + mVertices[3 * indexEnd + 1]) / 2.0f;
	mVertices[3 * mNumVertices+2] = (mVertices[3 * indexStart + 2] + mVertices[3 * indexEnd + 2]) / 2.0f;

	//
	// normalize the new vertex
	//
	float length = sqrt(
		mVertices[3 * mNumVertices] * mVertices[3 * mNumVertices] +
		mVertices[3 * mNumVertices + 1] * mVertices[3 * mNumVertices+1] +
		mVertices[3 * mNumVertices + 2] * mVertices[3 * mNumVertices+2]
	);
	length = 1/length;

	mVertices[3 * mNumVertices] *= length;
	mVertices[3 * mNumVertices+1] *= length;
	mVertices[3 * mNumVertices+2] *= length;

	mNumVertices++;
	mEdgeWalk++;
	return mMidpoint[mEdgeWalk-1];
} 
