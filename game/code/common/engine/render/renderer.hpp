#ifndef RENDERER_HPP
#define RENDERER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/texture.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/plane.hpp"
#include "common/engine/math/rectangle.hpp"

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <windows.h>
#include <list>
#endif

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <agile.h>
#endif

#if defined PLAT_ANDROID
#include <android_native_app_glue.h>
#endif

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class RenderFactory;
class Mesh;
class EffectPass;
class SimpleMesh;
class VertexDeclaration;
class RenderTarget;

struct FontVertex;

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#define PIX_BEGIN(name) Renderer::Get()->CaptureMarkBegin( name );
#define PIX_END() Renderer::Get()->CaptureMarkEnd();

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum RendererType
{
	RENDERER_OPENGL,
	RENDERER_D3D9,
	RENDERER_D3D11,
	RENDERER_OPENGLES1_1,
	RENDERER_OPENGLES2,
	RENDERER_XBOX360,
	RENDERER_PS3
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Renderer
{
public:

    static Renderer* Create( RendererType type );

    static void Destroy();
    static Renderer* Get();

	RenderFactory* GetRenderFactory()
	{
		Assert( mRenderFactory != NULL, "" );
		return mRenderFactory;
	}

#if defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
	virtual void Initialize( Platform::Agile<Windows::UI::Core::CoreWindow> window ) = 0;
#endif

#if defined( PLAT_PC )
    virtual void Initialize( HWND hWnd ) = 0;
#endif

#if defined( PLAT_XBOX360 ) || defined( PLAT_PS3 ) || defined( PLAT_IOS )
	virtual void Initialize() = 0;
#endif

#if defined( PLAT_ANDROID )
	virtual void Initialize( ANativeWindow* window ) = 0;
	virtual void DeInitialize() = 0;
	virtual void ReInitialize( ANativeWindow* window ) = 0;
#endif
	
    virtual void SwapFrameBuffers() = 0;
    virtual void SetBackgroundColor( float red, float green, float blue, float alpha = 1.0f ) = 0;

	virtual void RenderBuffers( 
		Mesh* mesh, 
		VertexDeclaration* vertexDeclaration,
		EffectPass* effectPass,
		void* context
		) = 0;
	virtual void Render( 
		SimpleMesh* simpleMesh, 
		VertexDeclaration* vertexDeclaration,
		EffectPass* effectPass,
		int threadID,
		void* context
		) = 0;

	virtual void SetRenderTarget( RenderTarget* renderTarget, void* context ) = 0;		

    virtual void ClearRenderer( void* context ) const = 0;	
	virtual void ClearColor( void* context ) const = 0;	
	virtual void ClearDepth( void* context ) const = 0;
	
	virtual void SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context ) = 0;

	virtual void ModifyUV( float& /*u*/, float& /*v*/ )
	{
		
	}

	virtual void CaptureMarkBegin( const char* name ) = 0;
	virtual void CaptureMarkEnd() = 0;
	
	int GetNumTextureUnits()
	{
		return mNumTextureUnits;
	}

	virtual void SetViewport( int x, int y, int width, int height, void* context ) = 0;

	void SetCurrentScissorRectangle( const Math::Rectangle<int> &rect )
	{
		mScissorRectangle = rect;
	}

	Math::Rectangle<int>& GetCurrentScissorRectangle()
	{
		return mScissorRectangle;
	}

	virtual Math::Matrix44 GetPerspectiveProjection(
		float radians, 
		float aspect, 
		float zNear, 
		float zFar
		) = 0;

	virtual Math::Matrix44 GetOrthoProjection(
		float left, 
		float bottom, 
		float right, 
		float top, 
		float znear, 
		float zfar 
		) = 0;

    virtual void SetMainContext()
    {
    }

    virtual void SetThreadContext()
    {
    }

    virtual void FlushContext()
    {
    }

protected:

    Renderer();			
    virtual ~Renderer();

	RenderFactory* mRenderFactory;

	int mNumTextureUnits;

	Math::Rectangle<int> mScissorRectangle;

private:

    static Renderer* mInstance;
};

#endif
