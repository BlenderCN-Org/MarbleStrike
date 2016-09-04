//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// VERTEX GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gObjectToWorldMatrix;
uniform float4 gLightPosition;

//--------------------------------------------------------------------------------------
// ATTRIBUTE VARIABLES
//--------------------------------------------------------------------------------------
attribute vec4 inVertex;
attribute vec3 inNormal;
attribute vec2 inTexCoord;
attribute vec3 inTangent;

//--------------------------------------------------------------------------------------
// OUTPUTS
//--------------------------------------------------------------------------------------
varying vec3 vLightDirectionTangent;
varying vec2 vTexCoord;

//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void main()
{	
	float3 normal = mul( gObjectToWorldMatrix, float4( inNormal, 0 ) ).xyz;
	float3 tangent = mul( gObjectToWorldMatrix, float4( inTangent, 0 ) ).xyz;
	float3 binormal = cross( normal, tangent );
	
	float4 vertexViewPosition = mul( gObjectToWorldMatrix, inVertex );
	float3 lightDirection = gLightPosition.xyz - vertexViewPosition.xyz;
		
	vLightDirectionTangent.x = dot( lightDirection, tangent );
	vLightDirectionTangent.y = dot( lightDirection, binormal );
	vLightDirectionTangent.z = dot( lightDirection, normal );
	
    gl_Position = mul( gObjectToProjectionMatrix, inVertex );
    
	vTexCoord = inTexCoord;    	
}