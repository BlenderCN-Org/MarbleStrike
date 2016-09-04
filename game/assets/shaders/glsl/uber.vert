//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// ATTRIBUTE VARIABLES
//--------------------------------------------------------------------------------------
attribute float3 inVertex;
attribute float2 inTexCoord;
attribute float3 inNormal;
attribute float3 inTangent;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gTransformMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;
uniform float4x4 gObjectToProjectionLightMatrix;
uniform float4 gLightPosition;
uniform float4 gCameraPosition;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float3 vNormal;
varying float3 vLightDirection;
varying float3 vCameraDirection;
varying float2 vTexCoord0;
varying float4 vPosLight;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
	float4x4 bias = float4x4( 0.5, 0.0, 0.0, 0.0, 
							  0.0, 0.5, 0.0, 0.0, 
							  0.0, 0.0, 0.5, 0.0, 
							  0.5, 0.5, 0.5, 1.0
							  );

	vNormal = ( gInverseTransposeObjectToWorldMatrix * float4( inNormal, 0 ) ).xyz;	
	
	float4 vertexViewPosition = gTransformMatrix * float4( inVertex, 1.0 );	
	vLightDirection = gLightPosition.xyz - vertexViewPosition.xyz;	 
	vCameraDirection = gCameraPosition.xyz - vertexViewPosition.xyz;	 
    
	vTexCoord0 = inTexCoord;	
	vPosLight = bias * gObjectToProjectionLightMatrix * float4( inVertex, 1.0 );	

	gl_Position = gObjectToProjectionMatrix * float4( inVertex, 1.0 );    
}