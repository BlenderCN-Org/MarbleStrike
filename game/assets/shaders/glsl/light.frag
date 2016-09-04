//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------
#include "glsldefines.h"

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform highp float4 gDiffuseColor;
uniform highp float4 gEnableTexture;
uniform highp float4 gEnableShadow;
uniform highp float4 gLightDiffuseColor;
uniform highp float4 gDofParams;
uniform sampler2D gTexture;
uniform sampler2D gShadowDepthTexture;

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------
varying highp float3 vNormal;
varying highp float3 vLightDirection;
varying highp float2 vDepth;
varying highp float2 vTexCoord0;
varying highp float4 vTexCoord1;
varying highp float vCameraDepth;

//--------------------------------------------------------------------------------------
// FUNCTIONS
//--------------------------------------------------------------------------------------
highp float ComputeDepthBlur( highp float depth )
{
	highp float f;	
	
	if ( depth < gDofParams.y )
	{
		f = ( depth - gDofParams.y ) / ( gDofParams.y - gDofParams.x );
	}
	else
	{
		f = ( depth - gDofParams.y ) / ( gDofParams.z - gDofParams.y );
		f = clamp( f, 0.0, gDofParams.w );
	}
	
	return f * 0.5 + 0.5;
}

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
void main()
{   
    highp float3 n = normalize( vNormal );
		
	highp float NdotL = max( dot( n, normalize( vLightDirection.xyz ) ), 0.0 );	
	highp float4 lightColor = gLightDiffuseColor * clamp( NdotL, 0.0, 1.0 );			
	
	//highp float4 shadowDepth = texture2DProj( gShadowDepthTexture, vTexCoord1 );
	//highp float finalDepth = vDepth.x / vDepth.y;
	//highp float shadowValue = 1.0f;
	//if ( shadowDepth.x > finalDepth - 0.005 )
	//{	
	//	shadowValue = 1.0f;		
	//}
	//else
	//{
	//	shadowValue = 0.5f;
	//}	
	
	highp float4 finalColor = lightColor;
	
	if ( bool( gEnableTexture.x ) )
	{
		highp float4 texColor = texture2D( gTexture, vTexCoord0 );
		finalColor *= texColor;
	}
	else
	{
		finalColor *= gDiffuseColor;
	}
	
	//if ( bool( gEnableShadow.x ) )
	//{
	//	finalColor.xyz *= shadowValue;
	//}
	
	finalColor.a = ComputeDepthBlur( vCameraDepth );
	
	gl_FragData[0] = finalColor;
}