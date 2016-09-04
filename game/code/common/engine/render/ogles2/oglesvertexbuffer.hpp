#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLES_VERTEXBUFFER_HPP
#define OGLES_VERTEXBUFFER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexelement.hpp"
#include "common/engine/render/vertexbuffer.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Mesh;
class Effect;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLESVertexBuffer : public VertexBuffer
{
public:

	OGLESVertexBuffer();
	~OGLESVertexBuffer();

	void Create( Mesh* mesh, VertexDeclaration* vertexDeclaration );
    
    void CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration );
    void CreateIndexBuffer( Mesh* mesh );

	void SetOGLPointer( 
		VertexElement vertexElement, 
		VertexDeclaration* vertexDeclaration,
		EffectPass* effectPass, 
		int offset 
		);
	void Render( 
		VertexDeclaration* vertexDeclaration, 
		EffectPass* effectPass,
		void* context
		);

private:

	unsigned int* mBufferID;
	unsigned int mIndexBuffer;
    int mNumVertices;
	PrimitiveType mPrimitiveType;
	int mNumPrimitives;
	int mVertexPerPrimitive;
	int mNumStreams;
	int* mOffset;
};

#endif

#endif