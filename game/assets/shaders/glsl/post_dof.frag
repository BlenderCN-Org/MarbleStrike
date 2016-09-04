//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"
#include "utilities.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------

#define gTextureHiRes gTexture0
#define gTextureLowRes gTexture1
#define gTextureDepth gTexture2

uniform sampler2D gTextureHiRes;
uniform sampler2D gTextureLowRes;
uniform sampler2D gTextureDepth;

#define near gDoFParams.z
#define Q gDoFParams.w
#define focusDist gDoFParams.x
#define focusRange  gDoFParams.y

uniform highp float4 gDoFParams;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// CONSTANTS
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// GLOBAL FUNCTIONS
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{
	lowp float4 depthTexel = texture2D( gTextureDepth, vTexCoord );

	lowp float depth = depthTexel.x;
#if defined(PLAT_WEBGL)	
	depth = unpack_depth( depthTexel );	
#endif

	lowp float4 sceneTexel = texture2D( gTextureHiRes, vTexCoord );
	lowp float4 blurTexel = texture2D( gTextureLowRes, vTexCoord );

	lowp float viewZ = ( near * Q ) / ( 1.0 - depth );

	lowp float blurFactor = clamp( abs ( viewZ - focusDist ) / focusRange, 0.0, 1.0 );

	lowp float3 color = mix( sceneTexel.rgb, blurTexel.rgb, blurFactor );
	lowp float4 finalColor = float4( color, 1.0 );
		
    gl_FragColor = finalColor;
}


