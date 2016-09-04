//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// PIXEL GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4 gDiffuseColor;
uniform float4 gEnableTexture;
uniform sampler2D gTexture;
uniform float4 gEnableNormalMapTexture;
uniform sampler2D gTextureNormalMap;
uniform float4 gLightDiffuseColor;

//--------------------------------------------------------------------------------------
// INPUTS
//--------------------------------------------------------------------------------------
varying vec3 vLightDirectionTangent;
varying vec2 vTexCoord;

//--------------------------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------------------------
void main()
{   
	//--------------------------------------------------------------------------------------
	// Lighting
	//--------------------------------------------------------------------------------------
    float3 normal = ( tex2D( gTextureNormalMap, inTexCoord ) * 2.0 - 1.0 ).xyz;
	float NdotL = max( dot( normal, normalize( inLightDirectionTangent ) ), 0.0 );	
	float4 lightColor = gLightDiffuseColor * clamp( NdotL, 0.0, 1.0 );	
	float4 finalColor = lightColor;

	//--------------------------------------------------------------------------------------
	// Diffuse Map
	//--------------------------------------------------------------------------------------
	if ( gEnableTexture.x )
	{
		float4 texColor = tex2D( gTexture, inTexCoord );
		finalColor *= texColor;
	}	
	else
	{
		finalColor *= gDiffuseColor;
	}
	
	gl_FragColor = finalColor;
}