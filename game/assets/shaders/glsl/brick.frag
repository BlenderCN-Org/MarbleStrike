//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
const highp float4 gBrickColor = float4( 1.0, 0.0, 0.0, 1.0 );
const highp float4 gMortarColor = float4( 0.0, 0.0, 0.0, 1.0 );
const highp float4 gBrickSize = float4( 0.5, 0.5, 0.0, 0.0 );
const highp float4 gBrickPct = float4( 0.8, 0.8, 0.0, 0.0 );

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float3 vNormal;
varying highp float3 vPosition;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	lowp float2 position = vPosition.xy / gBrickSize.xy;

	if ( fract( position.y * 0.5 ) > 0.5)
	{
		position.x += 0.5;
	}

	position = fract(position);
	lowp float2 useBrick = step( position, gBrickPct.xy );
	lowp float3 color = mix( gMortarColor.xyz, gBrickColor.xyz, useBrick.x * useBrick.y );

    gl_FragColor = float4( color, 1 );
}