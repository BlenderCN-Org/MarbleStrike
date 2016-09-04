//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------

#define gPermutationTexture gTexture0
#define gGradientTexture gTexture1

uniform sampler2D gPermutationTexture;
uniform sampler2D gGradientTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float3 vWorldPosition;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------

highp float3 fade( highp float3 t )
{
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

highp float perm( highp float i )
{
	highp float2 texCoord = float2( i / 256.0, 0 );
	highp float3 data = texture2D( gPermutationTexture, texCoord ).xyz;
	highp float p = data.x * 255.0;
	return p;
}

highp float grad( highp float x, highp float3 p )
{
	highp float modx = mod( x, 16.0 );
	highp float2 texCoord = float2( modx / 16.0, 0 );
	highp float3 gradientValue = texture2D( gGradientTexture, texCoord ).xyz;
	gradientValue = gradientValue * 2.0 - 1.0;

	return dot( gradientValue, p );		
}

highp float Noise3D( highp float3 position )
{
	// FIND UNIT CUBE THAT CONTAINS POINT
	
	highp float x = floor( position.x );
	highp float y = floor( position.y );
	highp float z = floor( position.z );
	
	highp float X = mod( x, 256.0 );	
	highp float Y = mod( y, 256.0 );
	highp float Z = mod( z, 256.0 );
	
	// FIND RELATIVE X,Y,Z OF POINT IN CUBE.
	position -= floor( position ); 
	
	// COMPUTE FADE CURVES FOR EACH OF X,Y,Z.
	highp float3 f = fade( position );
	
    // HASH COORDINATES OF THE 8 CUBE CORNERS
  	highp float A = perm( X ) + Y;  	
  	highp float AA = perm( A ) + Z;
	highp float AB = perm( A + 1.0 ) + Z;
  	highp float B =  perm( X + 1.0 ) + Y;
  	highp float BA = perm( B ) + Z;
  	highp float BB = perm( B + 1.0 ) + Z;
  	  	
	// AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE
   	highp float grad1 = grad( perm( AA ), position );
  	highp float grad2 = grad( perm( AB ), position + float3( 0, -1, 0 ) );
  	highp float grad3 = grad( perm( AA + 1.0 ), position + float3( 0, 0, -1 ) );
  	highp float grad4 = grad( perm( AB + 1.0 ), position + float3( 0, -1, -1 ) );  	
  	highp float grad5 = grad( perm( BA ), position + float3( -1, 0, 0 ) );
  	highp float grad6 = grad( perm( BB ), position + float3( -1, -1, 0 ) );
  	highp float grad7 = grad( perm( BA + 1.0 ), position + float3( -1, 0, -1 ) );
  	highp float grad8 = grad( perm( BB + 1.0 ), position + float3( -1, -1, -1 ) );
  	
  	highp float lerp1 = mix( grad1, grad5, f.x );
  	highp float lerp2 = mix( grad2, grad6, f.x );
  	highp float lerp3 = mix( grad3, grad7, f.x );
  	highp float lerp4 = mix( grad4, grad8, f.x );
  	highp float lerp5 = mix( lerp1, lerp2, f.y );
  	highp float lerp6 = mix( lerp3, lerp4, f.y );
  	highp float lerp7 = mix( lerp5, lerp6, f.z );
  	
  	return lerp7;
}

void main()
{   
	highp float noiseValue = 0.0;
	
	noiseValue += Noise3D( vWorldPosition * 5.0 );
	for ( highp float i = 1.0; i < 8.0; ++i )
	{
		noiseValue += Noise3D( vWorldPosition * 5.0 * 2.0 * i ) * 1.0 / ( 2.0 * i );
	}
	
	noiseValue = noiseValue * 0.5 + 0.5;
		
	gl_FragData[0] = float4( noiseValue, noiseValue, noiseValue, 1 );	
}