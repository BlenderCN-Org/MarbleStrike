#ifndef PARTICLE_EDITOR_APP_HPP
#define PARTICLE_EDITOR_APP_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/game.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/vector4.hpp"
#include <string>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Camera;
class ParticleSystem;
class RenderObject;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ParticleEditorApp : public Game
{
public:

	ParticleEditorApp();
	~ParticleEditorApp();

	void Initialize();
	void Update( float elapsedTime );
	void RenderUpdate();
	
	void OnLeftDown( int x, int y );
	void OnLeftUp( int x, int y );
	void OnMotion( int x, int y );	
	
	void Open( const char* filename );
	void Save( const char* filename );	
	void Clear();
		
	void PlayAnimation();
	void StopAnimation();

	ParticleSystem* GetParticleSystem()
	{
		return mParticleSystem;
	}

private:	

	RenderObject* mRenderObjectCamera;
	Camera* mViewCamera;	
	Math::Vector2Base<int> mOnDownPosition;
	Math::Vector2Base<int> mMotionData;	
	bool mOnDown;
	ParticleSystem* mParticleSystem;
};

#endif
