#ifndef VERTEXBUFFER_HPP
#define VERTEXBUFFER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Mesh;
class EffectPass;
class VertexDeclaration;

///////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#define PRIMITIVE_TYPE_TUPLE \
PRIMITIVE_TYPE_ENTRY( PRIMITIVE_TRIANGLES, D3DPT_TRIANGLELIST, GL_TRIANGLES, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) \
PRIMITIVE_TYPE_ENTRY( PRIMITIVE_TRIANGLE_FAN, D3DPT_TRIANGLEFAN, GL_TRIANGLE_FAN, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ) \
PRIMITIVE_TYPE_ENTRY( PRIMITIVE_LINES, D3DPT_LINELIST, GL_LINES, D3D11_PRIMITIVE_TOPOLOGY_LINELIST ) \
PRIMITIVE_TYPE_ENTRY( PRIMITIVE_LINE_STRIP, D3DPT_LINESTRIP, GL_LINE_STRIP, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP ) \
PRIMITIVE_TYPE_ENTRY( PRIMITIVE_POINTS, D3DPT_POINTLIST, GL_POINTS, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ) \

enum PrimitiveType
{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) ENUM,
	PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	PRIMITIVE_TYPE_MAX
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class VertexBuffer
{
public:

	VertexBuffer();
	virtual ~VertexBuffer();

	virtual void Create( Mesh* mesh, VertexDeclaration* vertexDeclaration ) = 0;	

	virtual void Render( 
		VertexDeclaration* vertexDeclaration, 
		EffectPass* effectPass,
		void* context 
		) = 0;

	void SetVertexDeclaration( VertexDeclaration* vertexDeclaration )
	{
		mVertexDeclaration = vertexDeclaration;
	}

	VertexDeclaration* GetVertexDeclaration()
	{
		return mVertexDeclaration;
	}

protected:

	VertexDeclaration* mVertexDeclaration;
};

#endif

