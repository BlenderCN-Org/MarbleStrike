//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// View
//--------------------------------------------------------------------------------------
uniform mat4 gObjectToProjectionMatrix;
uniform mat4 gObjectToWorldMatrix;
uniform mat4 gBoneMatrix[16];

//--------------------------------------------------------------------------------------
// Light
//--------------------------------------------------------------------------------------
uniform vec4 gLightPosition;
uniform mat4 gObjectToLightProjection;
uniform mat4 gTexAdjust;

//--------------------------------------------------------------------------------------
// Outputs to Pixel Shader
//--------------------------------------------------------------------------------------
varying vec3 normal;
varying vec3 lightDirectionNormalized;
varying vec2 depth;

//--------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------
void main()
{	
	vec4 boneIndex = gl_MultiTexCoord1;
	vec4 boneWeights = gl_MultiTexCoord2;
	mat4 identityMatrix = mat4(	1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0 );
								
	mat4 boneMatrix = gBoneMatrix[boneIndex.x];	
	mat4 currentBoneMatrix = boneMatrix * boneWeights.x;
	
	vec4 skinPosition = currentBoneMatrix * gl_Vertex;
	
	normal = ( gObjectToWorldMatrix * currentBoneMatrix * vec4( gl_Normal, 0 ) ).xyz;
		
	vec4 vertexViewPosition = gObjectToWorldMatrix * skinPosition;

	vec3 lightDirection = gLightPosition.xyz - vertexViewPosition.xyz;
	lightDirectionNormalized.xyz = normalize( lightDirection );
	
    gl_Position = gObjectToProjectionMatrix * skinPosition;
    
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    vec4 position = gObjectToLightProjection * skinPosition;	
	depth.x = position.z;
	depth.y = position.w;		
	gl_TexCoord[1] = gTexAdjust * position;	    
}