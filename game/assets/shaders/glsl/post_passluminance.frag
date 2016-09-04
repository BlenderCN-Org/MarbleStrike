//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform sampler2D gTexture;

varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	// Acquire the luminance from the texture
    highp float4 l = tex2D( gTexture, vTexCoord );
        
    // Compute a simple scalar, due to the values being > 1.0f
    // the output is often all white, so just to make it a little
    // more informative we can apply a scalar to bring it closer
    // to the 0..1 range
    highp float scalar = 1.0;
        
    // Only the RED and GREEN channels have anything stored in them, but
    // we're not interested in the maximum value, so we just use the red
    // channel:
	highp float4 finalColor = float4( l.r * scalar, l.r * scalar, l.r * scalar, 1.0 );
    gl_FragColor = finalColor;    
}