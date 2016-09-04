#ifndef SHADER_PARAMETER_HPP
#define SHADER_PARAMETER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/resourcehandle.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// ENUMS
//////////////////////////////////////////////////////

enum ShaderParameterType
{
 	PARAM_TYPE_FLOAT,
  	PARAM_TYPE_TEXTURE
};

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class ShaderProgram;
class EffectPass;
class RenderStateShadowing;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ShaderParameter
{
public:
	ShaderParameter( int id );
	virtual ~ShaderParameter();

	//void SetName( const char* name )
	//{
	//	System::StringCopy( mName, 64, name );
	//}

	//const char* GetName()
	//{
	//	return mName;
	//}
	
	int GetID()
	{
		return mID;
	}

	virtual void SetParameter( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		) = 0;
	virtual int UpdateParameter( ResourceHandle<ShaderProgram> shaderProgram ) = 0;
	virtual bool operator != ( const ShaderParameter &other ) = 0;

private:

	//char mName[64];
	int mID;
};
#endif
