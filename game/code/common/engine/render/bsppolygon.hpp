#ifndef BSPPOLYGON_HPP
#define BSPPOLYGON_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/math/matrix44.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BSPPolygon
{
public:

	BSPPolygon();
	BSPPolygon( int numVertices );
	~BSPPolygon();
	BSPPolygon& operator=( const BSPPolygon& other );
	void Invert();
	void Transform( const Math::Matrix44 &transform );

	void SetVertex( int index, Math::Vector3 vertex )
	{
		mVertexList[index] = vertex;
	}

	inline const Math::Vector3& GetVertex( int index ) const
	{
		Assert( index >= 0 && index < mNumVertices, "" );
		return mVertexList[index];
	}

	int GetNumVertices() const
	{
		return mNumVertices;
	}

	int GetNumTriangles() const
	{
		return mNumTriangles;
	}

	void SetNormal( Math::Vector3 normal )
	{
		mNormal = normal;
	}

	Math::Vector3 GetNormal()
	{
		return mNormal;
	}

	void SetIsSplitter( bool value )
	{
		mIsSplitter = value;
	}

	bool IsSplitter()

	{
		return mIsSplitter;
	}

	void SetTriangleIndex( int index, int value )
	{
		mTriangleIndices[index] = value;
	}

	int GetTriangleIndex( int index )
	{
		return mTriangleIndices[index];
	}

	char name[32];
	
private:

	void Clean();
	
	int mNumVertices;
	Math::Vector3* mVertexList;
	Math::Vector3 mNormal;
	int mNumTriangles;
	int* mTriangleIndices;
	bool mIsSplitter;
};

#endif