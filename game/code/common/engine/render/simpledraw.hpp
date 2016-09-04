#ifndef SIMPLEDRAW_HPP
#define SIMPLEDRAW_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/sphere.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/vertexbuffer.hpp"
#include "common/engine/math/rectangle.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Camera;
class Effect;
class Material;
class Mesh;
class RenderObject;
class RenderTarget;
class SimpleMesh;
class RenderNode;
class Texture;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_SIMPLE_DRAW_OBJECTS = 300;
const int MAX_SIMPLE_VERTICES = 1000;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class SimpleDraw
{
public:

	struct SimpleVertex
	{
		Math::Vector3 mPosition;
		Math::Vector4 mColor;
		Math::Vector3 mNormal;
		Math::Vector2 mTexCoord;		
	};

public:

	SimpleDraw();
	~SimpleDraw();
	void Clear();

	int GetTechniqueIndex( const char* name );
	void SetTechniqueIndex( int index )
	{
		mTechniqueIndex = index;
	}

	void RenderLine( 
		const Math::Vector3 &v0, 
		const Math::Vector3 &v1,
		const Math::Vector4 &color = Math::Vector4( 1, 1, 1, 1 )
		);
	
	void RenderLineStrip( 
		const Math::Vector3* vertexList,
		int numVertices,
		const Math::Vector4 &color = Math::Vector4( 1, 1, 1, 1 )
		);
	
	void RenderTriangle( 
		const Math::Vector3 &v0, 
		const Math::Vector3 &v1, 
		const Math::Vector3 &v2
		);
	
	void RenderQuad( 
		const Math::Vector3 &v0, 
		const Math::Vector3 &v1, 
		const Math::Vector3 &v2, 
		const Math::Vector3 &v3,		
		const Math::Vector2 &tex0 = Math::Vector2( 0, 0 ),
		const Math::Vector2 &tex1 = Math::Vector2( 0, 0 ),
		const Math::Vector2 &tex2 = Math::Vector2( 0, 0 ),
		const Math::Vector2 &tex3 = Math::Vector2( 0, 0 ),
		const Math::Vector4 &color = Math::Vector4( 1, 1, 1, 1 )
		);
	
	void RenderPoint( const Math::Vector3 &v0 );	

	void RenderSphere( 
		const Math::Sphere &sphere, 
		const Math::Vector4 &color = Math::Vector4( 1, 1, 1, 1 ), 
		float stepSize = 1.0f 
		);

	void RenderMesh( 
		const SimpleVertex* vertices, 
		int numPrimitives, 
		PrimitiveType primitiveType = PRIMITIVE_TRIANGLES
		);
	
	void RenderMesh( Mesh* mesh	);

	void SetRenderTarget( RenderTarget* renderTarget )
	{
		mRenderTarget = renderTarget;
	}

	void SetProjectionMatrix( const Math::Matrix44 &projection )
	{
		mProjectionMatrix = projection;
	}

	void SetViewMatrix( const Math::Matrix44 &view )
	{
		mViewMatrix = view;
	}

	void SetTransformMatrix( const Math::Matrix44 &transform )
	{
		mTransformMatrix = transform;
	}

	void SetScissorRectangle( const Math::Rectangle<int> &scissorRectangle )
	{
		mScissorRectangle = scissorRectangle;
	}

	void SetColorTexture( Texture* texture );
	void SetDiffuseColor( const Math::Vector4 &color );

	void SetLight( RenderObject* light )
	{
		mLight = light;
	}

private:

	void UpdateConstants( RenderNode* renderNode );
	void ClearConstants();

	int mNumSimpleDrawObjects;
	SimpleMesh* mSimpleMeshList;	
	
	int mNumSimpleVertices;
	SimpleVertex* mSimpleVertexList;

	Math::Matrix44 mProjectionMatrix;
	Math::Matrix44 mViewMatrix;
	Math::Matrix44 mTransformMatrix;

	Math::Rectangle<int> mScissorRectangle;

	RenderTarget* mRenderTarget;
	Effect* mEffect;

	Material* mMaterial;	
	RenderObject* mLight;

	Texture* mEmptyTexture;	

	int mTechniqueIndex;

    int mDiffuseColorHash;
    int mLightAmbientColorHash;
    int mLightColorDiffuseColorHash;
    int mLightPositionHash;
    int mProjectionMatrixHash;
    int mViewMatrixHash;
    int mTransformMatrixHash;
    int mTextureHash;
};

#endif
