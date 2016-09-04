//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/objects/bowlerobject.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/timetask.hpp"
#include "common/engine/game/timescheduler.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

void BowlerObject::OnDelayTrigger( void* context )
{
	BowlerObject* bowlerObject = reinterpret_cast<BowlerObject*>( context );
	bowlerObject->mScriptState = SCRIPT_COMMAND_FETCH;
}

//===========================================================================

BowlerObject::BowlerObject()
{
	mRenderObject = NULL;
	mPhysicsObject = NULL;
	mIsVisible = true;
	mTechnique = TECHNIQUE_OPAQUE;
	mMaterial = NULL;
	mIsTransparent = false;
	mGameObjectType = GAME_OBJECT_NONE;
	mScriptState = SCRIPT_COMMAND_FETCH;
	mCurrentScriptCommandIndex = 0;
    mIsAlive = true;

	mToggleTransitionState = TOGGLE_TRANSITION_OUT;
	mToggleState = false;
	mToggleTouched = false;
}

//===========================================================================

BowlerObject::~BowlerObject()
{
}

//===========================================================================

void BowlerObject::Update( float elapsedTime )
{
	if ( mRenderObject )
	{
		mRenderObject->Update( elapsedTime );
	}

	UpdateScript( elapsedTime );
}

//===========================================================================

void BowlerObject::Reset()
{
	mToggleState = false;
}

//===========================================================================

void BowlerObject::LoadScript( const char* filename )
{
	char filenameFullPath[256];
	const char* resourcePath = Database::Get()->GetResourcePath();
	System::StringCopy( filenameFullPath, 256, resourcePath );
	System::StringConcat( filenameFullPath, 256, "game\\bowler\\scripts\\" );
	System::StringConcat( filenameFullPath, 256, filename );
	System::StringConcat( filenameFullPath, 256, ".script" );
	System::FixedString<256> loadPath = System::Path::SystemSlash( filenameFullPath ); 

	FILE* file = System::OpenFile( loadPath, "r" );
	char command[256];
	while ( !feof( file ) )
	{
		fgets( command, 256, file );
        
        size_t length = strlen( command );
        if ( command[length - 1] == '\n')
        {
            command[--length] = 0;
        }
        
        if ( command[length - 1] == '\r')
        {
            command[--length] = 0;
        }

		mScriptCommands.push_back( command );
	}
	fclose( file );
	file = NULL;
}

//===========================================================================

void BowlerObject::AddScriptCommand( const char* command )
{
	mScriptCommands.push_back( command );
}

//===========================================================================

void BowlerObject::UpdateScript( float elapsedTime )
{
	UNUSED_ALWAYS( elapsedTime );

	switch ( mScriptState )
	{
	case SCRIPT_COMMAND_FETCH:
		{
			if ( mCurrentScriptCommandIndex >= static_cast<int>( mScriptCommands.size() ) )
			{
				mCurrentScriptCommandIndex = 0;
			}

			while ( mCurrentScriptCommandIndex < static_cast<int>( mScriptCommands.size() ) )
			{
				char currentCommand[64];
				System::StringCopy( currentCommand, 64, mScriptCommands[mCurrentScriptCommandIndex].c_str() );
				mCurrentScriptCommandIndex++;

				std::vector< std::string > commandArgs;

				char* nextToken = 0;
				const char* delimiter = " ";
				char* token = System::StringToken( currentCommand, delimiter, &nextToken );
				while ( token != NULL )
				{					
					commandArgs.push_back( token );
					token = System::StringToken( NULL, delimiter, &nextToken );
				}

				if ( commandArgs.size() > 0 )
				{
					if ( System::StringICmp( commandArgs[0].c_str(), "time" ) == 0 )
					{
						//float endTime = static_cast<float>( atof( commandArgs[1].c_str() ) );					
						mScriptState = SCRIPT_COMMAND_RUN;
						break;
					}
					else if ( System::StringICmp( commandArgs[0].c_str(), "delay" ) == 0 )
					{
						float endTime = static_cast<float>( atof( commandArgs[1].c_str() ) );						
						TimeTask newTimeTask( 0.0f, endTime, OnDelayTrigger, this );

						GameApp::Get()->GetTimeScheduler()->AddTimeTask( newTimeTask );
						mScriptState = SCRIPT_COMMAND_RUN;
						break;
					}
					else if ( System::StringICmp( commandArgs[0].c_str(), "disappear" ) == 0 )
					{
						mIsVisible = false;
						BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
						bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_REMOVE_OBJECT_FROM_PHYSICS;
						bowlerEventMessage->mContext = this;
						GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
					}
					else if ( System::StringICmp( commandArgs[0].c_str(), "appear" ) == 0 )
					{
						mIsVisible = true;
						BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
						bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_ADD_OBJECT_FROM_PHYSICS;
						bowlerEventMessage->mContext = this;
						GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
					}
				}
			}			
		}
		break;
	case SCRIPT_COMMAND_RUN:
		{
		}
		break;
	default:
		break;
	};
}