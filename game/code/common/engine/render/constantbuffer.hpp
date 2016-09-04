#ifndef CONSTANT_BUFFER_HPP
#define CONSTANT_BUFFER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/resourcehandle.hpp"
#include <list>
#include <string>
#include <map>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// ENUMS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class ShaderParameterVector;
class ShaderParameterMatrix;
class ShaderParameterTexture;
class Texture;
class EffectPass;
class ShaderProgram;
class RenderStateShadowing;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ConstantBuffer
{
public:

	enum ConstantBufferType
	{
		CONSTANT_BUFFER_VERTEX,
		CONSTANT_BUFFER_PIXEL
	};

	typedef std::map<int, ShaderParameterVector*> ShaderParameterVectorMap;
	typedef std::map<int, ShaderParameterMatrix*> ShaderParameterMatrixMap;
	typedef std::map<int, ShaderParameterTexture*> ShaderParameterTextureMap;

public:
	ConstantBuffer( ConstantBufferType type );
	virtual ~ConstantBuffer();

	virtual void SetConstantBufferData( void* data, int size );

	virtual void SetVectorParameter( int nameHash, Math::Vector4* data, int numItems = 1 );
	virtual void SetMatrixParameter( int nameHash, Math::Matrix44* data, int index = 0 );	
	virtual void SetTextureParameter( int nameHash, Texture** data = NULL );

	virtual void Dispatch( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		) = 0;

protected:

	void DispatchInternal( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		);
	
	ShaderParameterVector* GetParameterVector( int nameHash );
	ShaderParameterMatrix* GetParameterMatrix( int nameHash );
	ShaderParameterTexture* GetParameterTexture( int nameHash );

	void UpdateParametersVector( ResourceHandle<ShaderProgram> shaderProgram );
	void UpdateParametersMatrix( ResourceHandle<ShaderProgram> shaderProgram );
	void UpdateParametersTexture( ResourceHandle<ShaderProgram> shaderProgram );	
	virtual void UploadParameters( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		);

	ShaderParameterVectorMap mShaderParameterVectorMap;
	ShaderParameterMatrixMap mShaderParameterMatrixMap;
	ShaderParameterTextureMap mShaderParameterTextureMap;

	void* mConstantBufferData;
	int mConstantBufferDataSize;
	ResourceHandle<ShaderProgram> mShaderProgramHandle;
	ConstantBufferType mConstantBufferType;
	int mUniqueVectorID;
    int mUniqueMatrixID;
    int mUniqueTextureID;
};

#endif
