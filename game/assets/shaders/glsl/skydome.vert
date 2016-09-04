//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// ATTRIBUTE VARIABLES
//--------------------------------------------------------------------------------------
attribute float3 inVertex;
attribute float2 inTexCoord;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gObjectToCameraMatrix;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying float2 vTexCoord0;
varying float vFogFactor;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
void main()
{	
	vTexCoord0 = inTexCoord;

	float fogEnd = 1000.0;
	float fogStart = 0.0;
	float4 cameraPosition = gObjectToCameraMatrix * float4( inVertex, 1.0 );
	//output.FogFactor = saturate( ( fogEnd - -cameraPosition.z) / (fogEnd - fogStart));        
	float fogDensity = 0.001;
	vFogFactor = 1.0 / ( pow( 2.71828, -cameraPosition.z * fogDensity ) );

	gl_Position = gObjectToProjectionMatrix * float4( inVertex, 1.0 );    
}