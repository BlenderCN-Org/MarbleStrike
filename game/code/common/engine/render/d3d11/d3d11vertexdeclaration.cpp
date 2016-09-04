#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11vertexdeclaration.hpp"
#include "common/engine/render/d3d11/d3d11vertexshader.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace D3D11
{
    static int VertexAttribFormatSize[VERTEX_ATTRIB_FORMAT_MAX] =
    {
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) SIZE,
        VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
    };	

    static DXGI_FORMAT VertexAttribFormatType[VERTEX_ATTRIB_FORMAT_MAX] =
    {
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) D3D11TYPE_PC,
        VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
    };

    static const char* VertexAttribFormatString[VERTEX_ATTRIB_FORMAT_MAX] =
    {
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) TYPENAME,
        VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
    };

    static const char* VertexAttribUsageType[VERTEX_ATTRIB_USAGE_MAX] =
    {
#define VERTEX_ATTRIB_USAGE_ENTRY( ENUM, USAGENAME, D3DTYPE, PS3TYPE, OGLTYPE ) USAGENAME,
        VERTEX_ATTRIB_USAGE_TUPLE
#undef VERTEX_ATTRIB_USAGE_ENTRY
    };
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11VertexDeclaration::D3D11VertexDeclaration()
{
    mVertexLayout = NULL;
}

//============================================================================

D3D11VertexDeclaration::~D3D11VertexDeclaration()
{	
    if ( mVertexLayout != NULL )
    {
        mVertexLayout->Release();            
        mVertexLayout = NULL;
    }	
}

//============================================================================

void D3D11VertexDeclaration::CreateVertexDeclaration( EffectPass* effectPass )
{  
    if  ( mVertexLayout == NULL )
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

        D3D11_INPUT_ELEMENT_DESC dec[16];

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
                dec[i],
                LPCSTR( D3D11::VertexAttribUsageType[vertexElement.GetUsageType()] ),
                vertexElement.GetUsageIndex(),
                D3D11::VertexAttribFormatType[vertexElement.GetFormatType()], 
                0,
                offset[vertexElement.GetStream()],
                D3D11_INPUT_PER_VERTEX_DATA,
                0
                );
        
            offset[vertexElement.GetStream()] += D3D11::VertexAttribFormatSize[vertexElement.GetFormatType()];

            mStreamSize[vertexElement.GetStream()] += D3D11::VertexAttribFormatSize[vertexElement.GetFormatType()];

            i++;
        }

		ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( effectPass->GetShaderProgram() );
		VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( shaderProgram->GetVertexShader() );
		D3D11VertexShader* d3d11VertexShader = reinterpret_cast<D3D11VertexShader*>( vertexShader );
        
        D3D11Renderer::GetDevice()->CreateInputLayout( 
            dec, 
            mVertexElements.size(), 
            d3d11VertexShader->GetShaderByteCode(),
            d3d11VertexShader->GetShaderByteCodeSize(), 
            &mVertexLayout 
            );
    }
}

//============================================================================

void D3D11VertexDeclaration::SetVertexElement( 
    D3D11_INPUT_ELEMENT_DESC &vertexElement, 
    LPCSTR semanticName,
    UINT semanticIndex,
    DXGI_FORMAT format,
    UINT inputSlot,
    UINT alignedByteOffset,
    D3D11_INPUT_CLASSIFICATION inputSlotClass,
    UINT instanceDataStepRate
    )
{
    vertexElement.SemanticName = semanticName;
    vertexElement.SemanticIndex = semanticIndex;
    vertexElement.Format = format;
    vertexElement.InputSlot = inputSlot;
    vertexElement.AlignedByteOffset = alignedByteOffset;
    vertexElement.InputSlotClass = inputSlotClass;
    vertexElement.InstanceDataStepRate = instanceDataStepRate;
}

//============================================================================

void D3D11VertexDeclaration::Dispatch( void* context )
{
    ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
    if ( context )
    {
        deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
    }

    deviceContext->IASetInputLayout( mVertexLayout );	
}

#endif