#ifndef PARTICLE_HPP
#define PARTICLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/math/vector4.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum ParticleState
{
	PARTICLE_STATE_CREATED,
	PARTICLE_STATE_LIVE,
	PARTICLE_STATE_DESTROYED
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Particle
{
public:

    Particle();
	~Particle();

	void Update( float elapsedTime );
	
	ParticleState mState;
	Math::Vector3 mPosition;
	Math::Vector3 mVelocity;		
	Math::Vector4 mColor;	
	float mLife;
	float mSize;
	float mShrink;
	float mDecay;
	float mStartLife;
};

#endif