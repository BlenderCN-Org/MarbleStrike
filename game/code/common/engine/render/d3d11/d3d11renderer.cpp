#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11renderfactory.hpp"
#include "common/engine/render/d3d11/d3d11renderqueue.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/simplemesh.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/taskmanager.hpp"

#if defined WINDOWS_PHONE
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
#endif

#if defined _DEBUG
#include <DXGIDebug.h>
#endif

#if defined PLAT_WINDOWS_8
#include <vsgcapture.h>
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

#if defined PLAT_PC
	IDXGISwapChain* D3D11Renderer::mSwapChain = NULL;
#endif

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	IDXGISwapChain1* D3D11Renderer::mSwapChain = NULL;
#endif

#if defined _DEBUG
    ID3D11Debug* D3D11Renderer::mD3DDebug = NULL;
#endif

ID3D11Device* D3D11Renderer::mD3DDevice = NULL;
ID3D11DeviceContext* D3D11Renderer::mImmediateContext = NULL;

namespace D3D11
{
	static D3D11_PRIMITIVE_TOPOLOGY PrimitiveTypeD3D11[VERTEX_ATTRIB_USAGE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) D3D11TYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};
}

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

const int MAX_VERTEX_BUFFER_MEMORY = 2 * 1024 * 1024;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11Renderer::D3D11Renderer()
{
	memset( mClearColor, 0, sizeof( float ) * 4 );
	mDepthStencilTexture = NULL;
	mRenderTargetView = NULL;
	mDepthStencilView = NULL;
	mVertexBuffer = NULL;	
	mCurrentRenderTarget = NULL;
	mVertexBuffer = 0;
	mOffsetVertexBuffer = 0;
	mNumThreads = 0;
}

//===========================================================================

D3D11Renderer::~D3D11Renderer()
{   
	std::list<D3D11ColorTargetData>::iterator colorIter;
	for ( colorIter = mColorTargetDataList.begin(); colorIter != mColorTargetDataList.end(); ++colorIter )
	{
		D3D11ColorTargetData data = *colorIter;

		DELETE_PTR( data.mTexture );
		RELEASE_D3D_OBJECT( data.mColorTargetView );
	}

	std::list<D3D11DepthTargetData>::iterator depthIter;
	for ( depthIter = mDepthTargetDataList.begin(); depthIter != mDepthTargetDataList.end(); ++depthIter )
	{
		D3D11DepthTargetData data = *depthIter;

		DELETE_PTR( data.mTexture );
		RELEASE_D3D_OBJECT( data.mDepthTargetView );
        RELEASE_D3D_OBJECT( data.mDepthTargetTexture );
	}

	RELEASE_D3D_OBJECT( mDepthStencilTexture );
	RELEASE_D3D_OBJECT( mRenderTargetView );
	RELEASE_D3D_OBJECT( mDepthStencilView );

	for ( int i = 0; i < mNumThreads; ++i )
	{
		RELEASE_D3D_OBJECT( mVertexBuffer[i] );
	}
	DELETE_PTR_ARRAY( mVertexBuffer );
	DELETE_PTR_ARRAY( mOffsetVertexBuffer );
	DELETE_PTR_ARRAY( mVertexBufferWriteMode );

	if ( mImmediateContext ) 
	{
		mImmediateContext->ClearState();
		mImmediateContext->Flush();
	}

	RELEASE_D3D_OBJECT( mSwapChain );
	RELEASE_D3D_OBJECT( mImmediateContext );
	RELEASE_D3D_OBJECT( mD3DDevice );

	DELETE_PTR( mRenderFactory );

#if defined _DEBUG
    RELEASE_D3D_OBJECT( mD3DDebug );
#endif
}

//===========================================================================

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
void D3D11Renderer::Initialize( Platform::Agile<Windows::UI::Core::CoreWindow> window )
#endif

#if defined PLAT_PC
	void D3D11Renderer::Initialize( HWND hWnd )
#endif
{
#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	D3D11Renderer::CreateDevice( window );
#endif

#if defined PLAT_PC
	D3D11Renderer::CreateDevice( hWnd );
#endif

	// Create a render target view
	ID3D11Texture2D* backBuffer = NULL;

	HRESULT hr;

    Assert( mSwapChain != NULL, "" );
	hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
	if ( FAILED( hr ) )
	{
		Assert( false, "" );
	}

	hr = mD3DDevice->CreateRenderTargetView( backBuffer, NULL, &mRenderTargetView );
	backBuffer->Release();
	Assert( hr == S_OK, "" );

	//
	// Create depth stencil texture
	//
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
#if defined PLAT_WINDOWS_PHONE
	descDepth.Width = Database::Get()->GetBackBufferHeight();
	descDepth.Height = Database::Get()->GetBackBufferWidth();
#else
	descDepth.Width = Database::Get()->GetBackBufferWidth();
	descDepth.Height = Database::Get()->GetBackBufferHeight();
#endif
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D( &descDepth, NULL, &mDepthStencilTexture );
	Assert( hr == S_OK, "" );

	//
	// Create the depth stencil view
	//
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView( mDepthStencilTexture, &descDSV, &mDepthStencilView );
	Assert( hr == S_OK, "" );

	mRenderFactory = NEW_PTR( "D3D11 Render Factory" ) D3D11RenderFactory;

#if defined PLAT_PC
	mNumThreads = GameApp::Get()->GetTaskManager()->GetNumThreads();
#else
	mNumThreads = 1;
#endif
	mVertexBuffer = NEW_PTR( "VertexBuffer" ) ID3D11Buffer*[mNumThreads];
	mOffsetVertexBuffer = NEW_PTR( "OffsetVertexBuffer" ) int[mNumThreads];
	mVertexBufferWriteMode = NEW_PTR( "WriteMode" ) D3D11_MAP[mNumThreads];

	for ( int i = 0; i < mNumThreads; ++i )
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		memset( &vertexBufferDesc, 0, sizeof( vertexBufferDesc ) );
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = MAX_VERTEX_BUFFER_MEMORY;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	
		hr = mD3DDevice->CreateBuffer( &vertexBufferDesc, NULL, &mVertexBuffer[i] );
		Assert( hr == S_OK, "" );

		mOffsetVertexBuffer[i] = 0;
		mVertexBufferWriteMode[i] = D3D11_MAP_WRITE_DISCARD;
	}

	mNumTextureUnits = 4096;
}

//===========================================================================

void D3D11Renderer::SwapFrameBuffers()
{	
#if defined PLAT_WINDOWS_8

    static int breakpoint = 0;
    static int count = 0;
    if ( breakpoint )
    {
        g_pVsgDbg->CaptureCurrentFrame();

        count++;
        if ( count > 2 )
        {
            count = 0;
            breakpoint = 0;
        }        
    }
#endif

	mSwapChain->Present( 1, 0 );
	
	for ( int i = 0; i < mNumThreads; ++i )
	{
		mOffsetVertexBuffer[i] = 0;
		mVertexBufferWriteMode[i] = D3D11_MAP_WRITE_DISCARD;
	}
}

//===========================================================================

void D3D11Renderer::SetBackgroundColor( float red, float green, float blue, float alpha )
{
	mClearColor[0] = red;
	mClearColor[1] = green;
	mClearColor[2] = blue;
	mClearColor[3] = alpha;
}

//===========================================================================

void D3D11Renderer::SetRenderTarget( RenderTarget* renderTarget, void* context )
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	if ( renderTarget == NULL )
	{
		deviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
		SetViewport( 0, 0, Database::Get()->GetBackBufferWidth(), Database::Get()->GetBackBufferHeight(), deviceContext );
		SetScissorRectangle( 
			0, 
			0, 
			Database::Get()->GetBackBufferWidth(), 
			Database::Get()->GetBackBufferHeight(), 
			renderTarget, 
			deviceContext 
			);
		
		mCurrentRenderTarget = NULL;
	}
	else
	{
		Assert( renderTarget != NULL, "" );
		D3D11RenderTarget* d3d11RenderTarget = reinterpret_cast<D3D11RenderTarget*>( renderTarget );

		ID3D11RenderTargetView* colorTargetview = d3d11RenderTarget->GetColorRenderTargetView();
		deviceContext->OMSetRenderTargets( 
			1,
			&( colorTargetview ),
			d3d11RenderTarget->GetDepthRenderTargetView() 
			);

		
		if ( Database::Get()->GetIsLandscape() )
		{
			SetViewport( 0, 0, renderTarget->GetHeight(), renderTarget->GetWidth(), context );
		}
		else
		{
			SetViewport( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), context );
		}
		SetScissorRectangle( 0, 0, renderTarget->GetWidth(), renderTarget->GetHeight(), renderTarget, deviceContext );
				
		mCurrentRenderTarget = d3d11RenderTarget;
	}
}

//===========================================================================

void D3D11Renderer::ClearRenderer( void* context ) const
{    
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	if ( mCurrentRenderTarget )
	{
		ID3D11RenderTargetView* colorTargetview = mCurrentRenderTarget->GetColorRenderTargetView();
		ID3D11DepthStencilView* depthTargetview = mCurrentRenderTarget->GetDepthRenderTargetView();
		deviceContext->ClearRenderTargetView( colorTargetview, mClearColor );
		deviceContext->ClearDepthStencilView( depthTargetview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
	}
	else
	{
		deviceContext->ClearRenderTargetView( mRenderTargetView, mClearColor );
		deviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
	}
}

//===========================================================================

void D3D11Renderer::ClearColor( void* context ) const
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	if ( mCurrentRenderTarget )
	{
		ID3D11RenderTargetView* colorTargetview = mCurrentRenderTarget->GetColorRenderTargetView();		
		deviceContext->ClearRenderTargetView( colorTargetview, mClearColor );		
	}
	else
	{
		deviceContext->ClearRenderTargetView( mRenderTargetView, mClearColor );		
	}
}

//===========================================================================

void D3D11Renderer::ClearDepth( void* context ) const
{    
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	if ( mCurrentRenderTarget )
	{		
		ID3D11DepthStencilView* depthTargetview = mCurrentRenderTarget->GetDepthRenderTargetView();	
		deviceContext->ClearDepthStencilView( depthTargetview, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
	else
	{		
		deviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
}

//===========================================================================

void D3D11Renderer::SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context )
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	int screenHeight = Database::Get()->GetBackBufferHeight();
	if ( renderTarget )
	{
		screenHeight = renderTarget->GetHeight();
	}

	D3D11_RECT rect;

	if ( Database::Get()->GetIsLandscape() )
	{
#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
		rect.left = y;
 		rect.right = y + height;
		rect.top = x;
		rect.bottom = x + width;
#endif
	}
	else
	{
		rect.left = x;
		rect.right = x + width;
		rect.bottom = screenHeight - y;
		rect.top = screenHeight - ( y + height );
	}

	int numRect = 1;

	deviceContext->RSSetScissorRects( numRect, &rect );
}

//===========================================================================

void D3D11Renderer::RenderBuffers( 
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

void D3D11Renderer::Render( 
	SimpleMesh* simpleMesh, 
	VertexDeclaration* vertexDeclaration, 
	EffectPass* effectPass,
	int threadID,
	void* context
	)
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	UNUSED_ALWAYS( effectPass );	

	int numVertices = simpleMesh->mNumPrimitives * simpleMesh->mVertexPerPrimitive;
	unsigned int stride = vertexDeclaration->GetStreamSize( 0 );
	unsigned int offset = mOffsetVertexBuffer[threadID];

	D3D11_MAPPED_SUBRESOURCE mappedResource;	
	deviceContext->Map( mVertexBuffer[threadID], 0, mVertexBufferWriteMode[threadID], 0, &mappedResource );
	unsigned char* data = reinterpret_cast<unsigned char*>( mappedResource.pData ) + mOffsetVertexBuffer[threadID];
	memcpy( (void*)data, simpleMesh->mVertexData, numVertices * stride );
	deviceContext->Unmap( mVertexBuffer[threadID], 0 );
	mOffsetVertexBuffer[threadID] += numVertices * stride;
	Assert( mOffsetVertexBuffer[threadID] < MAX_VERTEX_BUFFER_MEMORY, "" );

	vertexDeclaration->Dispatch( context );

	deviceContext->IASetVertexBuffers( 0, 1, &mVertexBuffer[threadID], &stride, &offset );
	deviceContext->IASetPrimitiveTopology( D3D11::PrimitiveTypeD3D11[simpleMesh->mPrimitiveType] );
	deviceContext->Draw( numVertices, 0 );

	mVertexBufferWriteMode[threadID] = D3D11_MAP_WRITE_NO_OVERWRITE;
}

//===========================================================================

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
void D3D11Renderer::CreateDevice( Platform::Agile<Windows::UI::Core::CoreWindow> window )
{
    HRESULT hr = S_OK;
	// This flag adds support for surfaces with a different color channel ordering than the API default.
	// It is recommended usage, and is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers with this flag.
	// WP8: Disabled until WP8 131955 is fixed.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description.  All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// Create the DX11 API device object, and get a corresponding context.
	D3D_FEATURE_LEVEL featureLevel;

	D3D11CreateDevice(
		nullptr,                    // specify null to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,                    // leave as nullptr unless software device
		creationFlags,              // optionally set debug and Direct2D compatibility flags
		featureLevels,              // list of feature levels this app can support
        ARRAYSIZE(featureLevels),   // number of entries in above list
        D3D11_SDK_VERSION,          // always set this to D3D11_SDK_VERSION
        &mD3DDevice,                    // returns the Direct3D device created
        &featureLevel,            // returns feature level of device created
        &mImmediateContext                    // returns the device immediate context
        );

    D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT d3d9ShadowSupportResults;
    ZeroMemory(&d3d9ShadowSupportResults, sizeof(D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT)); 

    mD3DDevice->CheckFeatureSupport(
        D3D11_FEATURE_D3D9_SHADOW_SUPPORT, 
        &d3d9ShadowSupportResults,
        sizeof(D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT)
        );

	// Create a descriptor for the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
    swapChainDesc.Width = 0;
    swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; 
	swapChainDesc.Stereo = false; 
	swapChainDesc.SampleDesc.Count = 1;        
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	             	      
#if defined PLAT_WINDOWS_PHONE
    swapChainDesc.BufferCount = 1;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
#endif
#if defined PLAT_WINDOWS_8
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    
#endif
	swapChainDesc.Flags = 0;
		
	IDXGIDevice1* dxgIDevice;
	hr = mD3DDevice->QueryInterface(__uuidof(IDXGIDevice2), (void **)&dxgIDevice );	
    Assert( hr == S_OK, "" );
	
	IDXGIAdapter* dxgIAdapter;
	hr = dxgIDevice->GetParent( __uuidof( IDXGIAdapter), (void **)&dxgIAdapter );
    Assert( hr == S_OK, "" );

	IDXGIFactory2* dxgIFactory;	
	hr = dxgIAdapter->GetParent( __uuidof(IDXGIFactory2), (void **)&dxgIFactory );
    Assert( hr == S_OK, "" );

	Windows::UI::Core::CoreWindow^ testWindow = window.Get();
	// Create a swap chain for this window from the DXGI factory.
	hr = dxgIFactory->CreateSwapChainForCoreWindow(
		mD3DDevice,
		reinterpret_cast<IUnknown*>( testWindow ),
		&swapChainDesc,
		nullptr,    // allow on all displays
		&mSwapChain	
		);

    Assert( hr == S_OK, "" );
    Assert( mSwapChain != NULL, "" );

	// Ensure that DXGI does not queue more than one frame at a time. This both reduces 
	// latency and ensures that the application will only render after each VSync, minimizing 
	// power consumption.
	dxgIDevice->SetMaximumFrameLatency(1);
}
#endif

#if defined PLAT_PC
void D3D11Renderer::CreateDevice( HWND hWnd )
{
    HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Database::Get()->GetBackBufferWidth();
	sd.BufferDesc.Height = Database::Get()->GetBackBufferHeight();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL inFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_10_1,
		//D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	UINT numFeatureLevels = ARRAYSIZE( inFeatureLevels );

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];		
		hr = D3D11CreateDeviceAndSwapChain( 
			NULL, 
			driverType, 
			NULL, 
			createDeviceFlags, 
			inFeatureLevels, 
			numFeatureLevels,
			D3D11_SDK_VERSION, 
			&sd, 
			&mSwapChain, 
			&mD3DDevice, 
			&featureLevel, 
			&mImmediateContext 
			);

		if ( SUCCEEDED( hr ) )
		{
			break;
		}
	}
    Assert( SUCCEEDED( hr ), "" );

#if defined _DEBUG
    hr = mD3DDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)(&mD3DDebug)); 
    Assert( SUCCEEDED( hr ), "" );
#endif
}
#endif

//===========================================================================

void D3D11Renderer::SetViewport( int x, int y, int width, int height, void* context )
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	D3D11_VIEWPORT vp;
	
	if ( Database::Get()->GetIsLandscape() )
	{
		vp.Width = static_cast<float>( height );
		vp.Height = static_cast<float>( width );
	}
	else
	{
		vp.Width = static_cast<float>( width );
		vp.Height = static_cast<float>( height );
	}
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = static_cast<float>( x );
	vp.TopLeftY = static_cast<float>( y );
	deviceContext->RSSetViewports( 1, &vp );
}

//===========================================================================

void D3D11Renderer::AddColorTarget( D3D11ColorTargetData data )
{
	mColorTargetDataList.push_back( data );
}

//===========================================================================

void D3D11Renderer::AddDepthTarget( D3D11DepthTargetData data )
{
	mDepthTargetDataList.push_back( data );
}

//===========================================================================

Math::Matrix44 D3D11Renderer::GetPerspectiveProjection(
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

Math::Matrix44 D3D11Renderer::GetOrthoProjection(
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

//============================================================================

int D3D11Renderer::GetNumThreads()
{
#if defined( SINGLE_THREAD_RENDER )
	return 1;
#elif defined( MULTI_THREAD_RENDER )
	return mNumThreads;
#endif
}

#endif