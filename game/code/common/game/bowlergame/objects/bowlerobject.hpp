#ifndef BOWLER_OBJECT_HPP
#define BOWLER_OBJECT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include <vector>
#include <string>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderObject;
class PhysicsObject;
class Material;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum TechniqueType
{
	TECHNIQUE_OPAQUE,
	TECHNIQUE_ALPHA,
    TECHNIQUE_HIDDEN
};

enum GameObjectType
{
    GAME_OBJECT_NONE,
    GAME_OBJECT_WORLD,
    GAME_OBJECT_BALL,
    GAME_OBJECT_COLLECTIBLE,
    GAME_OBJECT_TRIGGER,
    GAME_OBJECT_GOAL,
	GAME_OBJECT_TOGGLE
};

enum ScriptState
{
	SCRIPT_COMMAND_FETCH,
	SCRIPT_COMMAND_RUN
};

enum ToggleTransitionState
{
	TOGGLE_TRANSITION_IN,
	TOGGLE_TRANSITION_OUT
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerObject
{
public:

	static void OnDelayTrigger( void* context );

	BowlerObject();
	~BowlerObject();

    virtual void Update( float elapsedTime );
	void Reset();

	void SetID( int id )
	{
		mID = id;
	}

	int GetID()
	{
		return mID;
	}

    void SetGameObjectType( GameObjectType gameObjectType )
    {
        mGameObjectType = gameObjectType;
    }

    GameObjectType GetGameObjectType()
    {
        return mGameObjectType;
    }

	RenderObject* GetRenderObject()
	{
		return mRenderObject;
	}

	void SetRenderObject( RenderObject* renderObject )
	{
		mRenderObject = renderObject;
	}

	PhysicsObject* GetPhysicsObject()
	{
		return mPhysicsObject;
	}

	void SetPhysicsObject( PhysicsObject* physicsObject )
	{
		mPhysicsObject = physicsObject;
	}
	
	void SetVisible( bool value )
	{
		mIsVisible = value;
	}

	bool GetVisible()
	{
		return mIsVisible;
	}

	void SetTransform( const Math::Matrix44 &transform )
	{
		mTransform = transform;
	}

	Math::Matrix44 GetTransform()
	{
		return mTransform;
	}

	void SetRenderTransform( const Math::Matrix44 &transform )
	{
		mRenderTransform = transform;
	}

	Math::Matrix44 GetRenderTransform()
	{
		return mRenderTransform;
	}

	void SetTransparent( bool value )
	{
		mIsTransparent = value;
	}

	bool GetTransparent()
	{
		return mIsTransparent;
	}

	void SetMaterial( Material* material )
	{
		mMaterial = material;
	}

	Material* GetMaterial()
	{
		return mMaterial;
	}

	void SetTechniqueType( TechniqueType type )
	{
		mTechnique = type;
	}

	TechniqueType GetTechniqueType()
	{
		return mTechnique;
	}

	void LoadScript( const char* filename );
	void AddScriptCommand( const char* command );	

    void SetIsAlive( bool isAlive )
    {
        mIsAlive = isAlive;    
    }

    bool GetIsAlive()
    {
        return mIsAlive;
    }

	void ToggleState()
	{
		mToggleState = !mToggleState;
	}

	bool GetToggleState()
	{
		return mToggleState;
	}

	void SetToggleTransitionState( ToggleTransitionState state )
	{
		mToggleTransitionState = state;
	}

	ToggleTransitionState GetToggleTransitionState()
	{
		return mToggleTransitionState;
	}

	void SetToggleTouched( bool value )
	{
		mToggleTouched = value;
	}

	bool GetToggleTouched()
	{
		return mToggleTouched;
	}	

private:

	void UpdateScript( float elapsedTime );
	void OnTimeTriggerScript( int );

	int mID;	
	RenderObject* mRenderObject;	
	PhysicsObject* mPhysicsObject;
	Material* mMaterial;
	TechniqueType mTechnique;
	bool mIsVisible;
	Math::Matrix44 mTransform;
	Math::Matrix44 mRenderTransform;
	bool mIsTransparent;
    GameObjectType mGameObjectType;
	ScriptState mScriptState;
    bool mIsAlive;

	ToggleTransitionState mToggleTransitionState;
	bool mToggleTouched;
	bool mToggleState;

	std::vector<std::string> mScriptCommands;	
	int mCurrentScriptCommandIndex;	
};

#endif