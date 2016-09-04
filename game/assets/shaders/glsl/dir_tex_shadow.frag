//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"
#include "utilities.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform lowp float4 gDiffuseColor;
uniform lowp float4 gLightColor;
uniform lowp float4 gLightDirection;

uniform sampler2D gColorTexture;
uniform sampler2DShadow gShadowTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float3 vNormal;
varying highp float2 vTexCoord0;
varying highp float4 vPosLight;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
	lowp float shadow = 1.0;
    
#if defined(PLAT_OPENGL)
	highp float shadowResult = textureProj( gShadowTexture, vPosLight ).x;
	shadow = shadowResult;	
#endif

#if defined(PLAT_OPENGLES)
	highp float2 shadowTexCoord = ( vPosLight.xy / vPosLight.w );
	highp float depth = ( vPosLight.z / vPosLight.w );
	highp float4 depthMap = float4(0,0,0,0);
	depthMap.x = tex2D( gShadowTexture, shadowTexCoord ).x;	
    
	lowp float4 inLight = float4(0,0,0,0);
	if ( depth < depthMap.x ) inLight.x = 1.0;	
	shadow = dot( inLight, float4( 1.0, 0.0, 0.0, 0.0 ) );
#endif
	
#if defined(PLAT_WEBGL)
	highp float2 shadowTexCoord = ( vPosLight.xy / vPosLight.w );
	highp float depth = ( vPosLight.z / vPosLight.w );
	highp float4 pack = tex2D( gShadowTexture, shadowTexCoord );
	highp float depthMap = unpack_depth( pack );
	lowp float inLight;
	if ( depth > depthMap )
	{
		shadow = 0.0;
	}
#endif

	lowp float3 n = normalize( vNormal );

	lowp float3 lightDirection = normalize( float3( 0, 0.8, 0.5 ) );//gLightDirection.xyz;
	lowp float diffuse = max( dot( n, lightDirection ), 0.0 );	        
	
    lowp float4 texColor = texture2D( gColorTexture, vTexCoord0 );

	lowp float3 finalDiffuse = gLightColor.rgb * texColor.rgb * gDiffuseColor.rgb;

	lowp float3 finalColor = finalDiffuse * diffuse;
	lowp float alpha = texColor.a * gDiffuseColor.a;

	shadow = 1.0 - ( ( 1.0 - shadow ) * 0.6 );

    finalColor *= shadow;

    gl_FragColor = float4( finalColor, alpha );
}