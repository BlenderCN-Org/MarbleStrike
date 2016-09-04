//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// ATTRIBUTE VARIABLES
//--------------------------------------------------------------------------------------
attribute float3 inVertex;
attribute float3 inNormal;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gTransformMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float3 vNormal;
varying float3 vPosition;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
	vNormal = ( gInverseTransposeObjectToWorldMatrix * float4( inNormal, 0.0 ) ).xyz;

	vPosition = ( gTransformMatrix * float4( inVertex, 1.0 ) ).xyz;
	gl_Position = gObjectToProjectionMatrix * float4( inVertex, 1.0 );
}