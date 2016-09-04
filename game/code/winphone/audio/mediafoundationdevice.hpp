#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8

#ifndef MEDIA_FOUNDATION_DEVICE
#define MEDIA_FOUNDATION_DEVICE

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <mfapi.h>
#include <mfidl.h>
#include <Mfmediaengine.h>
#include <Strsafe.h>
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
    class MediaFoundationDevice;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class MediaEngineNotify : public IMFMediaEngineNotify
	{
	public:
		MediaEngineNotify( MediaFoundationDevice* mediaFoundationDevice )			
		{
			m_cRef = 1;
            mMediaFoundationDevice = mediaFoundationDevice;
		}

		STDMETHODIMP QueryInterface( REFIID riid, void** ppv )
		{
			if ( __uuidof(IMFMediaEngineNotify) == riid )
			{
				*ppv = static_cast<IMFMediaEngineNotify*>( this );
			}
			else
			{
				*ppv = nullptr;
				return E_NOINTERFACE;
			}

			AddRef();

			return S_OK;
		}      

		STDMETHODIMP_(ULONG) AddRef()
		{
			return InterlockedIncrement( &m_cRef );
		}

		STDMETHODIMP_(ULONG) Release()
		{
			LONG cRef = InterlockedDecrement( &m_cRef );
			if ( cRef == 0 )
			{
				delete this;
			}
			return cRef;
		}

		STDMETHODIMP EventNotify( DWORD meEvent, DWORD_PTR param1, DWORD param2 );

	private:

        MediaFoundationDevice* mMediaFoundationDevice;
		long m_cRef;
	};

	//===========================================================================

	class MediaFoundationDevice
	{
	public:
		static void Create();
		static void Destroy();
		static MediaFoundationDevice* Get();

		IMFMediaEngine* GetMediaEngine()
		{
			Assert( mMediaEngine != NULL, "" );
			return mMediaEngine;
		}

        void SetEventState( DWORD eventState )
        {
            mEventState = eventState;
        }

        DWORD GetEventState()
        {
            return mEventState;
        }

	private:
		MediaFoundationDevice();
		~MediaFoundationDevice();

		void Initialize();
		void Shutdown();

		static MediaFoundationDevice* mInstance;

		IMFMediaEngineClassFactory* mMediaEngineFactory;
		IMFMediaEngine* mMediaEngine;
		IMFAttributes* mMediaAttributes;
		MediaEngineNotify* mMediaEngineNotify;

        DWORD mEventState;
	};
}

#endif

#endif