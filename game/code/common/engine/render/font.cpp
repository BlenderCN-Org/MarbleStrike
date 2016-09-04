//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include <string>
#include <algorithm>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Font::Font()
    :	mFontSize( 1 )
    ,	mImageWidth( 0 )
    ,	mImageHeight( 0 )
    ,	mCellWidth( 0 )
    ,	mCellHeight( 0 )
    ,	mBase( 0 )
    ,	mFontTexture( NULL )
{	
    mNumVertices = 0;
    mTotalQuads = 0;
    System::StringCopy( mFontName, 64, "" );
    mNumFontBuffers = 0;
}

//===========================================================================

Font::~Font()
{	
    if ( mFontTexture )
    {
        GameApp::Get()->GetTextureManager()->RemoveTexture( mFontTexture->GetName() );
    }
}

//===========================================================================

void Font::Load( const char* fontFilename )
{		
    System::StringCopy( mFontName, 64, fontFilename );

    char buffer[256];
    System::StringCopy( buffer, 256, "font\\" );
    System::StringConcat( buffer, 256, fontFilename );
    System::StringConcat( buffer, 256, ".dat" );
    System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );
    char filenameFullPath[256];
    System::StringCopy( filenameFullPath, 256, "" );
    const char* resourcePath = Database::Get()->GetResourcePath();
    System::StringCopy( filenameFullPath, 256, resourcePath );
    System::StringConcat( filenameFullPath, 256, loadPath );

    FILE* file = System::OpenFile( filenameFullPath, "rb" );
    int fileSize = System::GetFileSize( file );
    char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];
    char* bufferStart = dataBuffer;
    fread( dataBuffer, sizeof( char ), fileSize, file );
    fclose( file );
	file = NULL;

    System::ReadInteger( dataBuffer, mImageWidth, true );
    System::ReadInteger( dataBuffer, mImageHeight, true );
    System::ReadInteger( dataBuffer, mCellWidth, true );
    System::ReadInteger( dataBuffer, mCellHeight, true );
    System::ReadByte( dataBuffer, mBase, true );

    memcpy( mCharWidth, dataBuffer, sizeof( signed char ) * 256 );

    DELETE_PTR_ARRAY( bufferStart );

    System::StringCopy( buffer, 256, "font\\textures\\" );
    System::StringConcat( buffer, 256, fontFilename );	
    mFontTexture = GameApp::Get()->GetTextureManager()->GetTexture( buffer );	
}

//===========================================================================

void Font::LoadOther( const char* fontFilename )
{
    System::StringCopy( mFontName, 64, fontFilename );

    char buffer[256];
    System::StringCopy( buffer, 256, "font\\" );
    System::StringConcat( buffer, 256, fontFilename );
    System::StringConcat( buffer, 256, ".xml" );
    System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );
    char filenameFullPath[256];
    System::StringCopy( filenameFullPath, 256, "" );
    const char* resourcePath = Database::Get()->GetResourcePath();
    System::StringCopy( filenameFullPath, 256, resourcePath );
    System::StringConcat( filenameFullPath, 256, loadPath );

    System::StringCopy( buffer, 256, "font\\textures\\" );
    System::StringConcat( buffer, 256, fontFilename );	
    mFontTexture = GameApp::Get()->GetTextureManager()->GetTexture( buffer );	

    mImageWidth = mFontTexture->GetWidth();
    mImageHeight = mFontTexture->GetHeight();

    FILE* file = System::OpenFile( filenameFullPath, "rb" );
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError loadOkay = doc.LoadFile( file );
	fclose( file );
	file = NULL;

    Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
    if ( loadOkay == tinyxml2::XML_SUCCESS )
    {
        tinyxml2::XMLNode* node = &doc;

        for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
        {
            tinyxml2::XMLElement* element = currentNode->ToElement();
            if ( element )
            {	
                const char* elementValue = element->Value();
                if ( strcmp( elementValue, "fontMetrics" ) == 0 )
                {
                    LoadFont( currentNode );
                }
            }
        }
    }
}

//===========================================================================

Math::Vector2 Font::CalculateStartingPosition( const FontBufferData &data, int start, int end )
{
	Math::Vector2 startPos;
	float totalStringWidth = 0;
	
	switch ( data.mHorizontalAlignment )
	{
	case TEXT_HORIZONTAL_ALIGN_CENTRE:
		{
			for ( int i = start; i < end; ++i )
			{			
				int ascii = static_cast<int>( data.mText.GetRawData()[i] );
				FontCharacterData characterData = mCharacterData[ascii];
				float width = static_cast<float>( characterData.mWidth );
				totalStringWidth += width * data.mScale;
			}

			startPos[0] = ( data.mWidth / 2.0f ) - ( totalStringWidth / 2.0f );
		}
		break;
	case TEXT_HORIZONTAL_ALIGN_RIGHT:
		{
			for ( int i = start; i < end; ++i )
			{			
				int ascii = static_cast<int>( data.mText.GetRawData()[i] );
				FontCharacterData characterData = mCharacterData[ascii];
				float width = static_cast<float>( characterData.mWidth );
				totalStringWidth += width * data.mScale;
			}

			startPos[0] = data.mWidth - totalStringWidth;
		}
		break;
	default:
		break;
	}

	switch ( data.mVerticalAlignment )		
	{
	case TEXT_VERTICAL_ALIGN_CENTRE:
		{
			int ascii = static_cast<int>( data.mText.GetRawData()[0] );
			FontCharacterData characterData = mCharacterData[ascii];
			float height = static_cast<float>( characterData.mHeight );
			startPos[1] = ( data.mHeight / 2.0f ) - ( height * data.mScale / 2.0f );
		}
		break;
	case TEXT_VERTICAL_ALIGN_TOP:
		{
			int ascii = static_cast<int>( data.mText.GetRawData()[0] );
			FontCharacterData characterData = mCharacterData[ascii];
			float height = static_cast<float>( characterData.mHeight );
			startPos[1] = ( data.mHeight ) - ( height * data.mScale );
		}
		break;
	default:
		break;
	}

	return startPos;
}

//===========================================================================

int Font::RenderText( const FontBufferData &data, int start, int end, const Math::Vector2 &startPos, int lineHeight, int* heights )
{
	float imageWidth = static_cast<float>( mImageWidth );
    float imageHeight = static_cast<float>( mImageHeight );
    int numQuads = 0;

	Math::Vector2 currentPos = startPos + Math::Vector2( data.mScreenX, data.mScreenY );

	for ( int heightIndex = 0; heightIndex < lineHeight; heightIndex++ )
	{
		currentPos[1] -= static_cast<float>( heights[heightIndex] ) * data.mScale;
	}

	for ( int i = start; i < end; ++i )
	{			
		int ascii = static_cast<int>( data.mText.GetRawData()[i] );
				
		FontCharacterData characterData = mCharacterData[ascii];
		float cx = static_cast<float>( characterData.mX );
		float cy = static_cast<float>( characterData.mY );
		float width = static_cast<float>( characterData.mWidth );
		float height = static_cast<float>( characterData.mHeight );

		float texX1 = cx / imageWidth;
		float texX2 = ( cx + width ) / imageWidth;

		float texY1 = ( imageHeight - ( cy + height ) ) / imageHeight;
		float texY2 = ( imageHeight - cy ) / imageHeight;

		float widthSize = width * data.mScale;
		float heightSize = height * data.mScale;

		float xPos1 = currentPos[0];
		float xPos2 = currentPos[0] + widthSize;

		currentPos[0] += widthSize;

		float yPos1 = currentPos[1];
		float yPos2 = currentPos[1] + heightSize;

		Math::Vector4 color = data.mTextColor;

		Renderer::Get()->ModifyUV( texX1, texY1 );
		Renderer::Get()->ModifyUV( texX2, texY2 );

		mVertexBuffer[mNumVertices].mPosition.Set( xPos1, yPos1, 0 );
		mVertexBuffer[mNumVertices].mTexCoord.Set( texX1, texY1 );
		mVertexBuffer[mNumVertices].mColor = color;
		mNumVertices++;

		mVertexBuffer[mNumVertices].mPosition.Set( xPos2, yPos1, 0 );
		mVertexBuffer[mNumVertices].mTexCoord.Set( texX2, texY1 );
		mVertexBuffer[mNumVertices].mColor = color;
		mNumVertices++;

		mVertexBuffer[mNumVertices].mPosition.Set( xPos2, yPos2, 0 );
		mVertexBuffer[mNumVertices].mTexCoord.Set( texX2, texY2 );
		mVertexBuffer[mNumVertices].mColor = color;
		mNumVertices++;

		mVertexBuffer[mNumVertices].mPosition.Set( xPos1, yPos1, 0 );
		mVertexBuffer[mNumVertices].mTexCoord.Set( texX1, texY1 );
		mVertexBuffer[mNumVertices].mColor = color;
		mNumVertices++;

		mVertexBuffer[mNumVertices].mPosition.Set( xPos2, yPos2, 0 );
		mVertexBuffer[mNumVertices].mTexCoord.Set( texX2, texY2 );
		mVertexBuffer[mNumVertices].mColor = color;
		mNumVertices++;

		mVertexBuffer[mNumVertices].mPosition.Set( xPos1, yPos2, 0 );
		mVertexBuffer[mNumVertices].mTexCoord.Set( texX1, texY2 );
		mVertexBuffer[mNumVertices].mColor = color;
		mNumVertices++;

		numQuads++;
		mTotalQuads++;

		Assert( mTotalQuads < MAX_CHARS, "Too many font characters!" );
	}

	return numQuads;
}

//===========================================================================

void Font::RenderBuffers( 
    const Math::Matrix44 &projectionMatrix,
    const Math::Matrix44 &transformMatrix,
    const Math::Rectangle<int> &scissorRectangle
    )
{    
	const int MAX_HEIGHT = 50;

    int startIndex = mNumVertices;	
	int numQuads = 0;
	int heights[MAX_HEIGHT];

    for ( int bufferIndex = 0; bufferIndex < mNumFontBuffers; ++bufferIndex)
    {	
        int start = 0;
	    int end = 0;
	    int lineHeight = 0;

        FontBufferData data = mFontBufferData[bufferIndex];

        int length = data.mText.GetSize();			

		while ( start < length )
		{
			end = length;
			for ( int i = start; i < length; ++i )
			{			
				int ascii = static_cast<int>( data.mText.GetRawData()[i] );
				if ( ascii == '\n' )
				{
					end = i;
					break;
				}
			}

			Math::Vector2 startPos;
			startPos = CalculateStartingPosition( data, start, end );

			int tallestCharacter = 0;
			for ( int i = start; i < end; ++i )
			{			
				int ascii = static_cast<int>( data.mText.GetRawData()[i] );
				FontCharacterData characterData = mCharacterData[ascii];
				int height = characterData.mHeight;
				if ( height > tallestCharacter )
				{
					tallestCharacter = height;
				}
			}

			if ( tallestCharacter == 0 && lineHeight > 0 )
			{
				heights[lineHeight] = heights[lineHeight-1];
			}
			else
			{
				heights[lineHeight] = tallestCharacter;
			}

			numQuads += RenderText( data, start, end, startPos, lineHeight, heights );

			lineHeight++;
			Assert( lineHeight < MAX_HEIGHT, "" );
			start = end + 1;
		}
    }

    if ( numQuads > 0 )
    {		
        Math::Matrix44 identity;
        SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
        simpleDraw->SetProjectionMatrix( projectionMatrix );
        simpleDraw->SetViewMatrix( identity );
        simpleDraw->SetTransformMatrix( transformMatrix );
        simpleDraw->SetColorTexture( mFontTexture );	
        simpleDraw->SetScissorRectangle( scissorRectangle );		
        simpleDraw->RenderMesh( &mVertexBuffer[startIndex], numQuads * 2, PRIMITIVE_TRIANGLES );
    }

    mNumFontBuffers = 0;    
}

//===========================================================================

void Font::AddToBuffer( 
    float x, 
    float y, 
    float width,
    float height,
    TextHorizontalAlignment horizontalAlignment, 
    TextVerticalAlignment verticalAlignment,  
    Math::Vector4 textColor, 
    const char* format, 
    ... )
{   
    char text[1024];
    va_list ap;

    if ( format == NULL )
    {
        *text = 0;
    }
    else 
    {
        va_start( ap, format );
#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
        vsprintf_s( text, 1024, format, ap );
#else
        vsprintf( text, format, ap );
#endif
        va_end( ap );
    }

    FontBufferData newFontBufferData;
    newFontBufferData.mHorizontalAlignment = horizontalAlignment;
    newFontBufferData.mVerticalAlignment = verticalAlignment;
    newFontBufferData.mTextColor = textColor;
    newFontBufferData.mScreenX = x;
    newFontBufferData.mScreenY = y;
    newFontBufferData.mWidth = width;
    newFontBufferData.mHeight = height;
    newFontBufferData.mText = text;
    newFontBufferData.mScale = mFontSize;

    mFontBufferData[mNumFontBuffers] = newFontBufferData;
    mNumFontBuffers++;
}

//===========================================================================

void Font::ClearBuffers()
{
    mTotalQuads = 0;
    mNumVertices = 0;
    mNumFontBuffers = 0;
}

//===========================================================================

void Font::LoadFont( tinyxml2::XMLNode* node )
{
    for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
    {
        tinyxml2::XMLElement* element = currentNode->ToElement();
        if ( element )
        {	
            const char* elementValue = element->Value();
            if ( strcmp( elementValue, "character" ) == 0 )
            {
                LoadCharacter( currentNode );
            }
        }
    }
}

//===========================================================================

void Font::LoadCharacter( tinyxml2::XMLNode* node )
{
    tinyxml2::XMLElement* element = node->ToElement();

    int key = element->IntAttribute( "key" );

    FontCharacterData data;
    data.mX = 0;
    data.mY = 0;
    data.mWidth = 0;
    data.mHeight = 0;

    for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
    {
        tinyxml2::XMLElement* element = currentNode->ToElement();
        if ( element )
        {	
            const char* elementValue = element->Value();
            if ( strcmp( elementValue, "x" ) == 0 )
            {				
                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                data.mX = atoi( str.c_str() );
            }
            else if ( strcmp( elementValue, "y" ) == 0 )
            {				
                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                data.mY = atoi( str.c_str() );
            }
            else if ( strcmp( elementValue, "width" ) == 0 )
            {				
                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                data.mWidth = atoi( str.c_str() );
            } 
            else if ( strcmp( elementValue, "height" ) == 0 )
            {				
                const char* text = element->GetText();

                std::string str = text;
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                data.mHeight = atoi( str.c_str() );
            }
        }
    }

    mCharacterData[key] = data;
}
