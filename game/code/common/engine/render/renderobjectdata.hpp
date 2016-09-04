#ifndef RENDER_OBJECT_DATA_HPP
#define RENDER_OBJECT_DATA_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "tinyxml2.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

enum RenderObjectDataType
{
	RENDER_OBJECT_DATA_TYPE_MESH,
	RENDER_OBJECT_DATA_TYPE_CAMERA,
	RENDER_OBJECT_DATA_TYPE_LIGHT,
	RENDER_OBJECT_DATA_TYPE_CURVE,
	RENDER_OBJECT_DATA_TYPE_BOUNDING_VOLUME,
	RENDER_OBJECT_DATA_TYPE_CAMERA_VOLUME,
	RENDER_OBJECT_DATA_TYPE_TRIGGER,
	RENDER_OBJECT_DATA_TYPE_EMPTY
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderObjectData
{
public:

	static RenderObjectDataType GetRenderObjectDataType( const char* typeName );

	RenderObjectData( RenderObjectDataType type );
	virtual ~RenderObjectData();
		
	virtual void Load( tinyxml2::XMLNode* node );
	
	RenderObjectDataType GetRenderObjectDataType() const
	{
		return mRenderObjectDataType;
	}	
		
protected:
		
	RenderObjectDataType mRenderObjectDataType;	
};

#endif