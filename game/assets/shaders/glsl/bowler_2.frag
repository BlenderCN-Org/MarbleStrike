//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"
#include "utilities.h"
#include "shadow.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform lowp float4 gDiffuseColor;
uniform highp float4 gSpecularColor;
uniform lowp float4 gLightColor;
uniform highp float4 gLightDirection;

uniform sampler2D gColorTexture;
uniform sampler2D gToonTexture;
uniform sampler2DShadow gShadowTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float3 vNormal;
varying highp float3 vCameraDirection;
varying highp float2 vTexCoord0;
varying highp float4 vPosLight;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{       
	//highp float shadow = Shadow( gShadowTexture, vPosLight );
	//shadow = shadow * 0.25 + 0.75;

	lowp float3 n = vNormal;//normalize( vNormal );
	
	lowp float3 lightDirection = normalize( -gLightDirection.xyz );
	lowp float diffuse = max( dot( n, lightDirection ), 0.0 );	        
	
	lowp float3 reflectLight = reflect( -lightDirection, n );
	lowp float3 viewDirection = normalize( vCameraDirection );
	lowp float specular = pow( max( dot( reflectLight, viewDirection ), 0.0), gSpecularColor.a );

    lowp float4 texColor = texture2D( gColorTexture, vTexCoord0 );
	lowp float3 diffuseColor = gLightColor.rgb * texColor.rgb * gDiffuseColor.rgb;

	lowp float3 specularColor = gLightColor.rgb * gSpecularColor.rgb;

	lowp float4 toonDiffuse = texture2D( gToonTexture, float2( diffuse, 0 ) );
	lowp float3 finalDiffuse = diffuseColor * toonDiffuse.rgb;	
	lowp float3 finalSpecular = specularColor * specular;

	lowp float rim = smoothstep( 0.5 , 1.0 , pow( abs( 1.0 - dot( n, viewDirection ) ), 0.5 ) );
		
	lowp float3 finalColor = finalDiffuse + finalSpecular + float3( rim, rim, rim ) * 0.6;    
	lowp float alpha = texColor.a * gDiffuseColor.a;
	//finalColor.rgb *= shadow;

    gl_FragColor = float4( finalColor, alpha );
}