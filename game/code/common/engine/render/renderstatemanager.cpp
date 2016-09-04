//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/depthstencilstate.hpp"
#include "common/engine/render/blendstate.hpp"
#include "common/engine/render/rasterizerstate.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RenderStateManager::RenderStateManager()
{
}

//===========================================================================

RenderStateManager::~RenderStateManager()
{	
	List< DepthStencilState* > itemDepthStencilStateList = mDepthStencilStates.GetValueSet();
	List< DepthStencilState* >::Iterator itemDepthStencilStateIterator = itemDepthStencilStateList.CreateIterator();
	while ( itemDepthStencilStateIterator.HasNext() )
	{
		DepthStencilState* data = itemDepthStencilStateIterator.Next();		
		Assert( data != NULL, "" );		
		DELETE_PTR( data );
	}

	List< BlendState* > itemBlendStateList = mBlendStates.GetValueSet();
	List< BlendState* >::Iterator itemBlendStateIterator = itemBlendStateList.CreateIterator();
	while ( itemBlendStateIterator.HasNext() )
	{
		BlendState* data = itemBlendStateIterator.Next();		
		Assert( data != NULL, "" );
		DELETE_PTR( data );
	}

	List< RasterizerState* > itemRasterizerStateList = mRasterizerStates.GetValueSet();
	List< RasterizerState* >::Iterator itemRasterizerStateIterator = itemRasterizerStateList.CreateIterator();
	while( itemRasterizerStateIterator.HasNext() )
	{
		RasterizerState* data = itemRasterizerStateIterator.Next();		
		Assert( data != NULL, "" );
		DELETE_PTR( data );
	}

	List< SamplerState* > itemSamplerStateList = mSamplerStates.GetValueSet();
	List< SamplerState* >::Iterator itemSamplerStateIterator = itemSamplerStateList.CreateIterator();
	while ( itemSamplerStateIterator.HasNext() )
	{
		SamplerState* data = itemSamplerStateIterator.Next();		
		Assert( data != NULL, "" );
		DELETE_PTR( data );
	}
}

//===========================================================================

ResourceHandle<DepthStencilState> RenderStateManager::GetDepthStencilState( const char* name )
{
	unsigned int hash = System::StringHash( name );
	
	ResourceHandle<DepthStencilState> resource;
	resource.SetIndex( hash );
	
	DepthStencilState* depthStencilState = NULL;
	bool isFound = mDepthStencilStates.GetItem( hash, &depthStencilState );

	if ( isFound == false )
	{
		depthStencilState = Renderer::Get()->GetRenderFactory()->CreateDepthStencilState();
		System::FixedString<256> loadPath = System::Path::SystemSlash( "effects\\states\\depthstencil\\" );
		loadPath += name;
		loadPath += ".xml";
		depthStencilState->Load( loadPath );
		mDepthStencilStates.Insert( hash, depthStencilState );
	}

	return resource;
}

//===========================================================================

ResourceHandle<BlendState> RenderStateManager::GetBlendState( const char* name )
{
	unsigned int hash = System::StringHash( name );
	
	ResourceHandle<BlendState> resource;
	resource.SetIndex( hash );

	BlendState* blendState = NULL;
	bool isFound = mBlendStates.GetItem( hash, &blendState );
		
	if ( isFound == false )
	{
		blendState = Renderer::Get()->GetRenderFactory()->CreateBlendState();
		System::FixedString<256> loadPath = System::Path::SystemSlash( "effects\\states\\blend\\" );
		loadPath += name;
		loadPath += ".xml";
		blendState->Load( loadPath );				
		mBlendStates.Insert( hash, blendState );
	}
	
	return resource;
}

//===========================================================================

ResourceHandle<RasterizerState> RenderStateManager::GetRasterizerState( const char* name )
{
	unsigned int hash = System::StringHash( name );
	
	ResourceHandle<RasterizerState> resource;
	resource.SetIndex( hash );

	RasterizerState* rasterizerState = NULL;
	bool isFound = mRasterizerStates.GetItem( hash, &rasterizerState );
		
	if ( isFound == false )
	{
		rasterizerState = Renderer::Get()->GetRenderFactory()->CreateRasterizerState();
		System::FixedString<256> loadPath = System::Path::SystemSlash( "effects\\states\\rasterizer\\" );
		loadPath += name;
		loadPath += ".xml";
		rasterizerState->Load( loadPath );		
		mRasterizerStates.Insert( hash, rasterizerState );
	}

	return resource;
}

//===========================================================================

ResourceHandle<SamplerState> RenderStateManager::GetSamplerState( const char* name )
{
	unsigned int hash = System::StringHash( name );
	
	ResourceHandle<SamplerState> resource;
	resource.SetIndex( hash );

	SamplerState* samplerState = NULL;	
	bool isFound = mSamplerStates.GetItem( hash, &samplerState );
	
	if ( isFound == false )
	{
		samplerState = Renderer::Get()->GetRenderFactory()->CreateSamplerState();
		System::FixedString<256> loadPath = System::Path::SystemSlash( "effects\\states\\sampler\\" );
		loadPath += name;
		loadPath += ".xml";
		samplerState->Load( loadPath );		
		mSamplerStates.Insert( hash, samplerState );
	}

	return resource;
}

//===========================================================================

DepthStencilState* RenderStateManager::GetDepthStencilStateByHandle( ResourceHandle<DepthStencilState> handle )
{
	DepthStencilState* depthStencilState = mDepthStencilStates.Get( handle.GetIndex() );
	return depthStencilState;
}

//===========================================================================

BlendState* RenderStateManager::GetBlendStateByHandle( ResourceHandle<BlendState> handle )
{
	BlendState* blendState = mBlendStates.Get( handle.GetIndex() );
	return blendState;
}

//===========================================================================

RasterizerState* RenderStateManager::GetRasterizerStateByHandle( ResourceHandle<RasterizerState> handle )
{
	RasterizerState* rasterizerState = mRasterizerStates.Get( handle.GetIndex() );
	return rasterizerState;
}

//===========================================================================

SamplerState* RenderStateManager::GetSamplerStateByHandle( ResourceHandle<SamplerState> handle )
{	
	SamplerState* samplerState = mSamplerStates.Get( handle.GetIndex() );
	return samplerState;
}