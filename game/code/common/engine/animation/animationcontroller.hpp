#ifndef ANIMATION_CONTROLLER_HPP
#define ANIMATION_CONTROLLER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/quaternion.hpp"
#include "common/engine/math/vector4.hpp"
#include <vector>
#include <algorithm>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum AnimationState
{
    ANIM_STATE_PLAYING,
    ANIM_STATE_STOPPED,
    ANIM_STATE_LOOPING
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

template <typename T>
class AnimationController
{
public:

    struct KeyFrameData
    {
        int mKeyFrame;
        T mData;
    };

    AnimationController();
    ~AnimationController();

    void Update( float elapsedTime, T ( *Interpolate )( const T&, const T&, float t ) );
    void Clear();
    void SetPlaybackSettings( float framesPerSecond );		
    float GetFramesPerSecond() const;	
    void AddKeyFrame( const T &data, int keyFrame );
    void DeleteKeyFrame( int keyFrame );
    int GetPreviousFrame( int frame );
    int GetNextFrame( int frame );
    AnimationState GetState() const;
    T GetAnimationData() const;
    void SetAnimationData( const T* data );
    KeyFrameData* GetAnimationKeyFrameData( int index );
    void UpdateKeyFrameData( T data, int frame );
    int GetNumKeyFrames() const;
    T GetAnimationFrameData( int frame, T ( *Interpolate )( const T&, const T&, float t ) );
    void Play();
    void Stop();
    void Loop();	
    void GoToKeyFrame( int frame );

    int GetCurrentKeyFrame()
    {
        return mKeyFrame;
    }

    static bool EqualComparison( const KeyFrameData &a, const KeyFrameData &b );

protected:

    float mT;
    float mFramesPerSecond;	
    AnimationState mState;
    std::vector<KeyFrameData> mKeyFrames;
    T mAnimationData;
    int mKeyFrame;
};

//===========================================================================

template<class T> AnimationController<T>::AnimationController()
    :	mT( 0 )
    ,	mFramesPerSecond( 0 )
    ,	mState( ANIM_STATE_STOPPED )
    ,	mKeyFrame( 0 )
{
}

//===========================================================================

template<class T> AnimationController<T>::~AnimationController()
{
    Clear();
}

//===========================================================================

template<class T> void AnimationController<T>::Update( float elapsedTime, T ( *Interpolate )( const T&, const T&, float t ) )
{	
    int numKeyFrames = static_cast<int>( mKeyFrames.size() );

    if ( numKeyFrames >= 2 )
    {
        if ( mState == ANIM_STATE_PLAYING || mState == ANIM_STATE_LOOPING )
        {	
            typename AnimationController<T>::KeyFrameData frame0 = mKeyFrames[mKeyFrame];
            typename AnimationController<T>::KeyFrameData frame1 = mKeyFrames[mKeyFrame + 1];

            int numFrames = frame1.mKeyFrame - frame0.mKeyFrame;

            mAnimationData = Interpolate( frame0.mData, frame1.mData, mT );

            mT += mFramesPerSecond * elapsedTime / numFrames;

            if ( mT > 1.0f )
            {
                mAnimationData = Interpolate( frame0.mData, frame1.mData, 1.0f );

                mT = 0;
                mKeyFrame++;

                if ( mKeyFrame + 1 >= numKeyFrames )
                {
                    if ( mState != ANIM_STATE_LOOPING )
                    {   
                        mState = ANIM_STATE_STOPPED;
                    }
                    else
                    {
                        mKeyFrame = 0;
                    }
                }
            }
        }
    }
}

//===========================================================================

template<class T> void AnimationController<T>::Clear()
{
    mKeyFrames.clear();
    mKeyFrame = 0;
}

//===========================================================================

template<class T> void AnimationController<T>::SetPlaybackSettings( float framesPerSecond )
{	
    mFramesPerSecond = framesPerSecond;
}

//===========================================================================

template<class T>float AnimationController<T>::GetFramesPerSecond() const
{
    return mFramesPerSecond;
}

//===========================================================================

template<class T> void AnimationController<T>::AddKeyFrame( const T &data, int keyFrame )
{
    typename AnimationController<T>::KeyFrameData tempKeyFrame;
    tempKeyFrame.mKeyFrame = keyFrame;

    std::pair< 
        typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator, 
        typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator 
    > result = 
	std::equal_range( 
    mKeyFrames.begin(), 
    mKeyFrames.end(), 
    tempKeyFrame, 
    AnimationController<T>::EqualComparison 
    );

    if ( result.first != mKeyFrames.end() )
    {
        mKeyFrames.erase( result.first );
    }

    KeyFrameData keyFrameData;
    keyFrameData.mKeyFrame = keyFrame;
    keyFrameData.mData = data;
    mKeyFrames.push_back( keyFrameData );

    std::sort( mKeyFrames.begin(), mKeyFrames.end(), EqualComparison );		
}

//===========================================================================

template<class T> void AnimationController<T>::DeleteKeyFrame( int keyFrame )
{
    typename AnimationController<T>::KeyFrameData tempKeyFrame;
    tempKeyFrame.mKeyFrame = keyFrame;

    std::pair< 
        typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator, 
        typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator 
    > result = 
    std::equal_range( 
    mKeyFrames.begin(), 
    mKeyFrames.end(), 
    tempKeyFrame, 
    AnimationController<T>::EqualComparison 
    );

    if ( result.first != mKeyFrames.end() )
    {
        if ( result.first->mKeyFrame == keyFrame )
        {
            mKeyFrames.erase( result.first );
            std::sort( mKeyFrames.begin(), mKeyFrames.end(), EqualComparison );		
        }
    }
}

//===========================================================================

template<class T> int AnimationController<T>::GetPreviousFrame( int frame )
{
    typename AnimationController<T>::KeyFrameData tempKeyFrame;
    tempKeyFrame.mKeyFrame = frame;

    std::pair< typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator, typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator > result = 
        std::equal_range( mKeyFrames.begin(), mKeyFrames.end(), tempKeyFrame, AnimationController<T>::EqualComparison );

    int numKeyFrames = GetNumKeyFrames();
    Assert( numKeyFrames > 0, "" );

    int keyFrame0 = mKeyFrames[numKeyFrames-1].mKeyFrame;

    if ( result.first != mKeyFrames.end() )
    {		
        if ( result.first == mKeyFrames.begin() )
        {
            keyFrame0 = 0;
        }
        else
        {
            result.first--;
            keyFrame0 = result.first->mKeyFrame;
        }
    }	

    return keyFrame0;
}

//===========================================================================

template<class T> int AnimationController<T>::GetNextFrame( int frame )
{
    typename AnimationController<T>::KeyFrameData tempKeyFrame;
    tempKeyFrame.mKeyFrame = frame;

    std::pair< typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator, typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator > result = 
        std::equal_range( mKeyFrames.begin(), mKeyFrames.end(), tempKeyFrame, AnimationController<T>::EqualComparison );

    int numKeyFrames = GetNumKeyFrames();
    Assert( numKeyFrames > 0, "" );

    int keyFrame0 = mKeyFrames[numKeyFrames-1].mKeyFrame;

    if ( result.first != mKeyFrames.end() )
    {		
        result.first++;
        if ( result.first != mKeyFrames.end() )
        {
            keyFrame0 = result.first->mKeyFrame;			
        }
    }	

    return keyFrame0;
}

//===========================================================================

template<class T> AnimationState AnimationController<T>::GetState() const
{
    return mState;
}

//===========================================================================

template<class T> T AnimationController<T>::GetAnimationData() const
{
    return mAnimationData;
}

//===========================================================================

template<class T> void AnimationController<T>::SetAnimationData( const T* data )
{
    mAnimationData = *data;
}

//===========================================================================

template<class T> typename AnimationController<T>::KeyFrameData* AnimationController<T>::GetAnimationKeyFrameData( int index )
{
    return &mKeyFrames[index];
}

//===========================================================================

template<class T> void AnimationController<T>::UpdateKeyFrameData( T data, int frame )
{
    typename AnimationController<T>::KeyFrameData tempKeyFrame;
    tempKeyFrame.mKeyFrame = frame;

    std::pair< typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator, typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator > result = 
        std::equal_range( mKeyFrames.begin(), mKeyFrames.end(), tempKeyFrame, AnimationController<T>::EqualComparison );

    if ( result.first != mKeyFrames.end() )
    {		
        if ( result.first->mKeyFrame == frame )
        {
            result.first->mData = data;
        }
    }	
}

//===========================================================================

template<class T> int AnimationController<T>::GetNumKeyFrames() const
{
    return (int)mKeyFrames.size();
}

//===========================================================================

template<class T> bool AnimationController<T>::EqualComparison( 
    const typename AnimationController<T>::KeyFrameData &a, 
    const typename AnimationController<T>::KeyFrameData &b )
{
    return ( a.mKeyFrame < b.mKeyFrame );
}

//===========================================================================

template<class T> T AnimationController<T>::GetAnimationFrameData( int frame, T ( *Interpolate )( const T&, const T&, float t )  )
{
    typename AnimationController<T>::KeyFrameData tempKeyFrame;
    tempKeyFrame.mKeyFrame = frame;

    std::pair< typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator, typename std::vector< typename AnimationController<T>::KeyFrameData >::iterator > result = 
        std::equal_range( mKeyFrames.begin(), mKeyFrames.end(), tempKeyFrame, AnimationController<T>::EqualComparison );

    int keyFrame0 = 0;
    int keyFrame1 = 0;

    int numKeyFrames = GetNumKeyFrames();
    Assert( numKeyFrames > 0, "" );

    T* keyFrameData0 = &( mKeyFrames[numKeyFrames-1].mData );
    T* keyFrameData1 = &( mKeyFrames[numKeyFrames-1].mData );

    if ( result.first != mKeyFrames.end() )
    {
        if ( result.first->mKeyFrame == frame )
        {
            keyFrame0 = result.first->mKeyFrame;
            keyFrameData0 = &( result.first->mData );
        }
        else
        {
            result.first--;
            keyFrame0 = result.first->mKeyFrame;			
            keyFrameData0 = &( result.first->mData );
        }

        keyFrameData1 = keyFrameData0;
    }	

    if ( result.second != mKeyFrames.end() )
    {		
        keyFrame1 = result.second->mKeyFrame;
        keyFrameData1 = &( result.second->mData );
    }

    float td = static_cast<float>( keyFrame1 - keyFrame0 );
    float t = 0;
    if ( td != 0 )
    {
        t = static_cast<float>( frame - keyFrame0 ) / td;
    } 

    T newFrameData = Interpolate( *keyFrameData0, *keyFrameData1, t );

    return newFrameData;	
}

//===========================================================================

template<class T> void AnimationController<T>::Play()
{
    mKeyFrame = 0;
    mAnimationData = mKeyFrames[0].mData;
    mState = ANIM_STATE_PLAYING;
}

//===========================================================================

template<class T> void AnimationController<T>::Stop()
{
    mState = ANIM_STATE_STOPPED;
}

//===========================================================================

template<class T> void AnimationController<T>::Loop()
{
    mState = ANIM_STATE_LOOPING;
}

//===========================================================================

template<class T> void AnimationController<T>::GoToKeyFrame( int frame )
{
    size_t numKeyFrames = mKeyFrames.size();
    for ( int i = 0; i < numKeyFrames; ++i )
    {
        typename AnimationController<T>::KeyFrameData* keyFrameData = &mKeyFrames[i];
        if ( keyFrameData->mKeyFrame > frame )
        {		
            break;
        }
        else
        {            
            mAnimationData = keyFrameData->mData;
        }
    }
}

#endif
