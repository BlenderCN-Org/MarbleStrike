//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderer.hpp"

#if defined RENDER_PLAT_OPENGL
#include "common/engine/render/ogl/oglrenderer.hpp"
#endif

#if defined RENDER_PLAT_D3D9
#include "common/engine/render/d3d/d3drenderer.hpp"
#endif 

#if defined RENDER_PLAT_D3D11
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#endif 

#if defined RENDER_PLAT_OGLES1_1
#include "common/engine/render/ogles1.1/fixedoglrenderer.hpp"
#endif

#if defined RENDER_PLAT_OGLES2
#include "common/engine/render/ogles2/oglesrenderer.hpp"
#endif

#if defined PLAT_XBOX360
#include "xbox360/render/xbox360renderer.hpp"
#endif

#if defined PLAT_PS3
#include "ps3/render/ps3renderer.hpp"
#endif

#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

Renderer* Renderer::mInstance = NULL;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Renderer* Renderer::Create( RendererType type )
{
	if ( mInstance == NULL )
	{	
		if ( type == RENDERER_OPENGL )
		{
#if defined RENDER_PLAT_OPENGL
			mInstance = NEW_PTR( "OGL Instance" ) OGLRenderer;
#endif
		}
		else if ( type == RENDERER_D3D9 )
		{
#if defined RENDER_PLAT_D3D9
			mInstance = NEW_PTR( "D3D Instance" ) D3DRenderer;
#endif
		}
		else if ( type == RENDERER_D3D11 )
		{
#if defined RENDER_PLAT_D3D11
			mInstance = NEW_PTR( "D3D11 Instance" ) D3D11Renderer;
#endif
		}
		else if ( type == RENDERER_OPENGLES1_1 )
		{
#if defined RENDER_PLAT_OGLES1_1
			mInstance = NEW_PTR( "Fixed OGL Instance" ) FixedOGLRenderer;
#endif
		}
		else if ( type == RENDERER_OPENGLES2 )
		{
#if defined RENDER_PLAT_OGLES2
			mInstance = NEW_PTR( "OGL ES Instance" ) OGLESRenderer;
#endif
		}
		else if ( type == RENDERER_XBOX360 )
		{
#if defined PLAT_XBOX360
			mInstance = NEW_PTR( "XBox 360 Instance" ) Xbox360Renderer;
#endif
		}
		else if ( type == RENDERER_PS3 )
		{
#if defined PLAT_PS3
			mInstance = NEW_PTR( "PS3 Instance" ) PS3Renderer;
#endif
		}

		Assert( mInstance != NULL, "" );
	}

	return mInstance;
}


//===========================================================================

void Renderer::Destroy()
{
	if ( mInstance != NULL )
	{
		DELETE_PTR( mInstance );
	}
}

//===========================================================================

Renderer* Renderer::Get()
{
	Assert( mInstance != NULL, "Data not allocated." );
	return mInstance;
}

//===========================================================================

Renderer::Renderer()
:	mRenderFactory( NULL )
,	mNumTextureUnits( 0 )
{

}

//===========================================================================

Renderer::~Renderer()
{
}