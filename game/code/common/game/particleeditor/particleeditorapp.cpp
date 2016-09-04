//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/render/texture.hpp"
#include "common/game/particleeditor/particleeditorapp.hpp"
#include "common/engine/render/particlesystem.hpp"
#include <algorithm>
#include <stdlib.h>
#include <vector>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ParticleEditorApp::ParticleEditorApp()
{
	mViewCamera = NULL;
	mOnDown = false;
	mRenderObjectCamera = NULL;

}

//===========================================================================

ParticleEditorApp::~ParticleEditorApp()
{	
	DELETE_PTR( mRenderObjectCamera );
	DELETE_PTR( mViewCamera );	
	DELETE_PTR( mParticleSystem );
}

//===========================================================================

void ParticleEditorApp::Initialize()
{
	mViewCamera = NEW_PTR( "Camera" ) Camera;	
	mRenderObjectCamera = NEW_PTR( "RenderObject" ) RenderObject;

	mParticleSystem = NEW_PTR( "Particle System" ) ParticleSystem;
	mParticleSystem->SetName( "Particle System" );
	mParticleSystem->SetMaxParticles( 100 );
	mParticleSystem->SetEmissionRange( 10.0f, 1000.0f );
	mParticleSystem->SetSizeRange( 1.0f, 2.0f );
	mParticleSystem->SetShrinkRange( 0.3f, 0.5f );
	mParticleSystem->SetVelocityXRange( -20.0f, 20.0f );
	mParticleSystem->SetVelocityYRange( -15.0f, 20.0f );	
	mParticleSystem->SetVelocityZRange( 0.0f, 0.0f );
	mParticleSystem->SetLifeRange( 1.0f, 3.0f );
	mParticleSystem->SetDecayRange( 2.0f, 2.0f );
	mParticleSystem->SetColorRange( Math::Vector4( 1.0f, 1.0f, 1.0f, 1.0f ), Math::Vector4( 1.0f, 0.0f, 0.0f, 1.0f ) ); 
	mParticleSystem->SetEmitTime( -1.0f );
	mParticleSystem->SetAlphaRange( 1.0f, 1.0f );

	PlayAnimation();
}

//===========================================================================

void ParticleEditorApp::Update( float elapsedTime )
{	
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );
	float cameraHeight = screenHeightSize/2.0f;

	Math::Matrix44 transform = Camera::CreateTransform(
		Math::Vector3( 0, cameraHeight, 10 ), 
		Math::Vector3( 0, 1, 0 ), 
		Math::Vector3( 0, cameraHeight, 0 ) 
		);
	mRenderObjectCamera->SetTransform( transform );

	if ( mParticleSystem )
	{
		mParticleSystem->Update( elapsedTime );
	}
}

//===========================================================================

void ParticleEditorApp::RenderUpdate()
{
	Renderer::Get()->SetBackgroundColor( 0, 0, 0 );
	Renderer::Get()->ClearRenderer( NULL );
	GameApp::Get()->GetRenderQueue()->ClearStates();

	float screenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() );
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );

	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection( 
		-screenWidthSize / 20.0f,
		-screenHeightSize / 20.0f,
		screenWidthSize / 20.0f,
		screenHeightSize / 20.0f,
		-100,
		100
		);

	const Math::Matrix44 identity;

	if ( mParticleSystem )
	{
		mParticleSystem->Render( orthoMatrix, identity, identity );		
	}

	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void ParticleEditorApp::OnLeftDown( int x, int y )
{
	int screenHeightSize = Database::Get()->GetBackBufferHeight();

	int motionX = x;
	int motionY = screenHeightSize - y;

	mOnDown = true;
	mOnDownPosition.Set( motionX, motionY );
}

//===========================================================================

void ParticleEditorApp::OnLeftUp( int /*x*/, int /*y*/ )
{
	mOnDown = false;
}

//===========================================================================

void ParticleEditorApp::OnMotion( int x, int y )
{
	int screenHeightSize = Database::Get()->GetBackBufferHeight();

	int motionX = x;
	int motionY = screenHeightSize - y;

	Math::Vector2Base<int> motionPosition( motionX, motionY );
	Math::Vector2Base<int> deltaPosition = motionPosition - mOnDownPosition;
}

//===========================================================================

void ParticleEditorApp::Open( const char* filename )
{	
	if ( mParticleSystem )
	{
		DELETE_PTR( mParticleSystem );
	}
	mParticleSystem = NEW_PTR( "Particle System" ) ParticleSystem;
	mParticleSystem->Load( filename );
}

//===========================================================================

void ParticleEditorApp::Save( const char* filename )
{
	mParticleSystem->Save( filename );
}

//===========================================================================

void ParticleEditorApp::Clear()
{
}

//===========================================================================

void ParticleEditorApp::PlayAnimation()
{
	if ( mParticleSystem )
	{
		mParticleSystem->Play();
		mParticleSystem->IsLooping( true );
	}
}

//===========================================================================

void ParticleEditorApp::StopAnimation()
{
	if ( mParticleSystem )
	{
		mParticleSystem->Stop();
	}
}
