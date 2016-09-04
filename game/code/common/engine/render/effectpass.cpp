//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/blendstate.hpp"
#include "common/engine/render/depthstencilstate.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/pixelshader.hpp"
#include "common/engine/render/rasterizerstate.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

EffectPass::EffectPass() 
{    
}

//============================================================================

EffectPass::~EffectPass()
{
}

//============================================================================

void EffectPass::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	const std::map<int, const void*> &constantMap,
	int threadID,
	void* context
	)
{
	RenderStateManager* renderStateManager = GameApp::Get()->GetRenderStateManager();
	DepthStencilState* depthStencilState = renderStateManager->GetDepthStencilStateByHandle( mDepthStencilState );
	Assert( depthStencilState != NULL, "" );
	depthStencilState->Dispatch( renderStateShadowing, context );

	BlendState* blendState = renderStateManager->GetBlendStateByHandle( mBlendState );
	Assert( blendState != NULL, "" );
	blendState->Dispatch( renderStateShadowing, context );

	RasterizerState* rasterizerState = renderStateManager->GetRasterizerStateByHandle( mRasterizerState );
	Assert( rasterizerState != NULL, "" );
	rasterizerState->Dispatch( renderStateShadowing, context );
		
	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( mShaderProgramHandle );

	shaderProgram->DisableTextureSamplers( context );

	shaderProgram->SetProgram( this, renderStateShadowing, constantMap, threadID, context );	
}

//============================================================================

bool EffectPass::IsTranslucent() const
{
	RenderStateManager* renderStateManager = GameApp::Get()->GetRenderStateManager();
	BlendState* blendState = renderStateManager->GetBlendStateByHandle( mBlendState );
	Assert( blendState != NULL, "" );
	const BlendStateStruct* blendStateData = blendState->GetData();

	return blendStateData->mBlendEnable[0];
}

//============================================================================

void EffectPass::AddSamplerStatePair( int nameHash, ResourceHandle<SamplerState> state )
{
	mSamplerStateMap[nameHash] = state;
}

//============================================================================

void EffectPass::Reload()
{
	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( mShaderProgramHandle );
	shaderProgram->Reload();
}

//============================================================================

void EffectPass::SetName( const char* name )
{
	System::StringCopy( mName, 32, name );
}

//============================================================================

void EffectPass::SetVertexShaderName( const char* name )
{
	System::StringCopy( mVertexShaderName, 64, name );
}

//============================================================================

void EffectPass::SetPixelShaderName( const char* name )
{
	System::StringCopy( mPixelShaderName, 64, name );
}