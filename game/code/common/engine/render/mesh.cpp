//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/math/intersections.hpp"
#include "common/engine/math/quaternion.hpp"
#include "common/engine/math/triangle.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/vertexbuffer.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/profiler.hpp"
#include "common/engine/system/utilities.hpp"
#include <algorithm>
#include <string>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Mesh::Mesh() 
    :   mPrimitiveType( PRIMITIVE_TRIANGLES )
    ,	mNumVertices( 0 )
    ,	mNumPrimitives( 0 )
    ,	mPositions( NULL )
    ,	mNumPositions( 0 )
    ,	mPositionsStride( 0 )
    ,	mNormals( NULL )
    ,	mNumNormals( 0 )
    ,	mNormalsStride( 0 )
    ,	mTexCoords( NULL )
    ,	mNumTexCoords( 0 )
    ,	mTexCoordsStride( 0 )
    ,	mTangents( NULL )
    ,	mNumTangents( 0 )
    ,	mTangentsStride( 0 )
    ,	mBinormals( NULL )
    ,	mNumBinormals( 0 )
    ,	mBinormalsStride( 0 )
    ,	mColors( NULL )
    ,	mNumColors( 0 )
    ,	mColorsStride( 0 )
    ,	mFaceIndices( NULL )
    ,	mNumFaceIndices( 0 )
    ,	mFaceIndicesStride( 0 )
    ,	RenderObjectData( RENDER_OBJECT_DATA_TYPE_MESH )
{
    System::StringCopy( mMeshName, 32, "None" );
    System::StringCopy( mMaterialName, 32, "None" );	
}

//============================================================================

Mesh::~Mesh()
{
    int numVertexBuffers = mVertexBuffers.GetSize();
    for ( int i = 0; i < numVertexBuffers; ++i )
    {
        DELETE_PTR( mVertexBuffers[i] );
    }

    DELETE_PTR_ARRAY( mPositions );
    DELETE_PTR_ARRAY( mNormals );
    DELETE_PTR_ARRAY( mTexCoords );
    DELETE_PTR_ARRAY( mTangents );
    DELETE_PTR_ARRAY( mBinormals );
    DELETE_PTR_ARRAY( mColors );
    DELETE_PTR_ARRAY( mFaceIndices );
}

//===========================================================================

void Mesh::Update( float /*elapsedTime*/ )
{
}

//============================================================================

void Mesh::TransformVertices( const Math::Matrix44 &transform )
{	
    int numVertices = mNumPositions / mPositionsStride;
    for ( int i = 0; i < numVertices; ++i )
    {
        int index0 = i * mPositionsStride;
        int index1 = i * mPositionsStride + 1;
        int index2 = i * mPositionsStride + 2;

        Math::Vector3 position( 
            mPositions[index0],
            mPositions[index1],
            mPositions[index2]
        );

        position = transform * position;

        mPositions[index0] = position[0];
        mPositions[index1] = position[1];
        mPositions[index2] = position[2];
    }

    int numNormals = mNumNormals / mNormalsStride;
    for ( int i = 0; i < numNormals; ++i )
    {
        int index0 = i * mNormalsStride;
        int index1 = i * mNormalsStride + 1;
        int index2 = i * mNormalsStride + 2;

        Math::Vector3 normal( 
            mNormals[index0],
            mNormals[index1],
            mNormals[index2]
        );

        normal = transform * normal;

        mNormals[index0] = normal[0];
        mNormals[index1] = normal[1];
        mNormals[index2] = normal[2];
    }
}

//============================================================================

void Mesh::SetPositions( Math::Vector3* positions, int size )
{
    Assert( mPositions == NULL, "" );

    mPositionsStride = 3;
    mNumPositions = size * mPositionsStride;	
    mPositions = NEW_PTR( "Positions" ) float[mNumPositions];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mPositions[index] = positions[i][0];
        index++;
        mPositions[index] = positions[i][1];
        index++;
        mPositions[index] = positions[i][2];
        index++;
    }
}

//============================================================================

void Mesh::AddPositions( int size )
{
    Assert( mPositions == NULL, "" );

    mPositionsStride = 3;
    mNumPositions = size * mPositionsStride;	
    mPositions = NEW_PTR( "Positions" ) float[mNumPositions];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mPositions[index] = 0;
        index++;
        mPositions[index] = 0;
        index++;
        mPositions[index] = 0;
        index++;
    }
}

//============================================================================

void Mesh::SetNormals( Math::Vector3* normals, int size )
{
    Assert( mNormals == NULL, "" );

    mNormalsStride = 3;
    mNumNormals = size * mNormalsStride;	
    mNormals = NEW_PTR( "Normals" ) float[mNumNormals];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mNormals[index] = normals[i][0];
        index++;
        mNormals[index] = normals[i][1];
        index++;
        mNormals[index] = normals[i][2];
        index++;
    }
}

//============================================================================

void Mesh::AddNormals( int size )
{
    Assert( mNormals == NULL, "" );

    mNormalsStride = 3;
    mNumNormals = size * mNormalsStride;	
    mNormals = NEW_PTR( "Normals" ) float[mNumNormals];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mNormals[index] = 0;
        index++;
        mNormals[index] = 0;
        index++;
        mNormals[index] = 0;
        index++;
    }
}

//============================================================================

void Mesh::SetTexCoords( Math::Vector2* texCoords, int size )
{
    Assert( mTexCoords == NULL, "" );

    mTexCoordsStride = 2;
    mNumTexCoords = size * mTexCoordsStride;	
    mTexCoords = NEW_PTR( "TexCoords" ) float[mNumTexCoords];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mTexCoords[index] = texCoords[i][0];
        index++;
        mTexCoords[index] = texCoords[i][1];
        index++;
    }
}

//============================================================================

void Mesh::AddTexCoords( int size )
{
    Assert( mTexCoords == NULL, "" );

    mTexCoordsStride = 2;
    mNumTexCoords = size * mTexCoordsStride;	
    mTexCoords = NEW_PTR( "TexCoords" ) float[mNumTexCoords];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mTexCoords[index] = 0;
        index++;
        mTexCoords[index] = 0;
        index++;
    }
}

//============================================================================

void Mesh::SetColors( Math::Vector4* colors, int size )
{
    Assert( mColors == NULL, "" );

    mColorsStride = 4;
    mNumColors = size * mColorsStride;	
    mColors = NEW_PTR( "Colors" ) float[mNumColors];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mColors[index] = colors[i][0];
        index++;
        mColors[index] = colors[i][1];
        index++;
        mColors[index] = colors[i][2];
        index++;
        mColors[index] = colors[i][3];
        index++;
    }
}

//============================================================================

void Mesh::AddColors( int size )
{
    Assert( mColors == NULL, "" );

    mColorsStride = 4;
    mNumColors = size * mColorsStride;	
    mColors = NEW_PTR( "Colors" ) float[mNumColors];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mColors[index] = 0;
        index++;
        mColors[index] = 0;
        index++;
        mColors[index] = 0;
        index++;
        mColors[index] = 0;
        index++;
    }
}

//============================================================================

void Mesh::SetTangents( Math::Vector3* tangents, int size )
{
    Assert( mTangents == NULL, "" );

    mTangentsStride = 3;
    mNumTangents = size * mTangentsStride;	
    mTangents = NEW_PTR( "Tangents" ) float[mNumTangents];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mTangents[index] = tangents[i][0];
        index++;
        mTangents[index] = tangents[i][1];
        index++;
        mTangents[index] = tangents[i][2];
        index++;
    }
}

//============================================================================

void Mesh::AddTangents( int size )
{
    Assert( mTangents == NULL, "" );

    mTangentsStride = 3;
    mNumTangents = size * mTangentsStride;	
    mTangents = NEW_PTR( "Tangents" ) float[mNumTangents];

    int index = 0;
    for ( int i = 0; i < size; ++i )
    {
        mTangents[index] = 0;
        index++;
        mTangents[index] = 0;
        index++;
        mTangents[index] = 0;
        index++;
    }
}

//============================================================================

void Mesh::SetFaceIndices( unsigned int* indices, int numPrimitives, int vertexPerPrimitive )
{
    Assert( mFaceIndices == NULL, "" );

    mNumFaceIndices = numPrimitives * vertexPerPrimitive;
    mFaceIndices = NEW_PTR( "Face Indices" ) unsigned int[mNumFaceIndices];
    mFaceIndicesStride = vertexPerPrimitive;
    mNumPrimitives = numPrimitives;

    for ( int i = 0; i < mNumFaceIndices; ++i )
    {
        mFaceIndices[i] = indices[i];
    }	
}

//============================================================================

VertexBuffer* Mesh::CreateVertexBuffer()
{
    VertexBuffer* vertexBuffer = Renderer::Get()->GetRenderFactory()->CreateVertexBuffer();
    mVertexBuffers.PushBack( vertexBuffer );
    return vertexBuffer;
}

//===========================================================================

void Mesh::Load( tinyxml2::XMLNode* node )
{	
    for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
    {
        tinyxml2::XMLElement* element = currentNode->ToElement();
        const char* elementValue = element->Value();
        if ( strcmp( elementValue, "mesh" ) == 0 )
        {
            mNumVertices = element->IntAttribute( "vertexcount" );

            const char* meshName = element->Attribute( "name" );
            System::StringCopy( mMeshName, 32, meshName );

            const char* drawType = element->Attribute( "draw_type" );
            if ( drawType && strcmp( drawType, "WIRE" ) == 0 )
            {
                mPrimitiveType = PRIMITIVE_LINES;
            }

            const char* materialName = element->Attribute( "material" );
            if ( materialName )
            {
                System::StringCopy( mMaterialName, 32, materialName );
            }

            LoadMesh( currentNode );
        }		
    }
}

//===========================================================================

void Mesh::LoadMesh( tinyxml2::XMLNode* node )
{
    for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
    {
        tinyxml2::XMLElement* element = currentNode->ToElement();
        if ( element )		
        {            
            const char* elementValue = element->Value();

            if ( strcmp( elementValue, "pos" ) == 0 )
            {					
                mPositionsStride = element->IntAttribute( "stride" );
                mNumPositions = mNumVertices * mPositionsStride;
                mPositions = NEW_PTR( "Positions" ) float[mNumPositions];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumPositions, "" );
                    mPositions[i] = static_cast<float>( atof( token ) );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }
            }
            else if ( strcmp( elementValue, "nor" ) == 0 )
            {											
                mNormalsStride = element->IntAttribute( "stride" );
                mNumNormals = mNumVertices * mNormalsStride;
                mNormals = NEW_PTR( "Normals" ) float[mNumNormals];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumNormals, "" );
                    mNormals[i] = static_cast<float>( atof( token ) );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }
            }
            else if ( strcmp( elementValue, "tex" ) == 0 )
            {	
                mTexCoordsStride = element->IntAttribute( "stride" );
                mNumTexCoords = mTexCoordsStride * mNumVertices;
                mTexCoords = NEW_PTR( "TexCoords" ) float[mNumTexCoords];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumTexCoords, "" );
                    mTexCoords[i] = static_cast<float>( atof( token ) );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }
            }
            else if ( strcmp( elementValue, "tan" ) == 0 )
            {	
                mTangentsStride = element->IntAttribute( "stride" );
                mNumTangents = mTangentsStride * mNumVertices;
                mTangents = NEW_PTR( "Tangents" ) float[mNumTangents];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumTangents, "" );
                    mTangents[i] = static_cast<float>( atof( token ) );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }
            }
            else if ( strcmp( elementValue, "binor" ) == 0 )
            {
                mBinormalsStride = element->IntAttribute( "stride" );
                mNumBinormals = mBinormalsStride * mNumVertices;
                mBinormals = NEW_PTR( "Binormals" ) float[mNumBinormals];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumBinormals, "" );
                    mBinormals[i] = static_cast<float>( atof( token ) );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }
            }
            else if ( strcmp( elementValue, "col" ) == 0 )
            {

                mColorsStride = element->IntAttribute( "stride" );
                mNumColors = mColorsStride * mNumVertices;
                mColors = NEW_PTR( "Colors" ) float[mNumColors];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumColors, "" );
                    mColors[i] = static_cast<float>( atof( token ) );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }
            }
            else if ( strcmp( elementValue, "faceidx" ) == 0 )
            {
                int count = 0;
                count = element->IntAttribute( "count" );

                mFaceIndicesStride = element->IntAttribute( "stride" );
                mNumFaceIndices = count * mFaceIndicesStride;
                mFaceIndices = NEW_PTR( "FacesIndices" ) unsigned int[mNumFaceIndices];

                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                char* tokenString = const_cast<char*>( str.c_str() );
                const char* delimiter = " ";

                int i = 0;
                char* nextToken = 0;
                char* token = System::StringToken( tokenString, delimiter, &nextToken );
                while ( token != NULL )
                {	
                    Assert( i < mNumFaceIndices, "" );
                    mFaceIndices[i] = atoi( token );
                    token = System::StringToken( NULL, delimiter, &nextToken );
                    i++;
                }

                mNumPrimitives = mNumFaceIndices / mFaceIndicesStride;
            }				
        }
    }
}

//===========================================================================

void Mesh::Copy( const Mesh &other )
{
    System::StringCopy( mMeshName, 32, other.mMeshName );
    System::StringCopy( mMaterialName, 32, other.mMaterialName );
    mPrimitiveType = other.mPrimitiveType;
    mNumVertices = other.mNumVertices;
    mNumPrimitives = other.mNumPrimitives;

    mNumPositions = other.mNumPositions;
    mPositionsStride = other.mPositionsStride;	
    Assert( mPositions == NULL, "" );
    Assert( other.mPositions != NULL, "" );
    mPositions = NEW_PTR( "Positions" ) float[mNumPositions];
    memcpy( mPositions, other.mPositions, sizeof( float ) * mNumPositions );

    mNumNormals = other.mNumNormals;
    mNormalsStride = other.mNormalsStride;
    Assert( mNormals == NULL, "" );
    Assert( other.mNormals != NULL, "" );
    mNormals = NEW_PTR( "Normals" ) float[mNumNormals];
    memcpy( mNormals, other.mNormals, sizeof( float ) * mNumNormals );

    mNumTexCoords = other.mNumTexCoords;
    mTexCoordsStride = other.mTexCoordsStride;
    Assert( mTexCoords == NULL, "" );
    Assert( other.mTexCoords != NULL, "" );
    mTexCoords = NEW_PTR( "TexCoords" ) float[mNumTexCoords];
    memcpy( mTexCoords, other.mTexCoords, sizeof( float ) * mNumTexCoords );

    mNumColors = other.mNumColors;
    mColorsStride = other.mColorsStride;
    Assert( mColors == NULL, "" );
    Assert( other.mColors != NULL, "" );
    mColors = NEW_PTR( "Colors" ) float[mNumColors];
    memcpy( mColors, other.mColors, sizeof( float ) * mNumColors );

    mNumFaceIndices = other.mNumFaceIndices;
    mFaceIndicesStride = other.mFaceIndicesStride;
    Assert( mFaceIndices == NULL, "" );
    Assert( other.mFaceIndices != NULL, "" );
    mFaceIndices = NEW_PTR( "FaceIndices" ) unsigned int[mNumFaceIndices];
    memcpy( mFaceIndices, other.mFaceIndices, sizeof( unsigned int ) * mNumFaceIndices );
}