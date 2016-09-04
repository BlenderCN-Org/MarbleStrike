//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import <AVFoundation/AVFoundation.h>

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "iosaudiostreamsource.h"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

class iOSAudioPlayer : public iOSAudioPlayerInterface
{
public:
    iOSAudioPlayer()
    {
        mAVAudioPlayer = NULL;
    }
    
    ~iOSAudioPlayer()
    {
        
    }
    
    void Load( const char* filename )
    {
        char fullPathFilename[256];
        System::StringCopy( fullPathFilename, 256, "" );
        System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
        System::StringConcat( fullPathFilename, 256, filename );
        System::StringConcat( fullPathFilename, 256, ".mp4" );
        
        System::FixedString<256> loadPath = System::Path::SystemSlash( fullPathFilename );
        
        NSString* nsstring = [NSString stringWithUTF8String:loadPath.GetRawData()];
        NSURL* url = [NSURL fileURLWithPath:nsstring];
        NSError* error;
        
        Assert( mAVAudioPlayer == NULL, "" );
        mAVAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
        mAVAudioPlayer.numberOfLoops = -1;        
    }
    
    void Unload()
    {
        if ( mAVAudioPlayer )
        {            
            [mAVAudioPlayer stop];
            [mAVAudioPlayer release];
            mAVAudioPlayer = NULL;
        }
    }
    
    void Play()
    {                
        if ( mAVAudioPlayer != nil )
        {
            [mAVAudioPlayer play];
        }
    }
    
    void Stop()
    {
        if ( mAVAudioPlayer != nil )
        {
            [mAVAudioPlayer stop];
        }
    }
    
    void Pause()
    {
        if ( mAVAudioPlayer != nil )
        {
            [mAVAudioPlayer pause];
        }
    }
    
    void Unpause()
    {
        Play();
    }
    
    void Rewind()
    {
        if ( mAVAudioPlayer != nil )
        {
            Stop();
            Play();
        }
    }
    
    void SetVolume( float volume )
    {
        if ( mAVAudioPlayer != nil )
        {
            [mAVAudioPlayer setVolume:volume];
        }
    }
    
private:
    
    AVAudioPlayer* mAVAudioPlayer;
};

iOSAudioStreamSource::iOSAudioStreamSource()
{
    mAudioPlayer = NEW_PTR( "iOSAudioPlayer" ) iOSAudioPlayer;
}

//===========================================================================

iOSAudioStreamSource::~iOSAudioStreamSource()
{    
    DELETE_PTR( mAudioPlayer );
}

//===========================================================================

void iOSAudioStreamSource::Load( const char *filename )
{
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Load( filename );
    }
}

//===========================================================================

void iOSAudioStreamSource::Unload()
{
    AudioStreamSource::Unload();
    
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Unload();
    }
}

//===========================================================================

void iOSAudioStreamSource::Play()
{
    AudioStreamSource::Play();
    
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Play();
    }
}

//===========================================================================

void iOSAudioStreamSource::Stop()
{
    AudioStreamSource::Stop();
    
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Stop();
    }
}

//===========================================================================

void iOSAudioStreamSource::Pause()
{
    AudioStreamSource::Pause();
    
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Pause();
    }
}

//===========================================================================

void iOSAudioStreamSource::Unpause()
{
    AudioStreamSource::Unpause();
    
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Unpause();
    }
}

//===========================================================================

void iOSAudioStreamSource::Rewind()
{
    if ( mAudioPlayer )
    {        
        mAudioPlayer->Rewind();
    }
}

//===========================================================================

void iOSAudioStreamSource::SetVolume( float volume )
{
    AudioStreamSource::SetVolume( volume );
    
    if ( mVolume != volume )
    {
        
        if ( mAudioPlayer )
        {        
            mAudioPlayer->SetVolume( volume );
        }
    }
}