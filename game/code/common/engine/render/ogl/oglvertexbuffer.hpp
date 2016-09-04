#if defined( RENDER_PLAT_OPENGL )

#ifndef OGL_VERTEXBUFFER_HPP
#define OGL_VERTEXBUFFER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexelement.hpp"
#include "common/engine/render/vertexbuffer.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Mesh;
class EffectPass;
class VertexDeclaration;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLVertexBuffer : public VertexBuffer
{
public:

	OGLVertexBuffer();
	~OGLVertexBuffer();

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