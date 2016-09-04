//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// ATTRIBUTE VARIABLES
//--------------------------------------------------------------------------------------
attribute float4 inVertex;
attribute float2 inTexCoord;
attribute float3 inNormal;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;
uniform float4x4 gObjectToWorldMatrix;
uniform float4x4 gObjectToCameraMatrix;
uniform float4 gLightPosition;
uniform float4x4 gObjectToLightProjection;
uniform float4x4 gTexAdjust;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float3 vNormal;
varying float3 vLightDirection;
varying float2 vDepth;
varying float2 vTexCoord0;
varying float4 vTexCoord1;
varying float vCameraDepth;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
	vNormal = ( gInverseTransposeObjectToWorldMatrix * float4( inNormal, 0 ) ).xyz;
		
	float4 vertexViewPosition = gObjectToWorldMatrix * inVertex;	

	float3 lightDirection = gLightPosition.xyz - vertexViewPosition.xyz;
	vLightDirection.xyz = lightDirection;
	
    gl_Position = gObjectToProjectionMatrix * inVertex;
    
    vTexCoord0 = inTexCoord;
    
    float4 position = gObjectToLightProjection * inVertex;	
	vDepth.x = position.z;
	vDepth.y = position.w;		
	vTexCoord1 = gTexAdjust * position;	  
	float4 vertexCameraPosition = gObjectToCameraMatrix * inVertex;
	vCameraDepth = -vertexCameraPosition.z;  
}