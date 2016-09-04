//////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////

#if defined PLAT_XBOX360
#define snprintf _snprintf
#endif

#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
#define snprintf _snprintf_s
#endif

#if defined( PLAT_IOS ) || defined( PLAT_ANDROID )
#define sscanf_s sscanf
#endif

//#define PROFILE_SYSTEM

#define UNUSED_ALWAYS(x) (void)x