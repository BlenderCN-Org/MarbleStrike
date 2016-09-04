//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/particle.hpp"
#include "common/engine/render/particlesystem.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/texturemanager.hpp"
#include <tinyxml2.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ParticleSystem::ParticleSystem()
{
    mMaxParticles = 0;
    mNumLiveParticles = 0;
    mEmitTime = 0;
    mCurrentEmitTime = 0;
    mIsEmitting = false;
    mParticles = NULL;
    mVertexData = NULL;
    mParticleAnimationState = PARTICLE_ANIMATION_STOP;
    mTexture = NULL;
	mEmptyTexture = GameApp::Get()->GetTextureManager()->GetTexture( "emptytexture" );
    mParticleAccumulation = 0;

    System::StringCopy( mName, 32, "" );

	mEffect = GameApp::Get()->GetEffectManager()->GetEffect( "particle" );		
    mObjectToProjectionMatrixHash = System::StringHash( "gObjectToProjectionMatrix" );
    mTextureHash = System::StringHash( "gTexture" );  
	mSimpleMesh = NEW_PTR( "Simple Mesh" ) SimpleMesh;

	mEmissionRange.SetRange( 0, 0 );
    mSizeRange.SetRange( 0, 0 );
    mShrinkRange.SetRange( 0, 0 );
    mVelocityXRange.SetRange( 0, 0 );
    mVelocityYRange.SetRange( 0, 0 );
    mVelocityZRange.SetRange( 0, 0 );
    mLifeRange.SetRange( 0, 0 );
    mDecayRange.SetRange( 0, 0 );
	mAlphaRange.SetRange( 0, 0 );

	mIsLooping = false;
}

//===========================================================================

ParticleSystem::~ParticleSystem()
{
    DELETE_PTR_ARRAY( mParticles );
    DELETE_PTR_ARRAY( mVertexData );
	DELETE_PTR( mSimpleMesh );
}

//===========================================================================

void ParticleSystem::Load( const char* filename )
{
    char filenameFullPath[256];
    System::StringCopy( filenameFullPath, 256, "" );
    const char* resourcePath = Database::Get()->GetResourcePath();
    System::StringCopy( filenameFullPath, 256, resourcePath );
    System::StringConcat( filenameFullPath, 256, filename );

	FILE* file = System::OpenFile( System::Path::SystemSlash( filenameFullPath ), "rb" );
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError loadOkay = doc.LoadFile( file );
	fclose( file );
	file = NULL;

    Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
    if ( loadOkay == tinyxml2::XML_SUCCESS )
    {	
        tinyxml2::XMLNode* node = &doc;
        LoadXML( node );
    }

    DELETE_PTR_ARRAY( mParticles );
    DELETE_PTR_ARRAY( mVertexData );
    mParticles = NEW_PTR( "Particles List" ) Particle[mMaxParticles];
    mVertexData = NEW_PTR( "Vertex Data" ) ParticleVertex[mMaxParticles * 6];
    mNumLiveParticles = 0;
}

//===========================================================================

void ParticleSystem::Save( const char* filename )
{
    tinyxml2::XMLDocument doc; 

    tinyxml2::XMLElement* element = doc.NewElement( "Name" );
    element->SetAttribute( "value", mName );
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "MaxParticles");
    element->SetAttribute( "value", mMaxParticles );
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "EmissionRange" );
    element->SetAttribute( "min", mEmissionRange.GetMin() ); 
    element->SetAttribute( "max", mEmissionRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "SizeRange" );
    element->SetAttribute( "min", mSizeRange.GetMin() ); 
    element->SetAttribute( "max", mSizeRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "ShrinkRange" );
    element->SetAttribute( "min", mShrinkRange.GetMin() ); 
    element->SetAttribute( "max", mShrinkRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "VelocityXRange" );
    element->SetAttribute( "min", mVelocityXRange.GetMin() ); 
    element->SetAttribute( "max", mVelocityXRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "VelocityYRange" );
    element->SetAttribute( "min", mVelocityYRange.GetMin() ); 
    element->SetAttribute( "max", mVelocityYRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "VelocityZRange" );
    element->SetAttribute( "min", mVelocityZRange.GetMin() ); 
    element->SetAttribute( "max", mVelocityZRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "LifeRange" );
    element->SetAttribute( "min", mLifeRange.GetMin() ); 
    element->SetAttribute( "max", mLifeRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "DecayRange" );
    element->SetAttribute( "min", mDecayRange.GetMin() ); 
    element->SetAttribute( "max", mDecayRange.GetMax() ); 
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "ColorRange" );
    element->SetAttribute( "rmin", mColorRange.GetMin()[0] * 255.0f ); 
    element->SetAttribute( "gmin", mColorRange.GetMin()[1] * 255.0f ); 
    element->SetAttribute( "bmin", mColorRange.GetMin()[2] * 255.0f ); 
    element->SetAttribute( "rmax", mColorRange.GetMax()[0] * 255.0f ); 
    element->SetAttribute( "gmax", mColorRange.GetMax()[1] * 255.0f ); 
    element->SetAttribute( "bmax", mColorRange.GetMax()[2] * 255.0f ); 
    doc.LinkEndChild( element ); 

	element = doc.NewElement( "AlphaRange" );
	element->SetAttribute( "min", mAlphaRange.GetMin() * 255.0f );
	element->SetAttribute( "max", mAlphaRange.GetMax() * 255.0f );
	doc.LinkEndChild( element );

    element = doc.NewElement( "EmitTime" );
    element->SetAttribute( "value", mEmitTime );
    doc.LinkEndChild( element ); 

    element = doc.NewElement( "Texture" );
    const char* textureName = NULL;
    if ( mTexture )
    {		
        textureName = mTexture->GetName();
		element->SetAttribute( "name", textureName );
		doc.LinkEndChild( element );
    }    

    std::list<ForceData>::iterator forceIter;
    for ( forceIter = mForces.begin(); forceIter != mForces.end(); ++forceIter )
    {
        ForceData forceData = *forceIter;
        element = doc.NewElement( "Force" );
        element->SetAttribute( "name", forceData.mForceName );
        element->SetAttribute( "x", forceData.mForce[0] );
        element->SetAttribute( "y", forceData.mForce[1] );
        element->SetAttribute( "z", forceData.mForce[2] );
        doc.LinkEndChild( element ); 
    }

    doc.LinkEndChild( doc.NewDeclaration() ); 

    doc.SaveFile( filename ); 
}

//===========================================================================

void ParticleSystem::Update( float elapsedTime )
{	
	if ( mParticleAnimationState == PARTICLE_ANIMATION_PLAY && mNumLiveParticles > 0 )
	{
		for ( int i = 0; i < mMaxParticles; ++i )
		{
			const ParticleState particleState = mParticles[i].mState;
			if ( particleState == PARTICLE_STATE_LIVE || particleState == PARTICLE_STATE_CREATED )
			{
				Particle* particle = &mParticles[i];

				float t = particle->mLife / particle->mStartLife;
				particle->mColor = mColorRange.GetMin() * t + mColorRange.GetMax() * ( 1.0f - t );
				particle->mColor[3] = mAlphaRange.GetMin() * t + mAlphaRange.GetMax() * ( 1.0f - t );

				std::list<ForceData>::iterator forceIter;
				for ( forceIter = mForces.begin(); forceIter != mForces.end(); ++forceIter )
				{
					ForceData currentForceData = *forceIter;
					particle->mVelocity += currentForceData.mForce * elapsedTime;
				}

				particle->mPosition += particle->mVelocity * elapsedTime;
				particle->mLife -= particle->mDecay * elapsedTime;
				particle->mSize -= particle->mShrink * elapsedTime;			
				particle->mState = PARTICLE_STATE_LIVE;

				if ( particle->mSize < 0 )
				{
					particle->mSize = 0;
				}

				if ( particle->mLife <= 0 || particle->mSize <= 0 )
				{
					particle->mState = PARTICLE_STATE_DESTROYED;
					mNumLiveParticles--;
				}
			}
		}	

		if ( mIsLooping )
		{
			EmitParticles( elapsedTime );
		}
	}
}

//===========================================================================

void ParticleSystem::Render( 
    const Math::Matrix44 &projectionMatrix,
    const Math::Matrix44 &viewMatrix,							
    const Math::Matrix44 &transformMatrix	
    )
{
	int vertexCount = 0;
    for ( int i = 0; i < mMaxParticles; ++i )
    {
        const ParticleState particleState = mParticles[i].mState;		
        if ( particleState == PARTICLE_STATE_LIVE )
        {			
            Particle* particle = &mParticles[i];
			
            float size = particle->mSize;
            Math::Vector3 bottomLeft( -size, -size, 0 );
            Math::Vector3 bottomRight( size, -size, 0 );
            Math::Vector3 topRight( size, size, 0 );
            Math::Vector3 topLeft( -size, size, 0 );
            Math::Vector4 color( particle->mColor[0], particle->mColor[1], particle->mColor[2], particle->mLife * particle->mColor[3] );

			Math::Vector3 particlePosition = particle->mPosition;

            mVertexData[vertexCount].mPosition.Set( particlePosition + bottomLeft );
            mVertexData[vertexCount].mTexCoord.Set( 0, 0 );
            mVertexData[vertexCount].mColor = color;
            vertexCount++;
            mVertexData[vertexCount].mPosition.Set( particlePosition + bottomRight );
            mVertexData[vertexCount].mTexCoord.Set( 1, 0 );
            mVertexData[vertexCount].mColor = color;
            vertexCount++;
            mVertexData[vertexCount].mPosition.Set( particlePosition + topRight );
            mVertexData[vertexCount].mTexCoord.Set( 1, 1 );
            mVertexData[vertexCount].mColor = color;
            vertexCount++;
            mVertexData[vertexCount].mPosition.Set( particlePosition + bottomLeft );
            mVertexData[vertexCount].mTexCoord.Set( 0, 0 );
            mVertexData[vertexCount].mColor = color;
            vertexCount++;
            mVertexData[vertexCount].mPosition.Set( particlePosition + topRight );
            mVertexData[vertexCount].mTexCoord.Set( 1, 1 );
            mVertexData[vertexCount].mColor = color;
            vertexCount++;
            mVertexData[vertexCount].mPosition.Set( particlePosition + topLeft );
            mVertexData[vertexCount].mTexCoord.Set( 0, 1 );
            mVertexData[vertexCount].mColor = color;
            vertexCount++;
        }
    }

    if ( vertexCount > 0 )
    {
        Math::Matrix44 temp = viewMatrix * transformMatrix;
        temp[0] = 1; temp[4] = 0; temp[8] = 0;
        temp[1] = 0; temp[5] = 1; temp[9] = 0;
        temp[2] = 0; temp[6] = 0; temp[10] = 1;
		Math::Matrix44 objectToProjection = projectionMatrix * temp;
		mSimpleMesh->mNumPrimitives = vertexCount / 3;
		mSimpleMesh->mPrimitiveType = PRIMITIVE_TRIANGLES;
		mSimpleMesh->mVertexPerPrimitive = 3;
		mSimpleMesh->mVertexData = ( void* )mVertexData;

		RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
		renderNode->mEffect = mEffect;		
		renderNode->mSimpleMesh = mSimpleMesh;
		renderNode->SetMatrix( mObjectToProjectionMatrixHash, &objectToProjection );
		Texture* texture = mEmptyTexture;
		if ( mTexture )
		{
			texture = mTexture;
		}
		renderNode->SetTexture( mTextureHash, texture );

		Math::Rectangle<int> mScissorRectangle;
		mScissorRectangle.SetPosition( 0, 0 );	
		mScissorRectangle.SetWidth( Database::Get()->GetBackBufferWidth() );
		mScissorRectangle.SetHeight( Database::Get()->GetBackBufferHeight() );
		renderNode->mScissorRectangle = mScissorRectangle;
    }	
}

//===========================================================================

void ParticleSystem::SetMaxParticles( int size )
{
	int numParticlesToCopy = size;
	if ( mMaxParticles < size )
	{
		numParticlesToCopy = mMaxParticles;
	}

    mNumLiveParticles = 0;
    mMaxParticles = size;
    mParticleAccumulation = 0;
    mCurrentEmitTime = mEmitTime;

    Particle* particles = NEW_PTR( "Particles List" ) Particle[mMaxParticles];
    ParticleVertex* vertexData = NEW_PTR( "Vertex Data" ) ParticleVertex[ mMaxParticles * 6];

	for ( int i = 0; i < numParticlesToCopy; ++i )
	{
		particles[i] = mParticles[i];
		if ( particles[i].mLife > 0 )
		{
			mNumLiveParticles++;
		}
	}

	int numVerticesToCopy = numParticlesToCopy * 6;
	for ( int i = 0; i < numVerticesToCopy; ++i )
	{
		vertexData[i] = mVertexData[i];
	}		

	DELETE_PTR_ARRAY( mParticles );
    DELETE_PTR_ARRAY( mVertexData );

	mParticles = particles;
	mVertexData = vertexData;
}

//===========================================================================

ForceData* ParticleSystem::GetForceData( const char* name )
{
    std::list<ForceData>::iterator forceIter;
    for ( forceIter = mForces.begin(); forceIter != mForces.end(); ++forceIter )
    {
        ForceData* forceData = &( *forceIter );
        if ( strcmp( forceData->mForceName, name ) == 0 )					
        {	
            return forceData;
        }
    }

    return NULL;
}


//===========================================================================

void ParticleSystem::AddForce( const char* name, float x, float y, float z )
{
    const ForceData* forceData = NULL;
    std::string newName = name;
    int count = 0;
    do
    {
        forceData = GetForceData( newName.c_str() );
        if ( forceData )
        {
            char buffer[32];
            snprintf( buffer, 32, "%i", count );
            newName += buffer;
            count++;
        }
    }
    while ( forceData != NULL );

    ForceData newForceData;
    System::StringCopy( newForceData.mForceName, 32, newName.c_str() );
    newForceData.mForce.Set( x, y, z );
	mForces.push_back( newForceData );
}

//===========================================================================

void ParticleSystem::DeleteForce( const char* name )
{
	const ForceData* forceData = GetForceData( name );
	mForces.remove( *forceData );
}

//===========================================================================

void ParticleSystem::Play()
{
	mParticleAnimationState = PARTICLE_ANIMATION_PLAY;
	mParticleAccumulation = 0;
	mCurrentEmitTime = mEmitTime;

	EmitParticles( 1.0f );
}

//===========================================================================

void ParticleSystem::Stop()
{
	mParticleAnimationState = PARTICLE_ANIMATION_STOP;
}	

//===========================================================================

void ParticleSystem::EmitParticles( float elapsedTime )
{	
    if ( mEmitTime == -1 )
    {
        mIsEmitting = true;
    }
    else if ( mCurrentEmitTime > 0 )
    {
        mCurrentEmitTime -= elapsedTime;
        mIsEmitting = true;
    }
    else if ( mCurrentEmitTime <= 0 )
    {
        mIsEmitting = false;
    }

    if ( mIsEmitting == false )
    {
        return;
    }

    if ( mNumLiveParticles >= mMaxParticles )
    {
        return;
    }

    mParticleAccumulation += mEmissionRange.RandomValue() * elapsedTime;		
    if ( mParticleAccumulation < 1.0 )
    {
        return;
    }

    int emitNumParticles = static_cast<int>( mParticleAccumulation );
    mParticleAccumulation -= static_cast<float>( emitNumParticles );

    int count = 0;
    for ( int i = 0; i < mMaxParticles; ++i )
    {
        const ParticleState particleState = mParticles[i].mState;
        if ( particleState == PARTICLE_STATE_DESTROYED )
        {
            mParticles[i].mState = PARTICLE_STATE_CREATED;
            mParticles[i].mPosition = mPosition;
            mParticles[i].mVelocity.Set( 
                mVelocityXRange.RandomValue(), 
                mVelocityYRange.RandomValue(), 
                mVelocityZRange.RandomValue() 
                );
            mParticles[i].mLife = mLifeRange.RandomValue();
            mParticles[i].mSize = mSizeRange.RandomValue();
            mParticles[i].mShrink = mShrinkRange.RandomValue();				
            mParticles[i].mDecay = mDecayRange.RandomValue();				
            mParticles[i].mStartLife = mParticles[i].mLife;

            mNumLiveParticles++;
            if ( mNumLiveParticles >= mMaxParticles )
            {
                return;
            }

            count++;				
            if ( count >= emitNumParticles )
            {
                return;
            }
        }
    }
}

//===========================================================================

void ParticleSystem::LoadXML( tinyxml2::XMLNode* node )
{
    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() )
    {
        tinyxml2::XMLElement* element = item->ToElement();
        if ( element )
        {
            tinyxml2::XMLElement* element = item->ToElement();
            const char* elementValue = element->Value();
            if ( strcmp( elementValue, "Name" ) == 0 )
            {
                const char* name = element->Attribute( "value" );
                System::StringCopy( mName, 32, name );
            }
            else if ( strcmp( elementValue, "MaxParticles" ) == 0 )
            {
                int value = element->IntAttribute( "value" );
                mMaxParticles = value;
            }
            else if ( strcmp( elementValue, "EmissionRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mEmissionRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "SizeRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mSizeRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "ShrinkRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mShrinkRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "VelocityXRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mVelocityXRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "VelocityYRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mVelocityYRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "VelocityZRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mVelocityZRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "LifeRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mLifeRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "DecayRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );
                double max = element->DoubleAttribute( "max" );

                float minf = static_cast<float>( min );
                float maxf = static_cast<float>( max );
                mDecayRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "ColorRange" ) == 0 )
            {
                double r = element->DoubleAttribute( "rmin" );
                double g = element->DoubleAttribute( "gmin" );
                double b = element->DoubleAttribute( "bmin" );
                double a = element->DoubleAttribute( "amin" );

                float rf = static_cast<float>( r ) / 255.0f;
                float gf = static_cast<float>( g ) / 255.0f;
                float bf = static_cast<float>( b ) / 255.0f;

                Math::Vector4 minColor( rf, gf, bf, 1.0f );

                r = element->DoubleAttribute( "rmax" );
                g = element->DoubleAttribute( "gmax" );
                b = element->DoubleAttribute( "bmax" );
                a = element->DoubleAttribute( "amax" );

                rf = static_cast<float>( r ) / 255.0f;
                gf = static_cast<float>( g ) / 255.0f;
                bf = static_cast<float>( b ) / 255.0f;

                Math::Vector4 maxColor( rf, gf, bf, 1.0f );

                mColorRange.SetRange( minColor, maxColor );
            }
            else if ( strcmp( elementValue, "AlphaRange" ) == 0 )
            {
                double min = element->DoubleAttribute( "min" );                
                float minf = static_cast<float>( min ) / 255.0f;
                
                double max = element->DoubleAttribute( "max" );                
                float maxf = static_cast<float>( max ) / 255.0f;
                
                mAlphaRange.SetRange( minf, maxf );
            }
            else if ( strcmp( elementValue, "EmitTime" ) == 0 )
            {
                double value = element->DoubleAttribute( "value" );	

                float valuef = static_cast<float>( value );
                mEmitTime = valuef;
            }
            else if ( strcmp( elementValue, "Texture" ) == 0 )
            {					
                const char* textureName = element->Attribute( "name" );					
                if ( strcmp( textureName, "" ) != 0 )
                {
                    mTexture = GameApp::Get()->GetTextureManager()->GetTexture( textureName );
                }					
            }
            else if ( strcmp( elementValue, "Force" ) == 0 )
            {					
                double x = element->DoubleAttribute( "x" );	
                double y = element->DoubleAttribute( "y" );	
                double z = element->DoubleAttribute( "z" );	

                const char* name = element->Attribute( "name" );

                AddForce( 
                    name, 
                    static_cast<float>( x ), 
                    static_cast<float>( y ), 
                    static_cast<float>( z ) 
                    );
            }
        }

        tinyxml2::XMLNode* child = item->FirstChild();
        if ( child != NULL )
        {
            LoadXML( child );
        }
    }		
}