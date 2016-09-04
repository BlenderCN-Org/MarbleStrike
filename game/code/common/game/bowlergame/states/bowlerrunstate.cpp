//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/resource.hpp"
#include "common/engine/game/resourcemanager.hpp"
#include "common/engine/game/timescheduler.hpp"
#include "common/engine/math/aabb.hpp"
#include "common/engine/math/intersections.hpp"
#include "common/engine/math/sphere.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/physics/physicsdata.hpp"
#include "common/engine/physics/physicsmanager.hpp"
#include "common/engine/physics/physicsobject.hpp"
#include "common/engine/render/blenderasset.hpp"
#include "common/engine/render/blendstate.hpp"
#include "common/engine/render/boundingvolume.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/cameravolume.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/light.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/particlesystem.hpp"
#include "common/engine/render/postfx.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/rendertarget.hpp"
#include "common/engine/render/renderutilities.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/render/trigger.hpp"
#include "common/engine/system/profiler.hpp"
#include "common/engine/system/timer.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/bowlerinput.hpp"
#include "common/game/bowlergame/bowlerutilities.hpp"
#include "common/game/bowlergame/gamescript/levelscript6.hpp"
#include "common/game/bowlergame/gamescript/levelscript7.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"
#include "common/game/bowlergame/objects/bowlerobject.hpp"
#include "common/game/bowlergame/objects/bowlerskydome.hpp"
#include "common/game/bowlergame/states/bowlerrunstate.hpp"
#include "common/engine/math/quaternion.hpp"
#include <algorithm>
#include <json/json.h>

//#pragma optimize( "", off )

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

Math::Vector3 gRenderXAxis;
Math::Vector3 gRenderZAxis;
Math::Vector3 gBallPosition;

static const char* gMusicFileNames[MUSIC_TYPE_MAX] =
{
#define MUSIC_TYPE_ENTRY( ENUM, FILENAME ) FILENAME,
	MUSIC_TYPE_TUPLE
#undef MUSIC_TYPE_ENTRY
};

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//#define DRAW_SHADOW_MAP
//#define USE_POSTFX

bool mCanJump = false;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

void BowlerRunState::OnMessageCallback( void* userData )
{
	Message* message = reinterpret_cast<Message*>( userData );
	switch ( message->GetType() )
	{
	case BOWLER_MESSAGE_EVENT:
		{
			BowlerEventMessage* msg = reinterpret_cast<BowlerEventMessage*>( message );

			if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_START_GO )
			{
				SetupGameMode();
				mPause = false;
			}
			else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_REPLAY )
			{
				Restart();
			}
			else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_PAUSE )
			{
				if ( mAudioManager->IsMusicPlaying() )
				{
					mAudioManager->PauseMusic();
				}

				mPause = true;
			}
			else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_UNPAUSE )
			{
				if ( mAudioManager->IsMusicPaused() )
				{
					mAudioManager->UnpauseMusic();
				}

				mPause = false;
			}
			else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_ADD_OBJECT_FROM_PHYSICS )
			{
				if ( !mIsShutdown )
				{
					BowlerObject* bowlerObject = reinterpret_cast<BowlerObject*>( msg->mContext );
					bowlerObject->SetIsAlive( true );
					mPhysicsManager->AddRigidBody( bowlerObject->GetPhysicsObject()->GetRigidBody() );
				}

			}
			else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_REMOVE_OBJECT_FROM_PHYSICS )
			{
				if ( !mIsShutdown )
				{
					BowlerObject* bowlerObject = reinterpret_cast<BowlerObject*>( msg->mContext );
					bowlerObject->SetIsAlive( false );
					mPhysicsManager->RemoveRigidBody( bowlerObject->GetPhysicsObject()->GetRigidBody() );
				}
			}
		}
		break;
	case BOWLER_MESSAGE_TRIGGER:
		{
			TriggerMessage* msg = reinterpret_cast<TriggerMessage*>( message );

			switch ( msg->mTriggerType )
			{
			case TRIGGER_TYPE_START_ANIMATION:
				{
					size_t numObjects = mRenderObjects.size();
					for ( int i = 0; i < numObjects; ++i )
					{
						char* data = (char*)msg->mData;
						if ( strcmp( mRenderObjects[i]->GetRenderObject()->GetObjectName(), data ) == 0 )
						{
							if ( mRenderObjects[i]->GetVisible() )
							{
								mRenderObjects[i]->GetRenderObject()->StartAnimation();
							}
							break;
						}
					}
				}
				break;
			case TRIGGER_TYPE_TOGGLE:
				{
					size_t numObjects = mRenderObjects.size();
					for ( int i = 0; i < numObjects; ++i )
					{
						char* data = (char*)msg->mData;
						if ( strcmp( mRenderObjects[i]->GetRenderObject()->GetObjectName(), data ) == 0 )
						{
							mRenderObjects[i]->ToggleState();
							break;
						}
					}
				}
				break;
			case TRIGGER_TYPE_REMOVE_OBJECT:
				{
					size_t numObjects = mRenderObjects.size();
					for ( int i = 0; i < numObjects; ++i )
					{
						char* data = (char*)msg->mData;
						if ( strcmp( mRenderObjects[i]->GetRenderObject()->GetObjectName(), data ) == 0 )
						{
							PhysicsObject* physicsObject = mRenderObjects[i]->GetPhysicsObject();
							if ( physicsObject )
							{
								if ( physicsObject->GetRigidBody() && mRenderObjects[i]->GetVisible() )
								{
									mPhysicsManager->RemoveRigidBody( physicsObject->GetRigidBody() );
									mRenderObjects[i]->SetVisible( false );
									mRemovedObjectsToRestore.push_back( mRenderObjects[i] );
								}
							}
							break;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

//===========================================================================

BowlerRunState::BowlerRunState( BowlerInput* bowlerInput, PhysicsManager* physicsManager, Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )
	: BowlerState( bowlerInput, physicsManager, audioManager, bowlerDatabase )
{
	mPause = true;
	mCurrentLevel = 0;
	mEmptyTexture = NULL;
	mBlenderAsset = NULL;
	mLight = NULL;
	mBowlerSkyDome = NULL;
	mBallObject = NULL;
	mTimer = NULL;
	mMainRT = NULL;
	mPostFX = NULL;
	mFinishedTimer = NULL;
	mTotalElapsedTime = 0;
	mItemsCollected = 0;
	mIsControlOn = false;
	mFinishedState = FINISHED_STATE_NONE;
	mCamera = NULL;
	mGoalRingsRemaining = 0;
	mControlStickID = -1;
	mJumpControlID = -1;
	mJumpControlIDPC = 102;
	mLevelGameState = LEVEL_GAME_STATE_RUNNING;
	mIsShutdown = false;
	mLevelScript = NULL;
	mTimeAttackTime = 0.0f;

#if defined SHOW_DRAW_CALLS
	mNumDrawCalls = 0;
#endif
}

//===========================================================================

BowlerRunState::~BowlerRunState()
{
	mAudioManager->StopMusic();
	mAudioManager->ClearMusicList();

	size_t numConstraints = mPhysicsConstraints.size();
	for ( int i = 0; i < numConstraints; ++i )
	{
		mPhysicsManager->RemoveConstraint( mPhysicsConstraints[i] );
	}

	size_t numObjects = mRenderObjects.size();
	for ( int i = 0; i < numObjects; ++i )
	{
		PhysicsObject* physicsObject = mRenderObjects[i]->GetPhysicsObject();
		if ( physicsObject )
		{
			btRigidBody* rigidBody = physicsObject->GetRigidBody();
			if ( rigidBody != NULL && mRenderObjects[i]->GetIsAlive() )
			{
				mPhysicsManager->RemoveRigidBody( rigidBody );
			}
			DELETE_PTR( physicsObject );
		}
		DELETE_PTR( mRenderObjects[i] );
	}

	size_t numTriggers = mTriggerObjects.size();
	for ( int i = 0; i < numTriggers; ++i )
	{
		PhysicsObject* physicsObject = mTriggerObjects[i]->GetPhysicsObject();
		btRigidBody* rigidBody = physicsObject->GetRigidBody();
		if ( rigidBody != NULL )
		{
			mPhysicsManager->RemoveRigidBody( rigidBody );
		}
		DELETE_PTR( physicsObject );
		DELETE_PTR( mTriggerObjects[i] );
	}

	GameApp::Get()->GetMessageHandler()->UnRegisterListener( PORT_GAME, &BowlerRunState::OnMessageCallback, this );

	DELETE_PTR( mBowlerSkyDome );
	DELETE_PTR( mShadowRT );
	DELETE_PTR( mTimer );
	DELETE_PTR( mFinishedTimer );

	DELETE_PTR( mParticleSystem );
	DELETE_PTR( mParticleSystemGoalSplash );

#if defined USE_POSTFX
	DELETE_PTR( mPostFX );
	DELETE_PTR( mMainRT );
#endif

	ResourceManager* resourceManager = GameApp::Get()->GetResourceManager();
	resourceManager->ClearResources();
}

//===========================================================================

void BowlerRunState::Initialize()
{
	mIsShutdown = false;
	mLevelGameState = LEVEL_GAME_STATE_LOADING;

	char buffer[256];
	snprintf( buffer, 256, "game\\bowler\\level%02i", mCurrentLevel );
	//mBlenderAsset = NEW_PTR( "Blender Asset" ) BlenderAsset;
	//mBlenderAsset->Load( buffer );

	DatabaseDataReadOnly data = mBowlerDatabase->GetReadOnlyData( mCurrentLevel );
	mTimeAttackTime = data.mTimeAttackTime;
	mBowlerDatabase->SetGameMode( GameMode_Main );

	ResourceManager* resourceManager = GameApp::Get()->GetResourceManager();
	mBlenderAssetResource = resourceManager->CreateResource( RESOURCE_BLENDER_ASSET, buffer );
	resourceManager->LoadResources();
}

//===========================================================================

void BowlerRunState::Shutdown()
{
	mIsShutdown = true;
	GameApp::Get()->GetTimeScheduler()->Clear();
}

//===========================================================================

void BowlerRunState::InitializeLevel()
{
	Renderer::Get()->SetMainContext();

	SelectResolutionVariables();

	mBlenderAsset = reinterpret_cast<BlenderAsset*>( mBlenderAssetResource->GetData() );

	mParticleSystem = NEW_PTR( "Particle System" ) ParticleSystem;
	mParticleSystem->Load( "fx\\sparkle.xml" );

	mParticleSystemGoalSplash = NEW_PTR( "Particle System" ) ParticleSystem;
	mParticleSystemGoalSplash->Load( "fx\\goalsplash.xml" );

	mCameraPositionHash = System::StringHash( "gCameraPosition" );
	mDiffuseColorHash = System::StringHash( "gDiffuseColor" );
	mSpecularColorHash = System::StringHash( "gSpecularColor" );
	mLightColorHash = System::StringHash( "gLightColor" );
	mLightDirectionHash = System::StringHash( "gLightDirection" );
	mObjectToProjectionMatrixHash = System::StringHash( "gObjectToProjectionMatrix" );
	mObjectToWorldMatrixHash = System::StringHash( "gObjectToWorldMatrix" );
	mObjectToProjectionLightMatrixHash = System::StringHash( "gObjectToProjectionLightMatrix" );
	mInverseTransposeObjectToWorldMatrixHash = System::StringHash( "gInverseTransposeObjectToWorldMatrix" );
	mToonTextureHash = System::StringHash( "gToonTexture" );
	mColorTextureHash = System::StringHash( "gColorTexture" );
	mShadowTextureHash = System::StringHash( "gShadowTexture" );

    mOldControlPosition[0] = mJoystickControlPositionLeft;
	mOldControlPosition[1] = mJoystickControlPositionHeight;
	mCurrentControlPosition[0] = mOldControlPosition[0];
	mCurrentControlPosition[1] = mOldControlPosition[1];
    mControlSize[0] = mJoystickSize;
    mControlSize[1] = mJoystickSize;
	mJumpControlPosition[0] = mJumpControlPositionRight;
	mJumpControlPosition[1] = mJumpControlPositionHeight;

	ControlPosition controlPosition = mBowlerDatabase->GetControlPosition();
	if ( controlPosition == ControlPosition_Left )
	{
		mOldControlPosition[0] = mJoystickControlPositionLeft;
		mCurrentControlPosition[0] = mOldControlPosition[0];
		mJumpControlPosition[0] = mJumpControlPositionRight;
	}
	else if ( controlPosition == ControlPosition_Right )
	{
		mOldControlPosition[0] = mJoystickControlPositionRight;
		mCurrentControlPosition[0] = mOldControlPosition[0];
		mJumpControlPosition[0] = mJumpControlPositionLeft;
	}

	mJumpControlSize[0] = mJumpControlWidth;
	mJumpControlSize[1] = static_cast<float>( Database::Get()->GetAppScreenHeight() );

	mOpaqueRenderObjects.clear();
	mAlphaRenderObjects.clear();
	mObstructionObjects.clear();

	mControlCircleTexture = GameApp::Get()->GetTextureManager()->GetTexture( mJoyStickCircleTextureFilename.c_str() );
	mControlThumbTexture = GameApp::Get()->GetTextureManager()->GetTexture( mJoyStickThumbTextureFilename.c_str() );
	mControlJumpTexture = GameApp::Get()->GetTextureManager()->GetTexture( mJumpIconTextureFilename.c_str() );

	GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &BowlerRunState::OnMessageCallback, this );    BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
	bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_LEVEL_START;
	bowlerEventMessage->AddParameterInt( 0, mCurrentLevel );
	GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

#if defined USE_POSTFX
	mPostFX = NEW_PTR( "Post FX" ) PostFX;
	mPostFX->Initialize();
	mMainRT = Renderer::Get()->GetRenderFactory()->CreateRenderTarget();
	mMainRT->SetWidth( 480 *1.5 );
	mMainRT->SetHeight( 320 *1.5 );
	mMainRT->CreateColorTarget( RT_FORMAT_RGBA8, true );
	mMainRT->CreateDepthTarget( RT_FORMAT_DEPTH, false );
#else
	mMainRT = NULL;
	mPostFX = NULL;
#endif

	mTimer = NEW_PTR( "Timer" ) System::Timer;
	mTimer->Start();

	mFinishedTimer = NEW_PTR( "Finished Timer" ) System::Timer;

	mShadowRT = Renderer::Get()->GetRenderFactory()->CreateRenderTarget();
	mShadowRT->SetWidth( 256 );
	mShadowRT->SetHeight( 256 );

	mShadowEffect = GameApp::Get()->GetEffectManager()->GetEffect( "shadow" );

#if defined PLAT_WINDOWS_PHONE || defined RENDER_PLAT_D3D11 || defined PLAT_WINDOWS_8
	mShadowRT->CreateColorTarget( RT_FORMAT_RGBA8, true );
	mShadowRT->CreateDepthTarget( RT_FORMAT_DEPTH, false );
	mShadowTechnique = mShadowEffect->GetTechniqueIndex( "shadowcolor" );
#else
	mShadowRT->CreateColorTarget( RT_FORMAT_RGBA8, false );
	mShadowRT->CreateDepthTarget( RT_FORMAT_DEPTH, true );
	mShadowTechnique = 0;
#endif

	mLight = mBlenderAsset->GetRenderObjectByName( "Light" );
	Assert( mLight != NULL, "" );
	mLightStartTransform = mLight->GetTransform();

	CreateLevelScript();
	InitializeBoundingVolume();
	InitializeObjects();
	if ( mLevelScript )
	{
		mLevelScript->Initialize( mRenderObjects, mTriggerObjects );
	}

	Assert( mBallObject != NULL, "" );

	mEmptyTexture = GameApp::Get()->GetTextureManager()->GetTexture( "emptytexture" );
	mToonTexture = GameApp::Get()->GetTextureManager()->GetTexture( "game\\bowler\\textures\\toon" );

	mBowlerSkyDome = NEW_PTR( "BowlerSkyDome" ) BowlerSkyDome;

	mTotalElapsedTime = 0;
	mItemsCollected = 0;
	mIsControlOn = false;

	mObjectID = -1;
	mTouchingObject = NULL;

	InitializeCameras();

	mPause = false;
	UpdateWorld( 1.0f / 60.0f );
	mPause = true;

	Audio::MusicList musicList;
	MusicType musicType = mBowlerDatabase->GetMusicType( mCurrentLevel );
	musicList.AddFile(gMusicFileNames[musicType]);
	musicList.SetLoop(true);
	mAudioManager->SetMusicList(musicList);
	mAudioManager->PlayMusic();

	mBowlerSkyDome->Initialize();
}

//===========================================================================

void BowlerRunState::Update( float elapsedTime )
{
	switch ( mLevelGameState )
	{
	case LEVEL_GAME_STATE_LOADING:
		{
			ResourceManager* resourceManager = GameApp::Get()->GetResourceManager();
			if ( resourceManager->GetResourceManagerState() == RESOURCE_MANAGER_STATE_READY )
			{
				BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
				bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_RUN;
				bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );

				InitializeLevel();
				mLevelGameState = LEVEL_GAME_STATE_RUNNING;
			}
		}
		break;
	case LEVEL_GAME_STATE_RUNNING:
		{
			if ( mFinishedState == FINISHED_STATE_NONE )
			{
				UpdateInput( elapsedTime );
			}
			UpdateWorld( elapsedTime );
			mBowlerSkyDome->Update( elapsedTime );
		}
		break;
	}
}

//===========================================================================

void BowlerRunState::RenderUpdate()
{
	switch ( mLevelGameState )
	{
	case LEVEL_GAME_STATE_LOADING:
		{
			Renderer::Get()->SetRenderTarget( mMainRT, NULL );
			Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.0f );
			Renderer::Get()->ClearRenderer( NULL );
			GameApp::Get()->GetRenderQueue()->ClearStates();
		}
		break;
	case LEVEL_GAME_STATE_RUNNING:
		{
			SeparateOpaqueAlpha();
			RenderShadow();
			RenderMain();

			float n = mCamera->GetZNear();
			float f = mCamera->GetZFar();
			Math::Vector4 dofParams;
			dofParams[0] = 0.0f;
			dofParams[1] = 0.0f;
			dofParams[2] = n;
			dofParams[3] = f / ( f - n );

			DrawControlUI();
			RenderText();

#if defined DRAW_SHADOW_MAP
			SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
			int shadowVisualizerTechnique = simpleDraw->GetTechniqueIndex( "shadow_visualizer" );

#if defined PLAT_WINDOWS_PHONE || defined RENDER_PLAT_D3D11 || defined PLAT_WINDOWS_8
			DrawMap( 0, 0, 200, 200, shadowVisualizerTechnique, mShadowRT->GetColorTexture() );
#else
			DrawMap( 0, 0, 200, 200, shadowVisualizerTechnique, mShadowRT->GetDepthTexture() );
#endif

#endif
		}
		break;
	}
}

//===========================================================================

void BowlerRunState::InitializeCameras()
{
	int screenWidth = Database::Get()->GetAppScreenWidth();
	int screenHeight = Database::Get()->GetAppScreenHeight();

	CameraVolume* cameraVolume = GetCurrentCameraVolume();
	Assert( cameraVolume != NULL, "" );

	RenderObject* viewCamera = mBlenderAsset->GetRenderObjectByName( cameraVolume->GetCameraName() );
	Assert( viewCamera != NULL, "" );
	mCamera = reinterpret_cast<Camera*>( viewCamera->GetRenderObjectData() );
	mCamera->SetScreenWidth( screenWidth );
	mCamera->SetScreenHeight( screenHeight );

	mOriginalCameraTransform = viewCamera->GetTransform();
	mRenderCameraTransform = mOriginalCameraTransform;
	mCurrentCameraTransform = mOriginalCameraTransform;
	mTargetCameraTransform = mOriginalCameraTransform;
	mTargetCameraPosition.Set( 0, 0, 0 );
	mCurrentCameraPosition.Set( 0, 0, 0 );

	const char* trackTo = mCamera->GetTrackToName();
	if ( strcmp( trackTo, "" ) != 0 )
	{
		RenderObject* trackToRenderObject = mBlenderAsset->GetRenderObjectByName( trackTo );

		mOriginalTrackToCameraTransform = trackToRenderObject->GetTransform();
		mTrackToCameraTransform = mOriginalTrackToCameraTransform;
		mTargetTrackToCameraPosition = mTrackToCameraTransform.GetTranslation();

		Math::Matrix44 newCameraTransform = Camera::CreateTransform(
			mRenderCameraTransform.GetTranslation(),
			Math::Vector3( 0, 1, 0 ),
			mTrackToCameraTransform.GetTranslation()
			);

		mRenderCameraTransform = newCameraTransform;
	}
}

//===========================================================================

void BowlerRunState::SelectResolutionVariables()
{
	const char* resolutionString = GameApp::GetResolutionString();

	char fullFilename[256];
	System::StringCopy( fullFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullFilename, 256, "game/bowler/settings" );
	System::StringConcat( fullFilename, 256, resolutionString );
	System::StringConcat( fullFilename, 256, ".json" );
    
    //System::FixedString<256> loadPath = System::Path::SystemSlash( fullFilename );

	Json::Value root;
	Json::Reader reader;
	FILE* dataFile = System::OpenFile( fullFilename, "r" );
	Assert( dataFile != NULL, "" );
	int dataFileSize = System::GetFileSize( dataFile );
	char* settingsDataBuffer = NEW_PTR( "Data Buffer" ) char[dataFileSize];
	fread( settingsDataBuffer, sizeof( char ), dataFileSize, dataFile );
	fclose( dataFile );
	dataFile = NULL;
	bool parsingSuccessful = reader.parse( settingsDataBuffer, root );
	if ( parsingSuccessful )
	{
		const Json::Value settings = root["Settings"];
		float joystickControlPositionLeft = settings["JoystickControlPositionLeft"].asFloat();
		float joystickControlPositionRight = settings["JoystickControlPositionRight"].asFloat();
		float jumpControlPositionLeft = settings["JumpControlPositionLeft"].asFloat();
		float jumpControlPositionRight = settings["JumpControlPositionRight"].asFloat();
		float joystickSize = settings["JoystickSize"].asFloat();
		float joystickControlPositionHeight = settings["JoystickControlPositionHeight"].asFloat();
		float circleGraphicSize = settings["CircleGraphicSize"].asFloat();
		float thumbGraphicSize = settings["ThumbGraphicSize"].asFloat();
		float jumpGrahpicSize = settings["JumpGrahpicSize"].asFloat();
		float jumpControlPositionHeight = settings["JumpControlPositionHeight"].asFloat();
		float jumpControlWidth = settings["JumpControlWidth"].asFloat();
		std::string joyStickCircleTextureFilename = settings["JoyStickCircleTextureFilename"].asString();
		std::string joyStickThumbTextureFilename = settings["JoyStickThumbTextureFilename"].asString();
		std::string jumpIconTextureFilename = settings["JumpIconTextureFilename"].asString();

		mJoystickControlPositionLeft = joystickControlPositionLeft;
        mJoystickControlPositionRight = joystickControlPositionRight;
        mJumpControlPositionLeft = jumpControlPositionLeft;
        mJumpControlPositionRight = jumpControlPositionRight;
        mJoystickSize = joystickSize;
        mJoystickControlPositionHeight = joystickControlPositionHeight;
		mCircleGraphicSize = circleGraphicSize;
		mThumbGraphicSize = thumbGraphicSize;
		mJumpGrahpicSize = jumpGrahpicSize;
		mJumpControlPositionHeight = jumpControlPositionHeight;
		mJumpControlWidth = jumpControlWidth;
		mJoyStickCircleTextureFilename = joyStickCircleTextureFilename;
		mJoyStickThumbTextureFilename = joyStickThumbTextureFilename;
		mJumpIconTextureFilename = jumpIconTextureFilename;
	}
	DELETE_PTR_ARRAY( settingsDataBuffer );
}

//===========================================================================

void BowlerRunState::CreateLevelScript()
{
	switch ( mCurrentLevel )
	{
	case 6:
		mLevelScript = NEW_PTR( "LevelScript" ) LevelScript6;
		break;
	case 7:
		mLevelScript = NEW_PTR( "LevelScript" ) LevelScript7;
		break;
	default:
		break;
	}
}

//===========================================================================

void BowlerRunState::InitializeBoundingVolume()
{
	mMaxBB[0] = -99999;
	mMaxBB[1] = -99999;
	mMaxBB[2] = -99999;
	mMinBB[0] = 99999;
	mMinBB[1] = 99999;
	mMinBB[2] = 99999;

	RenderObject* worldBBObject = mBlenderAsset->GetRenderObjectByName( "WorldBB" );
	Assert( worldBBObject != NULL, "" );
	if ( worldBBObject )
	{
		mBoundingVolume = reinterpret_cast<BoundingVolume*>( worldBBObject->GetRenderObjectData() );

		Math::Matrix44 transform = worldBBObject->GetTransform();

		Math::Vector3 bb = worldBBObject->GetBoundingBox();
		Math::Vector3 halfbb = bb / 2.0f;

		Math::Vector3 boxVertex[8];
		boxVertex[0].Set( -halfbb[0], halfbb[1], -halfbb[2] );
		boxVertex[1].Set( halfbb[0], halfbb[1], -halfbb[2] );
		boxVertex[2].Set( -halfbb[0], halfbb[1], halfbb[2] );
		boxVertex[3].Set( halfbb[0], halfbb[1], halfbb[2] );
		boxVertex[4].Set( -halfbb[0], -halfbb[1], -halfbb[2] );
		boxVertex[5].Set( halfbb[0], -halfbb[1], -halfbb[2] );
		boxVertex[6].Set( -halfbb[0], -halfbb[1], halfbb[2] );
		boxVertex[7].Set( halfbb[0], -halfbb[1], halfbb[2] );

		for ( int i = 0; i < 8; ++i )
		{
			Math::Vector4 newVertex = transform * Math::Vector4( boxVertex[i], 1.0f );

			if ( newVertex[0] > mMaxBB[0] )
			{
				mMaxBB[0] = newVertex[0];
			}
			else if ( newVertex[0] < mMinBB[0] )
			{
				mMinBB[0] = newVertex[0];
			}

			if ( newVertex[1] > mMaxBB[1] )
			{
				mMaxBB[1] = newVertex[1];
			}
			else if ( newVertex[1] < mMinBB[1] )
			{
				mMinBB[1] = newVertex[1];
			}

			if ( newVertex[2] > mMaxBB[2] )
			{
				mMaxBB[2] = newVertex[2];
			}
			else if ( newVertex[2] < mMinBB[2] )
			{
				mMinBB[2] = newVertex[2];
			}
		}
	}

	mOriginalMaxBB = mMaxBB;
	mOriginalMinBB = mMinBB;
}

//===========================================================================

void BowlerRunState::InitializeObjects()
{
	int uniqueID = 0;
	int numObjects = mBlenderAsset->GetNumRenderObjects();
	for ( int i = 0; i < numObjects; ++i )
	{
		RenderObject* renderObject = mBlenderAsset->GetRenderObjectByIndex( i );
		renderObject->Initialize();

		RenderObject* proxyObject = renderObject->GetProxyObject();
		if ( proxyObject )
		{
			std::vector<RenderObject*>::const_iterator findPosition = std::find( mProxyObjects.begin(), mProxyObjects.end(), proxyObject );
			if ( findPosition == mProxyObjects.end() )
			{
				mProxyObjects.push_back( proxyObject );
			}
		}

		Math::Matrix44 currentTransform;
		RenderObjectData* renderData = renderObject->GetRenderObjectData();

		if ( renderData && ( renderData->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_MESH ) )
		{
			BowlerObject* newBowlerObject = NULL;
			newBowlerObject = NEW_PTR( "BowlerObject" ) BowlerObject;
			mRenderObjects.push_back( newBowlerObject );
			newBowlerObject->SetRenderObject( renderObject );
			const char* scriptFile = renderObject->GetScriptFile();
			if ( scriptFile[0] != '\0' )
			{
				newBowlerObject->LoadScript( scriptFile );
			}
			newBowlerObject->SetID( uniqueID++ );

			PhysicsObject* physicsObject = NEW_PTR( "PhysicsObject" ) PhysicsObject;
			Math::Matrix44 transform = renderObject->GetTransform();
			Math::Matrix44 localTransform = renderObject->GetLocalTransform();

			physicsObject->Create( renderObject, transform, localTransform );

			if ( renderObject->GetPhysicsConstraintData() )
			{
				physicsObject->CreateConstraint( renderObject );
				mPhysicsManager->AddConstraint( physicsObject->GetConstraint() );
				mPhysicsConstraints.push_back( physicsObject->GetConstraint() );
			}

			newBowlerObject->SetPhysicsObject( physicsObject );

			Mesh* mesh = reinterpret_cast<Mesh*>( renderObject->GetRenderObjectData() );
			Material* material = mBlenderAsset->GetMaterialByName( mesh->GetMaterialName() );
			if ( material != NULL )
			{
				const char* gameType = renderObject->GetGameType();
				btRigidBody* rigidBody = physicsObject->GetRigidBody();

				if ( strcmp( gameType, "goalring" ) == 0 )
				{
					rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
					newBowlerObject->SetGameObjectType( GAME_OBJECT_GOAL );
					mGoalRingObjects.push_back( newBowlerObject );
					mGoalRingsRemaining++;
				}
				else if ( strcmp( gameType, "collect" ) == 0 )
				{
					rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
					newBowlerObject->SetGameObjectType( GAME_OBJECT_COLLECTIBLE );
					mCollectibleObjects.push_back( newBowlerObject );
				}
				else if ( strcmp( gameType, "ball" ) == 0 )
				{
					mBallObject = newBowlerObject;
					newBowlerObject->SetGameObjectType( GAME_OBJECT_BALL );
					rigidBody->setContactProcessingThreshold( 0 );
					Math::Matrix44 ballTransform = GetCurrentPhysicsObjectTransform( mBallObject->GetPhysicsObject() );
					mBallStartPosition = ballTransform.GetTranslation();
					mBallPrevPosition = mBallStartPosition;
				}
				else
				{
					newBowlerObject->SetGameObjectType( GAME_OBJECT_WORLD );
				}

				if ( rigidBody )
				{
					rigidBody->setUserPointer( (void*) newBowlerObject );
					mPhysicsManager->AddRigidBody( rigidBody );

					rigidBody->getCollisionShape()->setLocalScaling(
						btVector3(
						renderObject->GetScale()[0],
						renderObject->GetScale()[1],
						renderObject->GetScale()[2] )
						);
				}
			}
			else
			{
				Assert( false, "" );
			}
		}

		if ( renderData && renderData->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_EMPTY )
		{
			BowlerObject* newBowlerObject = newBowlerObject = NEW_PTR( "BowlerObject" ) BowlerObject;
			newBowlerObject->SetRenderObject( renderObject );
			newBowlerObject->SetID( uniqueID++ );
			mRenderObjects.push_back( newBowlerObject );
		}

		if ( renderData && renderData->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_CAMERA_VOLUME )
		{
			mCameraVolumeRenderObject.push_back( renderObject );
		}

		if ( renderData && ( renderData->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_TRIGGER ) )
		{
			BowlerObject* newBowlerObject = newBowlerObject = NEW_PTR( "BowlerObject" ) BowlerObject;
			mTriggerObjects.push_back( newBowlerObject );
			newBowlerObject->SetRenderObject( renderObject );
			newBowlerObject->SetID( uniqueID++ );
			PhysicsObject* physicsObject = NEW_PTR( "PhysicsObject" ) PhysicsObject;
			Math::Matrix44 transform = renderObject->GetTransform();
			Math::Matrix44 localTransform = renderObject->GetLocalTransform();
			physicsObject->Create( renderObject, transform, localTransform );

			btRigidBody* rigidBody = physicsObject->GetRigidBody();
			rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
			newBowlerObject->SetGameObjectType( GAME_OBJECT_TRIGGER );
			newBowlerObject->SetPhysicsObject( physicsObject );
			if ( rigidBody )
			{
				rigidBody->setUserPointer( (void*) newBowlerObject );
				mPhysicsManager->AddRigidBody( rigidBody );
			}
		}
	}
}

//===========================================================================

void BowlerRunState::Restart()
{
	DatabaseDataReadOnly data = mBowlerDatabase->GetReadOnlyData( mCurrentLevel );
	mTimeAttackTime = data.mTimeAttackTime;

	BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
	bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_LEVEL_START;
	bowlerEventMessage->AddParameterInt( 0, mCurrentLevel );
	GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

	size_t numObjectsToRestore = mRemovedObjectsToRestore.size();
	for ( int i = 0; i < numObjectsToRestore; ++i )
	{
		BowlerObject* object = mRemovedObjectsToRestore[i];
		mRemovedObjectsToRestore[i]->Reset();
		mPhysicsManager->AddRigidBody( object->GetPhysicsObject()->GetRigidBody() );
		object->SetVisible( true );
	}
	mRemovedObjectsToRestore.clear();

	size_t numRenderObjects = mRenderObjects.size();
	for ( int i = 0; i < numRenderObjects; ++i )
	{
		RenderObject* renderObject = mRenderObjects[i]->GetRenderObject();
		mRenderObjects[i]->Reset();
		btTransform startTransform;
		startTransform.setIdentity();
		Math::Matrix44 transform = mRenderObjects[i]->GetTransform() * renderObject->GetTransform();
		startTransform.setFromOpenGLMatrix( &( transform[0] ) );
		PhysicsObject* physicsObject = mRenderObjects[i]->GetPhysicsObject();
		if ( physicsObject )
		{
			btRigidBody* currentRigidBody = physicsObject->GetRigidBody();
			if ( currentRigidBody != NULL )
			{
				currentRigidBody->setLinearVelocity( btVector3( 0, 0, 0 ) );
				currentRigidBody->setAngularVelocity( btVector3( 0, 0, 0 ) );
				currentRigidBody->setCenterOfMassTransform( startTransform );
			}
		}
	}

	mGoalRingsRemaining = 0;
	size_t numGoalRings = mGoalRingObjects.size();
	for ( int i = 0; i < numGoalRings; ++i )
	{
		BowlerObject* object = mGoalRingObjects[i];
		if ( object->GetVisible() == false )
		{
			mPhysicsManager->AddRigidBody( object->GetPhysicsObject()->GetRigidBody() );
			object->SetVisible( true );
		}
		mGoalRingsRemaining++;
	}

	GameMode gameMode = mBowlerDatabase->GetGameMode();
	if ( gameMode == GameMode_Main )
	{
		size_t numCollectibles = mCollectibleObjects.size();
		for ( int i = 0; i < numCollectibles; ++i )
		{
			BowlerObject* object = mCollectibleObjects[i];
			if ( object->GetVisible() == false )
			{
				mPhysicsManager->AddRigidBody( object->GetPhysicsObject()->GetRigidBody() );
				object->SetVisible( true );
			}
		}
	}

	btRigidBody* ballRigidBody = mBallObject->GetPhysicsObject()->GetRigidBody();
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin( btVector3( mBallStartPosition[0], mBallStartPosition[1], mBallStartPosition[2] ) );
	ballRigidBody->setLinearVelocity( btVector3( 0, 0, 0 ) );
	ballRigidBody->setAngularVelocity( btVector3( 0, 0, 0 ) );
	ballRigidBody->setCenterOfMassTransform( transform );
	ballRigidBody->setWorldTransform( transform );
	btMotionState* motionState = ballRigidBody->getMotionState();
	motionState->setWorldTransform( transform );

	ResetCameras();

	mLight->SetTransform( mLightStartTransform );

	mBallPrevPosition = mBallStartPosition;

	mTotalElapsedTime = 0;
	mItemsCollected = 0;
	mIsControlOn = false;
	mFinishedState = FINISHED_STATE_NONE;
	mControlStickID = -1;
	mCurrentControlPosition[0] = mOldControlPosition[0];
	mCurrentControlPosition[1] = mOldControlPosition[1];
	mJumpControlID = -1;

	mCurrentAngleOffset.Set( 0, 0, 0 );
	mOriginalAngleOffset.Set( 0, 0, 0 );
	mDesiredAngleOffset.Set( 0, 0, 0 );
	mCurrentPositionOffset.Set( 0, 0, 0 );
	mOriginalPositionOffset.Set( 0, 0, 0 );
	mDesiredPositionOffset.Set( 0, 0, 0 );

    mLevelGameState = LEVEL_GAME_STATE_LOADING;

	mPause = false;
	UpdateWorld( 1.0f / 60.0f );
	mPause = true;

    mLevelGameState = LEVEL_GAME_STATE_RUNNING;

	if (mAudioManager->IsMusicPaused())
	{
		mAudioManager->UnpauseMusic();
	}

	mMaxBB = mOriginalMaxBB;
	mMinBB = mOriginalMinBB;
}

//===========================================================================

void BowlerRunState::ResetCameras()
{
	mRenderCameraTransform = mOriginalCameraTransform;
	mCurrentCameraTransform = mOriginalCameraTransform;
	mTargetCameraTransform = mOriginalCameraTransform;
	mTargetCameraPosition.Set( 0, 0, 0 );
	mCurrentCameraPosition.Set( 0, 0, 0 );

	const char* trackTo = mCamera->GetTrackToName();
	if ( strcmp( trackTo, "" ) != 0 )
	{
		mTrackToCameraTransform = mOriginalTrackToCameraTransform;
		mTargetTrackToCameraPosition = mTrackToCameraTransform.GetTranslation();
	}
}

//===========================================================================

void BowlerRunState::UpdateWorld( float elapsedTime )
{
	const float maxSpeed = 15.0f;
	const float maxAngularSpeed = 15.0f;
	const float bottomResetDistance = -20.0f;

	Assert( mBallObject != NULL, "" );
	Assert( mPhysicsManager != NULL, "" );

	float runElapsedTime = mTimer->GetElapsedTime();
	if ( Database::Get()->GetWasSuspended() )
	{
		runElapsedTime = mTimer->GetElapsedTime();
		Database::Get()->SetWasSuspended( false );
	}

	if ( mPause )
	{
		mPhysicsManager->TurnOffSimulation();
	}
	else
	{
		mPhysicsManager->TurnOnSimulation();
	}

	if ( !mPause )
	{
		GameApp::Get()->GetTimeScheduler()->Update( elapsedTime );

        if ( mLevelGameState == LEVEL_GAME_STATE_RUNNING )
        {
		    mTotalElapsedTime += runElapsedTime;
        }

		GameMode gameMode = mBowlerDatabase->GetGameMode();
		if ( gameMode == GameMode_TimeAttack && mLevelGameState == LEVEL_GAME_STATE_RUNNING)
		{
			mTimeAttackTime -= runElapsedTime;
		}

		mParticleSystem->Update( elapsedTime );
		mParticleSystemGoalSplash->Update( elapsedTime );

		mCanJump = false;

		size_t numToggleObjects = mTriggerObjects.size();
		for ( int i = 0; i < numToggleObjects; ++i )
		{
			BowlerObject* object = mTriggerObjects[i];
			object->SetToggleTouched( false );
		}

		int numManifolds = mPhysicsManager->GetCollisionWorld()->getDispatcher()->getNumManifolds();
		for ( int i = 0; i < numManifolds; i++ )
		{
			btPersistentManifold* contactManifold =  mPhysicsManager->GetCollisionWorld()->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
			const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

			BowlerObject* bowlerA = static_cast<BowlerObject*>( obA->getUserPointer() );
			BowlerObject* bowlerB = static_cast<BowlerObject*>( obB->getUserPointer() );

			if ( bowlerA->GetGameObjectType() == GAME_OBJECT_BALL || bowlerB->GetGameObjectType() == GAME_OBJECT_BALL )
			{
				int numContactPoints = contactManifold->getNumContacts();
				for ( int i = 0; i < numContactPoints; ++i )
				{
					btManifoldPoint point = contactManifold->getContactPoint( 0 );
					if ( ( -point.m_normalWorldOnB[1] > 0.5 && bowlerB->GetGameObjectType() == GAME_OBJECT_BALL ) ||
						( point.m_normalWorldOnB[1] > 0.5 && bowlerB->GetGameObjectType() != GAME_OBJECT_BALL )
						)
					{
						mCanJump = true;
					}
				}
			}

			/*
			if (
				( bowlerA->GetGameObjectType() == GAME_OBJECT_BALL && bowlerB->GetGameObjectType() == GAME_OBJECT_WORLD ) ||
				( bowlerB->GetGameObjectType() == GAME_OBJECT_BALL && bowlerA->GetGameObjectType() == GAME_OBJECT_WORLD )
				)
			{
				if ( contactManifold->getNumContacts() > 0 )
				{
					BowlerObject* bowlerObject = NULL;
					if ( bowlerA->GetGameObjectType() == GAME_OBJECT_WORLD )
					{
						bowlerObject = bowlerA;
					}
					else
					{
						bowlerObject = bowlerB;
					}

					mTouchingObject = bowlerObject;
					mCurrentTransform = bowlerObject->GetRenderTransform();
				}
			}
			*/


			if (
				( bowlerA->GetGameObjectType() == GAME_OBJECT_BALL && bowlerB->GetGameObjectType() == GAME_OBJECT_GOAL ) ||
				( bowlerB->GetGameObjectType() == GAME_OBJECT_BALL && bowlerA->GetGameObjectType() == GAME_OBJECT_GOAL )
				)
			{
				int objectID = -1;
				if ( bowlerA->GetGameObjectType() == GAME_OBJECT_GOAL )
				{
					objectID = bowlerA->GetID();
				}
				else
				{
					objectID = bowlerB->GetID();
				}

				int numGoalRings = GetNumGoalRings();
				for ( int i = 0; i < numGoalRings; ++i )
				{
					BowlerObject* object = GetGoalRingObject( i );
					if ( object->GetID() == objectID  && object->GetVisible() )
					{
						AddToRemoveObject( object );
						object->SetVisible( false );
						break;
					}
				}
			}

			if (
				( bowlerA->GetGameObjectType() == GAME_OBJECT_BALL && bowlerB->GetGameObjectType() == GAME_OBJECT_TRIGGER ) ||
				( bowlerB->GetGameObjectType() == GAME_OBJECT_BALL && bowlerA->GetGameObjectType() == GAME_OBJECT_TRIGGER )
				)
			{
				int objectID = -1;
				if ( bowlerA->GetGameObjectType() == GAME_OBJECT_TRIGGER )
				{
					objectID = bowlerA->GetID();
				}
				else
				{
					objectID = bowlerB->GetID();
				}

				size_t numToggleObjects = mTriggerObjects.size();
				for ( int i = 0; i < numToggleObjects; ++i )
				{
					BowlerObject* object = mTriggerObjects[i];
					if ( object->GetID() == objectID  && object->GetVisible() )
					{
						object->SetToggleTouched( true );
						break;
					}
				}
			}

			if (
				( bowlerA->GetGameObjectType() == GAME_OBJECT_BALL && bowlerB->GetGameObjectType() == GAME_OBJECT_COLLECTIBLE ) ||
				( bowlerB->GetGameObjectType() == GAME_OBJECT_BALL && bowlerA->GetGameObjectType() == GAME_OBJECT_COLLECTIBLE )
				)
			{
				int objectID = -1;
				if ( bowlerA->GetGameObjectType() == GAME_OBJECT_COLLECTIBLE )
				{
					objectID = bowlerA->GetID();
				}
				else
				{
					objectID = bowlerB->GetID();
				}

				int numCollectibles = GetNumCollectibles();
				for ( int i = 0; i < numCollectibles; ++i )
				{
					BowlerObject* object = GetCollectibleObject( i );
					if ( object->GetID() == objectID  && object->GetVisible() )
					{
						RemoveCollectibleObject( object );
						break;
					}
				}
			}
		}

		numToggleObjects = mTriggerObjects.size();
		for ( int i = 0; i < numToggleObjects; ++i )
		{
			BowlerObject* object = mTriggerObjects[i];
			if ( object->GetToggleTouched() )
			{
				if ( object->GetToggleTransitionState() == TOGGLE_TRANSITION_OUT )
				{
					Trigger* trigger = reinterpret_cast<Trigger*>( object->GetRenderObject()->GetRenderObjectData() );
					if ( trigger->IsEnabled() )
					{
						trigger->OnTrigger();
					}

					object->SetToggleTransitionState( TOGGLE_TRANSITION_IN );
				}
			}
			else
			{
				if ( object->GetToggleTransitionState() == TOGGLE_TRANSITION_IN )
				{
					object->SetToggleTransitionState( TOGGLE_TRANSITION_OUT );
				}
			}
		}

		UpdateCollectibles();
		UpdateGoal();

		if ( mLevelScript )
		{
			mLevelScript->Update( elapsedTime, mRenderObjects, mTriggerObjects );
		}

		size_t numProxyObjects = mProxyObjects.size();
		for ( int i = 0; i < numProxyObjects; ++i )
		{
			mProxyObjects[i]->Update( elapsedTime );
		}

		size_t numRenderObjects = mRenderObjects.size();
		for ( int i = 0; i < numRenderObjects; ++i )
		{
			mRenderObjects[i]->Update( elapsedTime );
		}

		size_t numObjects = mRenderObjects.size();
		for ( int i = 0; i < numObjects; ++i )
		{
			BowlerObject* bowlerObject = mRenderObjects[i];
			RenderObject* renderObject = bowlerObject->GetRenderObject();
			PhysicsData* physicsData = renderObject->GetPhysicsData();
			if ( physicsData->GetPhysicsType() == PHYSICS_STATIC )
			{
				PhysicsObject* physicsObject = bowlerObject->GetPhysicsObject();
				if ( physicsObject )
				{
					btRigidBody* rigidBody = physicsObject->GetRigidBody();
					rigidBody->setCollisionFlags( rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

					Math::Matrix44 transform = bowlerObject->GetTransform() * renderObject->GetTransform();
					btTransform newTransform;
					newTransform.setFromOpenGLMatrix( &( transform[0] ) );
					rigidBody->setWorldTransform( newTransform );
					btMotionState* motionState = rigidBody->getMotionState();
					motionState->setWorldTransform( newTransform );

					rigidBody->setActivationState( DISABLE_DEACTIVATION );

					mRenderObjects[i]->SetRenderTransform( transform );

					rigidBody->getCollisionShape()->setLocalScaling(
						btVector3(
						renderObject->GetScale()[0],
						renderObject->GetScale()[1],
						renderObject->GetScale()[2] )
						);
				}
			}
			else if ( physicsData->GetPhysicsType() == PHYSICS_RIGID_BODY )
			{
				PhysicsObject* physicsObject = bowlerObject->GetPhysicsObject();
				if ( physicsObject )
				{
					Math::Matrix44 transform = GetCurrentPhysicsObjectTransform( physicsObject );
					mRenderObjects[i]->SetRenderTransform( transform );
				}
			}
		}

		btRigidBody* ballRigidBody = mBallObject->GetPhysicsObject()->GetRigidBody();
		Assert( ballRigidBody != NULL, "" );

		ballRigidBody->setDamping( 0.0f, 0.99999f );

		btVector3 linearVelocity = ballRigidBody->getLinearVelocity();
		float yVelocity = linearVelocity[1];
		linearVelocity[1] = 0;
		if ( linearVelocity.length() > maxSpeed )
		{
			linearVelocity = linearVelocity.normalize();
			linearVelocity *= maxSpeed;
			linearVelocity[1] = yVelocity;
			ballRigidBody->setLinearVelocity( linearVelocity );
		}

		btVector3 angularVelocity = ballRigidBody->getAngularVelocity();
		if ( angularVelocity.length() > maxAngularSpeed )
		{
			angularVelocity = angularVelocity.normalize();
			angularVelocity *= maxAngularSpeed;
			ballRigidBody->setAngularVelocity( angularVelocity );
		}

		Math::Matrix44 ballTransform = GetCurrentPhysicsObjectTransform( mBallObject->GetPhysicsObject() );
		Math::Vector3 ballPosition = ballTransform.GetTranslation();
		Math::Vector3 diffPosition = ballPosition - mBallPrevPosition;

		if ( mBoundingVolume->GetIsMoveableX() )
		{
			mMaxBB += Math::Vector3( diffPosition[0], 0, 0 );
			mMinBB += Math::Vector3( diffPosition[0], 0, 0 );
		}

		if ( mBoundingVolume->GetIsMoveableZ() )
		{
			mMaxBB += Math::Vector3( 0, 0, diffPosition[2] );
			mMinBB += Math::Vector3( 0, 0, diffPosition[2] );
		}

		Math::Vector3 currentBallPosition( ballPosition[0], ballPosition[1], ballPosition[2] );
		gBallPosition = currentBallPosition;

		UpdateCameras( elapsedTime );

		mBallPrevPosition = ballPosition;

		/*
		if ( mTouchingObject && mIsControlOn == false && mCanJump == true )
		{
			Math::Matrix44 bMatrix = mTouchingObject->GetRenderTransform();
			Math::Matrix44 aMatrix = mCurrentTransform;
			aMatrix.Inverse();
			btMotionState* motionState = ballRigidBody->getMotionState();
			btTransform transform;
			motionState->getWorldTransform(transform);
			btScalar m[16];
			transform.getOpenGLMatrix( m );
			Math::Matrix44 ballMatrix;
			ballMatrix.Set( m );
			ballMatrix = bMatrix * aMatrix * ballMatrix;
			transform.setFromOpenGLMatrix( & ballMatrix[0] );
			//ballRigidBody->setWorldTransform( transform );
			motionState->setWorldTransform( transform );
			ballRigidBody->setMotionState( motionState );
		}
		if ( mTouchingObject )
		{
			mCurrentTransform = mTouchingObject->GetRenderTransform();
		}
		*/


		if ( ballPosition[1] < bottomResetDistance && mFinishedState == FINISHED_STATE_NONE )
		{
			UpdateBallFalloff();
		}

		if ( mTimeAttackTime <= 0 && gameMode == GameMode_TimeAttack )
		{
			mTimeAttackTime = 0;
			mPause = true;

			BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
			bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_LEVEL_TIME_ATTACK_FAIL;
			GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
		}

		if ( mFinishedState != FINISHED_STATE_COMPLETE )
		{
			if ( mGoalRingsRemaining <= 0 && mFinishedState == FINISHED_STATE_NONE )
			{
				mFinishedTimer->Start();
				mFinishedState = FINISHED_STATE_START_TIMER;
				mTotalFinishedTime = 0;
			}

			if ( mFinishedState == FINISHED_STATE_START_TIMER )
			{
				mTotalFinishedTime += mFinishedTimer->GetElapsedTime();
				if ( mTotalFinishedTime > 1.0f )
				{
					mPause = true;

					if ( gameMode == GameMode_Main )
					{
						DatabaseDataUser data = mBowlerDatabase->GetUserData( mCurrentLevel );
						data.mCompleted = true;
						data.mNumCollected = mItemsCollected;
						if ( mTotalElapsedTime < data.mBestTime || data.mBestTime == 0 )
						{
							data.mBestTime = mTotalElapsedTime;
						}
						mBowlerDatabase->SetUserData( mCurrentLevel, data );
						mBowlerDatabase->SaveUserData();
					}
					else if ( gameMode == GameMode_TimeAttack )
					{
						DatabaseDataUser data = mBowlerDatabase->GetUserData( mCurrentLevel );

						if ( mTimeAttackTime > data.mBestTimeAttack || data.mBestTimeAttack == 0 )
						{
							data.mBestTimeAttack = mTimeAttackTime;
						}
						mBowlerDatabase->SetUserData( mCurrentLevel, data );
						mBowlerDatabase->SaveUserData();
					}

					BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
					bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_LEVEL_FINISH;
					bowlerEventMessage->AddParameterFloat( 0, mTimeAttackTime );
					GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

					mFinishedState = FINISHED_STATE_COMPLETE;
				}
			}
		}
	}
}

//===========================================================================

void BowlerRunState::UpdateCameras( float elapsedTime )
{
	CameraVolume* cameraVolume = GetCurrentCameraVolume();
	if ( cameraVolume )
	{
		RenderObject* targetCamera = mBlenderAsset->GetRenderObjectByName( cameraVolume->GetCameraName() );
		if ( targetCamera )
		{
			mTargetCameraTransform = targetCamera->GetTransform();
		}
	}

	Math::Matrix44 ballTransform = GetCurrentPhysicsObjectTransform( mBallObject->GetPhysicsObject() );
	Math::Vector3 ballPosition = ballTransform.GetTranslation();
	Math::Vector3 diffPosition = ballPosition - mBallPrevPosition;
	Math::Vector3 cameraDiffTranslation( diffPosition[0], diffPosition[1], diffPosition[2] );

	float diffMag = Math::Vector3::Magnitude( cameraDiffTranslation );
	if ( diffMag > 0.03 )
	{
		mTargetCameraPosition += cameraDiffTranslation;
		mTargetTrackToCameraPosition += cameraDiffTranslation;
	}

	Math::Vector3 cameraDir = mTargetCameraPosition - mCurrentCameraPosition;
	mCurrentCameraPosition = mCurrentCameraPosition + cameraDir * 5.0f * elapsedTime;
	mCurrentCameraTransform = mCurrentCameraTransform + ( mTargetCameraTransform - mCurrentCameraTransform ) * elapsedTime;
	mRenderCameraTransform = mCurrentCameraTransform;
	mRenderCameraTransform.SetTranslation( mCurrentCameraTransform.GetTranslation() + mCurrentCameraPosition );

	const char* trackTo = mCamera->GetTrackToName();
	if ( strcmp( trackTo, "" ) != 0 )
	{
		Math::Vector3 trackPos = mTrackToCameraTransform.GetTranslation();
		Math::Vector3 trackDir = mTargetTrackToCameraPosition - trackPos;
		trackPos = trackPos + trackDir * 5.0f * elapsedTime;
		mTrackToCameraTransform.SetTranslation( trackPos );

		Math::Matrix44 newCameraTransform = Camera::CreateTransform(
			mRenderCameraTransform.GetTranslation(),
			Math::Vector3( 0, 1, 0 ),
			mTrackToCameraTransform.GetTranslation()
			);

		mRenderCameraTransform = newCameraTransform;
	}
}

//===========================================================================

void BowlerRunState::UpdateInput( float /*elapsedTime*/ )
{
	Assert( mBowlerInput != NULL, "" );
	Assert( mBallObject != NULL, "" );

	static float jumpForce = 2400.0f;

	if ( mBowlerInput->GetToggleKey( ' ' ) )
	{
		mPause = !mPause;
	}

	if ( !mPause )
	{
		btRigidBody* ballRigidBody = mBallObject->GetPhysicsObject()->GetRigidBody();

		Assert( ballRigidBody != NULL, "" );

		std::vector<int> downResults = mBowlerInput->GetTouchInput( BOWLER_INPUT_TYPE_DOWN );
		for ( size_t i = 0; i < downResults.size(); ++i )
		{
			int ID = downResults[i];
			float currentX = mBowlerInput->GetX( ID );
			float currentY = mBowlerInput->GetY( ID );

			if (
				currentX > mOldControlPosition[0] - mControlSize[0] &&
				currentX < mOldControlPosition[0] + mControlSize[0] &&
				currentY > mOldControlPosition[1] - mControlSize[1] &&
				currentY < mOldControlPosition[1] + mControlSize[1]
			)
			{
				if ( mControlStickID == -1 )
				{
					mControlStickID = ID;
					mCurrentControlPosition[0] = currentX;
					mCurrentControlPosition[1] = currentY;
					mIsControlOn = true;
				}
			}


			if (
				currentX > mJumpControlPosition[0] - ( mJumpControlSize[0] / 2.0f ) &&
				currentX < mJumpControlPosition[0] + ( mJumpControlSize[0] / 2.0f ) &&
				currentY > 0 &&
				currentY < mJumpControlSize[1]
			)
			{
				if ( mJumpControlID == -1 )
				{
					if ( mCanJump )
					{
						ballRigidBody->applyForce( btVector3( 0, jumpForce, 0 ), btVector3( 0, 0, 0 ) );
					}

					mJumpControlID = ID;
				}
			}
		}

		std::vector<int> upResults = mBowlerInput->GetTouchInput( BOWLER_INPUT_TYPE_UP );
		for ( size_t i = 0; i < upResults.size(); ++i )
		{
			int ID = upResults[i];
			if ( ID == mControlStickID )
			{
				mCurrentControlPosition[0] = mOldControlPosition[0];
				mCurrentControlPosition[1] = mOldControlPosition[1];
				mIsControlOn = false;
				mControlStickID = -1;
			}

			if ( ID == mJumpControlID )
			{
				mJumpControlID = -1;
			}
		}

		if ( mJumpControlIDPC == 102 && mBowlerInput->GetInput( BOWLER_INPUT_TYPE_JUMP_UP ) )
		{
			mJumpControlIDPC = 103;
		}

		if ( mJumpControlIDPC == 103 && mBowlerInput->GetInput( BOWLER_INPUT_TYPE_JUMP_DOWN ) )
		{
			mJumpControlIDPC = 102;
			ballRigidBody->applyForce( btVector3( 0, jumpForce, 0 ), btVector3( 0, 0, 0 ) );
		}

		if ( mIsControlOn )
		{
			mCurrentControlPosition[0] = mBowlerInput->GetX( mControlStickID );
			mCurrentControlPosition[1] = mBowlerInput->GetY( mControlStickID );

			float xDelta = mCurrentControlPosition[0] - mOldControlPosition[0];
			float yDelta = mCurrentControlPosition[1] - mOldControlPosition[1];

			float distance2 = sqrtf( xDelta * xDelta + yDelta * yDelta );
			float targetDistance = 55.0f;
			if ( distance2 > targetDistance )
			{
				Math::Vector3 xAxis( 1, 0, 0 );
				Math::Vector3 controlDir( xDelta, -yDelta, 0 );
				controlDir = Math::Vector3::Normalize( controlDir );
				float dotProduct = Math::Vector3::Dot( xAxis, controlDir );
				float angle = Math::PIDIV2;
				if ( dotProduct != 0 )
				{
					angle = acos( dotProduct );
				}

				Math::Vector3 crossDir = Math::Vector3::Cross( xAxis, controlDir );
				if ( crossDir[2] < 0 )
				{
					angle *= -1;
				}

				mCurrentControlPosition[0] = targetDistance * cos( angle );
				mCurrentControlPosition[1] = targetDistance * -sin( angle );
				mCurrentControlPosition += mOldControlPosition;
				xDelta = mCurrentControlPosition[0] - mOldControlPosition[0];
				yDelta = mCurrentControlPosition[1] - mOldControlPosition[1];
			}

			ballRigidBody->activate( true );

			Math::Matrix44 projectionMatrix = Renderer::Get()->GetPerspectiveProjection(
				mCamera->GetFieldOfView(),
				mCamera->GetAspectRatio(),
				mCamera->GetZNear(),
				mCamera->GetZFar()
				);

			Math::Vector3 yAxis( 0, 1, 0 );

			Math::Vector3 p0( 100, 0, 0 );
			Math::Vector3 p1( 0, 0, 0 );
			Math::Matrix44 viewMatrix = mRenderCameraTransform;
			viewMatrix.InverseAffine();
			Math::Matrix44 viewPort;
			float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
			float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			viewPort.SetViewport( appScreenWidth, appScreenHeight );
			p0 = Math::UnProject( viewMatrix, projectionMatrix, viewPort, p0 );
			p1 = Math::UnProject( viewMatrix, projectionMatrix, viewPort, p1 );
			Math::Vector3 xAxis = p0 - p1;
			xAxis = Math::Vector3::Normalize( xAxis );

			Math::Vector3 zAxis = Math::Vector3::Cross( xAxis, yAxis );
			gRenderXAxis = xAxis;
			gRenderZAxis = zAxis;

			if ( mCanJump == false )
			{
				xDelta /= 1.0f;
				yDelta /= 1.0f;
				xAxis = Math::Vector3::Normalize( xAxis ) * xDelta;
				zAxis = Math::Vector3::Normalize( zAxis ) * yDelta;
				ballRigidBody->applyForce( btVector3( xAxis[0], xAxis[1], xAxis[2] ), btVector3(0,0,0) );
				ballRigidBody->applyForce( btVector3( zAxis[0], zAxis[1], zAxis[2] ), btVector3(0,0,0) );
			}
			else
			{
				float deadzone = 10.0f;
				if ( distance2 > deadzone )
				{
					xDelta *= distance2/15.0f;
					yDelta *= distance2/15.0f;
					xAxis = Math::Vector3::Normalize( xAxis ) * yDelta;
					zAxis = Math::Vector3::Normalize( zAxis ) * -xDelta;
					ballRigidBody->applyTorque( btVector3( zAxis[0], zAxis[1], zAxis[2] ) );
					ballRigidBody->applyTorque( btVector3( xAxis[0], xAxis[1], xAxis[2] ) );
				}
			}
		}
	}
}

//===========================================================================

CameraVolume* BowlerRunState::GetCurrentCameraVolume()
{
	size_t numCameraVolumes = mCameraVolumeRenderObject.size();
	for ( int i = 0; i < numCameraVolumes; ++i )
	{
		RenderObject* cameraVolumeRenderObject = mCameraVolumeRenderObject[i];

		Math::AABB aabb;
		aabb.SetPosition( cameraVolumeRenderObject->GetTransform().GetTranslation() );
		Math::Vector3 boundingBox = cameraVolumeRenderObject->GetBoundingBox();
		aabb.SetHalfSides( boundingBox / 2.0f );

		Math::Sphere sphere;
		float sphereRadius = mBallObject->GetRenderObject()->GetPhysicsData()->GetBoundingRadius();
		sphere.SetRadius( sphereRadius );
		Math::Matrix44 ballTransform = GetCurrentPhysicsObjectTransform( mBallObject->GetPhysicsObject() );
		Math::Vector3 ballPosition = ballTransform.GetTranslation();
		sphere.SetPosition( Math::Vector3( ballPosition[0], ballPosition[1], ballPosition[2] ) );

		bool intersect = Math::IntersectStatic( aabb, sphere );
		if ( intersect )
		{
			CameraVolume* cameraVolume = reinterpret_cast<CameraVolume*>( cameraVolumeRenderObject->GetRenderObjectData() );
			return cameraVolume;
		}
	}

	return NULL;
}

//===========================================================================

void BowlerRunState::SeparateOpaqueAlpha()
{
	mOpaqueRenderObjects.clear();
	mAlphaRenderObjects.clear();

	size_t numObjects = mRenderObjects.size();
	for ( int i = 0; i < numObjects; ++i )
	{
		BowlerObject* bowlerObject = mRenderObjects[i];
		mOpaqueRenderObjects.push_back( bowlerObject );
	}
}

//===========================================================================

void BowlerRunState::RenderShadow()
{
	Renderer::Get()->SetRenderTarget( mShadowRT, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );
	GameApp::Get()->GetRenderQueue()->ClearStates();

	RenderPhysicsObjectsShadow();

	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void BowlerRunState::RenderMain()
{
	Renderer::Get()->SetRenderTarget( mMainRT, NULL );
	Renderer::Get()->SetBackgroundColor( 1.0f, 0.0f, 0.0f );
	Renderer::Get()->ClearRenderer( NULL );
	GameApp::Get()->GetRenderQueue()->ClearStates();

	Math::Matrix44 projectionMatrix = Renderer::Get()->GetPerspectiveProjection(
		mCamera->GetFieldOfView(),
		mCamera->GetAspectRatio(),
		mCamera->GetZNear(),
		mCamera->GetZFar()
		);
	projectionMatrix = Render::ProjectionOrientation( projectionMatrix );

	Math::Matrix44 viewLightMatrix = GetLightView();
	Math::Matrix44 projectionLightMatrix = GetLightProjection();
	Math::Matrix44 viewMatrix = mRenderCameraTransform;
	viewMatrix.InverseAffine();

	Math::Frustum frustum;
	frustum.ExtractPlanes( projectionMatrix * viewMatrix );

	RenderPhysicsObjectsOpaque(
		mRenderCameraTransform,
		projectionMatrix,
		viewMatrix,
		projectionLightMatrix,
		viewLightMatrix,
		frustum
		);

	mBowlerSkyDome->SetRenderTarget( mMainRT );
	mBowlerSkyDome->RenderUpdate( mRenderCameraTransform, mCamera );

	mParticleSystem->Render( projectionMatrix, viewMatrix, mParticleSystemTransform );
	mParticleSystemGoalSplash->Render( projectionMatrix, viewMatrix, mParticleSystemGoalSplashTransform );

#if defined SHOW_DRAW_CALLS
	mNumDrawCalls = GameApp::Get()->GetRenderQueue()->GetNumCurrentRenderNodes();
#endif

#if defined USE_POSTFX
	GameApp::Get()->GetRenderQueue()->Render();
	mPostFX->PostPass( mMainRT );
#endif

#if defined ( _DEBUG )
	RenderDebugDraw( projectionMatrix, viewMatrix );
#endif
}

//===========================================================================

void BowlerRunState::RenderDebugDraw(
	const Math::Matrix44 &projectionMatrix,
	const Math::Matrix44 &viewMatrix
	)
{
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();

	Math::Vector3 v0 = gBallPosition + gRenderZAxis * -5.0f;
	Math::Vector3 v1 = gBallPosition + gRenderZAxis * 5.0f;

	simpleDraw->SetProjectionMatrix( projectionMatrix );
	simpleDraw->SetViewMatrix( viewMatrix );
	simpleDraw->RenderLine( v0, v1, Math::Vector4( 1, 0, 0, 1 ) );

	Math::Vector3 v2 = gBallPosition + gRenderXAxis * -5.0f;
	Math::Vector3 v3 = gBallPosition + gRenderXAxis * 5.0f;
	simpleDraw->SetProjectionMatrix( projectionMatrix );
	simpleDraw->SetViewMatrix( viewMatrix );
	simpleDraw->RenderLine( v2, v3, Math::Vector4( 0, 1, 0, 1 ) );
}

//===========================================================================

void BowlerRunState::RenderText()
{
	DatabaseDataUser data = mBowlerDatabase->GetUserData( mCurrentLevel );
	GameMode gameMode = mBowlerDatabase->GetGameMode();

	int minutes = 0;
	int seconds = 0;
	int fraction = 0;
	int prevMinutes = 0;
	int prevSeconds = 0;
	int prevFraction = 0;

	bool displayDefault = false;
	if ( gameMode == GameMode_Main )
	{
		BowlerUtilities::ExtractTime( mTotalElapsedTime, minutes, seconds, fraction );
		BowlerUtilities::ExtractTime( data.mBestTime, prevMinutes, prevSeconds, prevFraction );

		if ( data.mBestTime == 0 )
		{
			displayDefault = true;
		}
	}
	else if ( gameMode == GameMode_TimeAttack )
	{
		BowlerUtilities::ExtractTime( mTimeAttackTime, minutes, seconds, fraction );
		BowlerUtilities::ExtractTime( data.mBestTimeAttack, prevMinutes, prevSeconds, prevFraction );

		if ( data.mBestTimeAttack == 0 )
		{
			displayDefault = true;
		}
	}

	int screenWidthSizei = Database::Get()->GetAppScreenWidth();
	int screenHeightSizei = Database::Get()->GetAppScreenHeight();
	float screenWidthSizef = static_cast<float>( screenWidthSizei );
	float screenHeightSizef = static_cast<float>( screenHeightSizei );
	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection(
		0,
		0,
		screenWidthSizef,
		screenHeightSizef,
		-100,
		100
		);
	orthoMatrix = Render::ProjectionOrientation( orthoMatrix );
	Font* font = GameApp::Get()->GetFontManager()->GetFont( 0 );

	float textWidthLine1 = 0;
	float textHeightLine1 = 0;
	float textXLine1 = 0;
	float textYLine1 = 0;
	float textWidthLine2 = 0;
	float textHeightLine2 = 0;
	float textXLine2 = 0;
	float textYLine2 = 0;
	if ( screenWidthSizei == 960 && screenHeightSizei == 640 )
	{
		font->SetSize( 1.0 );

		textWidthLine1 = 400;
		textHeightLine1 = 25;
		textXLine1 = 0;
		textYLine1 = screenHeightSizef - 30;

		textWidthLine2 = 400;
		textHeightLine2 = 25;
		textXLine2 = 0;
		textYLine2 = screenHeightSizef - 70;
	}
	else if ( screenWidthSizei == 1136 && screenHeightSizei == 640 )
	{
		font->SetSize( 1.0 );

		textWidthLine1 = 400;
		textHeightLine1 = 25;
		textXLine1 = 0;
		textYLine1 = screenHeightSizef - 30;

		textWidthLine2 = 400;
		textHeightLine2 = 25;
		textXLine2 = 0;
		textYLine2 = screenHeightSizef - 70;
	}
	else if ( screenWidthSizei == 1280 && screenHeightSizei == 720 )
	{
		font->SetSize( 1.0 );

		textWidthLine1 = 400;
		textHeightLine1 = 25;
		textXLine1 = 0;
		textYLine1 = screenHeightSizef - 30;

		textWidthLine2 = 400;
		textHeightLine2 = 25;
		textXLine2 = 0;
		textYLine2 = screenHeightSizef - 70;
	}
	else if ( screenWidthSizei == 1024 && screenHeightSizei == 768 )
	{
		font->SetSize( 1.0 );

		textWidthLine1 = 400;
		textHeightLine1 = 25;
		textXLine1 = 0;
		textYLine1 = screenHeightSizef - 30;

		textWidthLine2 = 400;
		textHeightLine2 = 25;
		textXLine2 = 0;
		textYLine2 = screenHeightSizef - 70;
	}
	else
	{
		font->SetSize( 0.5 );

		textWidthLine1 = 300;
		textHeightLine1 = 20;
		textXLine1 = 0;
		textYLine1 = screenHeightSizef - 20;

		textWidthLine2 = 300;
		textHeightLine2 = 20;
		textXLine2 = 0;
		textYLine2 = screenHeightSizef - 40;
	}

	font->AddToBuffer(
		textXLine1,
		textYLine1,
		textWidthLine1,
		textHeightLine1,
		TEXT_HORIZONTAL_ALIGN_LEFT,
		TEXT_VERTICAL_ALIGN_CENTRE,
		Math::Vector4( 1, 1, 1, 0.7f ),
		"%02i'%02i\"%02i",
		minutes, seconds, fraction
		);

	char buffer[16];
	if ( displayDefault )
	{
		snprintf( buffer, 16, "--'--\"--" );
	}
	else
	{
		snprintf( buffer, 16, "%02i'%02i\"%02i", prevMinutes, prevSeconds, prevFraction  );
	}

	font->AddToBuffer(
		textXLine2,
		textYLine2,
		textWidthLine2,
		textHeightLine2,
		TEXT_HORIZONTAL_ALIGN_LEFT,
		TEXT_VERTICAL_ALIGN_CENTRE,
		Math::Vector4( 1, 1, 1, 0.7f ),
		buffer
		);


#if defined ( SHOW_DRAW_CALLS )
	font->AddToBuffer(
		0,
		screenHeightSizef - 60,
		300,
		20,
		TEXT_HORIZONTAL_ALIGN_LEFT,
		TEXT_VERTICAL_ALIGN_CENTRE,
		Math::Vector4( 1, 0, 0, 0.7f ),
		"%i",
		mNumDrawCalls
		);
#endif

	Math::Rectangle<int> scissorRectangle;
	scissorRectangle.SetPosition( 0, 0 );
	scissorRectangle.SetWidth( Database::Get()->GetBackBufferWidth() );
	scissorRectangle.SetHeight( Database::Get()->GetBackBufferHeight() );
	Math::Matrix44 transform;
	font->RenderBuffers( orthoMatrix, transform, scissorRectangle );
}

//===========================================================================

void BowlerRunState::RenderPhysicsObjectsShadow()
{
	Math::Matrix44 viewLightMatrix = GetLightView();
	Math::Matrix44 projectionLightMatrix = GetLightProjection();

	size_t numBlowerObjects = mRenderObjects.size();
	for ( int i = 0; i < numBlowerObjects; ++i )
	{
		RenderPhysicsShadowObject( mRenderObjects[i], projectionLightMatrix, viewLightMatrix );
	}
}

//===========================================================================

void BowlerRunState::RenderPhysicsShadowObject(
	BowlerObject* bowlerObject,
	const Math::Matrix44 &projectionMatrix,
	const Math::Matrix44 &viewMatrix
	)
{
	if ( bowlerObject->GetVisible() )
	{
		Math::Matrix44 transform;
		Math::Matrix44 objectToProjection;

		RenderObject* renderObject = bowlerObject->GetRenderObject();

		Math::Matrix44 scaleMatrix;
		scaleMatrix.Scale(
			renderObject->GetScale()[0],
			renderObject->GetScale()[1],
			renderObject->GetScale()[2]
		);

		if ( renderObject->GetRenderObjectData()->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_MESH )
		{
			Mesh* mesh = reinterpret_cast<Mesh*>( renderObject->GetRenderObjectData() );
			btRigidBody* rigidBody = bowlerObject->GetPhysicsObject()->GetRigidBody();
			if ( rigidBody )
			{
				transform = bowlerObject->GetRenderTransform();
			}
			else
			{
				transform = renderObject->GetTransform();
			}

			transform = transform * scaleMatrix;

			objectToProjection = projectionMatrix * viewMatrix * transform;

			RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
			char buffer[256];
			snprintf( buffer, 256, "Shadow - %s", mesh->GetMeshName() );
			renderNode->SetDescription( buffer );
#endif
			renderNode->mEffect = mShadowEffect;
			renderNode->mMesh = mesh;
			renderNode->mRenderTarget = mShadowRT;
			renderNode->mTechnique = mShadowTechnique;

			renderNode->SetMatrix( mObjectToProjectionMatrixHash, &objectToProjection );
		}
	}
}

//===========================================================================

void BowlerRunState::RenderPhysicsObjectsOpaque(
	const Math::Matrix44 &cameraMatrix,
	const Math::Matrix44 &projectionMatrix,
	const Math::Matrix44 &viewMatrix,
	const Math::Matrix44 &projectionLightMatrix,
	const Math::Matrix44 &viewLightMatrix,
	const Math::Frustum &frustum
	)
{
	BowlerObject* ballObject = NULL;
	size_t numOpaqueObjects = mOpaqueRenderObjects.size();
	for ( int i = 0; i < numOpaqueObjects; ++i )
	{
		if ( strcmp( mOpaqueRenderObjects[i]->GetRenderObject()->GetObjectName(), "Ball_proxy" ) == 0 )
		{
			ballObject = mOpaqueRenderObjects[i];
		}
		else
		{
			RenderPhysicsObject(
				mOpaqueRenderObjects[i],
				cameraMatrix,
				projectionMatrix,
				viewMatrix,
				projectionLightMatrix,
				viewLightMatrix,
				frustum
				);
		}
	}

	Assert( ballObject != NULL, "" );

	ballObject->SetTechniqueType( TECHNIQUE_OPAQUE );
	RenderPhysicsObject(
		ballObject,
		cameraMatrix,
		projectionMatrix,
		viewMatrix,
		projectionLightMatrix,
		viewLightMatrix,
		frustum
		);

	ballObject->SetTechniqueType( TECHNIQUE_HIDDEN );
	RenderPhysicsObject(
		ballObject,
		cameraMatrix,
		projectionMatrix,
		viewMatrix,
		projectionLightMatrix,
		viewLightMatrix,
		frustum
		);
}

//===========================================================================

void BowlerRunState::RenderPhysicsObjectsAlpha(
	const Math::Matrix44 &cameraMatrix,
	const Math::Matrix44 &projectionMatrix,
	const Math::Matrix44 &viewMatrix,
	const Math::Matrix44 &projectionLightMatrix,
	const Math::Matrix44 &viewLightMatrix,
	const Math::Frustum &frustum
	)
{
	size_t numAlphaObjects = mAlphaRenderObjects.size();
	for ( int i = 0; i < numAlphaObjects; ++i )
	{
		RenderPhysicsObject(
			mAlphaRenderObjects[i],
			cameraMatrix,
			projectionMatrix,
			viewMatrix,
			projectionLightMatrix,
			viewLightMatrix,
			frustum
			);
	}
}

//===========================================================================

void BowlerRunState::RenderPhysicsObject(
	BowlerObject* bowlerObject,
	const Math::Matrix44 &cameraMatrix,
	const Math::Matrix44 &projectionMatrix,
	const Math::Matrix44 &viewMatrix,
	const Math::Matrix44 &projectionLightMatrix,
	const Math::Matrix44 &viewLightMatrix,
	const Math::Frustum &frustum
	)
{
	if ( bowlerObject->GetVisible() )
	{
		Math::Matrix44 transform;
		Math::Matrix44 objectToProjection;
		Math::Matrix44 objectToProjectionLight;
		Math::Matrix44 objectToWorld;
		Math::Matrix44 inverseTransposeObjectToWorldMatrix;

		RenderObject* renderObject = bowlerObject->GetRenderObject();
		if ( renderObject->GetRenderObjectData()->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_MESH )
		{
			Mesh* mesh = reinterpret_cast<Mesh*>( renderObject->GetRenderObjectData() );
			btRigidBody* rigidBody = bowlerObject->GetPhysicsObject()->GetRigidBody();

			Material* material = mBlenderAsset->GetMaterialByName( mesh->GetMaterialName() );
			Assert( material != NULL, "" );

			bool isCulled = false;

			Math::Matrix44 scaleMatrix;
			scaleMatrix.Scale(
				renderObject->GetScale()[0],
				renderObject->GetScale()[1],
				renderObject->GetScale()[2]
			);

			if ( rigidBody )
			{
				transform = bowlerObject->GetRenderTransform();

				//if ( mIsControlOn )
				{
					rigidBody->setFriction( material->GetFriction() );
				}
				//else
				{
				//	rigidBody->setFriction( 0 );
				}

				btCollisionShape* collisionShape = rigidBody->getCollisionShape();
				if ( collisionShape )
				{
					btVector3 sphereCenter;
					btScalar sphereRadius;
					btTransform aabbTransform = rigidBody->getWorldTransform();
					btVector3 aabbMin;
					btVector3 aabbMax;
					collisionShape->getAabb( aabbTransform, aabbMin, aabbMax );
					btVector3 scaling = collisionShape->getLocalScaling();
					Math::AABB aabbShape;
					Math::Vector3 halfSides(
						(aabbMax[0] - aabbMin[0]) / 2.0f * ( scaling[0] ),
						(aabbMax[1] - aabbMin[1]) / 2.0f * ( scaling[1] ),
						(aabbMax[2] - aabbMin[2]) / 2.0f * ( scaling[2] )
						);
					aabbShape.SetHalfSides( halfSides );
					Math::Vector3 pos = transform.GetTranslation();
					Math::Sphere sphere( sphereRadius, Math::Vector3( pos[0], pos[1], pos[2] ) );
					aabbShape.SetPosition( pos );
					isCulled = !Math::IntersectStatic( frustum, aabbShape );
				}
			}
			else
			{
				transform = bowlerObject->GetTransform() * renderObject->GetTransform();
			}

			transform = transform * scaleMatrix;

			if ( !isCulled )
			{
				if ( strcmp( material->GetMaterialName(), "UIMaterial" ) == 0 )
				{
					Math::Matrix44 testMatrix = cameraMatrix;
					testMatrix.SetTranslation( Math::Vector3( 0, 0, 0 ) );
					transform = transform * testMatrix;
				}

				objectToProjection = projectionMatrix * viewMatrix * transform;
				objectToProjectionLight = projectionLightMatrix * viewLightMatrix * transform;
				objectToWorld = transform;

				inverseTransposeObjectToWorldMatrix = transform;
				inverseTransposeObjectToWorldMatrix.Inverse();
				inverseTransposeObjectToWorldMatrix = inverseTransposeObjectToWorldMatrix.Transpose();

				RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
				Texture* colorTexture = material->GetColorTexture();
#if defined ( _DEBUG )
				char buffer[256];
				snprintf( buffer, 256, "RenderPhysicsObjects - %s", mesh->GetMeshName() );
				renderNode->SetDescription( buffer );
#endif
				renderNode->mEffect = material->GetEffect();

				renderNode->mTechnique = TECHNIQUE_OPAQUE;
				if ( material->GetUseTransparency() || bowlerObject->GetTransparent() )
				{
					renderNode->mTechnique = TECHNIQUE_ALPHA;
				}

				if ( bowlerObject->GetTechniqueType() == TECHNIQUE_HIDDEN )
				{
					renderNode->mTechnique = 1;
				}

				renderNode->mMesh = mesh;
				renderNode->mRenderTarget = mMainRT;

				renderNode->SetMatrix( mObjectToProjectionMatrixHash, &objectToProjection );
				renderNode->SetMatrix( mInverseTransposeObjectToWorldMatrixHash, &inverseTransposeObjectToWorldMatrix );
				renderNode->SetMatrix( mObjectToWorldMatrixHash, &objectToWorld );
				renderNode->SetMatrix( mObjectToProjectionLightMatrixHash, &objectToProjectionLight );
				renderNode->SetVector( mCameraPositionHash, Math::Vector4( mRenderCameraTransform.GetTranslation(), 1.0f ) );
				renderNode->SetTexture( mToonTextureHash, mToonTexture );
				renderNode->SetTexture( mColorTextureHash, mEmptyTexture );
				if ( colorTexture )
				{
					renderNode->SetTexture( mColorTextureHash, colorTexture );
				}

#if defined PLAT_WINDOWS_PHONE || defined RENDER_PLAT_D3D11 || defined PLAT_WINDOWS_8
				renderNode->SetTexture( mShadowTextureHash, mShadowRT->GetColorTexture() );
#else
				renderNode->SetTexture( mShadowTextureHash, mShadowRT->GetDepthTexture() );
#endif

				Math::Vector4 diffuseColor = material->GetDiffuseColor();
				if ( bowlerObject->GetTransparent() )
				{
					diffuseColor[3] = 0.3f;
				}

				if ( bowlerObject->GetToggleState() )
				{
					diffuseColor[0] = 1.0f;
					diffuseColor[1] = 0.0f;
					diffuseColor[2] = 1.0f;
				}

				renderNode->SetVector( mDiffuseColorHash, diffuseColor );
				renderNode->SetVector( mSpecularColorHash, material->GetSpecularColor() );

				Light* light = reinterpret_cast<Light*>( mLight->GetRenderObjectData() );
				renderNode->SetVector( mLightColorHash, light->GetColor() );

				Math::Vector4 lightDirection( 0, 0, -1, 0 );
				lightDirection = mLight->GetTransform() * lightDirection;
				renderNode->SetVector( mLightDirectionHash, lightDirection );
			}
		}
	}
}

//===========================================================================

void BowlerRunState::DrawMap( int x, int y, int width, int height, int techniqueIndex, Texture* texture )
{
	float screenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() );
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );

	float fWidth = static_cast<float>( width );
	float fHeight = static_cast<float>( height );
	float fX = static_cast<float>( x );
	float fY = static_cast<float>( y );

	Math::Vector3 v0( 0, 0, 0 );
	Math::Vector3 v1( fWidth, 0, 0 );
	Math::Vector3 v2( fWidth, fHeight, 0 );
	Math::Vector3 v3( 0, fHeight, 0 );
	Math::Vector2 tex0( 0, 0 );
	Math::Vector2 tex1( 1, 0 );
	Math::Vector2 tex2( 1, 1 );
	Math::Vector2 tex3( 0, 1 );

	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection(
		0,
		0,
		screenWidthSize,
		screenHeightSize,
		-10,
		10
		);
	orthoMatrix = Render::ProjectionOrientation( orthoMatrix );

	Math::Matrix44 transform;
	transform.Translate( fX, fY, 0 );

	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();

	simpleDraw->SetTechniqueIndex( techniqueIndex );
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetColorTexture( texture );
	simpleDraw->RenderQuad( v0, v1, v2, v3, tex0, tex1, tex2, tex3 );
}

//===========================================================================

void BowlerRunState::DrawControlUI()
{
	float screenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() );
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );

	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection(
		0,
		0,
		screenWidthSize,
		screenHeightSize,
		-10,
		10
		);
	orthoMatrix = Render::ProjectionOrientation( orthoMatrix );

	Math::Matrix44 transform;

	Math::Vector3 v0( -mCircleGraphicSize, -mCircleGraphicSize, 0 );
	Math::Vector3 v1( mCircleGraphicSize, -mCircleGraphicSize, 0 );
	Math::Vector3 v2( mCircleGraphicSize, mCircleGraphicSize, 0 );
	Math::Vector3 v3( -mCircleGraphicSize, mCircleGraphicSize, 0 );

	Math::Vector2 t0( 0, 0 );
	Math::Vector2 t1( 1, 0 );
	Math::Vector2 t2( 1, 1 );
	Math::Vector2 t3( 0, 1 );

	transform.SetIdentity();
	transform.Translate( mOldControlPosition[0], screenHeightSize - mOldControlPosition[1], 0 );
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetDiffuseColor( Math::Vector4( 1, 1, 1, 0.5 ) );
	simpleDraw->SetColorTexture( mControlCircleTexture );
	simpleDraw->RenderQuad( v0, v1, v2, v3, t0, t1, t2, t3 );

	v0.Set( -mThumbGraphicSize, -mThumbGraphicSize, 0 );
	v1.Set( mThumbGraphicSize, -mThumbGraphicSize, 0 );
	v2.Set( mThumbGraphicSize, mThumbGraphicSize, 0 );
	v3.Set( -mThumbGraphicSize, mThumbGraphicSize, 0 );

	t0.Set( 0, 0 );
	t1.Set( 1, 0 );
	t2.Set( 1, 1 );
	t3.Set( 0, 1 );

	transform.SetIdentity();
	transform.Translate( mCurrentControlPosition[0], screenHeightSize - mCurrentControlPosition[1], 0 );
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetDiffuseColor( Math::Vector4( 1, 1, 1, 0.5 ) );
	simpleDraw->SetColorTexture( mControlThumbTexture );
	simpleDraw->RenderQuad( v0, v1, v2, v3, t0, t1, t2, t3 );

#if defined (_DEBUG)
	v0.Set( -mControlSize[0], -mControlSize[1], 0 );
	v1.Set( mControlSize[0], -mControlSize[1], 0 );
	v2.Set( mControlSize[0], mControlSize[1], 0 );
	v3.Set( -mControlSize[0], mControlSize[1], 0 );
	transform.SetIdentity();
	transform.Translate( mCurrentControlPosition[0], screenHeightSize - mCurrentControlPosition[1], 0 );
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetDiffuseColor( Math::Vector4( 1, 0, 0, 0.5 ) );
	simpleDraw->RenderQuad( v0, v1, v2, v3 );

	v0.Set( -( mJumpControlSize[0] / 2.0f ), 0, 0 );
	v1.Set( ( mJumpControlSize[0] / 2.0f ), 0, 0 );
	v2.Set( ( mJumpControlSize[0] / 2.0f ), mJumpControlSize[1], 0 );
	v3.Set( -( mJumpControlSize[0] / 2.0f ), mJumpControlSize[1], 0 );
	transform.SetIdentity();
	transform.Translate( mJumpControlPosition[0], 0, 0 );
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetDiffuseColor( Math::Vector4( 1, 0, 0, 0.5 ) );
	simpleDraw->RenderQuad( v0, v1, v2, v3 );
#endif

	v0.Set( -mJumpGrahpicSize, -mJumpGrahpicSize, 0 );
	v1.Set( mJumpGrahpicSize, -mJumpGrahpicSize, 0 );
	v2.Set( mJumpGrahpicSize, mJumpGrahpicSize, 0 );
	v3.Set( -mJumpGrahpicSize, mJumpGrahpicSize, 0 );;
	transform.SetIdentity();
	transform.Translate( mJumpControlPosition[0], screenHeightSize - mJumpControlPosition[1], 0 );
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetDiffuseColor( Math::Vector4( 1, 1, 1, 0.5 ) );
	simpleDraw->SetColorTexture( mControlJumpTexture );
	simpleDraw->RenderQuad( v0, v1, v2, v3, t0, t1, t2, t3 );
}

//===========================================================================

void BowlerRunState::RemoveCollectibleObject( BowlerObject* object )
{
	mAudioManager->PlayAudio(SOUND_TYPE_RING);

	AddToRemoveObject(object);
	object->SetVisible( false );
}

//===========================================================================

void BowlerRunState::UpdateCollectibles()
{
	std::vector< BowlerObject* > removeObjects;

	size_t numToRemove = mToRemove.size();
	for ( int i = 0; i < numToRemove; ++i )
	{
		BowlerObject* object = mToRemove[i];

		mParticleSystemTransform.SetTranslation( object->GetRenderObject()->GetTransform().GetTranslation() );
		mParticleSystem->Play();

		mUserCollectibles.push( object );
		mPhysicsManager->RemoveRigidBody( object->GetPhysicsObject()->GetRigidBody() );

		if ( object->GetGameObjectType() == GAME_OBJECT_COLLECTIBLE )
		{
			mItemsCollected++;

			BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
			bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_COLLECT;
			GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

			removeObjects.push_back( object );
		}
	}

	numToRemove = removeObjects.size();
	for ( int i = 0; i < numToRemove; ++i )
	{
		mToRemove.erase( std::find( mToRemove.begin(), mToRemove.end(), removeObjects[i] ) );
	}
}

//===========================================================================

void BowlerRunState::UpdateGoal()
{
	std::vector< BowlerObject* > removeObjects;

	size_t numToRemove = mToRemove.size();
	for ( int i = 0; i < numToRemove; ++i )
	{
		BowlerObject* object = mToRemove[i];

		mParticleSystemGoalSplashTransform.SetTranslation( object->GetRenderObject()->GetTransform().GetTranslation() );
		mParticleSystemGoalSplash->Play();

		mPhysicsManager->RemoveRigidBody( object->GetPhysicsObject()->GetRigidBody() );

		if ( object->GetGameObjectType() == GAME_OBJECT_GOAL )
		{
			mGoalRingsRemaining--;
			removeObjects.push_back( object );
		}
	}

	numToRemove = removeObjects.size();
	for ( int i = 0; i < numToRemove; ++i )
	{
		mToRemove.erase( std::find( mToRemove.begin(), mToRemove.end(), removeObjects[i] ) );
	}
}

//===========================================================================

void BowlerRunState::UpdateBallFalloff()
{
	btRigidBody* ballRigidBody = mBallObject->GetPhysicsObject()->GetRigidBody();

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin( btVector3( mBallStartPosition[0], mBallStartPosition[1], mBallStartPosition[2] ) );
	ballRigidBody->setLinearVelocity( btVector3( 0, 0, 0 ) );
	ballRigidBody->setAngularVelocity( btVector3( 0, 0, 0 ) );
	ballRigidBody->setCenterOfMassTransform( transform );

	ResetCameras();

	mBallPrevPosition = mBallStartPosition;

	BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
	bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_REMOVE_COLLECT;
	GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

	size_t numObjectsToRestore = mRemovedObjectsToRestore.size();
	for ( int i = 0; i < numObjectsToRestore; ++i )
	{
		BowlerObject* object = mRemovedObjectsToRestore[i];
		mPhysicsManager->AddRigidBody( object->GetPhysicsObject()->GetRigidBody() );
		object->SetVisible( true );
	}
	mRemovedObjectsToRestore.clear();

	if ( !mUserCollectibles.empty() )
	{
		BowlerObject* object = mUserCollectibles.top();
		mUserCollectibles.pop();
		mPhysicsManager->AddRigidBody( object->GetPhysicsObject()->GetRigidBody() );
		object->SetVisible( true );
		mItemsCollected--;
	}

	size_t numObjects = mRenderObjects.size();
	for ( int i = 0; i < numObjects; ++i )
	{
		mRenderObjects[i]->GetRenderObject()->Reset();
	}

	mMaxBB = mOriginalMaxBB;
	mMinBB = mOriginalMinBB;
}

//===========================================================================

Math::Matrix44 BowlerRunState::GetLightProjection()
{
	Math::Matrix44 lightTransform = mLight->GetTransform();
	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection(
		mMinBB[0],
		-mMaxBB[2],
		mMaxBB[0],
		-mMinBB[2],
		lightTransform.GetTranslation()[1] - mMaxBB[1],
		lightTransform.GetTranslation()[1] - mMinBB[1]
	);
	orthoMatrix = Render::ProjectionOrientation( orthoMatrix );

	return orthoMatrix;
}

//===========================================================================

Math::Matrix44 BowlerRunState::GetLightView()
{
	Math::Matrix44 lightTransform = mLight->GetTransform();
	Math::Matrix44 viewLightMatrix = lightTransform;
	viewLightMatrix.InverseAffine();

	return viewLightMatrix;
}

//===========================================================================

Math::Matrix44 BowlerRunState::GetCurrentPhysicsObjectTransform( PhysicsObject* physicsObject )
{
	Math::Matrix44 transform;
	btRigidBody* rigidBody = physicsObject->GetRigidBody();
	if ( rigidBody )
	{
		btScalar m[16];
		btDefaultMotionState* myMotionState = (btDefaultMotionState*)rigidBody->getMotionState();
		myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
		//btTransform worldTransform = rigidBody->getWorldTransform();
		//worldTransform.getOpenGLMatrix( m );

		transform.Set( m );
	}

	return transform;
}

//===========================================================================

void BowlerRunState::SetupGameMode()
{
	GameMode gameMode = mBowlerDatabase->GetGameMode();

	switch ( gameMode )
	{
	case GameMode_Main:
		{
			size_t numCollectibles = mCollectibleObjects.size();
			for ( size_t i = 0; i < numCollectibles; ++i )
			{
				BowlerObject* object = mCollectibleObjects[i];
				if ( object->GetVisible() == false )
				{
					mPhysicsManager->AddRigidBody( object->GetPhysicsObject()->GetRigidBody() );
					object->SetVisible( true );
				}
			}
		}
		break;
	case GameMode_TimeAttack:
		{
			DatabaseDataReadOnly data = mBowlerDatabase->GetReadOnlyData( mCurrentLevel );
			mTimeAttackTime = data.mTimeAttackTime;

			size_t numCollectibles = mCollectibleObjects.size();
			for ( size_t i = 0; i < numCollectibles; ++i )
			{
				BowlerObject* object = mCollectibleObjects[i];
				object->SetVisible( false );
				mPhysicsManager->RemoveRigidBody( object->GetPhysicsObject()->GetRigidBody() );
			}
		}
		break;
	default:
		break;
	}
}

//#pragma optimize( "", on )