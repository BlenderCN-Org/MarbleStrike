//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/bsp.hpp"
#include "common/engine/render/bsppolygon.hpp"
#include "common/engine/render/constantbuffer.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/light.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/rendertarget.hpp"
#include "common/engine/render/shaderparametermatrix.hpp"
#include "common/engine/render/shaderparametervector.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/math/rectangle.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/profiler.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BSP::BSP() 	
{  
	mRoot = NULL;
	mMesh = NULL;
	mRenderMeshObject = NULL;
}

//===========================================================================

BSP::~BSP() 
{ 
	Shutdown(); 
}

//===========================================================================

BSP::BSP(const BSP &other)
{
	Copy( other );
}

//===========================================================================

BSP& BSP::operator=( const BSP& other )
{
	if ( this != &other )
	{
		Copy( other );
	}
	return *this;
}

//===========================================================================

void BSP::Initialize()
{
	Assert( mBSPPolygonList.GetSize() == 0, "BSP Tree already built." );

	int i = 0;
	List<BSPPolygon*> newList;
	List<BSPPolygon*>::ConstIterator polyIter = mPolygonList.CreateConstIterator();
	while ( polyIter.HasNext() )
	{		
		BSPPolygon* poly = polyIter.Next();
		BSPPolygon* newPoly = NEW_PTR( "Init Poly" ) BSPPolygon;
		*newPoly = *poly;

		newList.InsertBack( newPoly );
		i++;
	}	

	mRoot = NEW_PTR( "BSP Node" )BSPNode;	

	//
	// Start building the BSP tree
	//
	BuildBSP( mRoot, newList );

	CreateMesh();		
}

//===========================================================================

void BSP::Initialize( const List<BSPPolygon*> &newList )
{
	Shutdown();

	List<BSPPolygon*>::ConstIterator polyIter = newList.CreateConstIterator();
	while ( polyIter.HasNext() )
	{
		BSPPolygon* poly = polyIter.Next();
		BSPPolygon* newPoly = NEW_PTR( "Init polygon with new list" ) BSPPolygon;
		*newPoly = *poly;

		mPolygonList.InsertBack( newPoly );
	}

	Initialize();
}

//===========================================================================

void BSP::Shutdown()
{
	DELETE_PTR( mMesh );
	DELETE_PTR( mRenderMeshObject );

	if ( mRoot != NULL )
	{
		DeleteBSP( mRoot );
		mRoot = NULL;
	}

	List<BSPPolygon*>::ConstIterator bspPolyIter = mBSPPolygonList.CreateConstIterator();
	while ( bspPolyIter.HasNext() )
	{
		BSPPolygon* poly = bspPolyIter.Next();
		DELETE_PTR( poly );
	}
	mBSPPolygonList.Clear();

	List<BSPPolygon*>::ConstIterator polyIter = mPolygonList.CreateConstIterator();
	while ( polyIter.HasNext() )
	{
		BSPPolygon* poly = polyIter.Next();
		DELETE_PTR( poly );
	}
	mPolygonList.Clear();
}

//===========================================================================

void BSP::DeleteBSP( BSPNode* &curNode )
{
	if ( curNode != NULL )
	{    
		if ( curNode->mIsLeaf == true )
		{      
			DELETE_PTR( curNode );
		}
		else
		{
			DeleteBSP( curNode->mFront );
			DeleteBSP( curNode->mBack );

			DELETE_PTR( curNode );
		}
	}
}

//===========================================================================

void BSP::AddPolygon( const List<Math::Vector3> &vertices, int numVertices )
{
	//
	// Make a new Polygon  
	//
	BSPPolygon* newPoly = NEW_PTR( "BSP Node" ) BSPPolygon( numVertices );

	Assert( numVertices == static_cast<int>( vertices.GetSize() ), "" );

	MakePolygon( newPoly, vertices );

	mPolygonList.InsertBack( newPoly );
}

//===========================================================================

void BSP::Clip( 
	const BSP* bsp, 
	List<BSPPolygon*> &returnList, 
	bool firstPass, 
	bool keepPolysInEmptySpace,
	bool invert
	)
{
	Math::Matrix44 curTransform = mRenderMeshObject->GetTransform();

	List<BSPPolygon*> newList;
	List<BSPPolygon*>::ConstIterator polyIter = mPolygonList.CreateConstIterator();
	while ( polyIter.HasNext() )
	{
		BSPPolygon* poly = polyIter.Next();
		BSPPolygon* newPoly = NEW_PTR( "Clip Poly" ) BSPPolygon;
		*newPoly = *poly;

		newPoly->Transform( curTransform );

		newList.InsertBack( newPoly );
	}

	ClipTree( 
		bsp->GetRoot(), 
		bsp->GetObject()->GetTransform(),
		newList, 
		returnList, 
		firstPass, 
		keepPolysInEmptySpace, 
		invert );
}

//===========================================================================

void BSP::ClipTree( 
	const BSPNode* curNode, 
	const Math::Matrix44 &curNodeTransform,
	List<BSPPolygon*> curPolyList, 
	List<BSPPolygon*> &returnList, 
	bool firstPass,
	bool keepPolysInEmptySpace,
	bool invert
	)
{
	if ( curNode->mIsLeaf )
	{
		List<BSPPolygon*>::ConstIterator curPolyListIter = curPolyList.CreateConstIterator();
		while ( curPolyListIter.HasNext() )
		{
			BSPPolygon* poly = curPolyListIter.Next();

			if ( keepPolysInEmptySpace )
			{
				if ( invert )
				{
					poly->Invert();
				}
				returnList.InsertBack( poly );
			}
			else
			{
				DELETE_PTR( poly );
			}
		}
	}
	else
	{

		List<BSPPolygon*> frontlist;
		List<BSPPolygon*> backlist;

		List<BSPPolygon*>::Iterator iter = curPolyList.CreateIterator();
		while ( iter.HasNext() )
		{
			BSPPolygon* curPoly = iter.Next();

			Math::Plane plane;
			plane = curNode->mPlane;
			plane.Transform( curNodeTransform );
			int result = ClassifyPolygon( plane, curPoly, firstPass );

			switch ( result )
			{
			case FRONT:
				{	
					frontlist.InsertBack( curPoly );
				} 
				break;
			case BACK:
				{	
					backlist.InsertBack( curPoly );
				}
				break;
			case SPANNING:				
				{
					BSPPolygon* newfrontsplit = NULL;
					BSPPolygon* newbacksplit = NULL;

					Split( curPoly, plane, newfrontsplit, newbacksplit );

					DELETE_PTR( curPoly );

					frontlist.InsertBack( newfrontsplit );
					backlist.InsertBack( newbacksplit );
				}
				break;
			default:
				Assert( false, "" );
				break;
			}
		}

		if ( curNode->mFront != NULL )
		{
			ClipTree( curNode->mFront, curNodeTransform, frontlist, returnList, firstPass, keepPolysInEmptySpace, invert );
		}

		if ( curNode->mBack != NULL )
		{
			ClipTree( curNode->mBack, curNodeTransform, backlist, returnList, firstPass, keepPolysInEmptySpace, invert );
		}
		else
		{
			List<BSPPolygon*>::ConstIterator backListIter = backlist.CreateConstIterator();
			while ( backListIter.HasNext() )
			{
				BSPPolygon* poly = backListIter.Next();
				if ( keepPolysInEmptySpace )
				{
					DELETE_PTR( poly );
				}
				else
				{
					if ( invert )
					{
						poly->Invert();
					}
					returnList.InsertBack( poly );
				}				
			}
		}	
	}
}

//===========================================================================

void BSP::CreateMesh()
{
	DELETE_PTR( mRenderMeshObject );
	DELETE_PTR( mMesh );

	mRenderMeshObject = NEW_PTR( "RenderObject" ) RenderObject;
	mMesh = NEW_PTR( "BSP Mesh" ) Mesh;

	Array<Math::Vector3> vertexList;
	Array<Math::Vector3> normalList;
	Array<Math::Vector2> texCoordList;
	Array<Math::Vector4> colorList;
	Array<unsigned int> indexList;
	int numPrimitives = 0;	
	int indexCount = 0;

	List<BSPPolygon*>::ConstIterator iter = mBSPPolygonList.CreateConstIterator();
	while ( iter.HasNext() )
	{
		BSPPolygon* poly = iter.Next();

		for ( int i = 0; i < poly->GetNumTriangles(); i++ )
		{
			int index = 0;

			index = poly->GetTriangleIndex( i * 3 );			
			Math::Vector3 v0 = poly->GetVertex( index );

			index = poly->GetTriangleIndex( i * 3 + 1 );			
			Math::Vector3 v1 = poly->GetVertex( index );

			index = poly->GetTriangleIndex( i * 3 + 2 );			
			Math::Vector3 v2 = poly->GetVertex( index );

			vertexList.PushBack( v0 );
			vertexList.PushBack( v1 );
			vertexList.PushBack( v2 );
			texCoordList.PushBack( Math::Vector2() );
			texCoordList.PushBack( Math::Vector2() );
			texCoordList.PushBack( Math::Vector2() );

			indexList.PushBack( indexCount );
			indexCount++;
			indexList.PushBack( indexCount );
			indexCount++;
			indexList.PushBack( indexCount );
			indexCount++;

			Math::Vector3 normal = Math::MakeNormal( v0, v1, v2 );			
			normalList.PushBack( normal );
			normalList.PushBack( normal );
			normalList.PushBack( normal );

			Math::Vector4 color( 1, 1, 1, 1 );
			colorList.PushBack( color );
			colorList.PushBack( color );
			colorList.PushBack( color );
		}		

		numPrimitives += poly->GetNumTriangles();
	}

	if ( vertexList.GetSize() > 0 )
	{
		mMesh->SetNumVertices( vertexList.GetSize() );
		mMesh->SetPrimitiveType( PRIMITIVE_TRIANGLES );
		mMesh->SetPositions( &( vertexList[0] ), vertexList.GetSize() );
		mMesh->SetTexCoords( &( texCoordList[0] ), vertexList.GetSize() );
		mMesh->SetColors( &( colorList[0] ), vertexList.GetSize() );
		mMesh->SetNormals( &( normalList[0] ), vertexList.GetSize() );
		mMesh->SetFaceIndices( &( indexList[0] ), numPrimitives, 3 );
	}
}

//===========================================================================

void BSP::MakePolygon( BSPPolygon* &newPoly, const List<Math::Vector3> &vertices )
{   
	int curVertex = 0;

	List<Math::Vector3>::ConstIterator iter = vertices.CreateConstIterator();
	while ( iter.HasNext() )
	{	
		newPoly->SetVertex( curVertex, iter.Next() );
		curVertex++;
	}

	int v0 = 0;
	int v1 = 0;
	int v2 = 0;
	int countIndices = 0;
	for ( int i = 0; i < newPoly->GetNumTriangles(); i++ )
	{
		if ( i == 0 )
		{
			v0 = 0;
			v1 = 1;
			v2 = 2;
		}
		else
		{
			v1 = v2;
			v2++;
		}

		newPoly->SetTriangleIndex( countIndices++, v0 );
		newPoly->SetTriangleIndex( countIndices++, v1 );
		newPoly->SetTriangleIndex( countIndices++, v2 );
	}

	//
	// Calculate the normal for the current polygon  
	//
	Math::Vector3 normal = Math::MakeNormal(
		newPoly->GetVertex( 0 ), 
		newPoly->GetVertex( 1 ), 
		newPoly->GetVertex( 2 ) 
		);
	newPoly->SetNormal( normal );
}

//===========================================================================

void BSP::BuildBSP( BSPNode* &curNode, List<BSPPolygon*> curPolyList )
{   
	BSPPolygon* splitterPoly = NULL;
	List<BSPPolygon*>::ConstIterator splitterIter = curPolyList.CreateConstIterator();
	while ( splitterIter.HasNext() )
	{
		BSPPolygon* splitter = splitterIter.Next();
		if ( splitter->IsSplitter() == false )
		{
			splitterPoly = splitter;
			splitter->SetIsSplitter( true );

			curNode->mPlane = Math::Plane( splitter->GetVertex( 0 ), splitter->GetNormal() );

			break;
		}
	}

	if ( splitterPoly == NULL )
	{
		curNode->mIsLeaf = true;

		List<BSPPolygon*>::ConstIterator curIter = curPolyList.CreateConstIterator();
		while ( curIter.HasNext() )
		{
			BSPPolygon* poly = curIter.Next();
			mBSPPolygonList.InsertBack( poly );
		}		
	}
	else
	{
		List<BSPPolygon*> frontlist;
		List<BSPPolygon*> backlist;

		List<BSPPolygon*>::Iterator iter = curPolyList.CreateIterator();
		while ( iter.HasNext() )
		{
			BSPPolygon* curPoly = iter.Next();

			int result = ClassifyPolygon( curNode->mPlane, curPoly );

			switch ( result )
			{
			case FRONT:
				{	
					frontlist.InsertBack( curPoly );
				} 
				break;
			case BACK:
				{					
					backlist.InsertBack( curPoly );
				}
				break;
			case SPANNING:				
				{
					BSPPolygon* newfrontsplit = NULL;
					BSPPolygon* newbacksplit = NULL;
					Split( curPoly, curNode->mPlane, newfrontsplit, newbacksplit );

					DELETE_PTR( curPoly );

					frontlist.InsertBack( newfrontsplit );
					backlist.InsertBack( newbacksplit );
				}
				break;
			default:
				Assert( false, "" );
				break;
			}
		}

		if ( frontlist.GetSize() > 0 )
		{
			BSPNode* newNode = NEW_PTR( "BSP Node" ) BSPNode;
			newNode->mFront = NULL;
			newNode->mBack = NULL;		
			newNode->mIsLeaf = false;		
			curNode->mFront = newNode;		
			BuildBSP( newNode, frontlist );
		}

		if ( backlist.GetSize() > 0 )
		{  
			BSPNode* newNode = NEW_PTR( "BSP Node" ) BSPNode;
			newNode->mFront = NULL;
			newNode->mBack = NULL;		
			newNode->mIsLeaf = false;		
			curNode->mBack = newNode;
			BuildBSP( newNode, backlist );
		}
	}
}

//===========================================================================

int BSP::ClassifyPoint( const Math::Plane &plane, const Math::Vector3 &point ) const
{
	float result = 0;

	result = plane.DistanceToPlane( point );

	if ( result > Epsilon ) 
	{
		return FRONT;
	}
	else if ( result < -Epsilon ) 
	{
		return BACK;
	}

	return ONPLANE;	
}

//===========================================================================

int BSP::ClassifyPolygon( const Math::Plane &plane, BSPPolygon* poly, bool firstPass ) const
{
	int front = 0;
	int back = 0;
	int onplane = 0;
	int result = 0;

	//
	// Loop through all the vertices and classify each point
	//
	int numVertices = poly->GetNumVertices();
	for ( int i = 0; i < numVertices; i++ )
	{
		result = ClassifyPoint( plane, poly->GetVertex( i ) );

		if ( result == FRONT ) 
		{
			front++;
		}

		if ( result == BACK ) 
		{
			back++;
		}

		if ( result == ONPLANE )
		{
			front++;
			back++;
			onplane++;
		}
	}

	//
	// Make sure we check onplane first before front and back because if all the points are
	// on the plane, they are also in front and behind the polygon
	//
	if ( onplane == numVertices ) 
	{		
		float dotProduct = Math::Vector3::Dot( plane.GetNormal(), poly->GetNormal() );

		if ( firstPass )
		{
			if ( dotProduct >= 0 )
			{
				return FRONT;
			}
			else
			{
				return BACK;
			}
		}
		else
		{
			if ( dotProduct >= 0 )
			{
				return BACK;
			}
			else
			{
				return FRONT;
			}			
		}		
	}

	if ( front == poly->GetNumVertices() ) 
	{
		return FRONT;
	}

	if ( back == poly->GetNumVertices() ) 
	{
		return BACK;
	}

	return SPANNING;
}

//===========================================================================

void BSP::Split( const BSPPolygon* poly, const Math::Plane &splitPlane, BSPPolygon* &frontsplit, BSPPolygon* &backsplit )
{
	List<Math::Vector3> frontVertices;
	List<Math::Vector3> backVertices;		

	for ( int i = 0; i < poly->GetNumVertices(); i++ )
	{		
		Math::Vector3 vertex( poly->GetVertex( i ) );

		int result = ClassifyPoint( splitPlane, vertex );
		switch(result)
		{
		case FRONT:			
			frontVertices.InsertBack( vertex );
			break;

		case BACK:
			backVertices.InsertBack( vertex );
			break;

		case ONPLANE:
			frontVertices.InsertBack( vertex );
			backVertices.InsertBack( vertex );
			break;
		}

		Math::Vector3 nextVertex;

		if ( i < poly->GetNumVertices() - 1 )
		{
			nextVertex = poly->GetVertex( i + 1 );
		}
		else
		{
			nextVertex = poly->GetVertex( 0 );
		}
		int nextresult = ClassifyPoint( splitPlane, nextVertex );

		Math::Vector3 newV;

		//
		// If two vertices are spanning a plane
		//
		if ( (result == FRONT && nextresult == BACK) || ( result == BACK && nextresult == FRONT ) )
		{
			//
			// Calculate the intersect point
			//
			float top = - ( splitPlane.GetCoeff()[3] ) - Math::Vector3::Dot( splitPlane.GetNormal(), vertex );
			float bottom = Math::Vector3::Dot( splitPlane.GetNormal(), nextVertex - vertex );
			float t = 0;
			if ( bottom != 0 )
			{
				t = top/bottom;
			}

			newV = vertex + ( nextVertex - vertex ) * t;

			//
			// Assign the intersect point to the front and back list
			//
			frontVertices.InsertBack( newV );
			backVertices.InsertBack( newV );
		}
	}

	int numFrontVertices = frontVertices.GetSize();
	int numBackVertices = backVertices.GetSize();

	frontsplit = NEW_PTR( "BSP Front Polygon" ) BSPPolygon( numFrontVertices );
	backsplit = NEW_PTR( "BSP Back Polygon" ) BSPPolygon( numBackVertices );

	MakePolygon( frontsplit, frontVertices );
	MakePolygon( backsplit, backVertices );	
}

//===========================================================================

void BSP::Display(
	RenderTarget* renderTarget,
	const Math::Matrix44 &projectionMatrix,
	const Math::Matrix44 &viewMatrix,
	const Math::Matrix44 &transformMatrix,
	const Math::Vector4 &diffuseColor,
	RenderObject* light
	)
{
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetRenderTarget( renderTarget );
	simpleDraw->SetProjectionMatrix( projectionMatrix );
	simpleDraw->SetViewMatrix( viewMatrix );
	simpleDraw->SetTransformMatrix( transformMatrix  * mRenderMeshObject->GetTransform() );
	simpleDraw->SetDiffuseColor( diffuseColor );
	simpleDraw->SetLight( light );	
	if ( renderTarget )
	{
		Math::Rectangle<int> rect;
		rect.SetWidth( renderTarget->GetWidth() );
		rect.SetHeight( renderTarget->GetHeight() );
		rect.SetPosition( 0, 0 );
		simpleDraw->SetScissorRectangle( rect );
	}
	simpleDraw->RenderMesh( mMesh );

	//
	// Wireframe
	//
	//List<BSPPolygon*>::ConstIterator iter = mBSPPolygonList.CreateConstIterator();
	//while ( iter.HasNext() )
	//{
	//	BSPPolygon* poly = iter.Next();

	//	for ( int i = 0; i < poly->GetNumTriangles(); i++ )
	//	{
	//		int index = 0;
	//		index = poly->GetTriangleIndex( i * 3 );			
	//		Math::Vector4 v0 = poly->GetVertex( index );

	//		index = poly->GetTriangleIndex( i * 3 + 1 );
	//		Math::Vector4 v1 = poly->GetVertex( index );

	//		index = poly->GetTriangleIndex( i * 3 + 2 );
	//		Math::Vector4 v2 = poly->GetVertex( index );

	//		simpleDraw->RenderLine( v0, v1 );
	//		simpleDraw->RenderLine( v1, v2 );
	//		simpleDraw->RenderLine( v2, v0 );
	//	}			
	//}
}

//===========================================================================

void BSP::SetPosition( const Math::Vector3 &pos )
{
	Math::Matrix44 transform = mRenderMeshObject->GetTransform();
	transform.SetTranslation( pos );
	mRenderMeshObject->SetTransform( transform );
}

//===========================================================================

Math::Vector3 BSP::GetPosition() const
{
	Math::Matrix44 transform = mRenderMeshObject->GetTransform();
	Math::Vector3 pos = transform.GetTranslation();

	return pos;
}

//===========================================================================

void BSP::Rotate( const Math::Matrix44 &rotateMatrix )
{
	Math::Matrix44 transform = rotateMatrix * mRenderMeshObject->GetTransform();
	mRenderMeshObject->SetTransform( transform );
}

//===========================================================================

void BSP::Scale( float scale )
{
	Math::Matrix44 scaleMatrix;
	scaleMatrix.Scale( scale );
	Math::Matrix44 transform = scaleMatrix * mRenderMeshObject->GetTransform();
	mRenderMeshObject->SetTransform( transform );	
}

//===========================================================================

void BSP::ResetTransform()
{
	Math::Matrix44 identity;
	mRenderMeshObject->SetTransform( identity );
}

//===========================================================================

int BSP::ClassifyCollisionPoint( const Math::Plane &plane, const Math::Vector3 &point ) const
{
	float result = 0;

	result = plane.DistanceToPlane( point ) - 2.0f;

	if ( result > Epsilon ) 
	{
		return FRONT;    
	}

	if ( result < -Epsilon ) 
	{
		return BACK;
	}

	return ONPLANE;	
}

//===========================================================================

void BSP::Copy( const BSP &other )
{
	Shutdown();

	if ( other.mRoot != NULL )
	{
		mRoot = NEW_PTR( "BSP Node" )BSPNode;
		CopyNode( mRoot, other.mRoot );
	}
	else
	{
		mRoot = NULL;
	}

	List<BSPPolygon*>::ConstIterator iter = other.mPolygonList.CreateConstIterator();
	while ( iter.HasNext() )
	{
		BSPPolygon* curPoly = iter.Next();
		BSPPolygon* newPoly = NEW_PTR( "BSP Polygon" ) BSPPolygon;
		*newPoly = *curPoly;
		mPolygonList.InsertBack( newPoly );
	}

	List<BSPPolygon*>::ConstIterator bspIter = other.mBSPPolygonList.CreateConstIterator();
	while ( bspIter.HasNext() )
	{
		BSPPolygon* curPoly = bspIter.Next();
		BSPPolygon* newPoly = NEW_PTR( "BSP Polygon" ) BSPPolygon;
		*newPoly = *curPoly;
		mBSPPolygonList.InsertBack( newPoly );
	}
}

//===========================================================================

void BSP::CopyNode( BSPNode* &node, const BSPNode* otherNode )
{
	node->mIsLeaf = otherNode->mIsLeaf;
	node->mPlane = otherNode->mPlane;

	if ( otherNode->mBack != NULL )
	{
		BSPNode* back = NEW_PTR( "BSP Node" )BSPNode;
		CopyNode( back, otherNode->mBack );
		node->mBack = back;
	}
	else
	{
		node->mBack = NULL;
	}

	if (  otherNode->mFront != NULL )
	{
		BSPNode* front = NEW_PTR( "BSP Node" )BSPNode;
		CopyNode( front,  otherNode->mFront );
		node->mFront = front;
	}
	else
	{
		node->mFront = NULL;
	}
}