#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/render/simpledraw.hpp"
#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Particle;
class SimpleDraw;
class Effect;
class SimpleMesh;
namespace tinyxml2
{
	class XMLNode;;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum ParticleAnimationState
{
	PARTICLE_ANIMATION_PLAY,
	PARTICLE_ANIMATION_STOP,	
};

struct ParticleVertex
{
	Math::Vector3 mPosition;
	Math::Vector4 mColor;
	Math::Vector2 mTexCoord;
};

//////////////////////////////////////////////////////
// STRUCTURE
//////////////////////////////////////////////////////

template <typename T>
class ValueRange
{
public:
	ValueRange()
	{		
	}

	T GetMin()
	{
		return mMin;
	}

	T GetMax()
	{
		return mMax;
	}

	void SetRange( T min, T max )
	{
		mMin = min;
		mMax = max;
		mLength = max - min;
	}

	T RandomValue()
	{
		T random = mLength * Math::RandomFloat1();
		random = mMin + random;
		return random;
	}

private:

	T mMin;
	T mMax;
	T mLength;
};

struct ForceData
{
	char mForceName[32];
	Math::Vector3 mForce;

	bool operator==( const ForceData &other ) const
	{
		if ( strcmp( other.mForceName, mForceName ) == 0 )
		{
			return true;
		}

		return false;
	}
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ParticleSystem
{
public:

    ParticleSystem();
	~ParticleSystem();

	void Load( const char* filename );
	void Save( const char* filename );
	void Update( float elapsedTime );	
	void Render( 
		const Math::Matrix44 &projectionMatrix,
		const Math::Matrix44 &viewMatrix,		
		const Math::Matrix44 &transformMatrix
		);

	void SetName( const char* name )
	{
		System::StringCopy( mName, 32, name );
	}

	const char* GetName()
	{
		return mName;
	}

	void SetPosition( const Math::Vector3 &position )
	{
		mPosition = position;
	}

	Math::Vector3 GetPosition()
	{
		return mPosition;
	}

	void SetMaxParticles( int size );

	int GetMaxParticles()
	{
		return mMaxParticles;
	}

	void SetEmissionRange( float minValue, float maxValue )
	{
		mEmissionRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetEmissionRange()
	{
		return mEmissionRange;
	}

	void SetSizeRange( float minValue, float maxValue )
	{
		mSizeRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetSizeRange()
	{
		return mSizeRange;
	}

	void SetShrinkRange( float minValue, float maxValue )
	{
		mShrinkRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetShrinkRange()
	{
		return mShrinkRange;
	}

	void SetVelocityXRange( float minValue, float maxValue )
	{
		mVelocityXRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetVelocityXRange()
	{
		return mVelocityXRange;
	}

	void SetVelocityYRange( float minValue, float maxValue )
	{
		mVelocityYRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetVelocityYRange()
	{
		return mVelocityYRange;
	}

	void SetVelocityZRange( float minValue, float maxValue )
	{
		mVelocityZRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetVelocityZRange()
	{
		return mVelocityZRange;
	}

	void SetLifeRange( float minValue, float maxValue )
	{
		mLifeRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetLifeRange()
	{
		return mLifeRange;
	}

	void SetDecayRange( float minValue, float maxValue )
	{
		mDecayRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetDecayRange()
	{
		return mDecayRange;
	}

	void SetColorRange( Math::Vector4 minValue, Math::Vector4 maxValue )
	{
		mColorRange.SetRange( minValue, maxValue );
	}

	ValueRange<Math::Vector4> GetColorRange()
	{
		return mColorRange;
	}

	void SetAlphaRange( float minValue, float maxValue )
	{
		mAlphaRange.SetRange( minValue, maxValue );
	}

	ValueRange<float> GetAlphaRange()
	{
		return mAlphaRange;
	}

	void SetEmitTime( float time )
	{
		mEmitTime = time;
		mCurrentEmitTime = mEmitTime;
	}

	float GetEmitTime()
	{
		return mEmitTime;
	}

	Texture* GetTexture()
	{
		return mTexture;
	}

	void SetTexture( Texture* texture )
	{
		mTexture = texture;
	}

	std::list<ForceData>& GetForceDataList()
	{
		return mForces;
	}

	void IsLooping( bool value )
	{
		mIsLooping = value;
	}

	ForceData* GetForceData( const char* name );
	void AddForce( const char* name, float x, float y, float z );
	void DeleteForce( const char* name );
	void Play();
	void Stop();

private:

	void EmitParticles( float elapsedTime );
	void LoadXML( tinyxml2::XMLNode* node );	
		
	ParticleAnimationState mParticleAnimationState;
	Particle* mParticles;
	ParticleVertex* mVertexData;	
	int mNumLiveParticles;
	Texture* mTexture;
	Texture* mEmptyTexture;
	float mParticleAccumulation;	

	Effect* mEffect;
	SimpleMesh* mSimpleMesh;
    int mObjectToProjectionMatrixHash;
    int mTextureHash;

	char mName[32];
	int mMaxParticles;
	Math::Vector3 mPosition;
	ValueRange<float> mEmissionRange;
	ValueRange<float> mSizeRange;
	ValueRange<float> mShrinkRange;
	ValueRange<float> mVelocityXRange;
	ValueRange<float> mVelocityYRange;
	ValueRange<float> mVelocityZRange;
	ValueRange<float> mLifeRange;
	ValueRange<float> mDecayRange;
	ValueRange<Math::Vector4> mColorRange;
	ValueRange<float> mAlphaRange;
	float mEmitTime;
	float mCurrentEmitTime;
	bool mIsEmitting;
	std::list<ForceData> mForces;
	bool mIsLooping;
};

#endif