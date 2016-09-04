#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_VERTEXBUFFER_HPP
#define D3D11_VERTEXBUFFER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexbuffer.hpp"
#include "common/engine/render/vertexelement.hpp"

#include <d3d11.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Mesh;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11VertexBuffer : public VertexBuffer
{
public:

	D3D11VertexBuffer();
	~D3D11VertexBuffer();

	void Create( Mesh* mesh, VertexDeclaration* vertexDeclaration );    
	void CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration);
	void CreateIndexBuffer( Mesh* mesh );

	//void SetVertexElement( 
	//	D3DVERTEXELEMENT9 &vertexElement, 
	//	WORD stream, 
	//	WORD offset, 
	//	D3DDECLTYPE type, 
	//	BYTE method, 
	//	BYTE usage, 
	//	BYTE usageIndex );

	void Render( 
		VertexDeclaration* vertexDeclaration,
		EffectPass* effectPass,
		void* context
		);

private:

	ID3D11Buffer** mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;	
	PrimitiveType mPrimitiveType;
	int mNumIndices;
	int mNumStreams;
	int mOffset[MAX_NUM_STREAMS];
};

#endif

#endif