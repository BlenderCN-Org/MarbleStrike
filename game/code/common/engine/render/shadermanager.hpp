#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/hashtable.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/resourcehandle.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

const int MAX_SHADER_PROGRAMS = 30;

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class VertexShader;
class PixelShader;
class ShaderProgram;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ShaderManager
{
public:

	ShaderManager();
	~ShaderManager();

	ResourceHandle<VertexShader> GetVertexShader( const char* name );
	ResourceHandle<PixelShader> GetPixelShader( const char* name );
	ResourceHandle<ShaderProgram> GetShaderProgram( 
		ResourceHandle<VertexShader> vertexShader, 
		ResourceHandle<PixelShader> pixelShader
		);

	VertexShader* GetVertexShaderByHandle( const ResourceHandle<VertexShader> &handle );
	PixelShader* GetPixelShaderByHandle( const ResourceHandle<PixelShader> &handle );
	ShaderProgram* GetShaderProgramByHandle( const ResourceHandle<ShaderProgram> &handle );
	
private:

	HashTable< VertexShader* > mVertexShaders;
	HashTable< PixelShader* > mPixelShaders;
	ShaderProgram* mShaderPrograms[MAX_SHADER_PROGRAMS];
	int mNumShaderPrograms;
	int mUniqueShaderCount;
};

#endif
