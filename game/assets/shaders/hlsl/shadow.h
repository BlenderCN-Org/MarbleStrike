//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------

float unpack_depth( const float4 rgba_depth )
{
	const float4 bit_shift = float4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);
	float depth = dot(rgba_depth, bit_shift);
	return depth;
}

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
float Shadow( Texture2D shadowTexture, SamplerState shadowTextureState, float4 posLight )
{ 
	float shadow = 1.0;
    
	float2 shadowTexCoord = 0.5 * ( posLight.xy / posLight.w ) + float2( 0.5, 0.5 );
	shadowTexCoord.y = 1.0f - shadowTexCoord.y;
	float depth = posLight.z / posLight.w;

    float4 depthMap = float4(0,0,0,0);

#if defined( D3D9_API )
    depthMap.x = shadowTexture.Sample( shadowTextureState, shadowTexCoord ).x;
#endif

#if defined( D3D11_API )	
	float4 pack = shadowTexture.Sample( shadowTextureState, shadowTexCoord );	
    depthMap.x = unpack_depth( pack );
#endif
	
	float4 inLight = depth < depthMap;
	shadow = dot( inLight, float4( 1.0, 0, 0, 0 ) );	

	return shadow;
}