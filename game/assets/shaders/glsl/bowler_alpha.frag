//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"
#include "utilities.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform lowp float4 gDiffuseColor;
uniform sampler2D gColorTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float2 vTexCoord0;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{       
    lowp float4 texColor = texture2D( gColorTexture, vTexCoord0 );
	lowp float3 diffuseColor = texColor.rgb * gDiffuseColor.rgb;
	lowp float3 finalDiffuse = diffuseColor;	
		
	lowp float3 finalColor = finalDiffuse;
	lowp float alpha = texColor.a * gDiffuseColor.a;

    gl_FragColor = float4( finalColor, alpha );
}