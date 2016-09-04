//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// ATTRIBUTE VARIABLES
//--------------------------------------------------------------------------------------
attribute float3 inVertex;
attribute float4 inColor;
attribute float3 inNormal;
attribute float2 inTexCoord;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gProjectionMatrix;
uniform float4x4 gViewMatrix;
uniform float4x4 gTransformMatrix;
uniform float4 gLightPosition;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float4 vColor;
varying float3 vNormal;
varying float2 vTexCoord;
varying float3 vLightDirection;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
    gl_Position = gProjectionMatrix * gViewMatrix * gTransformMatrix * float4( inVertex, 1.0 );
    vNormal = ( gTransformMatrix * float4( inNormal, 0.0 ) ).xyz;
    vColor = inColor;
    vTexCoord = inTexCoord;
    
    float4 worldPosition = gTransformMatrix * float4( inVertex, 1.0 );
    float3 lightDirection = gLightPosition.xyz - worldPosition.xyz;
	vLightDirection = lightDirection;
}