//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/math/rectangle.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FontManager::FontManager()
{	
}

//===========================================================================

FontManager::~FontManager()
{
	int numFonts = static_cast<int>( mFontList.size() );
	for ( int i = 0; i < numFonts; ++i )
	{
		Font* currentFont = mFontList[i];
		DELETE_PTR( currentFont );
	}
}

//===========================================================================

int FontManager::AddFont( const char* filename )
{
	int numFonts = static_cast<int>( mFontList.size() );
	for ( int i = 0; i < numFonts; ++i )
	{
		Font* currentFont = mFontList[i];
		if ( strcmp( currentFont->GetFontName(), filename ) == 0 )
		{
			return i;
		}
	}

	Font* newFont = NEW_PTR(  "Font" ) Font;
	newFont->LoadOther( filename );

	mFontList.push_back( newFont );

	int index = static_cast<int>( mFontList.size() ) - 1;
	return index;
}

//===========================================================================

Font* FontManager::GetFont( int index )
{
	Assert( index >= 0 && index <static_cast<int>( mFontList.size() ), "" );
	return mFontList[index];
}

//===========================================================================

void FontManager::RenderFonts( Math::Matrix44 orthoMatrix, Math::Matrix44 transform )
{
	int numFonts = static_cast<int>( mFontList.size() );
	for ( int i = 0; i < numFonts; ++i )
	{
		Font* currentFont = mFontList[i];

		Math::Rectangle<int> scissorRectangle;
		scissorRectangle.SetPosition( 0, 0 );
		scissorRectangle.SetWidth( Database::Get()->GetBackBufferWidth() );
		scissorRectangle.SetHeight( Database::Get()->GetBackBufferHeight() );
		currentFont->RenderBuffers( orthoMatrix, transform, scissorRectangle );
	}
}

//===========================================================================

void FontManager::ClearBuffers()
{
	int numFonts = static_cast<int>( mFontList.size() );
	for ( int i = 0; i < numFonts; ++i )
	{
		Font* currentFont = mFontList[i];		
		currentFont->ClearBuffers();
	}
}