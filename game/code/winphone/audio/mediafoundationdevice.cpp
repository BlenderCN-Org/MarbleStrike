#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "winphone/audio/mediafoundationdevice.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBAL
//////////////////////////////////////////////////////

namespace Audio
{
	MediaFoundationDevice* MediaFoundationDevice::mInstance = NULL;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{   	
    STDMETHODIMP MediaEngineNotify::EventNotify( DWORD meEvent, DWORD_PTR param1, DWORD param2 )
    {
        UNUSED_ALWAYS( param1 );
        UNUSED_ALWAYS( param2 );
        Assert( mMediaFoundationDevice != NULL, "" );
        mMediaFoundationDevice->SetEventState( meEvent );                        

        return S_OK;
    }

    //===========================================================================

    void MediaFoundationDevice::Create()
    {
        Assert( mInstance == NULL, "" );
		mInstance = NEW_PTR( "MediaFoundationDEvice" ) MediaFoundationDevice;
		mInstance->Initialize();
	}

	//===========================================================================

	void MediaFoundationDevice::Destroy()
	{
		Assert( mInstance != NULL, "" );
		mInstance->Shutdown();
		DELETE_PTR( mInstance );
	}

	//===========================================================================

	MediaFoundationDevice* MediaFoundationDevice::Get()
	{
		Assert( mInstance != NULL, "" );
		return mInstance;
	}

	//===========================================================================

	void MediaFoundationDevice::Initialize()
	{
		HRESULT hr = S_OK;

		mMediaEngineNotify = NEW_PTR( "MediaEngineNotify" ) MediaEngineNotify( this );

		hr = MFStartup( MF_VERSION );
		Assert( hr == S_OK, "" );

		hr = CoCreateInstance( CLSID_MFMediaEngineClassFactory, NULL, CLSCTX_ALL, IID_PPV_ARGS( &mMediaEngineFactory ) );
		Assert( hr == S_OK, "" );

		hr = MFCreateAttributes( &mMediaAttributes, 1 );
		Assert( hr == S_OK, "" );		

		hr = mMediaAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*)mMediaEngineNotify );
		Assert( hr == S_OK, "" );

		hr = mMediaAttributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, 20);
		Assert( hr == S_OK, "" );

		hr = mMediaEngineFactory->CreateInstance( 0, mMediaAttributes, &mMediaEngine );
		Assert( hr == S_OK, "" );

        mEventState = 0;

	}

	//===========================================================================

	void MediaFoundationDevice::Shutdown()
	{
		if ( mMediaEngine )
		{
			mMediaEngine->Release();
			mMediaEngine = NULL;
		}

		if ( mMediaAttributes )
		{
			mMediaAttributes->Release();
			mMediaAttributes = NULL;
		}

		if ( mMediaEngineFactory )
		{
			mMediaEngineFactory->Release();
			mMediaEngineFactory = NULL;
		}

		MFShutdown();

		DELETE_PTR( mMediaEngineNotify );
	}

	//===========================================================================

	MediaFoundationDevice::MediaFoundationDevice()
	{
		mMediaEngineFactory = NULL;
		mMediaEngine = NULL;
		mMediaAttributes = NULL;
		mMediaEngineNotify = NULL;
	}

	//===========================================================================

	MediaFoundationDevice::~MediaFoundationDevice()
	{
	}
}

#endif