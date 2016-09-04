#ifndef BSP_HPP
#define BSP_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/plane.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/system/list.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class BSPPolygon;
class Mesh;
class RenderObject;
class RenderTarget;
class Texture;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int ONPLANE = 0;
const int FRONT = 1;
const int BACK = 2;
const int SPANNING = 3;
const float Epsilon = 0.00001f;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct BSPNode
{	
	BSPNode()
		:	mFront( NULL )
		,	mBack( NULL )
		,	mIsLeaf( false )
	{

	}

	BSPNode* mFront;
	BSPNode* mBack;
	bool mIsLeaf;
	Math::Plane mPlane;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BSP
{
public:
	
	BSP();	
	~BSP();

	BSP( const BSP &other );
	BSP& operator=( const BSP& other );
	
	void Initialize();	
	void Initialize( const List<BSPPolygon*> &newList );	
	void Shutdown();
	void AddPolygon( const List<Math::Vector3> &vertices, int numVertices ); 	
	void Clip( 
		const BSP* bsp, 
		List<BSPPolygon*> &returnList, 
		bool firstPass, 
		bool keepPolysInEmptySpace,
		bool invert );
	void Invert();
	void Display(
		RenderTarget* renderTarget,
		const Math::Matrix44 &projectionMatrix,
		const Math::Matrix44 &viewMatrix,
		const Math::Matrix44 &transformMatrix,
		const Math::Vector4 &diffuseColor,
		RenderObject* light
		); 
	
	void SetPosition( const Math::Vector3 &pos );
	Math::Vector3 GetPosition() const;
	void Rotate( const Math::Matrix44 &rotateMatrix );
	void Scale( float scale );
	void ResetTransform();
	
	const BSPNode* GetRoot() const
	{
		return mRoot;
	}

	RenderObject* GetObject() const
	{
		return mRenderMeshObject;
	}

	Mesh* GetMesh() const
	{
		return mMesh;
	}

private:

	void CreateMesh();
	void MakePolygon( BSPPolygon*& newPoly, const List<Math::Vector3> &vertices );  
	int ClassifyPoint( const Math::Plane &plane, const Math::Vector3 &point ) const;
	int ClassifyCollisionPoint( const Math::Plane &plane, const Math::Vector3 &point ) const; 
	int ClassifyPolygon( const Math::Plane &plane, BSPPolygon* poly, bool firstPass = true ) const; 
	void Split( 
		const BSPPolygon* poly, 
		const Math::Plane &splitPlane, 
		BSPPolygon* &frontsplit, 
		BSPPolygon* &backsplit 
		); 
	void BuildBSP( BSPNode* &curNode, List<BSPPolygon*> curPolyList );
	void DeleteBSP( BSPNode* &curNode );  
	void Copy( const BSP &other );
	void CopyNode( BSPNode* &node, const BSPNode* otherNode );
	void ClipTree( 
		const BSPNode* curNode, 
		const Math::Matrix44 &curNodeTransform,
		List<BSPPolygon*> curPolyList, 
		List<BSPPolygon*> &returnList, 
		bool firstPass,
		bool keepPolysInEmptySpace,
		bool invert
		);
	
	BSPNode* mRoot;
	List<BSPPolygon*> mPolygonList;
	List<BSPPolygon*> mBSPPolygonList;

	RenderObject* mRenderMeshObject;
	Mesh* mMesh;
};

#endif