#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 ) || defined( PLAT_ANDROID )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/ogginterface.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	OggInterface::OggInterface()
		:	mVorbisInfo( NULL )
		,	mOggVorbisFile( NULL )
		,	mFile( NULL )
	{	
	}

	//===========================================================================

	OggInterface::~OggInterface()
	{
		Clear();
	}

	//===========================================================================

	void OggInterface::Clear()
	{
		if ( mOggVorbisFile != NULL )
		{
			ov_clear( mOggVorbisFile );
		}
		DELETE_PTR( mOggVorbisFile );

		if ( mFile != NULL )
		{
			fclose( mFile );
			mFile = NULL;
		}

		mVorbisInfo = NULL;
	}

	//===========================================================================

	OggVorbis_File* OggInterface::Load( const char* filename )
	{
		Clear();

		char fullPathFilename[256] = "";
		const char* resourcePath = Database::Get()->GetResourcePath();
		System::StringCopy( fullPathFilename, 256, resourcePath );
		System::StringConcat( fullPathFilename, 256, System::Path::SystemSlash( filename ) );
		System::StringConcat( fullPathFilename, 256, ".ogg" );

		mFile = System::OpenFile( fullPathFilename, "rb" );
		Assert( mFile != NULL, "" );

		mOggVorbisFile = NEW_PTR( "Ogg Vorbis File" ) OggVorbis_File;

		ov_open( mFile, mOggVorbisFile, NULL, 0 );
		mVorbisInfo = ov_info( mOggVorbisFile, -1 );

		return mOggVorbisFile;
	}
}

#endif
