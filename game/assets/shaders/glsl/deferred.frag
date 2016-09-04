//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform sampler2D gGBufferTexture;
uniform float4x4 gObjectToCameraMatrix;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float2 vTexCoord;
varying float3 vFrustumCorners;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	float2 texCoord = vTexCoord;
	float3 frustumCorners = vFrustumCorners;

	float4 gBufferData = tex2D( gGBufferTexture, texCoord );	
	
	float3 linearDepth = float3( gBufferData.w, gBufferData.w, -gBufferData.w );
	float3 viewPosition = frustumCorners * linearDepth;	
	
	float3 lightPosition = float3( 0, 10, 0 );
	float4 viewLightPosition = gObjectToCameraMatrix * float4( lightPosition, 1 );	
	float3 lightDirection = viewPosition - viewLightPosition.xyz;
	lightDirection = normalize( lightDirection );
	
	float3 normalizedNormal = gBufferData.xyz;
	float3 normal = ( normalizedNormal * 2.0 ) - 1.0;		
	
	float NdotL = max( dot( normal, -lightDirection ), 0.0 );
	
    gl_FragColor = float4( 1, 0, 0, 1.0 );
}