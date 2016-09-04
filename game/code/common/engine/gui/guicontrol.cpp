///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/gui/guicontrol.hpp" 
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/systemdefines.hpp"
#include "common/engine/system/utilities.hpp"

////////////////////////////////////////////////////// 
// CLASS METHODS 
////////////////////////////////////////////////////// 

GuiControl::GuiControl( GuiScreen* parentScreen )
{
	mVisible = true;
	mEnabled = true;
	mMouseOver = false;
	mHasFocus = false;
	mID = -1;
	mCurrentFrame = 0;
	mControlType = CONTROL_TYPE_STATIC;
	mParentScreen = parentScreen;
    mCurrentAnimationSequenceIndex = -1;
	
	SetName( "GuiControl" );
}

//=========================================================================== 

GuiControl::~GuiControl() 
{ 
    size_t numAnimationSequences = mAnimationSequences.size();
    for ( int i = 0; i < numAnimationSequences; ++i )
    {
        DELETE_PTR( mAnimationSequences[i] );
    }
} 

//===========================================================================

void GuiControl::Copy( GuiControl* control )
{
	if ( control )
	{
		SetName( control->GetName() );
		mID = control->mID;
		mVisible = control->mVisible;
		mEnabled = control->mEnabled;				
		mControlType = control->mControlType;
		mParentScreen = control->mParentScreen;
		mCurrentFrame = control->mCurrentFrame;
        mCurrentControlKeyFrameData = control->mCurrentControlKeyFrameData;
        mCurrentAnimationSequenceIndex = control->mCurrentAnimationSequenceIndex;

        size_t numAnimationSequences = control->mAnimationSequences.size();
        for ( int i = 0; i < numAnimationSequences; ++i )
        {
            AnimationSequence* newAnimationSequence = NEW_PTR( "AnimationSequence" ) AnimationSequence;
            mAnimationSequences.push_back( newAnimationSequence );
            *newAnimationSequence = *control->mAnimationSequences[i];
        }        

		GoToKeyFrame( mCurrentFrame );
	}
}

//===========================================================================

void GuiControl::SetName( const char* name )
{
    System::StringCopy( mName, 64, name );
}

//===========================================================================

void GuiControl::SetFramesPerSecond( float fps )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    { 
        animationSequence->mAnimationController.SetPlaybackSettings( fps );
    }
}

//===========================================================================

ControlKeyFrameData GuiControl::LinearInterpolation( const ControlKeyFrameData &data0, const ControlKeyFrameData &data1, float t )
{
    ControlKeyFrameData newData;

    Math::Vector2 position0(
		static_cast<float>( data0.mPosition[0] ),
		static_cast<float>( data0.mPosition[1] ) 
		);

	Math::Vector2 position1(
		static_cast<float>( data1.mPosition[0] ),
		static_cast<float>( data1.mPosition[1] ) 
		);

	Math::Vector2 newPosition = position0 * ( 1.0f - t ) + position1 * t;

	float newAlpha = data0.mAlpha * ( 1.0f - t ) + data1.mAlpha * t;

	newData.mPosition[0] = static_cast<int>( newPosition[0] + 0.5f );
	newData.mPosition[1] = static_cast<int>( newPosition[1] + 0.5f );
	newData.mSize = data0.mSize;
	newData.mAlpha = newAlpha;

	return newData;
}

//===========================================================================

void GuiControl::Update( float elapsedTime )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.Update( elapsedTime, LinearInterpolation );
        mCurrentControlKeyFrameData = animationSequence->mAnimationController.GetAnimationData();
    }
}

//===========================================================================

void GuiControl::PlayAnimation()
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.Play();
    }
}

//===========================================================================

void GuiControl::LoopAnimation()
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.Loop();
    }	
}

//===========================================================================

bool GuiControl::ContainsPoint( int /*x*/, int /*y*/ )
{
	return false;
}

//===========================================================================

int GuiControl::GetX()
{	
	return mCurrentControlKeyFrameData.mPosition[0];			
}

//===========================================================================

int GuiControl::GetY()
{	
	return mCurrentControlKeyFrameData.mPosition[1];		
}

//===========================================================================

void GuiControl::SetX( int x )
{
    mCurrentControlKeyFrameData.mPosition[0] = x;

    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
    }
}

//===========================================================================

void GuiControl::SetY( int y )
{    
    mCurrentControlKeyFrameData.mPosition[1] = y;	
    
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
    }
}

//===========================================================================

void GuiControl::SetPosition( int x, int y )
{	
    mCurrentControlKeyFrameData.mPosition[0] = x;
	mCurrentControlKeyFrameData.mPosition[1] = y;
    
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
    }
}

//===========================================================================

int GuiControl::GetWidth()
{
	return mCurrentControlKeyFrameData.mSize[0];
}

//===========================================================================

int GuiControl::GetHeight()
{
	return mCurrentControlKeyFrameData.mSize[1];
}

//===========================================================================

void GuiControl::SetWidth( int width )
{
	if ( width >= 0 )
	{
        mCurrentControlKeyFrameData.mSize[0] = width;
        AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
        if ( animationSequence )
        {        
            animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
        }
	}
}

//===========================================================================

void GuiControl::SetHeight( int height )
{		
    if ( height >= 0 )
    {
        mCurrentControlKeyFrameData.mSize[1] = height;
        AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
        if ( animationSequence )
        {        
            animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
        }
    }
}

//===========================================================================

void GuiControl::SetSize( int width, int height )
{
	if ( width >= 0 && height >= 0 )
	{
        mCurrentControlKeyFrameData.mSize[0] = width;
        mCurrentControlKeyFrameData.mSize[1] = height;
        
        AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
        if ( animationSequence )
        {        
            animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
        }
	}
}

//===========================================================================

float GuiControl::GetAlpha()
{
	return mCurrentControlKeyFrameData.mAlpha;
}

//===========================================================================

void GuiControl::SetAlpha( float alpha )
{    
    mCurrentControlKeyFrameData.mAlpha = alpha;        

    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.SetAnimationData( &mCurrentControlKeyFrameData );
    }
}

//===========================================================================

void GuiControl::ClearKeyFrames()
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.Clear();
    }
}

//===========================================================================

void GuiControl::InsertKeyFrame( ControlKeyFrameData data, int frame )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.AddKeyFrame( data, frame );
    }
}

//===========================================================================

void GuiControl::DeleteKeyFrame( int frame )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.DeleteKeyFrame( frame );
    }
}

//===========================================================================

void GuiControl::GoToKeyFrame( int frame )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {        
        animationSequence->mAnimationController.GoToKeyFrame( frame );
        mCurrentControlKeyFrameData = animationSequence->mAnimationController.GetAnimationData();
    }
}

//===========================================================================

int GuiControl::GetPreviousFrame( int frame )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {   
        int previousFrame = 0;
        int numKeyFrames = animationSequence->mAnimationController.GetNumKeyFrames();
        for ( int i = 0; i < numKeyFrames; ++i )
        {
            AnimationController<ControlKeyFrameData>::KeyFrameData* keyFrameData = animationSequence->mAnimationController.GetAnimationKeyFrameData( i );		
            if ( keyFrameData->mKeyFrame >= frame )
            {			
                break;
            }
            else
            {			
                previousFrame = keyFrameData->mKeyFrame;
            }
        }

        return previousFrame;
    }
    else
    {
        return 0;
    }
}

//===========================================================================

int GuiControl::GetNextFrame( int frame )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    {  
        int nextFrame = 0;
        int numKeyFrames = animationSequence->mAnimationController.GetNumKeyFrames();
        for ( int i = 0; i < numKeyFrames; ++i )
        {
            AnimationController<ControlKeyFrameData>::KeyFrameData* keyFrameData = animationSequence->mAnimationController.GetAnimationKeyFrameData( i );
            if ( keyFrameData->mKeyFrame > frame )
            {			
                nextFrame = keyFrameData->mKeyFrame;
                break;
            }
        }

        return nextFrame;
    }
    else
    {
        return 0;
    }
}

//===========================================================================

int GuiControl::GetNumKeyFrames()
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    { 
        int numKeyFrames = animationSequence->mAnimationController.GetNumKeyFrames();
        return numKeyFrames;
    }
    else
    {
        return 0;
    }
}

//===========================================================================

AnimationController<ControlKeyFrameData>::KeyFrameData* GuiControl::GetKeyFrameDataByIndex( int index )
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    { 
        AnimationController<ControlKeyFrameData>::KeyFrameData* keyFrameData = animationSequence->mAnimationController.GetAnimationKeyFrameData( index );
        return keyFrameData;
    }
    else
    {
        return NULL;
    }
}

//===========================================================================

void GuiControl::SetCurrentAnimationSequence( int index )
{
    mCurrentAnimationSequenceIndex = index;
}

//===========================================================================

void GuiControl::AddAnimationSequence( const char* name )
{
    AnimationSequence* newAnimationSequence = NEW_PTR( "AnimationSequence" ) AnimationSequence;
    System::StringCopy( newAnimationSequence->mAnimationSequenceName, 64, name );
    newAnimationSequence->mAnimationController.AddKeyFrame( mCurrentControlKeyFrameData, 0 );
    mAnimationSequences.push_back( newAnimationSequence );    
    mCurrentAnimationSequenceIndex = (int)mAnimationSequences.size() - 1;
}

//===========================================================================

void GuiControl::DeleteAnimationSequence( int index )
{   
    DELETE_PTR( mAnimationSequences[index] );
    mAnimationSequences.erase( mAnimationSequences.begin() + index );

    mCurrentAnimationSequenceIndex = (int)mAnimationSequences.size() - 1;
}

//===========================================================================

int GuiControl::FindAnimationSequenceIndexByName( const char* name )
{
    size_t numAnimationSequences = mAnimationSequences.size();
    for ( int i = 0; i < numAnimationSequences; ++i )
    {
        AnimationSequence* currentSequence = mAnimationSequences[i];
        if ( strcmp( name, currentSequence->mAnimationSequenceName ) == 0 )
        {
            return i;
        }
    }

    return -1;
}

//===========================================================================

int GuiControl::GetNumAnimationSequences()
{
    return (int)mAnimationSequences.size();
}

//===========================================================================

AnimationSequence* GuiControl::GetAnimationSquence( int index )
{
    if ( index >= 0 )
    {
        return mAnimationSequences[index];
    }
    else
    {
        return NULL;
    }
}

//===========================================================================

AnimationState GuiControl::GetAnimationState()
{
    AnimationSequence* animationSequence = GetAnimationSquence( mCurrentAnimationSequenceIndex );
    if ( animationSequence )
    { 
        return animationSequence->mAnimationController.GetState();
    }
    else
    {
        return ANIM_STATE_STOPPED;
    }
}