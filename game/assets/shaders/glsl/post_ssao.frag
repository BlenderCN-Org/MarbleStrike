//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// UNIFORM VARIABLES
//--------------------------------------------------------------------------------------
uniform highp float4 gKernel[16];
uniform highp float4x4 gProjectionMatrix;
uniform highp float4x4 gInverseProjectionMatrix;

uniform sampler2D gDepthTexture;
uniform sampler2D gNormalTexture;
uniform sampler2D gRandomTexture;

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
const highp float2 NOISE_SCALE = float2( 1024 / 4, 768 / 4 );
const highp int SAMPLE_KERNEL_SIZE = 16;
const highp float RADIUS = 0.8;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	highp float3 normal = texture2D( gNormalTexture, vTexCoord ).xyz * 2.0 - 1.0;
	normal = normalize( normal );

	highp float x = vTexCoord.x * 2.0 - 1.0;
	highp float y = ( vTexCoord.y ) * 2.0 - 1.0;
	highp float z = texture2D( gDepthTexture, vTexCoord ).r;
	
	highp float4 projPos = float4( x, y, z * 2.0 - 1.0, 1.0 );
	highp float4 viewPos = gInverseProjectionMatrix * projPos;	
	highp float3 origin = viewPos.xyz / viewPos.w;	

	highp float3 rvec = texture2D( gRandomTexture, vTexCoord * NOISE_SCALE ).xyz * 2.0 - 1.0;
	highp float3 tangent = normalize( rvec - normal * dot( rvec, normal ) );
	highp float3 bitangent = cross( normal, tangent );
	highp float3x3 tbn = float3x3( tangent, bitangent, normal );
		
	highp float occlusion = 0.0;
	for ( int i = 0; i < SAMPLE_KERNEL_SIZE; ++i ) 
	{		
		highp float3 samplePoint = tbn * gKernel[i].xyz;
		samplePoint = samplePoint * RADIUS + origin;

		highp float4x4 bias = float4x4( 0.5, 0.0, 0.0, 0.0, 
							  0.0, 0.5, 0.0, 0.0, 
							  0.0, 0.0, 0.5, 0.0, 
							  0.5, 0.5, 0.5, 1.0
							  );
		
		highp float4 offset = float4( samplePoint, 1.0 );
		offset = bias * gProjectionMatrix * offset;		
		offset.xyz /= offset.w;
	
		highp float sampleDepth = texture2D( gDepthTexture, offset.xy ).r;
		highp float rangeCheck = 0.0;//abs( z - sampleDepth ) < RADIUS ? 1.0 : 0.0;
		float depthDiff = abs( z - sampleDepth );		
		if ( depthDiff < RADIUS )
		{
			rangeCheck = 1.0;
		}
		//occlusion += ( (sampleDepth ) < offset.z ? 1.0 : 0.0 );// * rangeCheck;
		if ( sampleDepth < offset.z ) 
		{
			occlusion += 1.0 * rangeCheck;
		}
		else
		{
			occlusion += 0.0 * rangeCheck;
		}		
	}  

	occlusion = 1.0 - ( occlusion / float(SAMPLE_KERNEL_SIZE) );

	highp float4 finalColor = float4( occlusion, occlusion, occlusion, 1 );
    
    gl_FragColor = finalColor;    
}