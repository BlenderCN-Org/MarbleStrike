//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform highp float4 gBlurOffsets[3];
uniform highp float4 gBlurWeights[3];
uniform highp float4 gBlurDirection;
uniform sampler2D gTexture0;

varying highp float2 vTexCoord;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	highp float4 color = float4( 0.0, 0.0, 0.0, 0.0 );

	highp float blurOffsets[9];
	blurOffsets[0] = gBlurOffsets[0].x;
	blurOffsets[1] = gBlurOffsets[0].y;
	blurOffsets[2] = gBlurOffsets[0].z;
	blurOffsets[3] = gBlurOffsets[0].w;
	blurOffsets[4] = gBlurOffsets[1].x;
	blurOffsets[5] = gBlurOffsets[1].y;
	blurOffsets[6] = gBlurOffsets[1].z;
	blurOffsets[7] = gBlurOffsets[1].w;
	blurOffsets[8] = gBlurOffsets[2].x;

	highp float blurWeights[9];
	blurWeights[0] = gBlurWeights[0].x;
	blurWeights[1] = gBlurWeights[0].y;
	blurWeights[2] = gBlurWeights[0].z;
	blurWeights[3] = gBlurWeights[0].w;
	blurWeights[4] = gBlurWeights[1].x;
	blurWeights[5] = gBlurWeights[1].y;
	blurWeights[6] = gBlurWeights[1].z;
	blurWeights[7] = gBlurWeights[1].w;
	blurWeights[8] = gBlurWeights[2].x;
    
    for ( int i = 0; i < 9; i++ )
    {
        color += ( tex2D( gTexture0, vTexCoord + float2( blurOffsets[i], blurOffsets[i] ) * gBlurDirection.xy ) * blurWeights[i] );
    }
        
	highp float4 finalColor = float4( color.rgb, 1.0 );
    gl_FragColor = finalColor;    
}