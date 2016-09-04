#ifndef DATABASE_HPP
#define DATABASE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/hashtable.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct StartupData
{
	int mPort;
	char mIPAddress[32];
	bool mShowFPS;
	bool mShowMem;
	bool mShowCursor;
	bool mEnableAudio;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Database
{   
public:

	static void Create();
    static Database* Get();	
    static void Terminate();

    unsigned int GetBackBufferWidth() const;
    unsigned int GetBackBufferHeight() const;	
	void SetBackBufferWidth( unsigned int );
    void SetBackBufferHeight( unsigned int );
	
	int GetAppScreenWidth();
	int GetAppScreenHeight();
	void SetAppScreenWidth( int width );
	void SetAppScreenHeight( int height );

	int GetTouchScreenWidth();
	int GetTouchScreenHeight();
	void SetTouchScreenWidth( int width );
	void SetTouchScreenHeight( int height );
    
	void SetResourcePath( const char* path );
	const char* GetResourcePath();
	
    void SetLibraryPath( const char* path );
    const char* GetLibraryPath();
	
    void SetShaderResourcePath( const char* path );
	const char* GetShaderResourcePath();	   	
	
	void SetIsLandscape( bool value );
	bool GetIsLandscape();
	
	void SetIsRetinaDisplay( bool value );
	bool GetIsRetinaDisplay();
    
	const StartupData* GetStartupData() const
	{
		return &mStartupData;
	}

	void LoadStartupData( const char* filename );
    
    bool GetWasSuspended()
    {
        return mWasSuspended;
    }
    
    void SetWasSuspended( bool value )
    {
        mWasSuspended = value;
    }

    void SetVolume( float volume );
    float GetVolume();

	float GetXScreenScaleFactor();
	float GetYScreenScaleFactor();

private:

	void LoadStartupDataXML( tinyxml2::XMLNode* node );

    Database();
    ~Database();

    //
    // Instance
    //
    static Database* mInstance;

    int mCurrentBackBufferWidth;
    int mCurrentBackBufferHeight;
	int mCurrentAppScreenWidth;
	int mCurrentAppScreenHeight;
	int mCurrentTouchScreenWidth;
	int mCurrentTouchScreenHeight;
	
	char mResourcePath[256];
	char mShaderResourcePath[256];
    char mLibraryPath[256];
	
	bool mIsLandscape;
	bool mIsRetinaDisplay;

	StartupData mStartupData;
    
    bool mWasSuspended;

    float mCurrentVolume;
};

#endif
