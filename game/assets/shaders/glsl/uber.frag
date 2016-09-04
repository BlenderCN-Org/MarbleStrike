//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"
#include "utilities.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform lowp float4 gDiffuseColor;
uniform lowp float4 gSpecularColor;
uniform lowp float4 gLightColor;
uniform lowp float4 gReflectivity;

uniform sampler2D gColorTexture;
uniform sampler2DShadow gShadowTexture;
uniform samplerCube gCubeMap;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float3 vNormal;
varying highp float3 vLightDirection;
varying highp float3 vCameraDirection;
varying highp float2 vTexCoord0;
varying highp float4 vPosLight;

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
    
	lowp float shadow = 1.0;
    
	lowp float3 n = normalize( vNormal );
	
#if defined(PLAT_OPENGL)
	highp float shadowResult = textureProj( gShadowTexture, vPosLight ).x;
	shadow = shadowResult;	
#endif

#if defined(PLAT_OPENGLES)
	highp float2 shadowTexCoord = ( vPosLight.xy / vPosLight.w );
	highp float depth = ( vPosLight.z / vPosLight.w );
	highp float4 depthMap = float4(0,0,0,0);
	depthMap.x = tex2D( gShadowTexture, shadowTexCoord ).x;
	depthMap.y = tex2D( gShadowTexture, shadowTexCoord + float2( 1.0 / 512.0, 0.0 ) ).x;
	depthMap.z = tex2D( gShadowTexture, shadowTexCoord + float2( 0.0, 1.0 / 512.0 ) ).x;
	depthMap.w = tex2D( gShadowTexture, shadowTexCoord + float2( 1.0 / 512.0, 1.0 / 512.0 ) ).x;
    
	lowp float4 inLight = float4(0,0,0,0);
	if ( depth < depthMap.x ) inLight.x = 1.0;
	if ( depth < depthMap.y ) inLight.y = 1.0;
	if ( depth < depthMap.w ) inLight.w = 1.0;
	if ( depth < depthMap.z ) inLight.z = 1.0;
	shadow = dot( inLight, float4( 0.25, 0.25, 0.25, 0.25 ) );
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

	lowp float3 lightDirection = normalize( vLightDirection );
	lowp float diffuse = max( dot( n, lightDirection ), 0.0 );	        
	
	lowp float3 reflectLight = reflect( -lightDirection, n );
	lowp float3 viewDirection = normalize( vCameraDirection );
	highp float specular = pow( max( dot( reflectLight, viewDirection ), 0.0), gSpecularColor.a );

    lowp float4 texColor = texture2D( gColorTexture, vTexCoord0 );

	lowp float3 reflectView = reflect( -viewDirection, n );
	lowp float4 envMapColor = textureCube( gCubeMap, reflectView );
	lowp float3 diffuseColor = gLightColor.rgb * texColor.rgb * gDiffuseColor.rgb;

	lowp float3 finalDiffuse = mix( diffuseColor, envMapColor.rgb, gReflectivity.x );

	lowp float3 specularColor = gLightColor.rgb * gSpecularColor.rgb * specular;

	lowp float3 finalColor = finalDiffuse * diffuse + specularColor;    
	lowp float alpha = texColor.a * gDiffuseColor.a;
    finalColor *= shadow;

    gl_FragColor = float4( finalColor, alpha );
}