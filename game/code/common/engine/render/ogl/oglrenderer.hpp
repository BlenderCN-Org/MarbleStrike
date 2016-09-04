#if defined( RENDER_PLAT_OPENGL )

#ifndef OGL_RENDERER_HPP
#define OGL_RENDERER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/ogl/oglrendertarget.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class OGLContext;
class SimpleMesh;
class VertexDeclaration;
class RenderTarget;

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLRenderer : public Renderer
{
public:

    OGLRenderer();
    ~OGLRenderer();
		
    void Initialize( HWND hWnd );
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

	void CaptureMarkBegin( const char* name )
	{
		UNUSED_ALWAYS( name );
	}

	void CaptureMarkEnd()
	{

	}

	int GetMaxColorAttachments()
	{
		return mMaxColorAttachments;
	}	

	void SetViewport( int x, int y, int width, int height, void* context );

	void AddColorTarget( OGLTargetData data );
	void AddDepthTarget( OGLTargetData data );

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

    void SetThreadContext();
    void FlushContext();

private:

    OGLContext* mOGLContextMain;
    OGLContext* mOGLContextThread;
	unsigned int mFrameBuffer;	
	int mMaxColorAttachments;

	std::list<OGLTargetData> mColorTargetDataList;
	std::list<OGLTargetData> mDepthTargetDataList;
};

#endif

#endif