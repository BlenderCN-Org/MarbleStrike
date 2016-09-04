#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGL_RENDERER_HPP
#define FIXED_OGL_RENDERER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderer.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "iphone/render/ogles1.1//fixedoglwrapper.hpp"
#include "iphone/render/ogles1.1/fixedoglrendertarget.hpp"
#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

#if defined PLAT_PC
class FixedOGLContext;
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class FixedOGLRenderer : public Renderer
{
public:

    FixedOGLRenderer();
    ~FixedOGLRenderer();

#if defined PLAT_PC
    void Initialize( HWND hWnd );
#endif
#if defined PLAT_IOS
	void Initialize();
#endif
    void SwapFrameBuffers();
    void SetBackgroundColor( float red, float green, float blue, float alpha = 1.0f );
	
	void SetMainRenderTarget( void* context );
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

	void SetScissorRectangle( int x, int y, int width, int height, void* context );

	void CaptureMarkBegin( const char* /*name*/ )
	{
    }

	void CaptureMarkEnd()
	{

	}
	
	void SetScissorRectangle( int x, int y, int width, int height, RenderTarget* renderTarget, void* context );
	void SetViewport( int x, int y, int width, int height, void* context );
	void AddColorTarget( FixedOGLTargetData data );
	void AddDepthTarget( FixedOGLTargetData data );
	
	void SetDefaultFramebuffer( int framebuffer )
	{
		mDefaultFramebuffer = framebuffer;
	}
    
	static void SetCurrentSamplerState( int index, const SamplerState* state )
	{
		mCurrentSamplerState[index] = state;
	}

	static const SamplerState* GetCurrentSamplerState( int index )
	{
		return mCurrentSamplerState[index];
	}	

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
	
#if defined PLAT_PC
    FixedOGLContext* mOGLContext;
#endif
	
#if defined PLAT_IOS
	RenderTarget* mColorRenderTarget;
	RenderTarget* mDepthRenderTarget;
#endif

    static unsigned int mTextureID;
	static const SamplerState* mCurrentSamplerState[2];
	unsigned int mFramebuffer;		
	unsigned int mDefaultFramebuffer;
	
	std::list<FixedOGLTargetData> mColorTargetDataList;
	std::list<FixedOGLTargetData> mDepthTargetDataList;
};

#endif

#endif