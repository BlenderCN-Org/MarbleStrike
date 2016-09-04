//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform sampler2D gGBufferTexture;
uniform highp float4x4 gProjectionToWorldMatrix;
uniform highp float4 gLightWorldPosition;
uniform highp float4 gLightDiffuseColor;
uniform highp float4 gLightRadius;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	highp float2 texCoord = vTexCoord * 0.5 + 0.5;

	highp float4 gBufferData = tex2D( gGBufferTexture, texCoord );	
	
	highp float depth = gBufferData.w;
	
	//if ( depth == 1.0 )
	//{
	//	discard;
	//}
	
	highp float4 hPosition = float4( vTexCoord.xy, depth, 1 );
	highp float4 worldPosition = gProjectionToWorldMatrix * hPosition;
	worldPosition.xyzw /= worldPosition.w;
		
	highp float3 worldLightPosition = gLightWorldPosition.xyz;	
	highp float3 lightDirection = worldPosition.xyz - worldLightPosition.xyz;
	highp float distanceToLight = length( lightDirection );
	lightDirection = normalize( lightDirection );
	
	highp float3 normalizedNormal = gBufferData.xyz;
	highp float3 normal = ( normalizedNormal * 2.0 ) - 1.0;		
	
	highp float NdotL = max( dot( normal, -lightDirection ), 0.0 );
	highp float4 finalColor = gLightDiffuseColor * NdotL;
	
	highp float attenuation = 1.0 - distanceToLight / gLightRadius.x;
	
    gl_FragColor = float4( finalColor.rgb * attenuation, 1.0 );
}