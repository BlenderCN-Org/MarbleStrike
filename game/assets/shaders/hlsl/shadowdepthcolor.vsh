
//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;     
	float4 DepthPosition : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain( 
float3 vPos : POSITION
)
{
    VS_OUTPUT output;
	output.Position = mul( float4( vPos, 1.0 ), gObjectToProjectionMatrix );
	output.DepthPosition = output.Position;	
    return output;
}