//--------------------------------------------------------------------------------------
// INCLUDES
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// VARYING VARIABLES
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
highp float Shadow( sampler2DShadow shadowTexture, highp float4 posLight )
{  	
	highp float shadow = 1.0;

#if defined(PLAT_OPENGL)
	highp float shadowResult = textureProj( shadowTexture, posLight ).x;
	shadow = shadowResult;	
#endif

#if defined(PLAT_OPENGLES)
	//highp float2 shadowTexCoord = ( posLight.xy / posLight.w );
	highp float depth = ( posLight.z / posLight.w );
	highp float4 depthMap = float4(0,0,0,0);
    depthMap = texture2DProj( shadowTexture, posLight );	
    	
	if ( depth >= depthMap.x ) 
	{
		shadow = 0.0;	
	}

#endif
	
#if defined(PLAT_WEBGL)
	highp float2 shadowTexCoord = ( posLight.xy / posLight.w );
	highp float depth = ( posLight.z / posLight.w );
	highp float4 pack = tex2D( shadowTexture, shadowTexCoord );
	highp float depthMap = unpack_depth( pack );
	lowp float inLight;
	if ( depth > depthMap )
	{
		shadow = 0.0;
	}
#endif

	return shadow;
}