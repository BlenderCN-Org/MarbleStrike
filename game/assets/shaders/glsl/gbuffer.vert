//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
attribute float3 inVertex;
attribute float3 inNormal;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float3 vNormal;
varying float4 vHPosition;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
    gl_Position = gObjectToProjectionMatrix * float4( inVertex, 1 );
    
    vNormal = ( gInverseTransposeObjectToWorldMatrix * float4( inNormal, 0 ) ).xyz;
    vHPosition = gObjectToProjectionMatrix * float4( inVertex, 1 );
}