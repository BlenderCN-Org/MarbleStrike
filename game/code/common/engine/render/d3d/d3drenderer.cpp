#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3drenderfactory.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

LPDIRECT3DDEVICE9 D3DRenderer::mD3DDevice = NULL;
LPDIRECT3DTEXTURE9 D3DRenderer::mTextureID = NULL;

namespace D3D
{
	static D3DPRIMITIVETYPE PrimitiveTypeD3D[PRIMITIVE_TYPE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) D3DTYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};	
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DRenderer::D3DRenderer()
:   mRed( 0 )
,   mGreen( 0 )
,   mBlue( 0 )
,	mColorSurface( NULL )
,	mDepthSurface( NULL )
{
}

//===========================================================================

D3DRenderer::~D3DRenderer()
{    
	if ( mColorSurface != NULL )
	{
		mColorSurface->Release();
		mColorSurface = NULL;
	}

	if ( mDepthSurface != NULL )
	{
		mDepthSurface->Release();
		mDepthSurface = NULL;
	}

	std::list<D3DTargetData>::iterator i;
	for ( i = mColorTargetDataList.begin(); i != mColorTargetDataList.end(); ++i )
	{
		D3DTargetData data = *i;

		DELETE_PTR( data.mTexture );

		if ( data.mRenderSurface > 0 )
		{
			data.mRenderSurface->Release();
		}
	}

	for ( i = mDepthTargetDataList.begin(); i != mDepthTargetDataList.end(); ++i )
	{
		D3DTargetData data = *i;

		DELETE_PTR( data.mTexture );

		if ( data.mRenderSurface > 0 )
		{
			data.mRenderSurface->Release();
		}
	}

	DELETE_PTR( mRenderFactory );

	D3DRenderer::DestroyDevice();
}

//===========================================================================

void D3DRenderer::Initialize( HWND hWnd )
{
	D3DRenderer::CreateDevice( hWnd );

	D3DRenderer::GetDevice()->GetDeviceCaps( &mCaps );

	mRenderFactory = NEW_PTR( "D3D Render Factory" ) D3DRenderFactory;

	D3DRenderer::GetDevice()->GetRenderTarget( 0, &mColorSurface );
	D3DRenderer::GetDevice()->GetDepthStencilSurface( &mDepthSurface );

	mNumTextureUnits = mCaps.MaxTextureBlendStages;	

	D3DRenderer::GetDevice()->SetRenderState( D3DRS_POINTSIZE, 1 );
}

//===========================================================================

void D3DRenderer::SwapFrameBuffers()
{
	mD3DDevice->Present( NULL, NULL, NULL, NULL );
}

//===========================================================================

void D3DRenderer::SetBackgroundColor( float red, float green, float blue, float alpha )
{
	mRed = static_cast<int>( red * 255.0f );
	mGreen = static_cast<int>( green * 255.0f );
	mBlue = static_cast<int>( blue * 255.0f );
	mAlpha = static_cast<int>( alpha *255.0f );
}

//===========================================================================

void D3DRenderer::SetRenderTarget( RenderTarget* renderTarget, void* context )
{
	if ( renderTarget == NULL )
	{
		D3DRenderer::GetDevice()->SetRenderTarget( 0, mColorSurface );
		D3DRenderer::GetDevice()->SetDepthStencilSurface( mDepthSurface );

		SetViewport( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), context );
		SetScissorRectangle( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), renderTarget, context );
	}
	else
	{
		D3DRenderTarget* d3dRenderTarget = reinterpret_cast<D3DRenderTarget*>( renderTarget );
		D3DRenderer::GetDevice()->SetRenderTarget( 0, d3dRenderTarget->GetColorRenderSurface() );		
		D3DRenderer::GetDevice()->SetDepthStencilSurface( d3dRenderTarget->GetDepthRenderSurface() );	

		SetViewport( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), context );	
		SetScissorRectangle( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), renderTarget, context );
	}
}

//===========================================================================

void D3DRenderer::ClearRenderer( void* context ) const
{    
	UNUSED_ALWAYS( context );

	D3DRenderer::GetDevice()->Clear( 
		0, 
		NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DCOLOR_RGBA( mRed, mGreen, mBlue, mAlpha ), 
		1.0f, 
		0 );
}

//===========================================================================

void D3DRenderer::ClearColor( void* context ) const
{
	UNUSED_ALWAYS( context );

	D3DRenderer::GetDevice()->Clear( 
		0, 
		NULL, 
		D3DCLEAR_TARGET, 
		D3DCOLOR_RGBA( mRed, mGreen, mBlue, mAlpha ), 
		1.0f, 
		0 );
}

//===========================================================================

void D3DRenderer::ClearDepth( void* context ) const
{    
	UNUSED_ALWAYS( context );

	D3DRenderer::GetDevice()->Clear(
		0, 
		NULL, 
		D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 
		D3DCOLOR_RGBA( 0, 0, 0, 0 ), 
		1.0f, 
		0 );
}

//===========================================================================

void D3DRenderer::RenderBuffers( 
								Mesh* mesh, 
								VertexDeclaration* vertexDeclaration, 
								EffectPass* effectPass,
								void* context
								)
{
	VertexBuffer* vertexBufferFound = NULL;
	for ( int i = 0; i < mesh->GetNumVertexBuffers(); ++i )
	{
		VertexBuffer* vertexBuffer = mesh->GetVertexBuffer( i );		

		if ( vertexDeclaration == vertexBuffer->GetVertexDeclaration() )
		{
			vertexBufferFound = vertexBuffer;
			break;
		}
	}

	if ( vertexBufferFound == NULL )
	{
		vertexBufferFound = mesh->CreateVertexBuffer();
		vertexBufferFound->Create( mesh, vertexDeclaration );
	}

	vertexDeclaration->Dispatch( context );
	vertexBufferFound->Render( vertexDeclaration, effectPass, context );
}

//===========================================================================

void D3DRenderer::Render( 
						 SimpleMesh* simpleMesh, 
						 VertexDeclaration* vertexDeclaration, 
						 EffectPass* effectPass,
						 int threadID,
						 void* context
						 )
{
	UNUSED_ALWAYS( effectPass );
	UNUSED_ALWAYS( threadID );

	vertexDeclaration->Dispatch( context );

	D3DRenderer::GetDevice()->BeginScene();

	D3DRenderer::GetDevice()->DrawPrimitiveUP( 
		D3D::PrimitiveTypeD3D[simpleMesh->mPrimitiveType], 
		simpleMesh->mNumPrimitives,		
		simpleMesh->mVertexData,
		vertexDeclaration->GetStreamSize( 0 ) 
		);

	D3DRenderer::GetDevice()->EndScene();
}

//===========================================================================

void D3DRenderer::SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context )
{
	UNUSED_ALWAYS( context );

	int screenHeight = Database::Get()->GetBackBufferHeight();
	if ( renderTarget )
	{
		screenHeight = renderTarget->GetHeight();
	}

	RECT rect;
	rect.left = x;
	rect.right = x + width;
	rect.bottom = screenHeight - y;
	rect.top = screenHeight - ( y + height );

	D3DRenderer::GetDevice()->SetScissorRect( &rect );
}

//===========================================================================

void D3DRenderer::CreateDevice( HWND hWnd )
{
	if ( mD3DDevice == NULL )
	{
		LPDIRECT3D9 direct3D;
		if ( NULL == ( direct3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		{
			Assert( false, "Cannot Create Direct 3D" );
		}

		D3DPRESENT_PARAMETERS d3dpp; 
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.BackBufferWidth = Database::Get()->GetBackBufferWidth();
		d3dpp.BackBufferHeight = Database::Get()->GetBackBufferHeight();
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;    
		d3dpp.BackBufferCount = 1;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		if ( FAILED( direct3D->CreateDevice( 
			D3DADAPTER_DEFAULT, 
			D3DDEVTYPE_HAL, 
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED ,
			&d3dpp, &mD3DDevice 
			) 
			) 
			)
		{
			Assert( false, "Cannot Create Direct 3D Device" );
		}		

		direct3D->Release();
		direct3D = NULL;
	}
}

//===========================================================================

void D3DRenderer::DestroyDevice()
{
	if ( mD3DDevice != NULL )
	{
		mD3DDevice->Release();
		mD3DDevice = NULL;
	}
}

//===========================================================================

LPD3DXEFFECT D3DRenderer::LoadEffect( const char* filename )
{
    wchar_t filenameWCHAR[256];
    System::ConvertToWChar( filename, filenameWCHAR, 256 );

	LPD3DXEFFECT effect;
	LPD3DXBUFFER compilationErrors;
	HRESULT hr = D3DXCreateEffectFromFile(
		mD3DDevice,
		filenameWCHAR, 
		NULL, 
		NULL,	
		0, 
		NULL, 
		&effect, 
		&compilationErrors );

	if ( hr != S_OK )
	{
		System::PrintToConsole( "EFFECT COMPILING ERROR:\n %s\n", compilationErrors->GetBufferPointer() );
		return NULL;
	}

	if ( compilationErrors != NULL )
	{
		compilationErrors->Release();
	}

	return effect;
}

//===========================================================================

void D3DRenderer::ConvertD3DMatrixToMathMatrix( Math::Matrix44 &mat, D3DXMATRIX d3dMat )
{    
	mat[0] = d3dMat.m[0][0];
	mat[1] = d3dMat.m[1][0];
	mat[2] = d3dMat.m[2][0];
	mat[3] = d3dMat.m[3][0];
	mat[4] = d3dMat.m[0][1];
	mat[5] = d3dMat.m[1][1];
	mat[6] = d3dMat.m[2][1];
	mat[7] = d3dMat.m[3][1];
	mat[8] = d3dMat.m[0][2];
	mat[9] = d3dMat.m[1][2];
	mat[10] = d3dMat.m[2][2];
	mat[11] = d3dMat.m[3][2];
	mat[12] = d3dMat.m[0][3];
	mat[13] = d3dMat.m[1][3];
	mat[14] = d3dMat.m[2][3];
	mat[15] = d3dMat.m[3][3];    
}

//===========================================================================

void D3DRenderer::ConvertMathMatrixToD3DMatrix( D3DXMATRIX &d3dMat, Math::Matrix44 matrix )
{
	d3dMat.m[0][0] = matrix[0];
	d3dMat.m[0][1] = matrix[1];
	d3dMat.m[0][2] = matrix[2];
	d3dMat.m[0][3] = matrix[3];
	d3dMat.m[1][0] = matrix[4];
	d3dMat.m[1][1] = matrix[5];
	d3dMat.m[1][2] = matrix[6];
	d3dMat.m[1][3] = matrix[7];
	d3dMat.m[2][0] = matrix[8];
	d3dMat.m[2][1] = matrix[9];
	d3dMat.m[2][2] = matrix[10];
	d3dMat.m[2][3] = matrix[11];
	d3dMat.m[3][0] = matrix[12];
	d3dMat.m[3][1] = matrix[13];
	d3dMat.m[3][2] = matrix[14];
	d3dMat.m[3][3] = matrix[15];
}

//===========================================================================

void D3DRenderer::OnResize()
{
	int width = Database::Get()->GetBackBufferWidth();
	int height = Database::Get()->GetBackBufferHeight();

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;    
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	mColorSurface->Release();
	mDepthSurface->Release();	

	mD3DDevice->Reset( &d3dpp );

	D3DRenderer::GetDevice()->GetRenderTarget( 0, &mColorSurface );
	D3DRenderer::GetDevice()->GetDepthStencilSurface( &mDepthSurface );
}

//===========================================================================

void D3DRenderer::SetViewport( int x, int y, int width, int height, void* context )
{
	UNUSED_ALWAYS( context );

	D3DVIEWPORT9 viewPort;
	viewPort.X = x;
	viewPort.Y = y;
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinZ = 0.0f;
	viewPort.MaxZ = 1.0f;
	D3DRenderer::GetDevice()->SetViewport( &viewPort );
}

//===========================================================================

void D3DRenderer::AddColorTarget( D3DTargetData data )
{
	mColorTargetDataList.push_back( data );
}

//===========================================================================

void D3DRenderer::AddDepthTarget( D3DTargetData data )
{
	mDepthTargetDataList.push_back( data );
}

//===========================================================================

Math::Matrix44 D3DRenderer::GetPerspectiveProjection(
	float radians, 
	float aspect, 
	float zNear, 
	float zFar
	)
{
	Math::Matrix44 data;
	float f = 1.0f / tan( radians/2.0f );
	data[0] = f;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[4] = 0;
	data[5] = f / aspect;
	data[6] = 0;
	data[7] = 0;

	data[8] = 0;
	data[9] = 0;
	data[10] = ( zFar ) / ( zNear - zFar);
	data[11] = -1;

	data[12] = 0;
	data[13] = 0;
	data[14] = zFar*zNear / ( zNear - zFar );
	data[15] = 0;

	return data;
}


//===========================================================================

Math::Matrix44 D3DRenderer::GetOrthoProjection(
	float left, 
	float bottom, 
	float right, 
	float top, 
	float znear, 
	float zfar 
	)
{
	Math::Matrix44 data;

	float tx = -( right + left ) / ( right - left );
	float ty = -( top + bottom ) / ( top - bottom );
	float tz = -( znear ) / ( zfar - znear );

	data[0] = 2.0f / ( right - left );
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[4] = 0;
	data[5] = 2.0f / ( top - bottom );
	data[6] = 0;
	data[7] = 0;

	data[8] = 0;
	data[9] = 0;
	data[10] = -1.0f / ( zfar - znear );
	data[11] = 0;

	data[12] = tx;
	data[13] = ty;
	data[14] = tz;
	data[15] = 1;

	return data;
}

#endif