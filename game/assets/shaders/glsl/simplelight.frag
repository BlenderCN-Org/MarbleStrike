//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform lowp float4 gDiffuseColor;
uniform lowp float4 gLightColor;
uniform lowp float4 gLightAmbientColor;
uniform sampler2D gTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying lowp float4 vColor;
varying lowp float3 vNormal;
varying lowp float2 vTexCoord;
varying lowp float3 vLightDirection;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	lowp float4 materialColor = vColor;
	materialColor *= gDiffuseColor;
	lowp float4 texColor = tex2D( gTexture, vTexCoord );
	materialColor *= texColor;
		
	lowp float3 normal = normalize( vNormal );
	lowp float NdotL = max( dot( normal, normalize( vLightDirection.xyz ) ), 0.0 );		
	lowp float3 finalLight = gLightAmbientColor.rgb + gLightColor.rgb * NdotL;

	lowp float4 finalColor = float4( finalLight, 1.0 ) * materialColor;
	
    gl_FragColor = finalColor; 
}