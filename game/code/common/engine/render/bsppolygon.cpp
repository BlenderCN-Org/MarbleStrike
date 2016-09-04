//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/bsppolygon.hpp"
#include "common/engine/system/memory.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BSPPolygon::BSPPolygon()
:	mNumVertices( 0 )
,	mVertexList( NULL )
,	mTriangleIndices( NULL )
,	mNumTriangles( 0 )
,	mIsSplitter( false )
{

}

//===========================================================================

BSPPolygon::BSPPolygon( int numVertices )
{
	mNumVertices = numVertices;
	mVertexList = NEW_PTR( "Vertex List" ) Math::Vector3[mNumVertices]; 

	mNumTriangles = mNumVertices - 2;
	mTriangleIndices = NEW_PTR( "Indices List" ) int[mNumTriangles * 3];

	mIsSplitter = false;
}

//===========================================================================

BSPPolygon::~BSPPolygon()
{
	Clean();
}

//===========================================================================

BSPPolygon& BSPPolygon::operator=( const BSPPolygon& other )
{
	if ( this != &other )
	{
		Clean();

		mNumVertices = other.mNumVertices;

		mVertexList = NEW_PTR( "Vertex List" ) Math::Vector3[mNumVertices]; 
		for ( int i = 0; i < mNumVertices; ++i )
		{
			mVertexList[i] = other.mVertexList[i];
		}

		mNumTriangles = mNumVertices - 2;

		int numIndices = mNumTriangles * 3;
		mTriangleIndices = NEW_PTR( "Indices List" ) int[numIndices];
		for ( int i = 0; i < numIndices; ++i )
		{
			mTriangleIndices[i] = other.mTriangleIndices[i];
		}

		mNormal = other.mNormal;

		mIsSplitter = other.mIsSplitter;
	}

	return *this;
}

//===========================================================================

void BSPPolygon::Invert()
{
	mNormal *= -1;
	
	for ( int i = 0; i < GetNumTriangles(); i++ )
	{
		int temp = mTriangleIndices[ i * 3 ];
		mTriangleIndices[ i * 3 ] = mTriangleIndices[ i * 3 + 2 ];
		mTriangleIndices[ i * 3 + 2 ] = temp;
	}
}

//===========================================================================

void BSPPolygon::Transform( const Math::Matrix44 &transform )
{
	mNormal[3] = 0;
	mNormal = transform * mNormal;

	for ( int i = 0; i < mNumVertices; ++i )
	{
		mVertexList[i] = transform * mVertexList[i];
	}	
}

//===========================================================================

void BSPPolygon::Clean()
{
	DELETE_PTR_ARRAY( mVertexList );
	DELETE_PTR_ARRAY( mTriangleIndices );
}