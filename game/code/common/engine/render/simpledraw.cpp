//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/math/rectangle.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/light.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

SimpleDraw::SimpleDraw()
{	
	mTechniqueIndex = -1;
	mNumSimpleVertices = 0;
	mSimpleVertexList = NEW_PTR( "Vertex List" ) SimpleVertex[MAX_SIMPLE_VERTICES];	

	mNumSimpleDrawObjects = 0;
	mSimpleMeshList = NEW_PTR( "Mesh List" ) SimpleMesh[MAX_SIMPLE_DRAW_OBJECTS];	
	mMaterial = NEW_PTR( "SimpleDrawMaterial" ) Material;
	mLight = NULL;

	int width = Database::Get()->GetBackBufferWidth();
	int height = Database::Get()->GetBackBufferHeight();

	mScissorRectangle.SetPosition( 0, 0 );	
	mScissorRectangle.SetWidth( width );
	mScissorRectangle.SetHeight( height );
	
	mEffect = GameApp::Get()->GetEffectManager()->GetEffect( "simple" );		
	mRenderTarget = NULL;		

	mEmptyTexture = GameApp::Get()->GetTextureManager()->GetTexture( "emptytexture" );

    mDiffuseColorHash = System::StringHash( "gDiffuseColor" );
    mLightAmbientColorHash = System::StringHash( "gLightAmbientColor" );
    mLightColorDiffuseColorHash = System::StringHash( "gLightColor" );
    mLightPositionHash = System::StringHash( "gLightPosition" );
    mProjectionMatrixHash = System::StringHash( "gProjectionMatrix" );
    mViewMatrixHash = System::StringHash( "gViewMatrix" );
    mTransformMatrixHash = System::StringHash( "gTransformMatrix" );
    mTextureHash = System::StringHash( "gTexture" );    
}

//===========================================================================

SimpleDraw::~SimpleDraw()
{  	
	DELETE_PTR_ARRAY( mSimpleMeshList );
	DELETE_PTR_ARRAY( mSimpleVertexList );
	DELETE_PTR( mMaterial );
}

//===========================================================================

void SimpleDraw::Clear()
{
	mNumSimpleDrawObjects = 0;
	mNumSimpleVertices = 0;
}

//===========================================================================

int SimpleDraw::GetTechniqueIndex( const char* name )
{
	return mEffect->GetTechniqueIndex( name );
}

//===========================================================================

void SimpleDraw::RenderLine( 
	const Math::Vector3 &v0, 
	const Math::Vector3 &v1,
	const Math::Vector4 &color
	)
{
	SimpleVertex* lineVertex = &( mSimpleVertexList[mNumSimpleVertices] );
	mNumSimpleVertices += 2;
	Assert( mNumSimpleVertices < MAX_SIMPLE_VERTICES, "" );

	lineVertex[0].mPosition = v0;
	lineVertex[1].mPosition = v1;

	lineVertex[0].mColor = color;
	lineVertex[1].mColor = color;

	SimpleMesh* simpleMesh = &( mSimpleMeshList[mNumSimpleDrawObjects] );
	simpleMesh->mNumPrimitives = 1;
	simpleMesh->mPrimitiveType = PRIMITIVE_LINES;
	simpleMesh->mVertexPerPrimitive = 2;
	simpleMesh->mVertexData = ( void* )lineVertex;

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "SimpleDraw - Line");	
#endif
	renderNode->mSimpleMesh = simpleMesh;
	UpdateConstants( renderNode );

	mNumSimpleDrawObjects++;
	Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );

	ClearConstants();
}

//===========================================================================

void SimpleDraw::RenderLineStrip( 
	const Math::Vector3* vertexList,
	int numVertices,
	const Math::Vector4 &color
	)
{	
	SimpleVertex* lineVertex = &( mSimpleVertexList[mNumSimpleVertices] );
	mNumSimpleVertices += numVertices;
	Assert( mNumSimpleVertices < MAX_SIMPLE_VERTICES, "" );

	for ( int i = 0; i < numVertices; ++i )
	{
		lineVertex[i].mPosition = vertexList[i];		
		lineVertex[i].mColor = color;		
	}	

	SimpleMesh* simpleMesh = &( mSimpleMeshList[mNumSimpleDrawObjects] );
	simpleMesh->mNumPrimitives = numVertices - 1;
	simpleMesh->mPrimitiveType = PRIMITIVE_LINE_STRIP;
	simpleMesh->mVertexPerPrimitive = 1;
	simpleMesh->mVertexData = ( void* )lineVertex;
		
	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "SimpleDraw - LineStrip");	
#endif
	renderNode->mSimpleMesh = simpleMesh;
	UpdateConstants( renderNode );

	mNumSimpleDrawObjects++;
	Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );

	ClearConstants();
}

//===========================================================================

void SimpleDraw::RenderTriangle( 
	const Math::Vector3& /*v0*/, 
	const Math::Vector3& /*v1*/,
	const Math::Vector3& /*v2*/
	)
{
	//SimpleVertex* triVertex = &( mSimpleVertexList[mNumSimpleVertices] );
	//mNumSimpleVertices += 3;
	//Assert( mNumSimpleVertices < MAX_SIMPLE_VERTICES, "" );

	//triVertex[0].mPosition.Set( v0[0], v0[1], v0[2] );
	//triVertex[1].mPosition.Set( v1[0], v1[1], v1[2] );
	//triVertex[2].mPosition.Set( v2[0], v2[1], v2[2] );

	//Math::Vector3 normal = Math::MakeNormal( v0, v1, v2 );	
	//triVertex[0].mNormal = normal;
	//triVertex[1].mNormal = normal;
	//triVertex[2].mNormal = normal;

	//SimpleMesh* simpleMesh = &( mSimpleMeshList[mNumSimpleDrawObjects] );
	//simpleMesh->mNumPrimitives = 1;
	//simpleMesh->mPrimitiveType = PRIMITIVE_TRIANGLES;
	//simpleMesh->mVertexPerPrimitive = 3;
	//simpleMesh->mVertexData = ( void* )triVertex;

	//mVertexConstantBufferDataList[mNumSimpleDrawObjects] = mCurrentVertexConstantBuffer;
	//mPixelConstantBufferDataList[mNumSimpleDrawObjects] = mCurrentPixelConstantBuffer;
	//mTextureList[mNumSimpleDrawObjects] = mCurrentTexture;

	//RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
	//renderNode->SetDescription( "SimpleDraw - Triangle");
	//renderNode->SetRenderTarget( mRenderTarget );
	//renderNode->SetTechnique( mTechniqueIndex );
	//renderNode->SetPass( 0 );
	//renderNode->SetEffect( mEffect );
	//renderNode->SetVertexConstantBuffer( mVertexConstantBufferList[mNumSimpleDrawObjects] );
	//renderNode->SetPixelConstantBuffer( mPixelConstantBufferList[mNumSimpleDrawObjects] );
	//renderNode->SetSimpleMesh( simpleMesh );

	//mNumSimpleDrawObjects++;
	//Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );
}

//===========================================================================

void SimpleDraw::RenderQuad( 
	const Math::Vector3 &v0, 
	const Math::Vector3 &v1, 
	const Math::Vector3 &v2, 
	const Math::Vector3 &v3,							
	const Math::Vector2 &tex0,
	const Math::Vector2 &tex1,
	const Math::Vector2 &tex2,
	const Math::Vector2 &tex3,
	const Math::Vector4 &color
	)
{
	SimpleVertex* triVertex = &mSimpleVertexList[mNumSimpleVertices];
	mNumSimpleVertices += 6;
	Assert( mNumSimpleVertices < MAX_SIMPLE_VERTICES, "" );
	triVertex[0].mPosition = v0;
	triVertex[1].mPosition = v1;
	triVertex[2].mPosition = v2;
	triVertex[3].mPosition = v0;
	triVertex[4].mPosition = v2;
	triVertex[5].mPosition = v3;

	triVertex[0].mTexCoord = tex0;
	triVertex[1].mTexCoord = tex1;
	triVertex[2].mTexCoord = tex2;
	triVertex[3].mTexCoord = tex0;
	triVertex[4].mTexCoord = tex2;
	triVertex[5].mTexCoord = tex3;

	triVertex[0].mColor = color;
	triVertex[1].mColor = color;
	triVertex[2].mColor = color;
	triVertex[3].mColor = color;
	triVertex[4].mColor = color;
	triVertex[5].mColor = color;

	Renderer::Get()->ModifyUV( triVertex[0].mTexCoord[0], triVertex[0].mTexCoord[1] );
	Renderer::Get()->ModifyUV( triVertex[1].mTexCoord[0], triVertex[1].mTexCoord[1] );
	Renderer::Get()->ModifyUV( triVertex[2].mTexCoord[0], triVertex[2].mTexCoord[1] );
	Renderer::Get()->ModifyUV( triVertex[3].mTexCoord[0], triVertex[3].mTexCoord[1] );	
	Renderer::Get()->ModifyUV( triVertex[4].mTexCoord[0], triVertex[4].mTexCoord[1] );	
	Renderer::Get()->ModifyUV( triVertex[5].mTexCoord[0], triVertex[5].mTexCoord[1] );		

	Math::Vector3 normal = Math::MakeNormal( v0, v1, v2 );	
	triVertex[0].mNormal = normal;
	triVertex[1].mNormal = normal;
	triVertex[2].mNormal = normal;
	triVertex[3].mNormal = normal;
	triVertex[4].mNormal = normal;
	triVertex[5].mNormal = normal;

	SimpleMesh* simpleMesh = &mSimpleMeshList[mNumSimpleDrawObjects];
	simpleMesh->mNumPrimitives = 2;
	simpleMesh->mPrimitiveType = PRIMITIVE_TRIANGLES;
	simpleMesh->mVertexPerPrimitive = 3;
	simpleMesh->mVertexData = ( void* )triVertex;
		
	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "SimpleDraw - Quad");	
#endif
	renderNode->mSimpleMesh = simpleMesh;
	UpdateConstants( renderNode );
	
	mNumSimpleDrawObjects++;
	Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );

	ClearConstants();
}

//===========================================================================

void SimpleDraw::RenderPoint( const Math::Vector3 & /*v0*/ )
{
	//SimpleVertex* pointVertex = &( mSimpleVertexList[mNumSimpleVertices] );
	//mNumSimpleVertices += 1;
	//Assert( mNumSimpleVertices < MAX_SIMPLE_VERTICES, "" );

	//pointVertex->mPosition = v0;	

	//SimpleMesh* simpleMesh = &mSimpleMeshList[mNumSimpleDrawObjects];
	//simpleMesh->mNumPrimitives = 1;
	//simpleMesh->mPrimitiveType = PRIMITIVE_POINTS;
	//simpleMesh->mVertexPerPrimitive = 1;
	//simpleMesh->mVertexData = ( void* )&pointVertex;

	//mVertexConstantBufferDataList[mNumSimpleDrawObjects] = mCurrentVertexConstantBuffer;
	//mPixelConstantBufferDataList[mNumSimpleDrawObjects] = mCurrentPixelConstantBuffer;
	//mTextureList[mNumSimpleDrawObjects] = mCurrentTexture;

	//RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
	//renderNode->SetDescription( "SimpleDraw - Point");
	//renderNode->SetRenderTarget( mRenderTarget );
	//renderNode->SetTechnique( mTechniqueIndex );
	//renderNode->SetPass( 0 );
	//renderNode->SetEffect( mEffect );
	//renderNode->SetVertexConstantBuffer( mVertexConstantBufferList[mNumSimpleDrawObjects] );
	//renderNode->SetPixelConstantBuffer( mPixelConstantBufferList[mNumSimpleDrawObjects] );
	//renderNode->SetSimpleMesh( simpleMesh );

	//mNumSimpleDrawObjects++;
	//Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );
}

//===========================================================================

void SimpleDraw::RenderSphere( const Math::Sphere &sphere, const Math::Vector4 &color, float stepSize )
{	
	for ( float degree = 0; degree < 360; degree += stepSize )
	{
		float x0 = sin( Math::DegreeToRadians( degree ) );
		float y0 = cos( Math::DegreeToRadians( degree ) );

		float x1 = sin( Math::DegreeToRadians( degree + stepSize ) );
		float y1 = cos( Math::DegreeToRadians( degree + stepSize ) );

		Math::Vector3 v0( x0, y0, 0 );
		Math::Vector3 v1( x1, y1, 0 );

		v0 *= sphere.GetRadius();
		v1 *= sphere.GetRadius();

		RenderLine( v0, v1, color );

		Math::Vector3 v2( 0, y0, x0 );
		Math::Vector3 v3( 0, y1, x1 );

		v2 *= sphere.GetRadius();
		v3 *= sphere.GetRadius();

		RenderLine( v2, v3, color );
	}
}

//===========================================================================

void SimpleDraw::RenderMesh( 
	const SimpleVertex* vertices, 
	int numPrimitives,
	PrimitiveType primitiveType
	)
{
	SimpleMesh* simpleMesh = &mSimpleMeshList[mNumSimpleDrawObjects];
	simpleMesh->mNumPrimitives = numPrimitives;
	simpleMesh->mPrimitiveType = primitiveType;
	simpleMesh->mVertexPerPrimitive = 3;
	simpleMesh->mVertexData = ( void* )vertices;
		
	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "SimpleDraw - SimpleMesh");
#endif
	renderNode->mSimpleMesh = simpleMesh;
	UpdateConstants( renderNode );

	mNumSimpleDrawObjects++;
	Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );

	ClearConstants();
}

//===========================================================================

void SimpleDraw::RenderMesh( Mesh* mesh )
{
	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "SimpleDraw - Mesh");	
#endif
	renderNode->mMesh = mesh;
	UpdateConstants( renderNode );

	mNumSimpleDrawObjects++;
	Assert( mNumSimpleDrawObjects < MAX_SIMPLE_DRAW_OBJECTS, "" );

	ClearConstants();
}

//===========================================================================

void SimpleDraw::SetColorTexture( Texture* texture )
{
	mMaterial->SetColorTexture( texture );
}

//===========================================================================

void SimpleDraw::SetDiffuseColor( const Math::Vector4 &color )
{
	mMaterial->SetDiffuseColor( color );
}


//===========================================================================

void SimpleDraw::UpdateConstants( RenderNode* renderNode )
{	
	renderNode->mRenderTarget = mRenderTarget;	
	renderNode->mEffect = mEffect;		

	renderNode->SetMatrix( mProjectionMatrixHash, &mProjectionMatrix );
	renderNode->SetMatrix( mViewMatrixHash, &mViewMatrix );
	renderNode->SetMatrix( mTransformMatrixHash,  &mTransformMatrix );
	renderNode->SetVector( mDiffuseColorHash,  mMaterial->GetDiffuseColor() );
	
	renderNode->SetTexture( mTextureHash, mEmptyTexture );
	if ( mMaterial->GetColorTexture() )
	{
		renderNode->SetTexture( mTextureHash, mMaterial->GetColorTexture() );
	}
	
	if ( mTechniqueIndex == -1 )
	{
		if ( mLight )
		{
			Light* light = reinterpret_cast<Light*>( mLight->GetRenderObjectData() );
			renderNode->SetVector( mLightAmbientColorHash, light->GetAmbientColor() );
			renderNode->SetVector( mLightColorDiffuseColorHash, light->GetColor() );		
			renderNode->SetVector( mLightPositionHash, Math::Vector4( mLight->GetTransform().GetTranslation(), 1.0 ) );

			renderNode->mTechnique = mEffect->GetTechniqueIndex( "simplelight" );
		}
		else
		{
			renderNode->mTechnique = mEffect->GetTechniqueIndex( "simplenolight" );
		}
	}
	else
	{
		renderNode->mTechnique = mTechniqueIndex;
	}

	renderNode->mScissorRectangle = mScissorRectangle;
}

//===========================================================================

void SimpleDraw::ClearConstants()
{	
	mMaterial->SetDiffuseColor( Math::Vector4( 1, 1, 1, 1 ) );
	mMaterial->SetColorTexture( mEmptyTexture );

	int width = Database::Get()->GetBackBufferWidth();
	int height = Database::Get()->GetBackBufferHeight();

	mScissorRectangle.SetPosition( 0, 0 );	
	mScissorRectangle.SetWidth( width );
	mScissorRectangle.SetHeight( height );

	mProjectionMatrix.SetIdentity();
	mViewMatrix.SetIdentity();
	mTransformMatrix.SetIdentity();

	mTechniqueIndex = -1;

	mLight = NULL;
}