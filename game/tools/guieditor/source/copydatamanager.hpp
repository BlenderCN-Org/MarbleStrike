#ifndef COPY_DATA_MANAGER_HPP
#define COPY_DATA_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector2.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class CopyDataManager
{
public:

    static CopyDataManager* GetInstance();
    static void CreateCopyDataManager();
    static void DestroyCopyDataManager();

    void SetCopySizeData( Math::Vector2 data );
    void SetCopyPositionData( Math::Vector2 data );
    Math::Vector2 GetCopySizeData();
    Math::Vector2 GetCopyPositionData();

private:
    CopyDataManager();
    ~CopyDataManager();

    static CopyDataManager* mInstance;
    
    Math::Vector2 mSizeData;
    Math::Vector2 mPositionData;
};

#endif
