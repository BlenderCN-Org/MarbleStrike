#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d/d3dpixelshader.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3dshaderprogram.hpp"
#include "common/engine/render/d3d/d3dtexture.hpp"
#include "common/engine/render/d3d/d3dvertexshader.hpp"
#include "common/engine/render/effectpass.hpp"
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

D3DShaderProgram::D3DShaderProgram( int id )
	:	ShaderProgram( id )
{
}

//===========================================================================

D3DShaderProgram::~D3DShaderProgram()
{   
	ConstantList::iterator constantListIter = mConstantList.begin();
	for ( constantListIter; constantListIter != mConstantList.end(); ++constantListIter )
	{
		ConstantData* data = *constantListIter;
		DELETE_PTR( data );
	}
}

//===========================================================================

void D3DShaderProgram::Create()
{
    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
    PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	D3DVertexShader* d3dVertexShader = reinterpret_cast<D3DVertexShader*>( vertexShader );
	D3DPixelShader* d3dPixelShader = reinterpret_cast<D3DPixelShader*>( pixelShader );

	D3DXCONSTANTTABLE_DESC constantTableDesc;

	d3dVertexShader->GetConstantTable()->GetDesc( &constantTableDesc );	
	for ( unsigned int i = 0; i < constantTableDesc.Constants; ++i )
	{		
		ConstantData* data = NEW_PTR("ConstantData") ConstantData;
		D3DXHANDLE handle = d3dVertexShader->GetConstantTable()->GetConstant( NULL, i );
		data->mConstantType = CONSTANT_TYPE_VERTEX;

		D3DXCONSTANT_DESC constantDesc;
		unsigned int count = 0;
		d3dVertexShader->GetConstantTable()->GetConstantDesc( handle, &constantDesc, &count );
		//System::StringCopy( data->mName, 64, constantDesc.Name );
        data->mNameHash = System::StringHash( constantDesc.Name );
		data->mHandle = handle;
		data->mSize = constantDesc.Elements;

		switch ( constantDesc.Type )
		{
		case D3DXPT_FLOAT:
			{
				switch ( constantDesc.Class )
				{
				case D3DXPC_VECTOR:
					data->mVariableType = VAR_TYPE_VECTOR;
					break;
				case D3DXPC_MATRIX_ROWS:			
				case D3DXPC_MATRIX_COLUMNS:
					data->mVariableType = VAR_TYPE_MATRIX;
					break;
				default:
					Assert( false, "" );
					break;
				}		
			}
			break;
		default:
			Assert( false, "" );
			break;
		}

		mConstantList.push_back( data );
	}

	d3dPixelShader->GetConstantTable()->GetDesc( &constantTableDesc );
	for ( unsigned int i = 0; i < constantTableDesc.Constants; ++i )
	{
		D3DXCONSTANT_DESC constantDesc;
		unsigned int count = 0;
		D3DXHANDLE handle = d3dPixelShader->GetConstantTable()->GetConstant( NULL, i );
		d3dPixelShader->GetConstantTable()->GetConstantDesc( handle, &constantDesc, &count );
		
		switch ( constantDesc.Type )
		{
		case D3DXPT_FLOAT:
			{
				ConstantData* data = NEW_PTR("ConstantData") ConstantData;
				data->mConstantType = CONSTANT_TYPE_PIXEL;
				//System::StringCopy( data->mName, 64, constantDesc.Name );
                data->mNameHash = System::StringHash( constantDesc.Name );
				data->mHandle = handle;
				data->mSize = constantDesc.Elements;

				switch ( constantDesc.Class )
				{
				case D3DXPC_VECTOR:
					data->mVariableType = VAR_TYPE_VECTOR;
					break;
				case D3DXPC_MATRIX_ROWS:			
				case D3DXPC_MATRIX_COLUMNS:
					data->mVariableType = VAR_TYPE_MATRIX;
					break;
				default:
					Assert( false, "" );
					break;
				}

				mConstantList.push_back( data );		
			}
			break;		
		case D3DXPT_TEXTURE2D:
		case D3DXPT_TEXTURECUBE:
			{
				TextureData data;
				data.mConstantType = CONSTANT_TYPE_PIXEL;

				std::string constantName = constantDesc.Name;
				size_t lastOfIndex = constantName.find_last_of( "+" );
				constantName = constantName.substr( lastOfIndex + 1, std::string::npos );

				//System::StringCopy( data.mName, 64, constantName.c_str() );
                data.mNameHash = System::StringHash( constantName.c_str() );
				data.mSamplerIndex = constantDesc.RegisterIndex;

				data.mVariableType = VAR_TYPE_TEXTURE;

				mTextureList.push_back( data );		
			}
			break;
		default:
			Assert( false, "" );
			break;
		}
	}
}

//===========================================================================

void D3DShaderProgram::Destroy()
{    
}

//===========================================================================

void D3DShaderProgram::Reload()
{
    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	vertexShader->Reload();
	pixelShader->Reload();
}

//===========================================================================

void D3DShaderProgram::DisableTextureSamplers( void* context )
{
	UNUSED_ALWAYS( context );

	for ( int i = 0; i < Renderer::Get()->GetNumTextureUnits(); ++i )
	{
		D3DRenderer::GetDevice()->SetTexture( i, NULL );
	}
}

//===========================================================================

void D3DShaderProgram::SetProgram( 
	EffectPass* effectPass, 
	RenderStateShadowing* renderStateShadowing,
	const std::map<int, const void*> &constantMap, 
	int threadID,
	void* context 
	)
{
	UNUSED_ALWAYS( threadID );

    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	D3DVertexShader* d3dVertexShader = reinterpret_cast<D3DVertexShader*>( vertexShader );
	D3DPixelShader* d3dPixelShader = reinterpret_cast<D3DPixelShader*>( pixelShader );

	d3dVertexShader->Dispatch();
	d3dPixelShader->Dispatch();

	ConstantList::iterator constantListIter = mConstantList.begin();
	for ( constantListIter; constantListIter != mConstantList.end(); ++constantListIter )
	{
		ConstantData* data = *constantListIter;
		std::map<int, const void*>::const_iterator iter = constantMap.find( data->mNameHash );
		Assert( iter != constantMap.end(), "Parameter not set." );

		switch ( data->mVariableType )
		{
		case VAR_TYPE_VECTOR:
			{
				const Math::Vector4* value = reinterpret_cast<const Math::Vector4*>( iter->second );
				SetParamVector( data->mHandle, data->mConstantType, value, data->mSize );
			}
			break;
		case VAR_TYPE_MATRIX:
			{
				const Math::Matrix44* value = reinterpret_cast<const Math::Matrix44*>( iter->second );
				SetParamMatrix( data->mHandle, data->mConstantType, value, data->mSize );
			}
			break;
		default:
			break;
		}
	}

	TextureList::iterator textureListIter = mTextureList.begin();
	for ( textureListIter; textureListIter != mTextureList.end(); ++textureListIter )
	{
		TextureData data = *textureListIter;
		std::map<int, const void*>::const_iterator iter = constantMap.find( data.mNameHash );
		Assert( iter != constantMap.end(), "Parameter not set." );

		switch ( data.mVariableType )
		{
		case VAR_TYPE_TEXTURE:
			{
				const Texture* texture = reinterpret_cast<const Texture*>( iter->second );

                SamplerState* samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerStateByHandle( effectPass->GetSamplerState( data.mNameHash ) );

				SetParamTexture( texture, data.mSamplerIndex, renderStateShadowing, samplerState, context );				
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void D3DShaderProgram::SetParamVector( 
	D3DXHANDLE handle, 
	ConstantType constantType,
	const Math::Vector4* value, 
	int numItems 
	)
{
    VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	D3DVertexShader* d3dVertexShader = reinterpret_cast<D3DVertexShader*>( vertexShader );
	D3DPixelShader* d3dPixelShader = reinterpret_cast<D3DPixelShader*>( pixelShader );

	if ( constantType == CONSTANT_TYPE_VERTEX )
	{
		d3dVertexShader->GetConstantTable()->SetVectorArray( 
			D3DRenderer::GetDevice(),
			handle, 
			(D3DXVECTOR4*) value,
			numItems
			);
	}
	else
	{
		d3dPixelShader->GetConstantTable()->SetVectorArray( 
			D3DRenderer::GetDevice(), 
			handle, 
			(D3DXVECTOR4*) value,
			numItems
			);
	} 
}

//===========================================================================

void D3DShaderProgram::SetParamMatrix( 
	D3DXHANDLE handle, 
	ConstantType constantType,
	const Math::Matrix44* matrix,
	int numItems
	)
{
	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	D3DVertexShader* d3dVertexShader = reinterpret_cast<D3DVertexShader*>( vertexShader );
	D3DPixelShader* d3dPixelShader = reinterpret_cast<D3DPixelShader*>( pixelShader );

	D3DXMATRIX d3dMat[16];
	for ( int i = 0; i < numItems; ++i )
	{
		D3DRenderer::ConvertMathMatrixToD3DMatrix( d3dMat[i], matrix[i] );
	}

	if ( constantType == CONSTANT_TYPE_VERTEX )
	{
		d3dVertexShader->GetConstantTable()->SetMatrixArray( 
			D3DRenderer::GetDevice(), 
			handle, 
			d3dMat,
			numItems
			);
	}
	else
	{
		d3dPixelShader->GetConstantTable()->SetMatrixArray( 
			D3DRenderer::GetDevice(), 
			handle, 
			d3dMat,
			numItems
			);
	} 
}

//===========================================================================

void D3DShaderProgram::SetParamTexture( 
	const Texture* texture, 	
	int samplerIndex,
	RenderStateShadowing* renderStateShadowing,
	SamplerState* state,
	void* context
	)
{
	if ( texture != NULL )
	{		
		const D3DTexture* d3dTexture = reinterpret_cast<const D3DTexture*>( texture );

		LPDIRECT3DTEXTURE9 texture2D = reinterpret_cast<LPDIRECT3DTEXTURE9>( d3dTexture->GetHandle() );

		D3DRenderer::GetDevice()->SetTexture( samplerIndex, texture2D );
		state->Dispatch( renderStateShadowing, context, samplerIndex );
	}
}

#endif