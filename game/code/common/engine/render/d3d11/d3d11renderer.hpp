#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_RENDERER_HPP
#define D3D11_RENDERER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/d3d11/d3d11rendertarget.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/assert.hpp"
#include <list>

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <d3d11_1.h>
#include <DXGI1_2.h>
#endif

#if defined PLAT_WINDOWS_8
#include <agile.h>
#endif

#if defined PLAT_PC
#include <d3d11.h>
#endif

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#define RELEASE_D3D_OBJECT( object ) \
	if ( object )\
{\
	object->Release();\
	object = NULL;\
}

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class SimpleMesh;
class VertexDeclaration;

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

#define SINGLE_THREAD_RENDER
//#define MULTI_THREAD_RENDER 

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11Renderer : public Renderer
{
public:

	D3D11Renderer();
	~D3D11Renderer();

	void ClearD3D11DeviceContext( ID3D11DeviceContext* pd3dDeviceContext );

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	void Initialize( Platform::Agile<Windows::UI::Core::CoreWindow> window );
#endif

#if defined PLAT_PC
	void Initialize( HWND hWnd );
#endif
	void SwapFrameBuffers();
	void SetBackgroundColor( float red, float green, float blue, float alpha = 1.0f );	

	void RenderBuffers( 
		Mesh* mesh, 
		VertexDeclaration* vertexDeclaration, 
		EffectPass* effectPass,
		void* context
		);
	void Render( 
		SimpleMesh* simpleMesh, 
		VertexDeclaration* vertexDeclaration, 
		EffectPass* effectPass,
		int threadID,
		void* context
		);

	void SetRenderTarget( RenderTarget* renderTarget, void* context );
	void ClearRenderer( void* context ) const;
	void ClearColor( void* context ) const;
	void ClearDepth( void* context ) const;

	void SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context );
	
	void ModifyUV( float &u, float &v )
	{
		UNUSED_ALWAYS( u );
		v = 1.0f - v;
	}

	void CaptureMarkBegin( const char* name )
	{
		wchar_t result[1024];
		result[0] = '\0';
		size_t returnValue;
		mbstowcs_s(&returnValue, result, 1024, name, strlen(name));
		//D3DPERF_BeginEvent(0xFF00FFFF, result );
	}

	void CaptureMarkEnd()
	{
		//D3DPERF_EndEvent();
	}

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	static void CreateDevice( Platform::Agile<Windows::UI::Core::CoreWindow> window );	
#endif

#if defined PLAT_PC
	static void CreateDevice( HWND hWnd );	
#endif
	
	static ID3D11Device* GetDevice()
	{
		Assert( mD3DDevice != NULL, "Data not allocated." );
		return mD3DDevice;
	}

	static ID3D11DeviceContext* GetImmediateDeviceContext()
	{
		Assert( mImmediateContext != NULL, "Data not allocated." );
		return mImmediateContext;
	}

#if defined _DEBUG
    static ID3D11Debug* GetDebug()
    {
        Assert( mD3DDebug != NULL, "" );
        return mD3DDebug;
    }
#endif

	void SetViewport( int x, int y, int width, int height, void* context );

	void AddColorTarget( D3D11ColorTargetData data );
	void AddDepthTarget( D3D11DepthTargetData data );

	Math::Matrix44 GetPerspectiveProjection(
		float radians, 
		float aspect, 
		float zNear, 
		float zFar
		);

	Math::Matrix44 GetOrthoProjection(
		float left, 
		float bottom, 
		float right, 
		float top, 
		float znear, 
		float zfar 
		);

	int GetNumThreads();

private:

#if defined PLAT_PC
	static IDXGISwapChain* mSwapChain;
#endif

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	static IDXGISwapChain1* mSwapChain;
#endif

	static ID3D11Device* mD3DDevice;	
	static ID3D11DeviceContext* mImmediateContext;

	ID3D11RenderTargetView* mRenderTargetView;	
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11Texture2D* mDepthStencilTexture;	
	
	float mClearColor[4];

	ID3D11Buffer** mVertexBuffer;
	int* mOffsetVertexBuffer;
	D3D11_MAP* mVertexBufferWriteMode;
	int mNumThreads;

	std::list<D3D11ColorTargetData> mColorTargetDataList;
	std::list<D3D11DepthTargetData> mDepthTargetDataList;

	D3D11RenderTarget* mCurrentRenderTarget;

#if defined _DEBUG
    static ID3D11Debug* mD3DDebug;
#endif
};

#endif

#endif