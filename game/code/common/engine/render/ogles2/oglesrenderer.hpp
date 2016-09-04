#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLES_RENDERER_HPP
#define OGLES_RENDERER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/ogles2/oglesrendertarget.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/utilities.hpp"
#include <list>
#include <map>

#if defined PLAT_PC
#include "opengles2/include/gles2/gl2.h"
#endif

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
class OGLESContext;
#endif

class Font;
class SimpleMesh;
class VertexDeclaration;
class RenderTarget;
class SamplerState;

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLESRenderer : public Renderer
{    
public:

    OGLESRenderer();
    ~OGLESRenderer();

#if defined PLAT_PC
    void Initialize( HWND hWnd );
#endif
	
#if defined( PLAT_IOS )
	void Initialize();
#endif

#if defined( PLAT_ANDROID )
	void Initialize( ANativeWindow* window );
	void DeInitialize();
	void ReInitialize( ANativeWindow* window );
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
	
	void CaptureMarkBegin( const char* /*name*/ )
	{
	}

	void CaptureMarkEnd()
	{

	}
	
	void SetDefaultFramebuffer( int framebuffer )
	{
		mDefaultFramebuffer = framebuffer;
	}
    
	void SetViewport( int x, int y, int width, int height, void* context );
	void AddColorTarget( OGLESTargetData data );
	void AddDepthTarget( OGLESTargetData data );

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
    
    int GetCurrentSamplerUnit()
    {
        return mCurrentSamplerUnit;
    }
    
    void SetCurrentSamplerUnit( int i )
    {
        mCurrentSamplerUnit = i;
    }
    
    SamplerState* GetCurrentSamplerState( Texture* texture )
    {        
        std::map<Texture*, SamplerState*>::iterator iter = mCurrentTextureStateTable.find( texture );
        
        if ( iter != mCurrentTextureStateTable.end() )
        {            
            return mCurrentTextureStateTable[texture];
        }
        
        return NULL;    
    }
    
    void SetCurrentSamplerState( Texture* texture, SamplerState* state )
    {
        mCurrentTextureStateTable[texture] = state;
    }
    
    int GetCurrentBindBufferArrayIndex()
    {
        return mCurrentBindBufferArrayIndex;
    }
    
    void SetCurrentBindBufferArrayIndex( int index )
    {
        mCurrentBindBufferArrayIndex = index;
    }
    
    int GetCurrentBindBufferElementIndex()
    {
        return mCurrentBindBufferElementIndex;
    }
    
    void SetCurrentBindBufferElementIndex( int index )
    {
        mCurrentBindBufferElementIndex = index;
    }
    
    void SetMainContext();
    void SetThreadContext();
    void FlushContext();
    
private:
	   
#if defined( PLAT_PC ) || defined( PLAT_ANDROID )
    OGLESContext* mOGLESContext;
#endif

	unsigned int mFramebuffer;	
	unsigned int mDefaultFramebuffer;
    static unsigned int mTextureID;

	std::list<OGLESTargetData> mColorTargetDataList;
	std::list<OGLESTargetData> mDepthTargetDataList;
    
    int mCurrentSamplerUnit;
    std::map<Texture*, SamplerState*> mCurrentTextureStateTable;
    int mCurrentBindBufferArrayIndex;
    int mCurrentBindBufferElementIndex;
};

#endif

#endif