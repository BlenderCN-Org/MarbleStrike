//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------

// fExposure;         // A user configurable bias to under/over expose the image
// fGaussianScalar;   // Used in the post-processing, but also useful here
// g_rcp_bloom_tex_w; // The reciprocal WIDTH of the texture in 'bloom'
// g_rcp_bloom_tex_h; // The reciprocal HEIGHT of the texture in 'bloom'
uniform highp float4 gHDRParameter;

uniform sampler2D gMainTexture;
uniform sampler2D gLuminanceTexture;
uniform sampler2D gBloomTexture;

varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	// Read the HDR value that was computed as part of the original scene
    highp float4 c = tex2D( gMainTexture, vTexCoord );
    
    // Read the luminance value, target the centre of the texture
    // which will map to the only pixel in it!
    highp float4 l = tex2D( gLuminanceTexture, float2( 0.5, 0.5 ) );
        
    // Compute the blur value using a bilinear filter
    // It is worth noting that if the hardware supports linear filtering of a
    // floating point render target that this step can probably be skipped.
    highp float xWeight = fract( vTexCoord.x / gHDRParameter.z ) - 0.5;
    highp float xDir = xWeight;
    xWeight = abs( xWeight );
    xDir /= xWeight;
    xDir *= gHDRParameter.z;

    highp float yWeight = fract( vTexCoord.y / gHDRParameter.w ) - 0.5;
    highp float yDir = yWeight;
    yWeight = abs( yWeight );
    yDir /= yWeight;
    yDir *= gHDRParameter.w;

    // sample the blur texture for the 4 relevant pixels, weighted accordingly
    highp float4 b = ((1.0 - xWeight) * (1.0 - yWeight))  * tex2D( gBloomTexture, vTexCoord );        
    b +=       (xWeight * (1.0 - yWeight))					* tex2D( gBloomTexture, vTexCoord + float2( xDir, 0.0 ) );
    b +=       (yWeight * (1.0 - xWeight))					* tex2D( gBloomTexture, vTexCoord + float2( 0.0, yDir ) );
    b +=       (xWeight * yWeight)							* tex2D( gBloomTexture, vTexCoord + float2( xDir, yDir ) );
	
    // Compute the actual colour:
    highp float4 final = c + 0.25 * b;

	//highp float4 b = tex2D( gBloomTexture, vTexCoord );        
    //highp float4 final = c + b;
            
    // Reinhard's tone mapping equation (See Eqn#3 from 
    // "Photographic Tone Reproduction for Digital Images" for more details) is:
    //
    //      (      (   Lp    ))
    // Lp * (1.0 +(---------))
    //      (      ((Lm * Lm)))
    // -------------------------
    //         1.0 + Lp
    //
    // Lp is the luminance at the given point, this is computed using Eqn#2 from the above paper:
    //
    //        exposure
    //   Lp = -------- * HDRPixelIntensity
    //          l.r
    //
    // The exposure ("key" in the above paper) can be used to adjust the overall "balance" of 
    // the image. "l.r" is the average luminance across the scene, computed via the luminance
    // downsampling process. 'HDRPixelIntensity' is the measured brightness of the current pixel
    // being processed.
    
    highp float Lp = (gHDRParameter.x / l.r) * max( final.r, max( final.g, final.b ) );
    
    // A slight difference is that we have a bloom component in the final image - this is *added* to the 
    // final result, therefore potentially increasing the maximum luminance across the whole image. 
    // For a bright area of the display, this factor should be the integral of the bloom distribution 
    // multipled by the maximum value. The integral of the gaussian distribution between [-1,+1] should 
    // be AT MOST 1.0; but the sample code adds a scalar to the front of this, making it a good enough
    // approximation to the *real* integral.
    
    highp float LmSqr = (l.g + gHDRParameter.y * l.g) * (l.g + gHDRParameter.y * l.g);
    
    // Compute Eqn#3:
    highp float toneScalar = ( Lp * ( 1.0 + ( Lp / ( LmSqr ) ) ) ) / ( 1.0 + Lp );
    
    // Tonemap the final outputted pixel:
    c = final * toneScalar;
    
    // Return the fully composed colour
    c.a = 1.0;

	highp float4 finalColor = c;
    gl_FragColor = finalColor;    
}