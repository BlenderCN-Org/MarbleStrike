//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

uniform sampler2D gShadowDepthTexture;
uniform float4 gDiffuseColor;
uniform float4 gLightDiffuseColor;

varying float3 normal;
varying float3 lightDirectionNormalized;
varying float2 depth;

void main()
{   
	float3 n = normalize( normal );
		
	float NdotL = max( dot( n, normalize( lightDirectionNormalized.xyz ) ), 0.0 );
	float4 lightColor = gLightDiffuseColor * saturate( NdotL );
	
	float4 shadowDepth = texture2DProj( gShadowDepthTexture, gl_TexCoord[0] );
	
	float finalDepth = depth.x / depth.y;
	
	if ( shadowDepth.x > finalDepth - 0.005 )
	{		
		gl_FragColor = gDiffuseColor * lightColor;
	}
	else
	{
		gl_FragColor = float4( 0, 0, 0, 1 );
	}	
}