//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/oceanwave.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/material.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OceanWave::OceanWave()
:	mWidth( 0 )
{	
}

//============================================================================

OceanWave::~OceanWave()
{

}

//============================================================================

void OceanWave::Initialize()
{	
	mWidth = 10.0f;
	float halfWidth = mWidth/2.0f;
	float interval = 1.0;
	
	mHeightField.SetSize( static_cast<int>( halfWidth * 2.0f + 1.0f ) );
		
	for ( float z = -halfWidth; z <= halfWidth; z += interval )
	{
		for ( float x = -halfWidth; x <= halfWidth; x += interval )
		{
			Math::Vector3 pos1( x - interval/2.0f, 0, z + interval/2.0f );
			Math::Vector3 pos2( x + interval/2.0f, 0, z + interval/2.0f );
			Math::Vector3 pos3( x + interval/2.0f, 0, z - interval/2.0f );
			Math::Vector3 pos4( x - interval/2.0f, 0, z - interval/2.0f );

			mHeightField.PushBack( pos1 );
			mHeightField.PushBack( pos2 );
			mHeightField.PushBack( pos3 );
			mHeightField.PushBack( pos4 );
		}
	}
}

//============================================================================

void OceanWave::Render(
					   const Math::Matrix44 &projectionMatrix,
					   const Math::Matrix44 &viewMatrix,					   
					   const Math::Matrix44 &transformMatrix
					   )
{
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetProjectionMatrix( projectionMatrix );
	simpleDraw->SetViewMatrix( viewMatrix );
	simpleDraw->SetTransformMatrix( transformMatrix );
	
	float halfWidth = mWidth/2.0f;
	float interval = 1.0;

	Math::Vector3 direction( 1, 0, 1 );
	direction = Math::Vector3::Normalize( direction );

	float A = 1.0f;
	float lamda = 9.0f;
	float k = static_cast<float>( Math::PIMUL2 ) / lamda;	

	for ( float z = -halfWidth; z <= halfWidth; z += interval )
	{
		for ( float x = -halfWidth; x <= halfWidth; x += interval )
		{
			Math::Vector3 pos1( x - interval/2.0f, 0, z + interval/2.0f );
			Math::Vector3 pos2( x + interval/2.0f, 0, z + interval/2.0f );
			Math::Vector3 pos3( x + interval/2.0f, 0, z - interval/2.0f );
			Math::Vector3 pos4( x - interval/2.0f, 0, z - interval/2.0f );

			Math::Vector3 newPos1 = pos1 - direction * A * sin( ( Math::Vector3::Dot( direction, pos1 ) * k ) );
			Math::Vector3 newPos2 = pos2 - direction * A * sin( ( Math::Vector3::Dot( direction, pos2 ) * k ) );
			Math::Vector3 newPos3 = pos3 - direction * A * sin( ( Math::Vector3::Dot( direction, pos3 ) * k ) );
			Math::Vector3 newPos4 = pos4 - direction * A * sin( ( Math::Vector3::Dot( direction, pos4 ) * k ) );

			float y1 = A * cos( ( Math::Vector3::Dot( direction, pos1 ) * k ) );
			float y2 = A * cos( ( Math::Vector3::Dot( direction, pos2 ) * k ) );
			float y3 = A * cos( ( Math::Vector3::Dot( direction, pos3 ) * k ) );
			float y4 = A * cos( ( Math::Vector3::Dot( direction, pos4 ) * k ) );

			newPos1[1] = y1;
			newPos2[1] = y2;
			newPos3[1] = y3;
			newPos4[1] = y4;
			
			//simpleDraw->RenderLine( newPos1, newPos2 );
			//simpleDraw->RenderLine( newPos2, newPos3 );
			//simpleDraw->RenderLine( newPos3, newPos4 );
			//simpleDraw->RenderLine( newPos4, newPos1 );
		}
	}	
}