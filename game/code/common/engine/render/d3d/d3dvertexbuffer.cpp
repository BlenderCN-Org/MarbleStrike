#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3dvertexbuffer.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
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

	static D3DPRIMITIVETYPE PrimitiveTypeD3D[PRIMITIVE_TYPE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) D3DTYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};	
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DVertexBuffer::D3DVertexBuffer() 
:   mVertexBuffer( NULL )
,   mIndexBuffer( NULL )
,   mNumVertices( 0 )
,	mPrimitiveType( PRIMITIVE_TRIANGLES )
,   mNumPrimitives( 0 )
,	mNumStreams( 0 )
,	mOffset( 0 )
{    
}

//============================================================================

D3DVertexBuffer::~D3DVertexBuffer()
{
	for ( int i = 0; i < mNumStreams; ++i )
	{
		if ( mVertexBuffer[i] != NULL )
		{
			mVertexBuffer[i]->Release();
			mVertexBuffer[i] = NULL;
		}       
	}

	if ( mIndexBuffer != NULL )
	{
		mIndexBuffer->Release();
		mIndexBuffer = NULL;
	}

	DELETE_PTR_ARRAY( mVertexBuffer );
	DELETE_PTR_ARRAY( mOffset );
}
//============================================================================

void D3DVertexBuffer::Create( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{
	CreateVertexBuffer( mesh, vertexDeclaration );
	CreateIndexBuffer( mesh );
}

//============================================================================

void D3DVertexBuffer::CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{	
	int numStreams = vertexDeclaration->GetNumStreams();
	mNumStreams = numStreams;
	mVertexDeclaration = vertexDeclaration;

	mPrimitiveType = mesh->GetPrimitiveType();
	mNumVertices = mesh->GetNumVertices();
	mNumPrimitives = mesh->GetNumPrimitives();

	mVertexBuffer = NEW_PTR( "Vertex Buffers" ) LPDIRECT3DVERTEXBUFFER9[vertexDeclaration->GetNumStreams()];
	int numVertices = mesh->GetNumVertices();
	LPDIRECT3DDEVICE9 device = D3DRenderer::GetDevice();

	for ( int stream = 0; stream < numStreams; ++stream )
	{
		device->CreateVertexBuffer(
			vertexDeclaration->GetStreamSize( stream ) * numVertices,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&mVertexBuffer[stream],
			NULL );
	}

	mOffset = NEW_PTR( "Offset" ) int[numStreams];	
	memset( mOffset, 0, sizeof( int ) * numStreams );

	char** lockBuffers = NEW_PTR( "Lock Buffers" ) char*[numStreams];

	for ( int stream = 0; stream < numStreams; ++stream )
	{
		mVertexBuffer[stream]->Lock( 0, 0, (void**)&lockBuffers[stream], 0 );		
	}

	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
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

		mOffset[vertexElement.GetStream()] += D3D::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}	

	for ( int stream = 0; stream < numStreams; ++stream )
	{
		mVertexBuffer[stream]->Unlock();  
	}

	DELETE_PTR_ARRAY( lockBuffers );
}

//============================================================================

void D3DVertexBuffer::CreateIndexBuffer( Mesh* mesh )
{
	LPDIRECT3DDEVICE9 device = D3DRenderer::GetDevice();

	//
	// Index buffer
	//
	int indexBufferSize = sizeof( unsigned int ) * mesh->GetNumFaceIndices();

	device->CreateIndexBuffer( 
		indexBufferSize,
		D3DUSAGE_WRITEONLY, 
		D3DFMT_INDEX32, 
		D3DPOOL_MANAGED,
		&mIndexBuffer, 
		NULL );

	unsigned int* faceIndices = mesh->GetFaceIndices();

	void* pVoid;
	mIndexBuffer->Lock( 0, 0, (void**)&pVoid, 0 );
	int numFaceIndices = mesh->GetNumFaceIndices();
	for ( int i = 0; i < numFaceIndices; ++i )
	{   
		unsigned int** data = (unsigned int**)&pVoid;
		*(*data) = faceIndices[i];
		(*data) += 1;        
	}
	mIndexBuffer->Unlock();
}

//============================================================================

void D3DVertexBuffer::Render( 
							 VertexDeclaration* vertexDeclaration,
							 EffectPass* effectPass,
							 void* context
							 )
{
	UNUSED_ALWAYS( effectPass );
	UNUSED_ALWAYS( context );

	for ( int i = 0; i < vertexDeclaration->GetNumStreams(); ++i )
	{
		D3DRenderer::GetDevice()->SetStreamSource( 
			i, 
			mVertexBuffer[i], 
			0, 
			vertexDeclaration->GetStreamSize( i ) 
			);
	}

	D3DRenderer::GetDevice()->BeginScene();

	D3DRenderer::GetDevice()->SetIndices( mIndexBuffer );
	D3DRenderer::GetDevice()->DrawIndexedPrimitive( 
		D3D::PrimitiveTypeD3D[mPrimitiveType], 
		0, 
		0,
		mNumVertices,
		0,
		mNumPrimitives );

	D3DRenderer::GetDevice()->EndScene();
}

#endif