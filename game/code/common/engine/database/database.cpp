//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/pixelshader.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/vertexshader.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/pathutilities.hpp"

//////////////////////////////////////////////////////
// STATICS
//////////////////////////////////////////////////////

Database* Database::mInstance = NULL;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

void Database::Create()
{
    Assert( mInstance == NULL, "Data already allocated." )

        if ( mInstance == NULL )
        {  
            mInstance = NEW_PTR( "Database Instance" ) Database;      
        }	
}

//===========================================================================

Database* Database::Get()
{
    Assert( mInstance != NULL, "Data not allocated." )

        return mInstance;
}

//===========================================================================

void Database::Terminate()
{
    DELETE_PTR( mInstance );
}

//===========================================================================

unsigned int Database::GetBackBufferWidth() const
{
    return mCurrentBackBufferWidth;
}

//===========================================================================

unsigned int Database::GetBackBufferHeight() const
{
    return mCurrentBackBufferHeight;
}

//===========================================================================

void Database::SetBackBufferWidth( unsigned int width )
{
    mCurrentBackBufferWidth = width;
}

//===========================================================================

void Database::SetBackBufferHeight( unsigned int height )
{
    mCurrentBackBufferHeight = height;
}

//===========================================================================

int Database::GetAppScreenWidth()
{
    return mCurrentAppScreenWidth;
}

//===========================================================================

int Database::GetAppScreenHeight()
{
    return mCurrentAppScreenHeight;
}

//===========================================================================

void Database::SetAppScreenWidth( int width )
{
    mCurrentAppScreenWidth = width;
}

//===========================================================================

void Database::SetAppScreenHeight( int height )
{
    mCurrentAppScreenHeight = height;
}

//===========================================================================

int Database::GetTouchScreenWidth()
{
    return mCurrentTouchScreenWidth;
}

//===========================================================================

int Database::GetTouchScreenHeight()
{
    return mCurrentTouchScreenHeight;
}

//===========================================================================

void Database::SetTouchScreenWidth( int width )
{
    mCurrentTouchScreenWidth = width;
}

//===========================================================================

void Database::SetTouchScreenHeight( int height )
{
    mCurrentTouchScreenHeight = height;
}

//===========================================================================

void Database::SetIsLandscape( bool value )
{
    mIsLandscape = value;
}

//===========================================================================

bool Database::GetIsLandscape()
{
    return mIsLandscape;
}

//===========================================================================

void Database::SetIsRetinaDisplay( bool value )
{
    mIsRetinaDisplay = value;
}

//===========================================================================

bool Database::GetIsRetinaDisplay()
{
    return mIsRetinaDisplay;
}

//===========================================================================

void Database::SetResourcePath( const char* path )
{
    System::StringCopy( mResourcePath, 256, path );
}

//===========================================================================

const char* Database::GetResourcePath()
{
    return mResourcePath;
}

//===========================================================================

void Database::SetLibraryPath( const char* path )
{
    System::StringCopy( mLibraryPath, 256, path );
}

//===========================================================================

const char* Database::GetLibraryPath()
{
    return mLibraryPath;
}


//===========================================================================

void Database::SetShaderResourcePath( const char* path )
{
    System::StringCopy( mShaderResourcePath, 256, path );
}

//===========================================================================

const char* Database::GetShaderResourcePath()
{
    return mShaderResourcePath;
}

//===========================================================================

void Database::SetVolume( float volume )
{
    mCurrentVolume = volume;
    if ( mCurrentVolume > 1.0f )
    {
        mCurrentVolume = 1.0f;
    }
    else if ( mCurrentVolume < 0.0f )
    {
        mCurrentVolume = 0.0f;
    }
}

//===========================================================================

float Database::GetVolume()
{
    return mCurrentVolume;
}

//===========================================================================

float Database::GetXScreenScaleFactor()
{
    float appScreenWidth = static_cast<float>( mCurrentAppScreenWidth );
    float backBufferScreenWidth = static_cast<float>( mCurrentBackBufferWidth );	
    float xScale = ( backBufferScreenWidth / appScreenWidth );

    return xScale;
}

//===========================================================================

float Database::GetYScreenScaleFactor()
{	
    float appScreenHeight = static_cast<float>( mCurrentAppScreenHeight );
    float backBufferScreenHeight = static_cast<float>( mCurrentBackBufferHeight );	
    float yScale = ( backBufferScreenHeight  / appScreenHeight);

    return yScale;
}

//===========================================================================

Database::Database()
    :   mCurrentBackBufferWidth( 0 )
    ,	mCurrentBackBufferHeight( 0 )
    ,	mCurrentAppScreenWidth( 0 )
    ,   mCurrentAppScreenHeight( 0 )
    ,   mCurrentTouchScreenWidth( 0 )
    ,	mCurrentTouchScreenHeight( 0 )
    ,	mIsLandscape( false )
    ,	mIsRetinaDisplay( false )
    ,   mWasSuspended( false )
    ,   mCurrentVolume( 1.0f )
{
    System::StringCopy( mResourcePath, 256, "" );
    System::StringCopy( mLibraryPath, 256, "" );
    System::StringCopy( mShaderResourcePath, 256, "" );
}

//===========================================================================

Database::~Database()
{
}

//===========================================================================

void Database::LoadStartupData( const char* filename )
{	
    char buffer[256];
    System::StringCopy( buffer, 256, filename );
    System::StringConcat( buffer, 256, ".xml" );
    System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );

    char filenameFullPath[256];
    const char* resourcePath = Database::Get()->GetResourcePath();
    System::StringCopy( filenameFullPath, 256, "" );
    System::StringConcat( filenameFullPath, 256, resourcePath );
    System::StringConcat( filenameFullPath, 256, loadPath );

    tinyxml2::XMLDocument doc;

    FILE* file = System::OpenFile( filenameFullPath, "rb" );
    tinyxml2::XMLError loadOkay = doc.LoadFile( file );
	fclose( file );
	file = NULL;
    Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
    if ( loadOkay == tinyxml2::XML_SUCCESS )
    {	
        tinyxml2::XMLNode* node = &doc;
        LoadStartupDataXML( node );
    }	
}

//===========================================================================

void Database::LoadStartupDataXML( tinyxml2::XMLNode* node )
{
    for ( tinyxml2::XMLNode* item = node; item != NULL; item = item->NextSibling() )
    {
        tinyxml2::XMLElement* element = item->ToElement();
        if ( element )
        {
            const char* elementValue = element->Value();
            if ( strcmp( elementValue, "port" ) == 0 )
            {
                int value = element->IntAttribute( "value" );
                mStartupData.mPort = value;
            }
            else if ( strcmp( elementValue, "ipaddress" ) == 0 )
            {					
                const char* ipAddress = element->Attribute( "value" );
                System::StringCopy( mStartupData.mIPAddress, 32, ipAddress );
            }
            else if ( strcmp( elementValue, "enableaudio" ) == 0 )
            {
                int value = element->IntAttribute( "value" );

                mStartupData.mEnableAudio = false;
                if ( value == 1 )
                {
                    mStartupData.mEnableAudio = true;
                }
            }
            else if ( strcmp( elementValue, "showfps" ) == 0 )
            {
                int value = element->IntAttribute( "value" );

                mStartupData.mShowFPS = false;
                if ( value == 1 )
                {
                    mStartupData.mShowFPS = true;
                }
            }
            else if ( strcmp( elementValue, "showmem" ) == 0 )
            {
                int value = element->IntAttribute( "value" );

                mStartupData.mShowMem = false;
                if ( value == 1 )
                {
                    mStartupData.mShowMem = true;
                }
            }
            else if ( strcmp( elementValue, "showcursor" ) == 0 )
            {
                int value = element->IntAttribute( "value" );

                mStartupData.mShowCursor = false;
                if ( value == 1 )
                {
                    mStartupData.mShowCursor = true;
                }
            }
        }    

        tinyxml2::XMLNode* child = item->FirstChild();
        if ( child != NULL )
        {
            LoadStartupDataXML( child );
        }
    }
}