#if defined AUDIO_OPENSL

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/opensl/openslaudiomanager.hpp"
#include "common/engine/audio/opensl/openslaudiostaticsource.hpp"
#include "common/engine/audio/opensl/openslaudiostreamsource.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//namespace Audio
//{
//	const int AUDIO_DEFAULT_SAMPLE_RATE = 44100;
//}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	OpenSLAudioManager::OpenSLAudioManager()
	{
		mOpenSLEngineObject = NULL;
		mOpenSLEngineEngine = NULL;
		mOutputMixObject = NULL;
	}

	//===========================================================================

	OpenSLAudioManager::~OpenSLAudioManager()
	{
	}

	//===========================================================================

	void OpenSLAudioManager::Initialize()
	{
		InitializeOpenSL();
		OpenSLAudioStreamSource* openSLAudioStreamSource = NEW_PTR( "OpenSLAudioStreamSource" ) OpenSLAudioStreamSource;        
		mStreamSource = openSLAudioStreamSource;
		openSLAudioStreamSource->SetOpenSLEngineEngine( mOpenSLEngineEngine );
		openSLAudioStreamSource->SetOutputMixObject( mOutputMixObject );
	}

	//===========================================================================

	void OpenSLAudioManager::Shutdown()
	{
		AudioManager::Shutdown();

		ShutdownOpenSL();
	}

	//===========================================================================

	void OpenSLAudioManager::SetSoundBank( const SoundBank& soundBank )
	{        
		ClearSoundBank();

		int bankSize = soundBank.GetSize();

		mStaticSources.resize( bankSize );

		for ( int i = 0; i < bankSize; i++ )
		{
			const char* filename = soundBank.GetFileAt( i );
			OpenSLAudioStaticSource* src = NEW_PTR( "AudioStaticSource" ) OpenSLAudioStaticSource;
			src->SetOpenSLEngineEngine( mOpenSLEngineEngine );
			src->SetOutputMixObject( mOutputMixObject );
			src->Load( filename );
									
			mStaticSources[i] = src;			
		}
	}

	//===========================================================================

	void OpenSLAudioManager::InitializeOpenSL()
	{
		SLresult result;
		const SLuint32 numInterfaces = 1;
		const SLInterfaceID interfaceIDs[] = { SL_IID_ENGINE };
		const SLboolean interfaceRequired[] = { SL_BOOLEAN_TRUE };
		const SLuint32 outputNumInterfaces = 0;
		const SLInterfaceID outpuInterfaceIDs[] = {};
		const SLboolean outpuInterfaceRequired[]={};

		result = slCreateEngine( &mOpenSLEngineObject, 0, NULL, numInterfaces, interfaceIDs, interfaceRequired );
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mOpenSLEngineObject)->Realize( mOpenSLEngineObject, SL_BOOLEAN_FALSE);
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mOpenSLEngineObject)->GetInterface( mOpenSLEngineObject, SL_IID_ENGINE, &mOpenSLEngineEngine );
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mOpenSLEngineEngine)->CreateOutputMix( mOpenSLEngineEngine, &mOutputMixObject, outputNumInterfaces, outpuInterfaceIDs, outpuInterfaceRequired );
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mOutputMixObject)->Realize( mOutputMixObject, SL_BOOLEAN_FALSE );
		Assert( result == SL_RESULT_SUCCESS, "" );
	}

	//===========================================================================

	void OpenSLAudioManager::ShutdownOpenSL()
	{
		if ( mOutputMixObject ) 
		{
			(*mOutputMixObject)->Destroy( mOutputMixObject );
		}

		if ( mOpenSLEngineObject )
		{
			(*mOpenSLEngineObject)->Destroy( mOpenSLEngineObject );
		}
	}
}

#endif
