#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/mesh.hpp"
#include "common/engine/render/effect.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "iphone/render/ogles1.1/fixedoglvertexbuffer.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/render/shader.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/render/vertexelement.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/bone.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace FixedOGL
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
	
	static int VertexAttribUsageType[VERTEX_ATTRIB_USAGE_MAX] =
	{
#define VERTEX_ATTRIB_USAGE_ENTRY( ENUM, USAGENAME, D3DTYPE, PS3TYPE, OGLTYPE ) OGLTYPE,
		VERTEX_ATTRIB_USAGE_TUPLE
#undef VERTEX_ATTRIB_USAGE_ENTRY
	};
	
	static int PrimitiveTypeOGL[PRIMITIVE_TYPE_MAX] =
	{
#define PRIMITIVE_TYPE_ENTRY( ENUM, D3DTYPE, OGLTYPE, D3D1TYPE ) OGLTYPE,
		PRIMITIVE_TYPE_TUPLE
#undef PRIMITIVE_TYPE_ENTRY
	};	
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLVertexBuffer::FixedOGLVertexBuffer() 
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

FixedOGLVertexBuffer::~FixedOGLVertexBuffer()
{
	DELETE_PTR_ARRAY( mBufferID );
	DELETE_PTR_ARRAY( mOffset );
}

//============================================================================

void FixedOGLVertexBuffer::Create( Mesh* mesh, VertexDeclaration* vertexDeclaration )
{
	CreateVertexBuffer( mesh, vertexDeclaration );
	CreateIndexBuffer( mesh );
}

//============================================================================

void FixedOGLVertexBuffer::CreateVertexBuffer( Mesh* mesh, VertexDeclaration* vertexDeclaration )
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
		FixedOGL::oglGenBuffers( 1, &mBufferID[stream] );
		vertexData[stream] = NEW_PTR( "Data Vertices" ) char[mNumVertices * vertexDeclaration->GetStreamSize( stream ) ];
		memset( vertexData[stream], 0, sizeof( char ) * mNumVertices * vertexDeclaration->GetStreamSize( stream ) );
	}	

	mOffset = NEW_PTR( "Offset" ) int[mNumStreams];	
	memset( mOffset, 0, sizeof( int ) * mNumStreams );

	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		bool addToOffset = true;

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
		default:
			addToOffset = false;
			break;
		}

		if ( addToOffset )
		{
			mOffset[vertexElement.GetStream()] += FixedOGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
		}
	}

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		FixedOGL::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[stream] );
		FixedOGL::oglBufferData( GL_ARRAY_BUFFER, vertexDeclaration->GetStreamSize( stream ) * numVertices, vertexData[stream], GL_STATIC_DRAW );
	}

	for ( int stream = 0; stream < mNumStreams; ++stream )
	{
		DELETE_PTR_ARRAY( vertexData[stream] );
	}
	DELETE_PTR_ARRAY( vertexData );	
}

//============================================================================

void FixedOGLVertexBuffer::CreateIndexBuffer( Mesh* mesh )
{
	FixedOGL::oglGenBuffers( 1, &mIndexBuffer );
	FixedOGL::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer );

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

	FixedOGL::oglBufferData( GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, vertexData, GL_STATIC_DRAW );

	DELETE_PTR_ARRAY( vertexData );
}

//============================================================================

void FixedOGLVertexBuffer::SetOGLPointer( 
	VertexElement vertexElement, 
	VertexDeclaration* vertexDeclaration,
	EffectPass* /*effectPass*/, 
	int offset 
	)
{
	switch ( vertexElement.GetUsageType() )
	{
	case USAGE_POSITION:
		FixedOGL::oglVertexPointer( 
			vertexElement.GetNumCoordinates(), 
			FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
			vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
			(char*)0 + offset );
		break;
	case USAGE_TEXCOORD:
		FixedOGL::oglTexCoordPointer( 
			vertexElement.GetNumCoordinates(), 
			FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()],  
			vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
			(char*)0 + offset );
		break;
	case USAGE_NORMAL:
		FixedOGL::oglNormalPointer( 
			FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()], 
			vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
			(char*)0 + offset );
		break;
	case USAGE_COLOR:
		FixedOGL::oglColorPointer( 
			vertexElement.GetNumCoordinates(), 
			FixedOGL::VertexAttribFormatType[vertexElement.GetFormatType()],  
			vertexDeclaration->GetStreamSize( vertexElement.GetStream() ), 
			(char*)0 + offset );
		break;
	default:
		break;
	}
}

//============================================================================

void FixedOGLVertexBuffer::Render( 
	VertexDeclaration* vertexDeclaration, 
	EffectPass* effectPass,
	void* context 
	)
{
	UNUSED_ALWAYS( context );

	FixedOGL::oglBindBuffer( GL_ARRAY_BUFFER, 0 );
	FixedOGL::oglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	memset( mOffset, 0, sizeof( int ) * mNumStreams );
	
	std::list<VertexElement> &vertexElementList = vertexDeclaration->GetVertexElements();
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		FixedOGL::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[vertexElement.GetStream()] );

		if ( 
			( 
			vertexElement.GetUsageType() == USAGE_POSITION || 
			vertexElement.GetUsageType() == USAGE_TEXCOORD || 
			vertexElement.GetUsageType() == USAGE_NORMAL ||
			vertexElement.GetUsageType() == USAGE_COLOR
			)
			&& vertexElement.GetUsageIndex() == 0
			)
		{
			if ( vertexElement.GetUsageType() == USAGE_TEXCOORD )
			{
				FixedOGL::oglClientActiveTexture( GL_TEXTURE0 + vertexElement.GetUsageIndex() );
			}
			FixedOGL::oglEnableClientState( FixedOGL::VertexAttribUsageType[vertexElement.GetUsageType()] );			

			SetOGLPointer( 
				vertexElement, 
				vertexDeclaration,
				effectPass, 
				mOffset[vertexElement.GetStream()] 
			);
			mOffset[vertexElement.GetStream()] += FixedOGL::VertexAttribFormatSize[vertexElement.GetFormatType()];
		}
	}		

	FixedOGL::oglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	FixedOGL::oglDrawElements( 
		FixedOGL::PrimitiveTypeOGL[mPrimitiveType], 
		mNumPrimitives * mVertexPerPrimitive, 
		GL_UNSIGNED_SHORT, 
		0 
		);
		
	for ( std::list<VertexElement>::iterator iter = vertexElementList.begin(); iter != vertexElementList.end(); ++iter )
	{
		VertexElement vertexElement = *iter;

		FixedOGL::oglBindBuffer( GL_ARRAY_BUFFER, mBufferID[vertexElement.GetStream()] );
		if ( 
			(
			vertexElement.GetUsageType() == USAGE_POSITION || 
			vertexElement.GetUsageType() == USAGE_TEXCOORD || 
			vertexElement.GetUsageType() == USAGE_NORMAL ||
			vertexElement.GetUsageType() == USAGE_COLOR
			)
			&& vertexElement.GetUsageIndex() == 0
			)
		{			
			if ( vertexElement.GetUsageType() == USAGE_TEXCOORD )
			{
				FixedOGL::oglClientActiveTexture( GL_TEXTURE0 + vertexElement.GetUsageIndex() );
			}
			FixedOGL::oglDisableClientState( FixedOGL::VertexAttribUsageType[vertexElement.GetUsageType()] );		
		}
	}	
}

#endif