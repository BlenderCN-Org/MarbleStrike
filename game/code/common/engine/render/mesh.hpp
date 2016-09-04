#ifndef MESH_HPP
#define MESH_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "common/engine/render/vertexbuffer.hpp"
#include "common/engine/system/array.hpp"
#include "common/engine/system/array.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "tinyxml2.h"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Material;
class Shader;
class VertexBuffer;

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Mesh : public RenderObjectData
{
public:

	Mesh();
	~Mesh();

	void Update( float elapsedTime );
	bool IsTranslucent();
	void TransformVertices( const Math::Matrix44 &transform );

	const char* GetMeshName() const
	{
		return mMeshName;
	}

	const char* GetMaterialName() const
	{
		return mMaterialName;
	}

	void SetNumVertices( int value )
	{
		mNumVertices = value;
	}

	int GetNumVertices() const
	{
		return mNumVertices;
	}

	int GetNumPositions() const
	{
		return mNumPositions;
	}

	float* GetPositions() const
	{
		return mPositions;
	}

	int GetPositionsStride() const
	{
		return mPositionsStride;
	}	

	int GetNumNormals() const
	{
		return mNumNormals;
	}

	int GetNormalsStride() const 
	{
		return mNormalsStride;
	}

	float* GetNormals() const
	{
		return mNormals;
	}

	int GetNumTexCoords() const
	{
		return mNumTexCoords;
	}

	int GetTexCoordsStride() const
	{
		return mTexCoordsStride;
	}

	float* GetTexCoords() const
	{
		return mTexCoords;
	}

	int GetNumTangents() const
	{
		return mNumTangents;
	}

	int GetTangentsStride() const
	{
		return mTangentsStride;
	}

	float* GetTangents() const
	{
		return mTangents;
	}

	int GetNumBinormals() const
	{
		return mNumBinormals;
	}

	int GetBinormalsStride() const
	{
		return mBinormalsStride;
	}

	float* GetBinormals() const
	{
		return mBinormals;
	}

	int GetNumColors() const
	{
		return mNumColors;
	}

	int GetColorsStride() const
	{
		return mColorsStride;
	}

	float* GetColors() const
	{
		return mColors;
	}

	int GetNumFaceIndices() const
	{
		return mNumFaceIndices;
	}

	unsigned int* GetFaceIndices() const
	{
		return mFaceIndices;
	}

	int GetFaceIndicesStride() const
	{
		return mFaceIndicesStride;
	}	

	int GetNumPrimitives() const
	{
		return mNumPrimitives;
	}

	void SetPrimitiveType( PrimitiveType type )
	{
		mPrimitiveType = type;
	}

	PrimitiveType GetPrimitiveType() const
	{
		return mPrimitiveType;
	}

	int GetNumVertexBuffers() const
	{
		return mVertexBuffers.GetSize();
	}

	VertexBuffer* GetVertexBuffer( int i )
	{
		return mVertexBuffers[i];
	}
		
	void SetPositions( Math::Vector3* positions, int size );
	void AddPositions( int size );
	void SetNormals( Math::Vector3* normals, int size );
	void AddNormals( int size );
	void SetTexCoords( Math::Vector2* texCoords, int size );
	void AddTexCoords( int size );
	void SetColors( Math::Vector4* colors, int size );
	void AddColors( int size );
	void SetTangents( Math::Vector3* tangents, int size );
	void AddTangents( int size );
	void SetFaceIndices( unsigned int* indices, int numPrimitives, int vertexPerPrimitive );
	VertexBuffer* CreateVertexBuffer();
	void Load( tinyxml2::XMLNode* node );
	void Copy( const Mesh &other );

private:

	Mesh( const Mesh &other );
	Mesh& operator=( const Mesh& other );

	void LoadMesh( tinyxml2::XMLNode* node );
	
	char mMeshName[32];
	char mMaterialName[32];

	Array<VertexBuffer*> mVertexBuffers;
	PrimitiveType mPrimitiveType;

	int mNumVertices;
	int mNumPrimitives;

	float* mPositions;
	int mNumPositions;
	int mPositionsStride;

	float* mNormals;
	int mNumNormals;
	int mNormalsStride;

	float* mTexCoords;
	int mNumTexCoords;
	int mTexCoordsStride;

	float* mColors;
	int mNumColors;
	int mColorsStride;

	float* mTangents;
	int mNumTangents;
	int mTangentsStride;

	float* mBinormals;
	int mNumBinormals;
	int mBinormalsStride;

	unsigned int* mFaceIndices;
	int mNumFaceIndices;
	int mFaceIndicesStride;
};

#endif