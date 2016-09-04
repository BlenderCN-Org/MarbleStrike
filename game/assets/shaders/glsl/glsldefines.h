//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define float4x4 mat4
#define float3x3 mat3
#define tex2D texture2D
#define lerp mix

#ifdef PLAT_OPENGL
#define highp 
#endif

#ifdef PLAT_OPENGLES
#define sampler2DShadow sampler2D 
#endif

#ifdef PLAT_WEBGL
#define sampler2DShadow sampler2D 
#endif