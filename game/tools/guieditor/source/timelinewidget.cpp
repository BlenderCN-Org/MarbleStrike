//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "timelinewidget.hpp"
#include "guieditor.hpp"
#include "common/engine/system/systemdefines.hpp"
#include <math.h>
#include <algorithm>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

TimeLineWidget::TimeLineWidget( wxWindow *parent,
							   wxWindowID winid,
							   const wxPoint& pos,
							   const wxSize& size,
							   long style,
							   const wxString& name
			   ) : wxPanel( parent, winid, pos, size, style, name )
{
	Connect(wxEVT_PAINT, wxPaintEventHandler( TimeLineWidget::OnPaint ) );
	Connect(wxEVT_SIZE, wxSizeEventHandler( TimeLineWidget::OnSize ) );
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler( TimeLineWidget::OnLeftDown ) );
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler( TimeLineWidget::OnLeftUp ) );
	Connect(wxEVT_MOTION, wxMouseEventHandler( TimeLineWidget::OnMove ) );
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler( TimeLineWidget::OnRightDown ) );

	mSelectedIndex = -1;
	mAnimationLength = 100;
	mCurrentFrame = 0;
	mLeftMouseButtonDown = false;
	mGuiEditor = NULL;
}

//===========================================================================

void TimeLineWidget::AddKeyFrame( int keyFrame )
{
	mKeyFrames.push_back( keyFrame );
}

//===========================================================================

void TimeLineWidget::ClearKeyFrames()
{
	mKeyFrames.clear();
}

//===========================================================================

void TimeLineWidget::SetAnimationLength( int length )
{
	mAnimationLength = length;
}

//===========================================================================

void TimeLineWidget::OnPaint(wxPaintEvent& event)
{
	UNUSED_ALWAYS( event );

	wxFont font(
		9, 
		wxFONTFAMILY_DEFAULT, 
		wxFONTSTYLE_NORMAL,
		wxFONTWEIGHT_NORMAL, 
		false, 
		wxT( "Courier 10 Pitch" )
		);

	wxPaintDC dc(this);
	dc.SetFont(font);
	wxSize size = GetSize();
	float width = static_cast<float>( size.GetWidth() );
	
	int stepSize = 20;

	int numIntervals = mAnimationLength / stepSize;

	float widthStep = width / (float)numIntervals;
		
	dc.SetPen( wxPen( wxColour( 90, 80, 60 ) ) );
	for ( int i = 0; i <= numIntervals; ++i ) 
	{
		dc.DrawLine( i * widthStep, size.GetY(), i * widthStep, size.GetY() - 8 );	
		wxSize numSize = dc.GetTextExtent( wxString::Format( wxT( "%d" ), i * stepSize ) );

		if ( i == 0 )
		{
			dc.DrawText( wxString::Format( wxT( "%d" ), i * stepSize ), i * widthStep, 8 );
		}
		else if ( i == numIntervals )
		{
			dc.DrawText( wxString::Format( wxT( "%d" ), i * stepSize ), i * widthStep - numSize.GetWidth(), 8 );
		}
		else
		{
			dc.DrawText( wxString::Format( wxT( "%d" ), i * stepSize ), i * widthStep - numSize.GetWidth() / 2, 8 );
		}
	}

	dc.SetPen( wxPen( wxColour( 255, 0, 0 ) ) );
	int numKeyFrames = static_cast<int>( mKeyFrames.size() );
	for ( int i = 0; i < numKeyFrames; ++i )
	{
		float x = mKeyFrames[i] / static_cast<float>( mAnimationLength ) * width;
		dc.DrawLine( x, 0, x, size.GetY() );	
	}

	if ( mSelectedIndex != -1 )
	{
		dc.SetPen( wxPen( wxColour( 255, 255, 255 ) ) );
		dc.DrawLine( mKeyFrames[mSelectedIndex], 0, mKeyFrames[mSelectedIndex], size.GetY() );	
	}

	dc.SetPen( wxPen( wxColour( 0, 255, 0 ) ) );
	float x = static_cast<float>( mCurrentFrame ) / static_cast<float>( mAnimationLength ) * width;
	dc.DrawLine( x, 0, x, size.GetY() );	
}

//===========================================================================

void TimeLineWidget::OnSize( wxSizeEvent & event )
{
	UNUSED_ALWAYS( event );

	Refresh();
}

//===========================================================================

void TimeLineWidget::OnLeftDown( wxMouseEvent &event )
{
	SetCurrentFrameByPos( event.m_x );

	mLeftMouseButtonDown = true;
	
	Refresh();
}

//===========================================================================

void TimeLineWidget::OnLeftUp( wxMouseEvent &event )
{
	UNUSED_ALWAYS( event );

	mLeftMouseButtonDown = false;
}

//===========================================================================

void TimeLineWidget::OnMove( wxMouseEvent &event )
{
	if ( mLeftMouseButtonDown )
	{
		SetCurrentFrameByPos( event.m_x );

		Refresh();
	}	
}

//===========================================================================

void TimeLineWidget::OnRightDown( wxMouseEvent &event )
{
	mSelectedIndex = FindClosestKeyFrame( event.m_x );

	Refresh();
}

//===========================================================================

int TimeLineWidget::GetCurrentFrame() const
{
	return mCurrentFrame;
}

//===========================================================================

void TimeLineWidget::SetCurrentFrame( int frame )
{
	mCurrentFrame = frame;

	Refresh();
}

//===========================================================================

int TimeLineWidget::FindClosestKeyFrame( float value )
{
	int numKeyFrames = static_cast<int>( mKeyFrames.size() );
	int maxSize = numKeyFrames - 1;
	int minSize = 0;
	
	while ( ( maxSize - minSize ) > 1 )
	{
		int midIndex = ( maxSize - minSize ) / 2;		

		float midValue = mKeyFrames[midIndex];

		if ( value < midValue )
		{
			maxSize = midIndex;						
		}
		else if  ( value > midValue )
		{
			minSize = midIndex;
		}
	}

	if ( mKeyFrames[maxSize] == value )
	{
		return maxSize;
	}
	else if ( mKeyFrames[minSize] == value )
	{
		return minSize;		
	}
	else
	{
		return -1;
	}
}

//===========================================================================

void TimeLineWidget::SetCurrentFrameByPos( float xPos )
{
	wxSize size = GetSize();
	float width = static_cast<float>( size.GetWidth() );

	//mKeyFrames.push_back( x / width * mAnimationLength );	
	//std::sort( mKeyFrames.begin(), mKeyFrames.end() );

	mCurrentFrame = static_cast<int>( xPos / width * static_cast<float>( mAnimationLength ) );

	mGuiEditor->UpdateAnimationFrame( mCurrentFrame );	
}