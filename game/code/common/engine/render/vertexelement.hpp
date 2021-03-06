#ifndef VERTEXELEMENT_HPP
#define VERTEXELEMENT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#define VERTEX_ATTRIB_FORMAT_TUPLE \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_FLOAT1, 4, "float", 1, D3DDECLTYPE_FLOAT1, D3DDECLTYPE_FLOAT1, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_R32_FLOAT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_FLOAT2, 8, "float2", 2, D3DDECLTYPE_FLOAT2, D3DDECLTYPE_FLOAT2, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_R32G32_FLOAT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_FLOAT3, 12, "float3", 3, D3DDECLTYPE_FLOAT3, D3DDECLTYPE_FLOAT3, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_R32G32B32_FLOAT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_FLOAT4, 16, "float4", 4, D3DDECLTYPE_FLOAT4, D3DDECLTYPE_FLOAT4, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_R32G32B32A32_FLOAT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_Color, 4, "color", 4, D3DDECLTYPE_D3DCOLOR, D3DDECLTYPE_D3DCOLOR, GL_FLOAT, CELL_GCM_VERTEX_S1, DXGI_FORMAT_R8G8B8A8_UINT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_HALF2, 4, "half2", 2, D3DDECLTYPE_FLOAT16_2, D3DDECLTYPE_FLOAT16_2, GL_SHORT, CELL_GCM_VERTEX_SF, DXGI_FORMAT_R16G16_FLOAT  ) \
    VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_HALF3, 6, "half3", 3, D3DDECLTYPE_FLOAT16_2, D3DDECLTYPE_FLOAT16_2, GL_SHORT, CELL_GCM_VERTEX_SF, DXGI_FORMAT_R16G16_FLOAT  ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_HALF4, 8, "half4", 4, D3DDECLTYPE_FLOAT16_4, D3DDECLTYPE_FLOAT16_4, GL_SHORT, CELL_GCM_VERTEX_SF, DXGI_FORMAT_R16G16B16A16_FLOAT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_DEC4N, 4, "dec4n", 4, D3DDECLTYPE_DEC4N, D3DDECLTYPE_UNUSED, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_UNKNOWN ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_SHORT4N, 8, "short4n", 4, D3DDECLTYPE_SHORT4N, D3DDECLTYPE_SHORT4N, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_UNKNOWN ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_UBYTE4, 4, "ubyte4", 4, D3DDECLTYPE_UBYTE4, D3DDECLTYPE_UBYTE4, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_R8G8B8A8_UINT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_UINT1, 1, "uint1", 1, D3DDECLTYPE_UINT1, D3DDECLTYPE_UNUSED, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_R8_UINT ) \
	VERTEX_ATTRIB_FORMAT_ENTRY( FORMAT_TYPE_NONE, 0, "none", 0, D3DDECLTYPE_UNUSED, D3DDECLTYPE_UNUSED, GL_FLOAT, CELL_GCM_VERTEX_F, DXGI_FORMAT_UNKNOWN) \

enum FormatType
{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) ENUM,
	VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY
	VERTEX_ATTRIB_FORMAT_MAX
};

#define VERTEX_ATTRIB_USAGE_TUPLE \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_POSITION, "POSITION", D3DDECLUSAGE_POSITION, CG_ATTR0, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_BLENDWEIGHT, "BLENDWEIGHT", D3DDECLUSAGE_BLENDWEIGHT, CG_ATTR1, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_BLENDINDICES, "BLENDINDICES", D3DDECLUSAGE_BLENDINDICES, CG_ATTR7, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_NORMAL, "NORMAL", D3DDECLUSAGE_NORMAL, CG_ATTR2, GL_NORMAL_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_PSIZE, "PSIZE", D3DDECLUSAGE_PSIZE, CG_ATTR6, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_TEXCOORD, "TEXCOORD", D3DDECLUSAGE_TEXCOORD, CG_ATTR8, GL_TEXTURE_COORD_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_TANGENT, "TANGENT", D3DDECLUSAGE_TANGENT, CG_ATTR14, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_BINORMAL, "BINORMAL", D3DDECLUSAGE_BINORMAL, CG_ATTR15, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_TESSFACTOR, "TESSFACTOR", D3DDECLUSAGE_TESSFACTOR, CG_ATTR5, GL_VERTEX_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_COLOR, "COLOR", D3DDECLUSAGE_COLOR, CG_ATTR3, GL_COLOR_ARRAY ) \
	VERTEX_ATTRIB_USAGE_ENTRY( USAGE_FOG, "FOG", D3DDECLUSAGE_FOG, CG_ATTR5, GL_VERTEX_ARRAY ) \

enum UsageType
{
#define VERTEX_ATTRIB_USAGE_ENTRY( ENUM, USAGENAME, D3DTYPE, PS3TYPE, OGLTYPE ) ENUM,
	VERTEX_ATTRIB_USAGE_TUPLE
#undef VERTEX_ATTRIB_USAGE_ENTRY
	VERTEX_ATTRIB_USAGE_MAX
};

static const int MAX_NUM_STREAMS = 3;

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class VertexElement
{
public:

    VertexElement();
	~VertexElement();

	int GetStream()
	{
		return mStream;
	}

	UsageType GetUsageType()
	{
		return mUsage;
	}

	int GetUsageIndex()
	{
		return mUsageIndex;
	}

	UsageType GetDataType()
	{
		return mDataType;
	}

	int GetDataIndex()
	{
		return mDataIndex;
	}

	FormatType GetFormatType()
	{
		return mFormat;
	}

	int GetNumCoordinates()
	{
		return mNumCoordinates;
	}

	void SetStream( int i );

	void SetDataType( UsageType type )
	{
		mDataType = type;
	}

	void SetDataIndex( int index )
	{
		mDataIndex = index;
	}

	void SetUsageType( UsageType type )
	{
		mUsage = type;
	}

	void SetUsageIndex( int index )
	{
		mUsageIndex = index;
	}

	void SetFormatType( FormatType type )
	{
		mFormat = type;
	}

	void SetNumCoordinates( int num )
	{
		mNumCoordinates = num;
	}

	static void ExtractNameAndIndex( const char* usageType, char* usageName, int &index );

private:

	int mStream;
	UsageType mUsage;
	int mUsageIndex;
	UsageType mDataType;
	int mDataIndex;
	int mNumCoordinates;
	FormatType mFormat;
 
};

#endif
