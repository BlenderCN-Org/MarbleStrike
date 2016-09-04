#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/bone.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/ogl/oglrenderer.hpp"
#include "common/engine/render/ogl/oglshaderprogram.hpp"
#include "common/engine/render/ogl/oglvertexbuffer.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace OGL
{
	static int VertexAttribFormatSize[VERTEX_ATTRIB_FORMAT_MAX] =
	{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) SIZE,
		VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
	};

	static int VertexAttribFormatType[VERTEX_ATTRIB_FORMAT_MAX] =
	{
#define VERTEX_ATTRIB_FORMAT_ENTRY( ENUM, SIZE, TYPENAME, COORDINATE, D3DTYPE_XBOX360, D3DTYPE_PC, OGL_TYPE, PS3_TYPE, D3D11TYPE_PC ) OGL_TYPE,
		VERTEX_ATTRIB_FORMAT_TUPLE
#undef VERTEX_ATTRIB_FORMAT_ENTRY	
	};

	static int PrimitiveTypeOGL[PRIMITIVE_TYPE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D11TYPE ) OGLTYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLVertexBuffer::OGLVertexBuffer() 
:   mBufferID( NULL )
,	mIndexBuffer( 0 )
,   mNumVertices( 0 )
,	mPrimitiveType( PRIMITIVE_TRIANGLES )
,   mNumPrimitives( 0 )
,	mVertexPerPrimitive( 0 )
,	mNumStreams( 0 )
,	mOffset( NULL )
{    
}

//============================================================================

OGLVertexBuffer::~OGLVertexBuffer()
{
	DELETE_PTR_ARRAY( mBufferID );
	DELETE_PTR_ARRAY( mOffset );
}

//============================================================================

void OGLVertexBuffer::Create( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{
	CreateVertexBuffer( mesh, vertexDeclaration );
	CreateIndexBuffer( mesh );
}

//============================================================================

void OGLVertexBuffer::CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{	
	mVertexDeclaration = vertexDeclaration;

	mNumStreams = vertexDeclaration->GetNumStreams();
	Assert( mNumStreams != 0, "" );

	mPrimitiveType = mesh->GetPrimitiveType();

	mNumVertices = mesh->GetNumVertices();
	Assert( mNumVertices != 0, "" );

	mNumPrimitives = mesh->GetNumPrimitives();
	Assert( mNumPrimitives != 0, "" );

	mBufferID = NEW_PTR( "OGL Vertex Buffer ID" ) unsigned int[mNumStreams];
	memset( mBufferID, 0, sizeof( unsigned int ) * mNumStreams );

	char** vertexData = NEW_PTR( "Vertex Data" ) char*[mNumStreams];

	int numVertices = mesh->GetNumVertices();

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		OGL::oglGenBuffers( 1, &mBufferID[stream] );
		vertexData[stream] = NEW_PTR( "Data Vertices" ) char[mNumVertices * vertexDeclaration->GetStreamSize( stream ) ];
		memset( vertexData[stream], 0, sizeof( char ) * mNumVertices * vertexDeclaration->GetStreamSize( stream ) );
	}	

	mOffset = NEW_PTR( "Offset" ) int[mNumStreams];	
	memset( mOffset, 0, sizeof( int ) * mNumStreams );

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
				data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

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
				data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

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
		case USAGE_COLOR:
			{
				Assert( vertexElement.GetNumCoordinates() <= 4, "" );
				char* data = NULL;
				data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

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
		case USAGE_TEXCOORD:
			{
				Assert( vertexElement.GetNumCoordinates() <= 2, "" );
				char* data = NULL;
				data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numTexCoords = mesh->GetNumTexCoords();
				int texCoordsStride = mesh->GetTexCoordsStride();
				float* texCoords = mesh->GetTexCoords();

				Assert( texCoordsStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int t = 0; t < numTexCoords; t += texCoordsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = texCoords[t + index];
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
				data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];

				int numTangents = mesh->GetNumTangents();
				int tangentsStride = mesh->GetTangentsStride();
				float* tangents = mesh->GetTangents();

				Assert( tangentsStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int t = 0; t < numTangents; t += tangentsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = tangents[t + index];
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
				data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];
				
				int numBinormals = mesh->GetNumBinormals();
				int binormalsStride = mesh->GetBinormalsStride();
				float* binormals = mesh->GetBinormals();
				
				Assert( binormalsStride >= vertexElement.GetNumCoordinates(), "" );

				for ( int t = 0; t < numBinormals; t += binormalsStride )
				{ 	
					char* currentData = data;
					for ( int index = 0; index < vertexElement.GetNumCoordinates(); ++index )
					{
						*( ( float* )currentData ) = binormals[t + index];
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
				//data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];
				//int numBoneIndices = mesh->GetNumBoneIndices();
				//for ( int vert = 0; vert < numBoneIndices; ++vert )
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
				//data = vertexData[vertexElement.GetStream()] + mOffset[vertexElement.GetStream()];
				//int numBoneWeights = mesh->GetNumBoneWeights();
				//for ( int vert = 0; vert < numBoneWeights; ++vert )
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

		mOffset[vertexElement.GetStream()] += OGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		OGL::oglBindBuffer( GL_ARRAY_BUFFER_ARB, mBufferID[stream] );
		OGL::oglBufferData( GL_ARRAY_BUFFER_ARB, vertexDeclaration->GetStreamSize( stream ) * numVertices, vertexData[stream], GL_STATIC_DRAW_ARB);
	}

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		DELETE_PTR_ARRAY( vertexData[stream] );
	}
	DELETE_PTR_ARRAY( vertexData );	
}

//============================================================================

void OGLVertexBuffer::CreateIndexBuffer( Mesh* mesh )
{
	OGL::oglGenBuffers( 1, &mIndexBuffer );
	OGL::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer );
	
	int numFaceIndices = mesh->GetNumFaceIndices();
	int indexBufferSize = sizeof( unsigned int ) * numFaceIndices;	
	unsigned int* faceIndices = mesh->GetFaceIndices();

	mVertexPerPrimitive = mesh->GetFaceIndicesStride();

	char* vertexData = NEW_PTR( "Index Data" ) char[indexBufferSize];
	memset( vertexData, 0, sizeof( char ) * indexBufferSize );

	char* currentData = vertexData;
	for ( int i = 0; i < numFaceIndices; ++i )
	{   		
		*( ( unsigned int* )currentData ) = faceIndices[i];
		currentData += sizeof( unsigned int );		
	}

	OGL::oglBufferData( GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, vertexData, GL_STREAM_DRAW );

	DELETE_PTR_ARRAY( vertexData );
}

//============================================================================

void OGLVertexBuffer::SetOGLPointer( 
									VertexElement vertexElement, 
									VertexDeclaration* vertexDeclaration,
									EffectPass* effectPass, 
									int offset 
									)
{	
    ResourceHandle<ShaderProgram> shaderProgramHandle = effectPass->GetShaderProgram();	
	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( shaderProgramHandle );

	OGLShaderProgram* oglShaderProgram = 
		reinterpret_cast<OGLShaderProgram*>( shaderProgram );

	switch ( vertexElement.GetUsageType() )
	{
	case USAGE_POSITION:
		{			
			int index = oglShaderProgram->GetInVertex();			
			OGL::oglEnableVertexAttribArray( index );
			OGL::oglVertexAttribPointer( 
				index, 
				vertexElement.GetNumCoordinates(), 
				OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
				GL_FALSE, 
				vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
				(char*)0 + offset 
				);
		}
		break;
	case USAGE_TEXCOORD:
		{
			int index = oglShaderProgram->GetInTexCoord( vertexElement.GetUsageIndex() );
			if ( index >= 0 )
			{
				OGL::oglEnableVertexAttribArray( index );
				OGL::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_TANGENT:
		{
			int index = oglShaderProgram->GetInTangent();
			if ( index >= 0 )
			{
				OGL::oglEnableVertexAttribArray( index );
				OGL::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_BINORMAL:
		{
			int index = oglShaderProgram->GetInBinormal();
			if ( index >= 0 )
			{
				OGL::oglEnableVertexAttribArray( index );
				OGL::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_COLOR:
		{
			int index = oglShaderProgram->GetInColor();
			if ( index >= 0 )
			{
				OGL::oglEnableVertexAttribArray( index );
				OGL::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_NORMAL:
		{
			int index = oglShaderProgram->GetInNormal();
			if ( index >= 0 )
			{
				OGL::oglEnableVertexAttribArray( index );
				OGL::oglVertexAttribPointer( 
					index, 
					3, 
					OGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	default:
		break;
	}
}

//============================================================================

void OGLVertexBuffer::Render( 
	VertexDeclaration* vertexDeclaration, 
	EffectPass* effectPass, 
	void* context 
	)
{
	UNUSED_ALWAYS( context );

	memset( mOffset, 0, sizeof( int ) * mNumStreams );
	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	std::list<VertexElement>::iterator iter = vertexElementList.begin();
	for ( iter; iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		OGL::oglBindBuffer( GL_ARRAY_BUFFER_ARB, mBufferID[vertexElement.GetStream()] );

		SetOGLPointer( 
			vertexElement, 
			vertexDeclaration,
			effectPass, 
			mOffset[vertexElement.GetStream()] 
		);

		mOffset[vertexElement.GetStream()] += OGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}		

	OGL::oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, mIndexBuffer);
	OGL::oglDrawElements( 
		OGL::PrimitiveTypeOGL[mPrimitiveType], 
		mNumPrimitives * mVertexPerPrimitive, 
		GL_UNSIGNED_INT, 
		0 );

    ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( effectPass->GetShaderProgram() );

	OGLShaderProgram* oglShaderProgram = 
		reinterpret_cast<OGLShaderProgram*>( shaderProgram );

	iter = vertexElementList.begin();
	for ( iter; iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;
		int index = -1;

		switch ( vertexElement.GetUsageType() )
		{
		case USAGE_POSITION:
			index = oglShaderProgram->GetInVertex();
			break;
		case USAGE_TEXCOORD:
			index = oglShaderProgram->GetInTexCoord( vertexElement.GetUsageIndex() );	
			break;
		case USAGE_NORMAL:
			index = oglShaderProgram->GetInNormal();
			break;
		case USAGE_COLOR:
			index = oglShaderProgram->GetInColor();
			break;
		case USAGE_TANGENT:
			index = oglShaderProgram->GetInTangent();
			break;
		case USAGE_BINORMAL:
			index = oglShaderProgram->GetInTangent();
			break;
		default:
			Assert( false, "" );
			break;
		}

		if ( index >= 0 )
		{
			OGL::oglDisableVertexAttribArray( index );
		}
	}	
}

#endif