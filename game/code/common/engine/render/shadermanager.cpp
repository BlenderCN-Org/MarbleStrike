//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/vertexshader.hpp"
#include "common/engine/render/pixelshader.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/pathutilities.hpp"

//////////////////////////////////////////////////////
// GLOBAL
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ShaderManager::ShaderManager()
{
	mUniqueShaderCount = 0;
	mNumShaderPrograms = 0;
}

//============================================================================

ShaderManager::~ShaderManager()
{
	for ( int i = 0; i < mNumShaderPrograms; ++i )
	{
		ShaderProgram* shaderProgram = mShaderPrograms[i];		
		shaderProgram->Destroy();
		DELETE_PTR( shaderProgram );
	}

	List< VertexShader* > itemVertexShaderList = mVertexShaders.GetValueSet();
	List< VertexShader* >::Iterator itemVertexShaderIter = itemVertexShaderList.CreateIterator();	
	while ( itemVertexShaderIter.HasNext() )
	{	
		VertexShader* data = itemVertexShaderIter.Next();		
		Assert( data != NULL, "" );
		DELETE_PTR( data );
	}

	List< PixelShader* > itemPixelShaderList = mPixelShaders.GetValueSet();
	List< PixelShader* >::Iterator itemPixelShaderIter = itemPixelShaderList.CreateIterator();	
	while ( itemPixelShaderIter.HasNext() )
	{	
		PixelShader* data = itemPixelShaderIter.Next();
		Assert( data != NULL, "" );
		DELETE_PTR( data );
	}
}

//===========================================================================

ResourceHandle<VertexShader> ShaderManager::GetVertexShader( const char* name )
{
	unsigned int hash = System::StringHash( name );	
	
	ResourceHandle<VertexShader> resource;
	resource.SetIndex( hash );

	VertexShader* vertexShader = NULL;
	bool isFound = mVertexShaders.GetItem( hash, &vertexShader );
	
	if ( isFound == false )
	{
		VertexShader* vertexShader = Renderer::Get()->GetRenderFactory()->CreateVertexShader();
		System::FixedString<256> loadPath = System::Path::SystemSlash( "shaders\\" );		
		vertexShader->Load( loadPath, name );
		mVertexShaders.Insert( hash, vertexShader );
	}

	return resource;
}

//===========================================================================

ResourceHandle<PixelShader> ShaderManager::GetPixelShader( const char* name )
{
	unsigned int hash = System::StringHash( name );
	
	ResourceHandle<PixelShader> resource;
	resource.SetIndex( hash );

	PixelShader* pixelShader = NULL;
	bool isFound = mPixelShaders.GetItem( hash, &pixelShader );
	
	if ( isFound == false )
	{
		pixelShader = Renderer::Get()->GetRenderFactory()->CreatePixelShader();
		System::FixedString<256> loadPath = System::Path::SystemSlash( "shaders\\" );
		pixelShader->Load( loadPath, name );
		mPixelShaders.Insert( hash, pixelShader );
	}

	return resource;
}

//===========================================================================

ResourceHandle<ShaderProgram> ShaderManager::GetShaderProgram( 
	ResourceHandle<VertexShader> vertexShader, 
	ResourceHandle<PixelShader> pixelShader
	)
{
	ResourceHandle<ShaderProgram> resource;
	ShaderProgram* shaderProgram = NULL;

	bool isFound = false;
	for ( int i = 0; i < mNumShaderPrograms; ++i )
	{
		shaderProgram = mShaderPrograms[i];
		if ( shaderProgram->GetVertexShader() == vertexShader && shaderProgram->GetPixelShader() == pixelShader )
		{
			resource.SetIndex( i );
			isFound = true;
			break;
		}
	}
	
	if ( isFound == false )
	{		
		shaderProgram = Renderer::Get()->GetRenderFactory()->CreateShaderProgram( mUniqueShaderCount );
		shaderProgram->SetVertexShader( vertexShader );
		shaderProgram->SetPixelShader( pixelShader );
		shaderProgram->Create();

		mUniqueShaderCount++;
		
		resource.SetIndex( mNumShaderPrograms );
		mShaderPrograms[mNumShaderPrograms] = shaderProgram;
		mNumShaderPrograms++;
		Assert( mNumShaderPrograms < MAX_SHADER_PROGRAMS, "" );
	}

	return resource;
}

//===========================================================================

VertexShader* ShaderManager::GetVertexShaderByHandle( const ResourceHandle<VertexShader> &handle )
{
	VertexShader* shader = mVertexShaders.Get( handle.GetIndex() );
	return shader;
}

//===========================================================================

PixelShader* ShaderManager::GetPixelShaderByHandle( const ResourceHandle<PixelShader> &handle )
{
	PixelShader* shader = mPixelShaders.Get( handle.GetIndex() );
	return shader;
}

//===========================================================================

ShaderProgram* ShaderManager::GetShaderProgramByHandle( const ResourceHandle<ShaderProgram> &handle )
{
	ShaderProgram* shaderProgram = mShaderPrograms[handle.GetIndex()];
	return shaderProgram;
}