#ifndef RENDERNODE_HPP
#define RENDERNODE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/rectangle.hpp"
#include "common/engine/math/matrix44.hpp"
#include <stdio.h>
#include <map>
#include <string>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Effect;
class ConstantBuffer;
class SimpleMesh;
class Mesh;
class RenderStateShadowing;
class RenderTarget;
class Texture;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderNode
{
public:

	RenderNode();
	~RenderNode();
	void Clear();

#if defined ( _DEBUG )
	void SetDescription( const char* text );
#endif

	void Render( 
		RenderStateShadowing* renderStateShadowing, 
		int threadID,
		void* context 
		);

	RenderTarget* mRenderTarget;
	Effect* mEffect;	
	int mTechnique;
	SimpleMesh* mSimpleMesh;
	Mesh* mMesh;
	Math::Rectangle<int> mScissorRectangle;
	
	void SetVector( int nameHash, const Math::Vector4* data, int num = 1 );
	void SetVector( int nameHash, const Math::Vector4 &data );
	void SetMatrix( int nameHash, const Math::Matrix44* data, int num = 1 );
	void SetTexture( int nameHash, const Texture* texture );
	
private:
		
#if defined ( _DEBUG )
	char mDescription[64];
#endif

	float* mFloatData;
	int mCurrentNumFloats;
	std::map<int, const void*> mConstantMap;

};

#endif

