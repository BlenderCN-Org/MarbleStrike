//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "copydatamanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

CopyDataManager* CopyDataManager::mInstance = NULL;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

CopyDataManager* CopyDataManager::GetInstance()
{
    Assert( mInstance != NULL, "" );
    return mInstance;
}

//===========================================================================

void CopyDataManager::CreateCopyDataManager()
{
    if ( mInstance == NULL )
    {
        mInstance = NEW_PTR( "CopyDataManager" ) CopyDataManager;
    }
}

//===========================================================================

void CopyDataManager::DestroyCopyDataManager()
{
    if ( mInstance )
    {
        DELETE_PTR( mInstance );
    }
}

//===========================================================================

void CopyDataManager::SetCopySizeData( Math::Vector2 data )
{
    mSizeData = data;
}

//===========================================================================

void CopyDataManager::SetCopyPositionData( Math::Vector2 data )
{
    mPositionData = data;
}

//===========================================================================

Math::Vector2 CopyDataManager::GetCopySizeData()
{
    return mSizeData;
}

//===========================================================================

Math::Vector2 CopyDataManager::GetCopyPositionData()
{
    return mPositionData;
}

//===========================================================================

CopyDataManager::CopyDataManager()
{
}

//===========================================================================

CopyDataManager::~CopyDataManager()
{
}