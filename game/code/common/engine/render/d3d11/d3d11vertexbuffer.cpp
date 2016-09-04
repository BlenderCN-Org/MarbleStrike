#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11vertexbuffer.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/memory.hpp"

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

	static D3D11_PRIMITIVE_TOPOLOGY PrimitiveTypeD3D11[VERTEX_ATTRIB_USAGE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) D3D11TYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11VertexBuffer::D3D11VertexBuffer() 
:   mVertexBuffer( NULL )
,   mIndexBuffer( NULL )
,	mPrimitiveType( PRIMITIVE_TRIANGLES )
,   mNumIndices( 0 )
,	mNumStreams( 0 )
{    
}

//============================================================================

D3D11VertexBuffer::~D3D11VertexBuffer()
{
	for ( int i = 0; i < mNumStreams; ++i )
	{
		if ( mVertexBuffer[i] != NULL )
		{
			mVertexBuffer[i]->Release();
		}       
	}
	DELETE_PTR_ARRAY( mVertexBuffer );

	if ( mIndexBuffer != NULL )
	{
		mIndexBuffer->Release();	
	}	
}

//============================================================================

void D3D11VertexBuffer::Create( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{
	CreateVertexBuffer( mesh, vertexDeclaration );
	CreateIndexBuffer( mesh );
}

//============================================================================

void D3D11VertexBuffer::CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{	
	int numStreams = vertexDeclaration->GetNumStreams();
	mNumStreams = numStreams;
	mVertexDeclaration = vertexDeclaration;

	mPrimitiveType = mesh->GetPrimitiveType();

	mVertexBuffer = NEW_PTR( "Vertex Buffers" ) ID3D11Buffer*[vertexDeclaration->GetNumStreams()];
	int numVertices = mesh->GetNumVertices();	
	
	memset( mOffset, 0, sizeof( int ) * MAX_NUM_STREAMS );

	char** lockBuffers = NEW_PTR( "Lock Buffers" ) char*[numStreams];
	for ( int stream = 0; stream < numStreams; ++stream )
	{
		lockBuffers[stream] = NEW_PTR( "Lock Buffers" ) char[vertexDeclaration->GetStreamSize( stream ) * numVertices];
	}

	std::list<VertexElement> vertexElementList = vertexDeclaration->GetVertexElements();	
	std::list<VertexElement>::iterator iter = vertexElementList.begin();
	for ( iter; iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		switch ( vertexElement.GetDataType() )
		{
		case USAGE_POSITION:
			{
				Assert( vertexElement.GetNumCoordinates() <= 3, "" );
				char* data = NULL;
				data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numPositions = mesh->GetNumPositions();
				int positionStride = mesh->GetPositionsStride();
				float* positions = mesh->GetPositions();

				Assert( positionStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int p = 0; p < numPositions; p += positionStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = positions[p + index];
						currentData += sizeof( float );
					}

					data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				}
			}
			break;
		case USAGE_NORMAL:
			{
				Assert( vertexElement.GetNumCoordinates() <= 3, "" );
				char* data = NULL;
				data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numNormals = mesh->GetNumNormals();
				int normalsStride = mesh->GetNormalsStride();
				float* normals = mesh->GetNormals();

				Assert( normalsStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int n = 0; n < numNormals; n += normalsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = normals[n + index];
						currentData += sizeof( float );
					}

					data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				}
			}
			break;
		case USAGE_TEXCOORD:
			{
				Assert( vertexElement.GetNumCoordinates() <= 2, "" );
				char* data = NULL;
				data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numTexCoords = mesh->GetNumTexCoords();
				int texCoordsStride = mesh->GetTexCoordsStride();
				float* texCoords = mesh->GetTexCoords();

				Assert( texCoordsStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int t = 0; t < numTexCoords; t += texCoordsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						if ( index == 0 )                            
						{
							*( ( float* )currentData ) = texCoords[t + index];
						}
						else
						{
							*( ( float* )currentData ) = 1.0f - texCoords[t + index];
						}
						currentData += sizeof( float );
					}

					data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				}
			}
			break;
		case USAGE_COLOR:
			{
				Assert( vertexElement.GetNumCoordinates() <= 4, "" );
				char* data = NULL;
				data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numColors = mesh->GetNumColors();
				int colorsStride = mesh->GetColorsStride();
				float* colors = mesh->GetColors();

				Assert( colorsStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int c = 0; c < numColors; c += colorsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = colors[c + index];
						currentData += sizeof( float );
					}

					data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				}
			}
			break;
		case USAGE_TANGENT:
			{
				Assert( vertexElement.GetNumCoordinates() <= 3, "" );
				char* data = NULL;
				data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numTangents = mesh->GetNumTangents();
				int tangentsStride = mesh->GetTangentsStride();
				float* tangents = mesh->GetTangents();

				for ( int c = 0; c < numTangents; c += tangentsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = tangents[c + index];
						currentData += sizeof( float );
					}

					data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				}
			}
			break;
		case USAGE_BINORMAL:
			{
				Assert( vertexElement.GetNumCoordinates() <= 3, "" );
				char* data = NULL;
				data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];
				
				int numBinormals = mesh->GetNumBinormals();
				int binormalsStride = mesh->GetBinormalsStride();
				float* binormals = mesh->GetBinormals();
				
				for ( int c = 0; c < numBinormals; c += binormalsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = binormals[c + index];
						currentData += sizeof( float );
					}

					data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				}
			}
			break;
		case USAGE_BLENDINDICES:
			{
				//Assert( vertexElement.GetNumCoordinates() <= 4, "" );
				//char* data = NULL;
				//data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];
				//int numBlendIndices = mesh->GetNumBoneIndices();
				//for ( int vert = 0; vert < numBlendIndices; ++vert )
				//{ 					
				//	Math::Vector4 boneIndex = mesh->GetBoneIndex( vert );					

				//	char* currentData = data;
				//	for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
				//	{
				//		*( ( float* )currentData ) = boneIndex[index];
				//		currentData += sizeof( float );
				//	}

				//	data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				//}
			}
			break;
		case USAGE_BLENDWEIGHT:
			{
				//Assert( vertexElement.GetNumCoordinates() <= 4, "" );
				//char* data = NULL;
				//data = lockBuffers[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];
				//int numBlendWeights = mesh->GetNumBoneWeights();
				//for ( int vert = 0; vert < numBlendWeights; ++vert )
				//{ 					
				//	Math::Vector4 boneWeight = mesh->GetBoneWeight( vert );					

				//	char* currentData = data;
				//	for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
				//	{
				//		*( ( float* )currentData ) = boneWeight[index];
				//		currentData += sizeof( float );
				//	}

				//	data += vertexDeclaration->GetStreamSize( vertexElement.GetStream() );
				//}
			}
			break;
		default:
			break;
		}		

		mOffset[vertexElement.GetStream()] += D3D11::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}	

	for ( int stream = 0; stream < numStreams; ++stream )
	{
		D3D11_SUBRESOURCE_DATA initData;
		memset( &initData, 0, sizeof( initData ) );
		initData.pSysMem = lockBuffers[stream];	

		D3D11_BUFFER_DESC vertexBufferDesc;
		memset( &vertexBufferDesc, 0, sizeof( vertexBufferDesc ) );
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = vertexDeclaration->GetStreamSize( stream ) * numVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;

		HRESULT hr = D3D11Renderer::GetDevice()->CreateBuffer( &vertexBufferDesc, &initData, &mVertexBuffer[stream] );
		if ( hr != S_OK )
		{
			Assert( false, "Error creating buffer.")
		}
	}

	for ( int stream = 0; stream < numStreams; ++stream )
	{
		DELETE_PTR_ARRAY( lockBuffers[stream] );
	}
	DELETE_PTR_ARRAY( lockBuffers );
}

//============================================================================

void D3D11VertexBuffer::CreateIndexBuffer( Mesh* mesh )
{
	mNumIndices = mesh->GetNumFaceIndices();

	int numFaceIndices = mesh->GetNumFaceIndices();
	int indexBufferSize = sizeof( unsigned short ) * numFaceIndices;
	
	char* lockBuffer = NEW_PTR( "Lock Buffer" ) char[indexBufferSize];	
	char* lockBufferData = lockBuffer;

	unsigned int* faceIndices = mesh->GetFaceIndices();		
	for ( int i = 0; i < numFaceIndices; ++i )
	{   
		unsigned short** data = (unsigned short**)&lockBufferData;
		*(*data) = static_cast<unsigned short>( faceIndices[i] );
		(*data) += 1;        
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	memset( &indexBufferDesc, 0, sizeof( indexBufferDesc ) );
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = indexBufferSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	memset( &initData, 0, sizeof( initData ) );
	initData.pSysMem = lockBuffer;

	HRESULT hr = D3D11Renderer::GetDevice()->CreateBuffer( &indexBufferDesc, &initData, &mIndexBuffer );
	if ( hr != S_OK )
	{
		Assert( false, "Error creating buffer.")
	}

	DELETE_PTR_ARRAY( lockBuffer );
}

//============================================================================

void D3D11VertexBuffer::Render( 
							 VertexDeclaration* vertexDeclaration,
							 EffectPass* effectPass,
							 void* context
							 )
{
	UNUSED_ALWAYS( effectPass );

	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	for ( int i = 0; i < vertexDeclaration->GetNumStreams(); ++i )
	{
		unsigned int stride = vertexDeclaration->GetStreamSize( i );
		unsigned int offset = 0;
		deviceContext->IASetVertexBuffers( i, 1, &mVertexBuffer[i], &stride, &offset );
	}

	deviceContext->IASetIndexBuffer( mIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
	deviceContext->IASetPrimitiveTopology( D3D11::PrimitiveTypeD3D11[mPrimitiveType] );
	deviceContext->DrawIndexed( mNumIndices, 0, 0 );
}

#endif