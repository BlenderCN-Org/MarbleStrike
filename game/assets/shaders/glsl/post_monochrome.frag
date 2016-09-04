//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform sampler2D gTexture;

static vec3 gLuminanceConversion = vec3( 0.2125f, 0.7154f, 0.0721f );

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	vec4 textureColor = tex2D( gTexture, gl_TexCoord[0].st );
    
    vec4 finalColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    finalColor.rgb = dot( textureColor.rgb, gLuminanceConversion );    
    finalColor.a = textureColor.a;
		
    gl_FragColor = finalColor;    
}