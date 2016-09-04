#ifndef IOS_AUDIO_STREAM_SOURCE_HPP
#define IOS_AUDIO_STREAM_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiostreamsource.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class iOSAudioPlayerInterface
{
public:
    iOSAudioPlayerInterface()
    {
        
    }
    
    virtual ~iOSAudioPlayerInterface()
    {
        
    }
    
    virtual void Load( const char* filename ) = 0;
    virtual void Unload() = 0;    
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void Pause() = 0;
    virtual void Unpause() = 0;
    virtual void Rewind() = 0;
    virtual void SetVolume( float volume ) = 0;  
    
private:
    
};

class iOSAudioStreamSource : public Audio::AudioStreamSource
{
public:
    iOSAudioStreamSource();
    ~iOSAudioStreamSource();
    
    void Load( const char* filename );
    void Unload();
	void Play();
    void Stop();
    void Pause();
    void Unpause();
    void Rewind();    
    void SetVolume( float volume );
    
private:    
    
    iOSAudioPlayerInterface* mAudioPlayer;
	
};

#endif 
