//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/grid.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/system/fixedarray.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Grid::Grid()
{
	FixedArray<Math::Vector3> verticesList;	
    
	Math::Vector3* vertices;
	Math::Vector2* texCoords;
	Math::Vector4* colors;
    unsigned int* indices;
	Math::Vector3* normals;

	int width = 5;	
	int interval = 1;	
	int numLines = ( ( width / interval ) + 1 ) * 2;
	int numVertices = numLines * 2;
	verticesList.SetSize( numVertices );

	float halfWidth = static_cast<float>( width )/2.0f;
	for ( int i = 0; i <= width; i += interval )
	{
		float x = -halfWidth + static_cast<float>( i );
		verticesList.PushBack( Math::Vector3( x, 0, -halfWidth ) );
		verticesList.PushBack( Math::Vector3( x, 0, halfWidth ) );	
	}

	for ( int i = 0; i <= width; i += interval )
	{
		float z = -halfWidth + static_cast<float>( i );
		verticesList.PushBack( Math::Vector3( -halfWidth, 0, z ) );
		verticesList.PushBack( Math::Vector3( halfWidth, 0, z ) );
	}

	Assert( numVertices == verticesList.GetSize(), "" );

	vertices = NEW_PTR( "Grid Vertices" ) Math::Vector3[numVertices];
	texCoords = NEW_PTR( "Grid Texture Coords" ) Math::Vector2[numVertices];
	colors = NEW_PTR( "Grid Colors" ) Math::Vector4[numVertices];
	normals = NEW_PTR( "Grid Normals" ) Math::Vector3[numVertices];
	
	indices = NEW_PTR( "Grid Indices" ) unsigned int[numVertices];

	for ( int i = 0; i < numVertices; ++i )
	{
		vertices[i] = verticesList[i];
		texCoords[i] = Math::Vector2( 0, 0 );
		colors[i] = Math::Vector4( 1, 1, 1, 1 );
		indices[i] = i;
	}

    mMesh = NEW_PTR( "Grid Mesh" ) Mesh;
	mMesh->SetNumVertices( numVertices );	
    mMesh->SetPositions( vertices, numVertices );
	mMesh->SetNormals( normals, numVertices );
	mMesh->SetTexCoords( texCoords, numVertices );
	mMesh->SetColors( colors, numVertices );
    mMesh->SetFaceIndices( indices, numLines, 2 );
    mMesh->SetPrimitiveType( PRIMITIVE_LINES );

    DELETE_PTR_ARRAY( vertices );
    DELETE_PTR_ARRAY( indices );
	DELETE_PTR_ARRAY( normals );
	DELETE_PTR_ARRAY( colors );
	DELETE_PTR_ARRAY( texCoords );
}

//===========================================================================

Grid::~Grid()
{
    DELETE_PTR( mMesh );
}