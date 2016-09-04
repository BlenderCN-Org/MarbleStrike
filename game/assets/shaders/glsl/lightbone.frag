//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// Material
//--------------------------------------------------------------------------------------
uniform vec4 gDiffuseColor;
uniform sampler2D gTexture;
uniform vec4 gEnableTexture;
uniform sampler2D gShadowDepthTexture;
uniform vec4 gEnableShadow;

//--------------------------------------------------------------------------------------
// Light
//--------------------------------------------------------------------------------------
uniform vec4 gLightDiffuseColor;

//--------------------------------------------------------------------------------------
// Inputs from vertex shader
//--------------------------------------------------------------------------------------
varying vec3 normal;
varying vec3 lightDirectionNormalized;
varying vec2 depth;

//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void main()
{   
    vec3 n = normalize( normal );
		
	float NdotL = max( dot( n, normalize( lightDirectionNormalized.xyz ) ), 0.0 );
	vec4 lightColor = gLightDiffuseColor * clamp( NdotL, 0.0, 1.0 );		
	
	vec4 shadowDepth = texture2DProj( gShadowDepthTexture, gl_TexCoord[1] );
	float finalDepth = depth.x / depth.y;
	float shadowValue = 1.0f;
	if ( shadowDepth.x > finalDepth - 0.005 )
	{	
		shadowValue = 1.0f;		
	}
	else
	{
		shadowValue = 0.5f;
	}	
	
	vec4 finalColor = lightColor;
	
	if ( gEnableTexture.x )
	{
		vec4 texColor = texture2D( gTexture, gl_TexCoord[0].st );
		finalColor *= texColor;
	}
	else
	{
		finalColor *= gDiffuseColor;
	}
	
	if ( gEnableShadow.x )
	{
		finalColor.xyz *= shadowValue;
	}
	gl_FragColor = finalColor;
}