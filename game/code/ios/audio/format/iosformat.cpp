#ifdef PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "ios/audio/format/iosformat.hpp"
#include <vector>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	iOSFormat::iOSFormat()
	{
		mData = 0;
	}
	
	//===========================================================================

	
	iOSFormat::~iOSFormat()
	{
		DELETE_PTR_ARRAY( mData );
	}
	
	//===========================================================================
	
	void iOSFormat::Load( const char* filename )
	{
		char fullPathFilename[256] = "";
        
        const char* resourcePath = Database::Get()->GetResourcePath();
        System::StringCopy( fullPathFilename, 256, resourcePath );
        System::StringConcat( fullPathFilename, 256, filename );
		System::StringConcat( fullPathFilename, 256, ".mp4" );
		
		mData = miOSFormatInterface.LoadAllData( System::Path::SystemSlash( fullPathFilename ) );
	}
}

#endif