//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/bone.hpp"
#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Bone::Bone()
{
	System::StringCopy( mName, 32, "" );
}

//===========================================================================

Bone::~Bone()
{
}

//===========================================================================

void Bone::SetName( const char* name )
{
	System::StringCopy( mName, 32, name );
}