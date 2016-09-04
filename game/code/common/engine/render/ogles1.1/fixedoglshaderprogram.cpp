#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/textureparameter.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/string.hpp"
#include "common/engine/system/utilities.hpp"
#include "iphone/render/ogles1.1/fixedoglsamplerstate.hpp"
#include "iphone/render/ogles1.1/fixedoglpixelshader.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "iphone/render/ogles1.1/fixedoglshaderprogram.hpp"
#include "iphone/render/ogles1.1/fixedogltexture.hpp"
#include "iphone/render/ogles1.1/fixedoglvertexshader.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLShaderProgram::FixedOGLShaderProgram( int id )
:   ShaderProgram( id )
{
	mShaderProgramType = SHADER_PROGRAM_NO_LIGHT;
}

//===========================================================================

FixedOGLShaderProgram::~FixedOGLShaderProgram()
{   
}

//===========================================================================

void FixedOGLShaderProgram::Create()
{	
	if ( strstr( mVertexShader->GetFilename(), "simplelight" ) )
	{
		mShaderProgramType = SHADER_PROGRAM_LIGHT;
	}
	else if ( strstr( mVertexShader->GetFilename(), "simplenolight" ) )
	{
		mShaderProgramType = SHADER_PROGRAM_NO_LIGHT;
	}
}

//===========================================================================

void FixedOGLShaderProgram::Destroy()
{

}

//===========================================================================

void FixedOGLShaderProgram::DisableTextureSamplers( void* /*context*/ )
{
}

//===========================================================================

void FixedOGLShaderProgram::SetProgram( 
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing,
	const std::map<int, const void*> &constantMap,
	int /*threadID*/,
	void* context 
	)
{	
	SetConstants( effectPass, renderStateShadowing, constantMap, context );
}

//===========================================================================

void FixedOGLShaderProgram::SetConstants( 
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing, 
	const std::map<int, const void*> &constantMap,
	void* context 
	)
{
	switch ( mShaderProgramType )
	{
	case SHADER_PROGRAM_LIGHT:
		SetConstantsLight( effectPass, renderStateShadowing, constantMap, context );
		break;
	case SHADER_PROGRAM_NO_LIGHT:
		SetConstantsNoLight( effectPass, renderStateShadowing, constantMap, context );
		break;
	}	
}

//===========================================================================

void FixedOGLShaderProgram::SetConstantsNoLight( 
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing, 
	const std::map<int, const void*> &constantMap,
	void* context 
	)
{
	UNUSED_ALWAYS( renderStateShadowing );
	UNUSED_ALWAYS( context );

	std::map<int, const void*>::const_iterator iter;

	FixedOGL::oglShadeModel( GL_SMOOTH );
	FixedOGL::oglDisable( GL_TEXTURE_2D );

	FixedOGL::oglDisable( GL_LIGHTING );
	FixedOGL::oglDisable( GL_LIGHT0 );
	
	Math::Matrix44 identity;
	identity.SetIdentity();

	FixedOGL::oglMatrixMode( GL_PROJECTION );

	FixedOGL::oglLoadMatrixf( identity.GetRAWData() );
	
    iter = constantMap.find( System::StringHash( "gProjectionMatrix" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Matrix44* projectionMatrix = reinterpret_cast<const Math::Matrix44*>( iter->second );
	FixedOGL::oglMultMatrixf( projectionMatrix->GetRAWData() );	

	FixedOGL::oglMatrixMode( GL_MODELVIEW );
	FixedOGL::oglLoadMatrixf( identity.GetRAWData() );

	iter = constantMap.find( System::StringHash( "gViewMatrix" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Matrix44* viewMatrix = reinterpret_cast<const Math::Matrix44*>( iter->second );
	FixedOGL::oglMultMatrixf( viewMatrix->GetRAWData() );
		
	iter = constantMap.find( System::StringHash( "gTransformMatrix" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Matrix44* transformMatrix = reinterpret_cast<const Math::Matrix44*>( iter->second );
	FixedOGL::oglMultMatrixf( transformMatrix->GetRAWData() );

	FixedOGL::oglDisable( GL_COLOR_MATERIAL );

	iter = constantMap.find( System::StringHash( "gDiffuseColor" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Vector4* diffuseColor = reinterpret_cast<const Math::Vector4*>( iter->second );
	FixedOGL::oglMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseColor->GetRAWData() );
	FixedOGL::oglColor4fv( diffuseColor->GetRAWData() );

	iter = constantMap.find( System::StringHash( "gTexture0" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Texture* texture = reinterpret_cast<const Texture*>( iter->second );
	int samplerIndex = 0;
	if ( texture != NULL )	
	{
		FixedOGL::oglMatrixMode( GL_TEXTURE );
		FixedOGL::oglLoadMatrixf( identity.GetRAWData() );		

		FixedOGL::oglEnable( GL_TEXTURE_2D );
		FixedOGL::oglActiveTexture( GL_TEXTURE0 + samplerIndex );
		const FixedOGLTexture* mFixedOGLTexture = reinterpret_cast<const FixedOGLTexture*>( texture );
		mFixedOGLTexture->Bind( samplerIndex );
        SamplerState* state = effectPass->GetSamplerState( System::StringHash( "gTexture0" ) );
		Assert( state != NULL, "" );
		FixedOGLSamplerState* fixedOGLSamplerState = reinterpret_cast<FixedOGLSamplerState*>( state );
		fixedOGLSamplerState->SetTextureState();
	}
}

//===========================================================================

void FixedOGLShaderProgram::SetConstantsLight(
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing, 
	const std::map<int, const void*> &constantMap,
	void* context 
	)
{
	UNUSED_ALWAYS( renderStateShadowing );
	UNUSED_ALWAYS( context );

	std::map<int, const void*>::const_iterator iter;

	FixedOGL::oglShadeModel( GL_SMOOTH );
	FixedOGL::oglDisable( GL_TEXTURE_2D );

	FixedOGL::oglEnable( GL_LIGHTING );
	FixedOGL::oglEnable( GL_LIGHT0 );

	Math::Matrix44 identity;
	identity.SetIdentity();

	FixedOGL::oglMatrixMode( GL_PROJECTION );

	FixedOGL::oglLoadMatrixf( identity.GetRAWData() );

	iter = constantMap.find( System::StringHash( "gProjectionMatrix" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Matrix44* projectionMatrix = reinterpret_cast<const Math::Matrix44*>( iter->second );
	FixedOGL::oglMultMatrixf( projectionMatrix->GetRAWData() );	

	FixedOGL::oglMatrixMode( GL_MODELVIEW );
	FixedOGL::oglLoadMatrixf( identity.GetRAWData() );

	iter = constantMap.find( System::StringHash( "gViewMatrix" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Matrix44* viewMatrix = reinterpret_cast<const Math::Matrix44*>( iter->second );
	FixedOGL::oglMultMatrixf( viewMatrix->GetRAWData() );

	iter = constantMap.find( System::StringHash( "gLightColor" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Vector4* lightColor = reinterpret_cast<const Math::Vector4*>( iter->second );
	FixedOGL::oglLightfv( GL_LIGHT0, GL_DIFFUSE, lightColor->GetRAWData() );

	iter = constantMap.find( System::StringHash( "gLightAmbientColor" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Vector4* lightAmbientcolor = reinterpret_cast<const Math::Vector4*>( iter->second );
	FixedOGL::oglLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbientcolor->GetRAWData() );

	iter = constantMap.find( System::StringHash( "gLightPosition" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Vector4* lightPosition = reinterpret_cast<const Math::Vector4*>( iter->second );
	FixedOGL::oglLightfv( GL_LIGHT0, GL_POSITION, lightPosition->GetRAWData() );	

	iter = constantMap.find( System::StringHash( "gTransformMatrix" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Matrix44* transformMatrix = reinterpret_cast<const Math::Matrix44*>( iter->second );
	FixedOGL::oglMultMatrixf( transformMatrix->GetRAWData() );

	FixedOGL::oglDisable( GL_COLOR_MATERIAL );

	iter = constantMap.find( System::StringHash( "gDiffuseColor" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Math::Vector4* diffuseColor = reinterpret_cast<const Math::Vector4*>( iter->second );
	FixedOGL::oglMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuseColor->GetRAWData() );
	FixedOGL::oglColor4fv( diffuseColor->GetRAWData() );

	iter = constantMap.find( System::StringHash( "gTexture0" ) );
	Assert( iter != constantMap.end(), "Parameter not set." );
	const Texture* texture = reinterpret_cast<const Texture*>( iter->second );
	int samplerIndex = 0;
	if ( texture != NULL )	
	{
		FixedOGL::oglMatrixMode( GL_TEXTURE );
		FixedOGL::oglLoadMatrixf( identity.GetRAWData() );		

		FixedOGL::oglEnable( GL_TEXTURE_2D );
		FixedOGL::oglActiveTexture( GL_TEXTURE0 + samplerIndex );
		const FixedOGLTexture* mFixedOGLTexture = reinterpret_cast<const FixedOGLTexture*>( texture );
		mFixedOGLTexture->Bind( samplerIndex );
		SamplerState* state = effectPass->GetSamplerState( System::StringHash( "gTexture0" ) );
		Assert( state != NULL, "" );
		FixedOGLSamplerState* fixedOGLSamplerState = reinterpret_cast<FixedOGLSamplerState*>( state );
		fixedOGLSamplerState->SetTextureState();
	}
}

#endif