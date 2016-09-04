//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
attribute float4 inVertex;
attribute float2 inTexCoord;
attribute float3 inTexCoord1;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;

varying float2 vTexCoord;
varying float3 vFrustumCorners;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
    gl_Position = gObjectToProjectionMatrix * inVertex;
    vTexCoord = inTexCoord;
    vFrustumCorners = inTexCoord1;
}