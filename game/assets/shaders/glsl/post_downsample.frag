//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform highp float4 gDownSampleOffsets[16];
uniform sampler2D gTexture0;

varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	highp float4 average = float4( 0.0, 0.0, 0.0, 0.0 );

    for( int i = 0; i < 16; i++ )
    {
        average += tex2D( gTexture0, vTexCoord + gDownSampleOffsets[i].xy );
    }
        
    average *= ( 1.0 / 16.0 );

	highp float4 finalColor = average;
    gl_FragColor = finalColor;    
}