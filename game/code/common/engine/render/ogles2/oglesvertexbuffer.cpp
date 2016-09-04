#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/bone.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/ogles2/oglesrenderer.hpp"
#include "common/engine/render/ogles2/oglesshaderprogram.hpp"
#include "common/engine/render/ogles2/oglesvertexbuffer.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace OGLES
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

OGLESVertexBuffer::OGLESVertexBuffer() 
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

OGLESVertexBuffer::~OGLESVertexBuffer()
{
	DELETE_PTR_ARRAY( mBufferID );
	DELETE_PTR_ARRAY( mOffset );
}

//============================================================================

void OGLESVertexBuffer::Create( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{
	CreateVertexBuffer( mesh, vertexDeclaration );
	CreateIndexBuffer( mesh );
}

//============================================================================

void OGLESVertexBuffer::CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{
	mVertexDeclaration = vertexDeclaration;

	mNumStreams = vertexDeclaration->GetNumStreams();

	mPrimitiveType = mesh->GetPrimitiveType();
	mNumVertices = mesh->GetNumVertices();
	mNumPrimitives = mesh->GetNumPrimitives();

	mBufferID = NEW_PTR( "OGL Vertex Buffer ID" ) unsigned int[mNumStreams];
	memset( mBufferID, 0, sizeof( unsigned int ) * mNumStreams );

	char** vertexData = NEW_PTR( "Vertex Data" ) char*[mNumStreams];

	int numVertices = mesh->GetNumVertices();

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		OGLES::oglGenBuffers( 1, &mBufferID[stream] );
		vertexData[stream] = NEW_PTR( "Data Vertices" ) char[mNumVertices * vertexDeclaration->GetStreamSize( stream ) ];
		memset( vertexData[stream], 0, sizeof( char ) * mNumVertices * vertexDeclaration->GetStreamSize( stream ) );
	}	

	mOffset = NEW_PTR( "Offset" ) int[mNumStreams];	
	memset( mOffset, 0, sizeof( int ) * mNumStreams );

	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
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
		default:
			break;
		}

		mOffset[vertexElement.GetStream()] += OGLES::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		OGLES::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[stream] );
		OGLES::oglBufferData( GL_ARRAY_BUFFER, vertexDeclaration->GetStreamSize( stream ) * numVertices, vertexData[stream], GL_STATIC_DRAW );
	}

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		DELETE_PTR_ARRAY( vertexData[stream] );
	}
	DELETE_PTR_ARRAY( vertexData );	
}

//============================================================================

void OGLESVertexBuffer::CreateIndexBuffer( Mesh* mesh )
{
	OGLES::oglGenBuffers( 1, &mIndexBuffer );
	OGLES::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer );

	int numFaceIndices = mesh->GetNumFaceIndices();
	int indexBufferSize = sizeof( unsigned short ) * numFaceIndices;	
	unsigned int* faceIndices = mesh->GetFaceIndices();

	mVertexPerPrimitive = mesh->GetFaceIndicesStride();

	char* vertexData = NEW_PTR( "Index Data" ) char[indexBufferSize];
	memset( vertexData, 0, sizeof( char ) * indexBufferSize );

	char* currentData = vertexData;
	for ( int i = 0; i < numFaceIndices; ++i )
	{   		
		*( ( unsigned short* )currentData ) = static_cast<unsigned short>( faceIndices[i] );
		currentData += sizeof( unsigned short );		
	}

	OGLES::oglBufferData( GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, vertexData, GL_STREAM_DRAW );

	DELETE_PTR_ARRAY( vertexData );
}

//============================================================================

void OGLESVertexBuffer::SetOGLPointer( 
									  VertexElement vertexElement, 
									  VertexDeclaration* vertexDeclaration,
									  EffectPass* effectPass, 
									  int offset 
									  )
{
	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( effectPass->GetShaderProgram() );
	OGLESShaderProgram* oglesShaderProgram = 
		reinterpret_cast<OGLESShaderProgram*>( shaderProgram );

	switch ( vertexElement.GetUsageType() )
	{
	case USAGE_POSITION:
		{			
			int index = oglesShaderProgram->GetInVertex();			
			if ( index >= 0 )
			{
				OGLES::oglEnableVertexAttribArray( index );
				OGLES::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_TEXCOORD:
		{
			int index = oglesShaderProgram->GetInTexCoord();
			if ( index >= 0 )
			{
				OGLES::oglEnableVertexAttribArray( index );
				OGLES::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_COLOR:
		{
			int index = oglesShaderProgram->GetInColor();
			if ( index >= 0 )
			{
				OGLES::oglEnableVertexAttribArray( index );
				OGLES::oglVertexAttribPointer( 
					index, 
					vertexElement.GetNumCoordinates(), 
					OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
					GL_FALSE, 
					vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
					(char*)0 + offset 
					);
			}
		}
		break;
	case USAGE_NORMAL:
		{
			int index = oglesShaderProgram->GetInNormal();
			if ( index >= 0 )
			{
				OGLES::oglEnableVertexAttribArray( index );
				OGLES::oglVertexAttribPointer( 
					index, 
					3, 
					OGLES::VertexAttribFormatType[vertexElement.GetFormatType()], 
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

void OGLESVertexBuffer::Render( 
	VertexDeclaration* vertexDeclaration, 
	EffectPass* effectPass,
	void* /*context*/ 
	)
{	
	memset( mOffset, 0, sizeof( int ) * mNumStreams );
    
    OGLESRenderer* oglRenderer = reinterpret_cast<OGLESRenderer*>( Renderer::Get() );

	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;
        int arrayIndex = mBufferID[vertexElement.GetStream()];

        OGLES::oglBindBuffer( GL_ARRAY_BUFFER, arrayIndex );
                
		SetOGLPointer( vertexElement, vertexDeclaration, effectPass, mOffset[vertexElement.GetStream()] );

		mOffset[vertexElement.GetStream()] += OGLES::VertexAttribFormatSize[vertexElement.GetFormatType()];
	}		
    
    OGLES::oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    oglRenderer->SetCurrentBindBufferElementIndex( mIndexBuffer );
    
	OGLES::oglDrawElements( 
		OGLES::PrimitiveTypeOGL[mPrimitiveType], 
		mNumPrimitives * mVertexPerPrimitive, 
		GL_UNSIGNED_SHORT, 
		0 );

	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( effectPass->GetShaderProgram() );
	OGLESShaderProgram* oglesShaderProgram = reinterpret_cast<OGLESShaderProgram*>( shaderProgram );
			
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;
        int stream = vertexElement.GetStream();
        
        switch ( vertexElement.GetUsageType() )
        {
            case USAGE_POSITION:
                {	
                    int index = oglesShaderProgram->GetInVertex();
                    if ( index >= 0 )
                    {
                        OGLES::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[stream] );
                        OGLES::oglDisableVertexAttribArray( index );
                    }               
                }
                break;
            case USAGE_TEXCOORD:
                {
                    int index = oglesShaderProgram->GetInTexCoord();
                    if ( index >= 0 )
                    {
                        OGLES::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[stream] );
                        OGLES::oglDisableVertexAttribArray( index );
                    }
                }
                break;
            case USAGE_NORMAL:
                {
                    int index = oglesShaderProgram->GetInColor();
                    if ( index >= 0 )
                    {
                        OGLES::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[stream] );
                        OGLES::oglDisableVertexAttribArray( index );
                    }
                }
                break;
            case USAGE_COLOR:
                {
                    int index = oglesShaderProgram->GetInNormal();
                    if ( index >= 0 )
                    {
                        OGLES::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[stream] );
                        OGLES::oglDisableVertexAttribArray( index );
                    }
                }
                break;
            default:
                break;            		
		}
	}	
}

#endif