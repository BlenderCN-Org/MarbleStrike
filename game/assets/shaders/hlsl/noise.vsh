//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
	float3 WorldPosition : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4x4 gTransformMatrix;

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain( 
float3 vPos : POSITION
)
{
    VS_OUTPUT output;    
	
	float4 worldPosition = mul( float4( vPos, 1 ), gTransformMatrix );
    output.Position = mul( float4( vPos, 1 ), gObjectToProjectionMatrix );
    output.WorldPosition = worldPosition;
    
    return output;
}