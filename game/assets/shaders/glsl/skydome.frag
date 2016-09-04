//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform lowp float4 gDiffuseColor;
uniform sampler2D gColorTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float2 vTexCoord0;
varying highp float vFogFactor;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
    lowp float4 texColor = texture2D( gColorTexture, vTexCoord0 );
	lowp float4 finalColor = texColor * gDiffuseColor;

	lowp float4 fogColor = float4(0.11,0.576,0.827,1);
	finalColor = vFogFactor * finalColor + ( 1.0 - vFogFactor ) * fogColor;
	
    gl_FragColor = finalColor;
}