#if defined( RENDER_PLAT_D3D9 )

#ifndef D3D_RENDERER_HPP
#define D3D_RENDERER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/d3d/d3drendertarget.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/assert.hpp"
#include <d3d9.h>
#include <d3dx9.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class SimpleMesh;
class VertexDeclaration;

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3DRenderer : public Renderer
{
public:

    D3DRenderer();
    ~D3DRenderer();

    void Initialize( HWND hWnd );
    void SwapFrameBuffers();
    void SetBackgroundColor( float red, float green, float blue, float alpha = 1.0f );	
	void SetRenderTarget( RenderTarget* renderTarget, void* context );	

    void ClearRenderer( void* context ) const;
	void ClearColor( void* context ) const;
	void ClearDepth( void* context ) const;
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
		D3DPERF_BeginEvent(0xFF00FFFF, result );
	}

	void CaptureMarkEnd()
	{
		D3DPERF_EndEvent();
	}

	int GetMaxSimultaneousRT()
	{
		return mCaps.NumSimultaneousRTs;
	}
		
	static void CreateDevice( HWND hWnd );
	static void DestroyDevice();
	static LPDIRECT3DDEVICE9 GetDevice()
	{
		Assert( mD3DDevice != NULL, "Data not allocated." );
		return mD3DDevice;
	}
	static LPD3DXEFFECT LoadEffect( const char* filename );
    static void ConvertD3DMatrixToMathMatrix( Math::Matrix44 &mat, D3DXMATRIX d3dMat );
	static void ConvertMathMatrixToD3DMatrix( D3DXMATRIX &d3dMat, Math::Matrix44 matrix );

	void OnResize();
	void SetViewport( int x, int y, int width, int height, void* context );

	void AddColorTarget( D3DTargetData data );
	void AddDepthTarget( D3DTargetData data );

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

private:
	    
    static LPDIRECT3DDEVICE9 mD3DDevice;    
    static LPDIRECT3DTEXTURE9 mTextureID;

	LPDIRECT3DSURFACE9 mColorSurface;
	LPDIRECT3DSURFACE9 mDepthSurface;

	D3DCAPS9 mCaps;

    int mRed;
    int mGreen;
    int mBlue;
	int mAlpha;
    	
	std::list<D3DTargetData> mColorTargetDataList;
	std::list<D3DTargetData> mDepthTargetDataList;
};

#endif

#endif