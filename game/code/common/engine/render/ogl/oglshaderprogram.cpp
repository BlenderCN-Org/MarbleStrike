#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/ogl/oglpixelshader.hpp"
#include "common/engine/render/ogl/oglrenderer.hpp"
#include "common/engine/render/ogl/oglsamplerstate.hpp"
#include "common/engine/render/ogl/oglshaderprogram.hpp"
#include "common/engine/render/ogl/ogltexture.hpp"
#include "common/engine/render/ogl/oglvertexshader.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"
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

OGLShaderProgram::OGLShaderProgram( int id )
	:	ShaderProgram( id )
{   
}

//===========================================================================

OGLShaderProgram::~OGLShaderProgram()
{   
}

//===========================================================================

void OGLShaderProgram::Create()
{
	mShaderProgram = OGL::oglCreateProgramObject(); 
	LinkShaders(); 

	int numActiveUniforms = 0;
	OGL::oglGetProgramiv( mShaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms );
	for ( int i = 0; i < numActiveUniforms; ++i )
	{
		char nameBuffer[256];
		int length;
		int size;
		GLenum type;
		OGL::oglGetActiveUniform(
			mShaderProgram,
			i,
			256,
			&length,
			&size,
			&type,
			nameBuffer
			);

		ConstantData data;
		//System::StringCopy( data.mName, 64, nameBuffer );
        data.mNameHash = System::StringHash( nameBuffer );
		data.mHandle = OGL::oglGetUniformLocation( mShaderProgram, nameBuffer );
		data.mSize = size;

		switch ( type )
		{
		case GL_FLOAT_VEC4:
			data.mType = VAR_TYPE_VECTOR;			
			break;
		case GL_FLOAT_MAT4:
			data.mType = VAR_TYPE_MATRIX;			
			break;
		case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		case GL_SAMPLER_2D_SHADOW:
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

void OGLShaderProgram::Destroy()
{
	UnLinkShaders();
	OGL::oglDeleteObject( mShaderProgram );
}

//===========================================================================

void OGLShaderProgram::DisableTextureSamplers( void* context )
{
	UNUSED_ALWAYS( context );
}

//===========================================================================

void OGLShaderProgram::Reload()
{
	Destroy();

    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	vertexShader->Reload();
	pixelShader->Reload();

	Create();
}

//===========================================================================

void OGLShaderProgram::SetProgram( 
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing,
	const std::map<int, const void*> &constantMap, 
	int threadID,
	void* context 
	)
{
	UNUSED_ALWAYS( renderStateShadowing );
	UNUSED_ALWAYS( context );
	UNUSED_ALWAYS( threadID );

	OGL::oglUseProgramObject( mShaderProgram );

	int samplerIndex = 0;
	ConstantList::iterator constantListIter = mConstantList.begin();
	for ( constantListIter; constantListIter != mConstantList.end(); ++constantListIter )
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

                SamplerState* samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerStateByHandle( effectPass->GetSamplerState( data.mNameHash ) );

				Assert( samplerState != NULL, "" );
				SetParamTexture( data.mHandle, texture, samplerIndex, samplerState );
				samplerIndex++;
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void OGLShaderProgram::LinkShaders()
{   
    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
    PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	OGLVertexShader* oglVertexShader = reinterpret_cast<OGLVertexShader*>( vertexShader );
	OGLPixelShader* oglPixelShader = reinterpret_cast<OGLPixelShader*>( pixelShader );
	OGL::oglAttachObject( mShaderProgram, oglVertexShader->GetHandle() );
	OGL::oglAttachObject( mShaderProgram, oglPixelShader->GetHandle() );

	OGL::oglBindAttribLocation( mShaderProgram, VERTEX_ARRAY, "inVertex" );
	OGL::oglBindAttribLocation( mShaderProgram, TEXCOORD_ARRAY, "inTexCoord" );
	OGL::oglBindAttribLocation( mShaderProgram, TEXCOORD1_ARRAY, "inTexCoord1" );
	OGL::oglBindAttribLocation( mShaderProgram, TANGENT_ARRAY, "inTangent" );	
	OGL::oglBindAttribLocation( mShaderProgram, COLOR_ARRAY, "inColor" );	
	OGL::oglBindAttribLocation( mShaderProgram, NORMAL_ARRAY, "inNormal" );
	OGL::oglBindAttribLocation( mShaderProgram, BINORMAL_ARRAY, "inBinormal" );

	OGL::oglLinkProgram( mShaderProgram );
	
	mInVertex = GetAttribParamHandle( "inVertex" );
	mInTexCoord[0] = GetAttribParamHandle( "inTexCoord" );
	mInTexCoord[1] = GetAttribParamHandle( "inTexCoord1" );
	mInTangent = GetAttribParamHandle( "inTangent" );
	mInColor = GetAttribParamHandle( "inColor" );
	mInNormal = GetAttribParamHandle( "inNormal" );
	mInBinormal = GetAttribParamHandle( "inBinormal" );
	
	int linkStatus = 0;
	OGL::oglGetObjectParameteriv( mShaderProgram, GL_OBJECT_LINK_STATUS_ARB, &linkStatus );
	if ( linkStatus == 0 )
	{		
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		OGL::oglGetObjectParameteriv( mShaderProgram, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength );
		if ( infologLength > 0 )
		{
			infoLog = ( char* )malloc(infologLength);
			OGL::oglGetInfoLog( mShaderProgram, infologLength, &charsWritten, infoLog );
			Assert( false, "Error linking shader." );
			free(infoLog);
		}

		Assert( false, "Error linking shader." );
	}
}

//===========================================================================

void OGLShaderProgram::UnLinkShaders() const
{
    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	OGLVertexShader* oglVertexShader = reinterpret_cast<OGLVertexShader*>( vertexShader );
	OGLPixelShader* oglPixelShader = reinterpret_cast<OGLPixelShader*>( pixelShader );
	OGL::oglDetachObject( mShaderProgram, oglVertexShader->GetHandle() );
	OGL::oglDetachObject( mShaderProgram, oglPixelShader->GetHandle() );    
}

//===========================================================================

int OGLShaderProgram::GetAttribParamHandle( const char* paramName ) const
{
	int handle = OGL::oglGetAttribLocation( mShaderProgram, paramName ); 
	return handle;
}

//===========================================================================

void OGLShaderProgram::SetParamVector( int handle, const Math::Vector4* value, int numItems )
{
	OGL::oglUniform4fv( handle, numItems, &value[0][0] );
}

//===========================================================================

void OGLShaderProgram::SetParamMatrix( int handle, const Math::Matrix44* matrix, int numItems )
{
	OGL::oglUniformMatrix4fv( handle, numItems, false, &matrix[0][0] ); 
}

//===========================================================================

void OGLShaderProgram::SetParamTexture( int handle, const Texture* texture, int samplerIndex, SamplerState* state )
{
	if ( texture != NULL )
	{
		const OGLTexture* oglTexture = reinterpret_cast<const OGLTexture*>( texture );
		OGL::oglActiveTexture( GL_TEXTURE0 + samplerIndex );

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
		
		OGL::oglBindTexture( textureType, oglTexture->GetHandle() );
		OGL::oglUniform1i( handle, samplerIndex );
		OGLSamplerState* oglSamplerState = reinterpret_cast<OGLSamplerState*>( state );
		oglSamplerState->SetTextureState( textureType );
	}
}

#endif