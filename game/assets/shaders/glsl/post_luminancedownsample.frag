//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform highp float4 gDownSampleOffsets[9];
uniform sampler2D gTexture;

varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	// Compute the average of the 10 necessary samples
    highp float4 color = float4( 0.0, 0.0, 0.0, 0.0 );
    highp float maximum = -1e20;
    highp float average = 0.0;
        
    for( int i = 0; i < 9; i++ )
    {
        color = tex2D( gTexture, vTexCoord + gDownSampleOffsets[i].xy );
        average += color.r;
        maximum = max( maximum, color.g );
    }
        
    // We've just taken 9 samples from the
    // high resolution texture, so compute the
    // actual average that is written to the
    // lower resolution texture (render target).
    average /= 9.0;
     
	highp float4 finalColor = float4( average, maximum, 0.0, 1.0 );
    gl_FragColor = finalColor;    
}