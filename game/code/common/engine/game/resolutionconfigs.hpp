#ifndef RESOLUTION_CONFIGS_HPP
#define RESOLUTION_CONFIGS_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct ResolutionConfig
{
    int mGameScreenWidth;
    int mGameScreenHeight;
    int mFrameBufferWidth;
    int mFrameBufferHeight;
};

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static ResolutionConfig const giPhoneiPodConfig =
{
    480,
    320,
    480,
    320
};

static ResolutionConfig const giPhoneiPodRetinaConfig =
{
    960,
    640,
    960,
    640
};

static ResolutionConfig const giPhoneiPodWideRetinaConfig =
{
    1136,
    640,
    1136,
    640
};

static ResolutionConfig const giPadConfig =
{
    1024,
    768,
    1024,
    768
};

static ResolutionConfig const giPadRetinaConfig =
{
    2048,
    1536,
    2048,
    1536
};

static ResolutionConfig const gWindowsPhone8 =
{
    1280,
    720,
    1280,
    720
};

static ResolutionConfig const gWindowsPhone8_LowCost =
{
    960,
    640,
    800,
    480
};

static ResolutionConfig const gAndroid =
{
    1280,
    720,
    1280,
    720
};


#endif
