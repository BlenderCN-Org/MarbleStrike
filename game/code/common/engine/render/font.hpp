#ifndef FONT_HPP
#define FONT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedstring.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/rectangle.hpp"
#include "tinyxml2.h"
#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;

namespace Math
{
	class Matrix44;
}

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#define TEXT_COLOR_WHITE Math::Vector4( 1, 1, 1, 1 )
#define TEXT_COLOR_BLACK Math::Vector4( 0, 0, 0, 1 )
#define TEXT_COLOR_RED Math::Vector4( 1, 0, 0, 1 )
#define TEXT_COLOR_GREEN Math::Vector4( 0, 1, 0, 1 )
#define TEXT_COLOR_BLUE Math::Vector4( 0, 0, 1, 1 ) 

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

const int MAX_CHARS = 1024;
const int MAX_FONT_BUFFERS = 100;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum TextHorizontalAlignment
{
	TEXT_HORIZONTAL_ALIGN_LEFT,
	TEXT_HORIZONTAL_ALIGN_RIGHT,
	TEXT_HORIZONTAL_ALIGN_CENTRE
};

enum TextVerticalAlignment
{
	TEXT_VERTICAL_ALIGN_BOTTOM,
	TEXT_VERTICAL_ALIGN_TOP,
	TEXT_VERTICAL_ALIGN_CENTRE
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct FontBufferData
{
	TextHorizontalAlignment mHorizontalAlignment;
	TextVerticalAlignment mVerticalAlignment;
	Math::Vector4 mTextColor;
	System::FixedString<256> mText;
	float mScreenX;
	float mScreenY;
	float mWidth;
	float mHeight;
	float mScale;
};

struct FontCharacterData
{
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Font
{

public:
    
	Font();
	~Font();

	void Load( const char* fontFilename );
	void LoadOther( const char* fontFilename );
	void RenderBuffers( 
		const Math::Matrix44 &projectionMatrix,
		const Math::Matrix44 &transformMatrix,
		const Math::Rectangle<int> &scissorRectangle
		);

	const char* GetFontName()
	{
		return mFontName;
	}

	void SetSize( float size )
	{
		mFontSize = size;
	}

	void AddToBuffer( 
		float x, 
		float y, 
		float width, 
		float height,
		TextHorizontalAlignment horizontalAlignment, 
		TextVerticalAlignment verticalAlignment, 
		Math::Vector4 textColor, 
		const char* fmt, 
		... );    

	void ClearBuffers();

private:

	void LoadFont( tinyxml2::XMLNode* node );
	void LoadCharacter( tinyxml2::XMLNode* node );
	Math::Vector2 CalculateStartingPosition( const FontBufferData &data, int start, int end );
	int RenderText( const FontBufferData &data, int start, int end, const Math::Vector2 &startPos, int lineHeight, int* heights );

	char mFontName[64];
	int mImageWidth;
	int mImageHeight;
	int mCellWidth;
	int mCellHeight;	
	signed char mBase;
	signed char mCharWidth[256];

	FontCharacterData mCharacterData[256];

	int mNumVertices;
	int mTotalQuads;

	float mFontSize;
	Texture* mFontTexture;	
	SimpleDraw::SimpleVertex mVertexBuffer[MAX_CHARS * 6];

	int mNumFontBuffers;
	FontBufferData mFontBufferData[MAX_FONT_BUFFERS];
	
};

#endif