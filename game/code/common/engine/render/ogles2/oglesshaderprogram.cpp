#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/ogles2/oglespixelshader.hpp"
#include "common/engine/render/ogles2/oglesrenderer.hpp"
#include "common/engine/render/ogles2/oglessamplerstate.hpp"
#include "common/engine/render/ogles2/oglesshaderprogram.hpp"
#include "common/engine/render/ogles2/oglestexture.hpp"
#include "common/engine/render/ogles2/oglesvertexshader.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/textureparameter.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESShaderProgram::OGLESShaderProgram( int id )
	:	ShaderProgram( id )
{   
}

//===========================================================================

OGLESShaderProgram::~OGLESShaderProgram()
{   
}

//===========================================================================

void OGLESShaderProgram::Create()
{
	mShaderProgram = OGLES::oglCreateProgram(); 
	LinkShaders(); 

	int numActiveUniforms = 0;
	OGLES::oglGetProgramiv( mShaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms );
	for ( int i = 0; i < numActiveUniforms; ++i )
	{
		char nameBuffer[256];
		int length;
		int size;
		GLenum type;
		OGLES::oglGetActiveUniform(
			mShaderProgram,
			i,
			256,
			&length,
			&size,
			&type,
			nameBuffer
			);
        
        std::string constantName = nameBuffer;
        size_t findIndex = constantName.find( "[0]" );
        if ( findIndex != std::string::npos )
        {
            constantName.erase( findIndex, strlen( "[0]" ) );
        }

		ConstantData data;
		//System::StringCopy( data.mName, 64, constantName.c_str() );
        data.mNameHash = System::StringHash( constantName.c_str() );
		data.mSize = size;
		data.mHandle = OGLES::oglGetUniformLocation( mShaderProgram, nameBuffer );
        

		switch ( type )
		{
		case GL_FLOAT_VEC4:
			data.mType = VAR_TYPE_VECTOR;			
			break;
		case GL_FLOAT_MAT4:
			data.mType = VAR_TYPE_MATRIX;			
			break;
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			data.mType = VAR_TYPE_TEXTURE;
			break;
		default:
			Assert( false, "" ) ;
			break;
		}

		mConstantList.push_back( data );
	}
}

//===========================================================================

void OGLESShaderProgram::Destroy()
{
	UnLinkShaders();
	OGLES::oglDeleteProgram( mShaderProgram );
}

//===========================================================================

void OGLESShaderProgram::Reload()
{
	Destroy();
	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );
	vertexShader->Reload();
	pixelShader->Reload();
	Create();
}

//===========================================================================

void OGLESShaderProgram::DisableTextureSamplers( void* /*context*/ )
{
}

//===========================================================================

void OGLESShaderProgram::SetProgram(
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing,
	const std::map<int, const void*> &constantMap, 
	int /*threadID*/,
	void* /*context*/
	)
{
    if ( mShaderProgram != renderStateShadowing->GetShaderProgram() )
	{
        OGLES::oglUseProgram( mShaderProgram );
        renderStateShadowing->SetShaderProgram(mShaderProgram);
    }

	int samplerIndex = 0;
	for ( ConstantList::iterator constantListIter = mConstantList.begin(); constantListIter != mConstantList.end(); ++constantListIter )
	{
		ConstantData data = *constantListIter;
		std::map<int, const void*>::const_iterator iter = constantMap.find( data.mNameHash );
		Assert( iter != constantMap.end(), "Parameter not set." );

		switch ( data.mType )
		{
		case VAR_TYPE_VECTOR:
			{
				const Math::Vector4* value = reinterpret_cast<const Math::Vector4*>( iter->second );
				SetParamVector( data.mHandle, value, data.mSize );
			}
			break;
		case VAR_TYPE_MATRIX:
			{
				const Math::Matrix44* value = reinterpret_cast<const Math::Matrix44*>( iter->second );
				SetParamMatrix( data.mHandle, value, data.mSize );
			}
			break;
		case VAR_TYPE_TEXTURE:
			{
				const Texture* texture = reinterpret_cast<const Texture*>( iter->second );
				ResourceHandle<SamplerState> state = effectPass->GetSamplerState( data.mNameHash );
				SetParamTexture( data.mHandle, texture, samplerIndex, state );
				samplerIndex++;
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void OGLESShaderProgram::LinkShaders()
{   
	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );
	OGLESVertexShader* oglesVertexShader = reinterpret_cast<OGLESVertexShader*>( vertexShader );
	OGLESPixelShader* oglesPixelShader = reinterpret_cast<OGLESPixelShader*>( pixelShader );
	OGLES::oglAttachShader( mShaderProgram, oglesVertexShader->GetHandle() );
	OGLES::oglAttachShader( mShaderProgram, oglesPixelShader->GetHandle() );

	OGLES::oglBindAttribLocation( mShaderProgram, OGLESShaderProgram::VERTEX_ARRAY, "inVertex" );
	OGLES::oglBindAttribLocation( mShaderProgram, OGLESShaderProgram::TEXCOORD_ARRAY, "inTexCoord" );
	OGLES::oglBindAttribLocation( mShaderProgram, OGLESShaderProgram::COLOR_ARRAY, "inColor" );
	OGLES::oglBindAttribLocation( mShaderProgram, OGLESShaderProgram::NORMAL_ARRAY, "inNormal" );

	OGLES::oglLinkProgram( mShaderProgram );
	
	mInVertex = GetAttribParamHandle( "inVertex" );
	mInTexCoord = GetAttribParamHandle( "inTexCoord" );
	mInColor = GetAttribParamHandle( "inColor" );
	mInNormal = GetAttribParamHandle( "inNormal" );

	int linkStatus = 0;
	OGLES::oglGetProgramiv( mShaderProgram, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == 0 )
	{		
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		OGLES::oglGetProgramiv( mShaderProgram, GL_INFO_LOG_LENGTH, &infologLength );
		if ( infologLength > 0 )
		{
			infoLog = ( char* )malloc(infologLength);
			OGLES::oglGetProgramInfoLog( mShaderProgram, infologLength, &charsWritten, infoLog );
			Assert( false, "Error linking shader." );
			free(infoLog);
		}

		Assert( false, "Error linking shader." );
	}
}

//===========================================================================

void OGLESShaderProgram::UnLinkShaders() const
{
	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );
	OGLESVertexShader* oglesVertexShader = reinterpret_cast<OGLESVertexShader*>( vertexShader );
	OGLESPixelShader* oglesPixelShader = reinterpret_cast<OGLESPixelShader*>( pixelShader );
	OGLES::oglDetachShader( mShaderProgram, oglesVertexShader->GetHandle() );
	OGLES::oglDetachShader( mShaderProgram, oglesPixelShader->GetHandle() );    
}

//===========================================================================

int OGLESShaderProgram::GetAttribParamHandle( const char* paramName ) const
{
	int handle = OGLES::oglGetAttribLocation( mShaderProgram, paramName ); 
	return handle;
}

//===========================================================================

void OGLESShaderProgram::SetParamVector( int handle, const Math::Vector4* value, int numItems )
{	
	OGLES::oglUniform4fv( handle, numItems, &value[0][0] );
}

//===========================================================================

void OGLESShaderProgram::SetParamMatrix( int handle, const Math::Matrix44* matrix, int numItems )
{
	OGLES::oglUniformMatrix4fv( handle, numItems, false, &matrix[0][0] ); 
}

//===========================================================================

void OGLESShaderProgram::SetParamTexture( 
	int handle, 
	const Texture* texture, 
	int samplerIndex, 
	ResourceHandle<SamplerState> samplerStateHandle 
	)
{
	if ( texture != NULL )
	{
		const OGLESTexture* oglESTexture = reinterpret_cast<const OGLESTexture*>( texture );
		OGLES::oglActiveTexture( GL_TEXTURE0 + samplerIndex );

		GLenum textureType = GL_TEXTURE_2D;

		switch ( texture->GetTextureType() )
		{
		case TEXTURE_TYPE_2D:			
			textureType = GL_TEXTURE_2D;
			break;
		case TEXTURE_TYPE_CUBEMAP:
			textureType = GL_TEXTURE_CUBE_MAP;			
			break;
		}

		OGLES::oglBindTexture( textureType, oglESTexture->GetHandle() );
		OGLES::oglUniform1i( handle, samplerIndex );

		SamplerState* samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerStateByHandle( samplerStateHandle );	
		OGLESSamplerState* oglESSamplerState = reinterpret_cast<OGLESSamplerState*>( samplerState );
		oglESSamplerState->SetTextureState( textureType );
	}
}

#endif