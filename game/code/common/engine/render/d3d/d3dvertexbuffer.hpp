#if defined( RENDER_PLAT_D3D9 )

#ifndef D3D_VERTEXBUFFER_HPP
#define D3D_VERTEXBUFFER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexbuffer.hpp"
#include <d3d9.h>
#include <d3dx9.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Mesh;

///////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3DVertexBuffer : public VertexBuffer
{
public:

	D3DVertexBuffer();
	~D3DVertexBuffer();

	void Create( Mesh* mesh, VertexDeclaration* vertexDeclaration );    
    void CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration);
    void CreateIndexBuffer( Mesh* mesh );

    void SetVertexElement( 
        D3DVERTEXELEMENT9 &vertexElement, 
        WORD stream, 
        WORD offset, 
        D3DDECLTYPE type, 
        BYTE method, 
        BYTE usage, 
        BYTE usageIndex );

	void Render( 
		VertexDeclaration* vertexDeclaration,
		EffectPass* effectPass,
		void* context
		);

private:

    LPDIRECT3DVERTEXBUFFER9* mVertexBuffer;
    LPDIRECT3DINDEXBUFFER9 mIndexBuffer;
    int mNumVertices;
	PrimitiveType mPrimitiveType;
	int mNumPrimitives;	
	int mNumStreams;
	int* mOffset;
};

#endif

#endif