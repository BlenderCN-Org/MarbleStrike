#ifndef ICOSAHEDRON_SPHERE_HPP
#define ICOSAHEDRON_SPHERE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////

class IcosahedronSphere
{
public:				

	IcosahedronSphere();
	~IcosahedronSphere();

	void Initialize( float radius, int subdivision );

	float* GetVertices() const
	{
		return mVertices;
	}

	int* GetIndices() const
	{
		return mFaces;
	}

	int GetNumVertices() const
	{
		return mNumVertices;
	}

	int GetNumFaces() const
	{
		return mNumFaces;
	}

	int GetNumIndices() const
	{
		return mNumFaces * 3;
	}

private:

	void InitializeBase( float radius );
	void Subdivide();
	int SearchMidpoint( int indexStart, int indexEnd );

	int mNumVertices;
	int mNumFaces;
	int mNumEdges;

	float* mVertices;
	int* mFaces;

	int mEdgeWalk;

	int* mStart; 
	int* mEnd;
	int* mMidpoint;

};

#endif