//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderobjectdata.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RenderObjectDataType RenderObjectData::GetRenderObjectDataType( const char* typeName )
{
	if ( strcmp( typeName, "CAMERA" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_CAMERA;
	}
	else if ( strcmp( typeName, "LAMP" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_LIGHT;
	}				
	else if ( strcmp( typeName, "MESH" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_MESH;
	}
	else if ( strcmp( typeName, "CURVE" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_CURVE;
	}				
	else if ( strcmp( typeName, "EMPTY" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_EMPTY;
	}
	else if ( strcmp( typeName, "bounding_volume" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_BOUNDING_VOLUME;
	}
	else if ( strcmp( typeName, "camera_volume" ) == 0 )
	{
		return RENDER_OBJECT_DATA_TYPE_CAMERA_VOLUME;
	}
    else if ( strcmp( typeName, "trigger" ) == 0 )
    {
        return RENDER_OBJECT_DATA_TYPE_TRIGGER;
    }
	else
	{
		Assert( false, "" );
		return RENDER_OBJECT_DATA_TYPE_EMPTY;
	}
}

//============================================================================

RenderObjectData::RenderObjectData( RenderObjectDataType type ) 
	:	mRenderObjectDataType( type )
{	
}

//============================================================================

RenderObjectData::~RenderObjectData()
{	
}

//============================================================================

void RenderObjectData::Load( tinyxml2::XMLNode* /*node*/ )
{
}