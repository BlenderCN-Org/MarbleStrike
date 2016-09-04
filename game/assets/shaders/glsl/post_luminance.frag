//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform highp float4 gDownSampleOffsets[4];
uniform sampler2D gTexture;
varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	// Compute the average of the 4 necessary samples
    highp float average = 0.0;
    highp float maximum = -1e20;
    highp float4 color = float4( 0, 0, 0, 0 );
    highp float3 WEIGHT = float3( 0.299, 0.587, 0.114 );
        
    for( int i = 0; i < 4; i++ )
    {
        color = tex2D( gTexture, vTexCoord + gDownSampleOffsets[i].xy );
            
        // There are a number of ways we can try and convert the RGB value into
        // a single luminance value:
            
        // 1. Do a very simple mathematical average:
        //float GreyValue = dot( color.rgb, float3( 0.33f, 0.33f, 0.33f ) );
            
        // 2. Perform a more accurately weighted average:
        //float GreyValue = dot( color.rgb, WEIGHT );
            
        // 3. Take the maximum value of the incoming, same as computing the
        //    brightness/value for an HSV/HSB conversion:
        highp float GreyValue = max( color.r, max( color.g, color.b ) );
            
        // 4. Compute the luminance component as per the HSL colour space:
        //float GreyValue = 0.5f * ( max( color.r, max( color.g, color.b ) ) + min( color.r, min( color.g, color.b ) ) );
            
        // 5. Use the magnitude of the colour
        //float GreyValue = length( color.rgb );
                        
        maximum = max( maximum, GreyValue );
        average += (0.25 * log( 1e-5 + GreyValue )); //1e-5 necessary to stop the singularity at GreyValue=0
    }
        
    average = exp( average );

    gl_FragColor = float4( average, maximum, 0.0, 1.0 );    
}