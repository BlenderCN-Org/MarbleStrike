#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3dvertexdeclaration.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace D3D
{
    static int VertexAttribFormatSize[VERTEX_ATTRIB_FORMAT_MAX] =
    {
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) SIZE,
        VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
    };	

    static D3DDECLTYPE VertexAttribFormatType[VERTEX_ATTRIB_FORMAT_MAX] =
    {
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) D3DTYPE_PC,
        VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
    };

    static D3DDECLUSAGE VertexAttribUsageType[VERTEX_ATTRIB_USAGE_MAX] =
    {
#define VERTEX_ATTRIB_USAGE_ENTRY( ENUM, USAGENAME, D3DTYPE, PS3TYPE, OGLTYPE ) D3DTYPE,
        VERTEX_ATTRIB_USAGE_TUPLE
#undef VERTEX_ATTRIB_USAGE_ENTRY
    };
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DVertexDeclaration::D3DVertexDeclaration()
{
    mVertexDeclaration = NULL;
}

//============================================================================

D3DVertexDeclaration::~D3DVertexDeclaration()
{
    if ( mVertexDeclaration != NULL )
    {
        mVertexDeclaration->Release();            
        mVertexDeclaration = NULL;
    }	
}

//============================================================================

void D3DVertexDeclaration::CreateVertexDeclaration( EffectPass* effectPass )
{  
    UNUSED_ALWAYS( effectPass );
    if ( mVertexDeclaration == NULL )
    {
        std::list<VertexElement>::iterator iter = mVertexElements.begin();
        for ( iter; iter != mVertexElements.end(); ++iter )
        {
            VertexElement vertexElement = *iter;		
            if ( vertexElement.GetStream() > mNumStreams )
            {
                mNumStreams = vertexElement.GetStream();
            }
        }

        mNumStreams += 1;

        D3DVERTEXELEMENT9 dec[16];

        Assert( mNumStreams < MAX_NUM_STREAMS, "" );
        int offset[MAX_NUM_STREAMS];	
        memset( mStreamSize, 0, sizeof( int ) * MAX_NUM_STREAMS );
        memset( offset, 0, sizeof( int ) * MAX_NUM_STREAMS );

        int i = 0;
        iter = mVertexElements.begin();
        for ( iter; iter != mVertexElements.end(); ++iter )
        {
            VertexElement vertexElement = *iter;
            SetVertexElement( 
                dec[i++], 
                static_cast<WORD>( vertexElement.GetStream() ), 
                static_cast<WORD>( offset[vertexElement.GetStream()] ),
                D3D::VertexAttribFormatType[vertexElement.GetFormatType()], 
                D3DDECLMETHOD_DEFAULT, 
                static_cast<BYTE>( D3D::VertexAttribUsageType[vertexElement.GetUsageType()] ), 
                static_cast<BYTE>( vertexElement.GetUsageIndex() )
                );		

            offset[vertexElement.GetStream()] += D3D::VertexAttribFormatSize[vertexElement.GetFormatType()];

            mStreamSize[vertexElement.GetStream()] += D3D::VertexAttribFormatSize[vertexElement.GetFormatType()];
        }

        SetVertexElement( dec[i], 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 );

        D3DRenderer::GetDevice()->CreateVertexDeclaration( dec, &mVertexDeclaration );    
    }
}

//============================================================================

void D3DVertexDeclaration::SetVertexElement( 
    D3DVERTEXELEMENT9 &vertexElement, 
    WORD stream, 
    WORD offset, 
    D3DDECLTYPE type, 
    BYTE method, 
    BYTE usage, 
    BYTE usageIndex )
{
    vertexElement.Stream = stream;
    vertexElement.Method = method;
    vertexElement.Offset = offset;
    vertexElement.Type = static_cast<BYTE>( type );
    vertexElement.Usage = usage;
    vertexElement.UsageIndex = usageIndex;
}

//============================================================================

void D3DVertexDeclaration::Dispatch( void* context )
{
    UNUSED_ALWAYS( context );

    D3DRenderer::GetDevice()->SetVertexDeclaration( mVertexDeclaration );
}

#endif