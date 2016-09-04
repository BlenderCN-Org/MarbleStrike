//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4x4 gObjectToViewMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VertexInput
{
	float3 Position : POSITION;
	float3 Normal : NORMAL0;
};

struct VertexOutput
{
	float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Position = mul( float4( IN.Position, 1.0 ), gObjectToProjectionMatrix );
    output.Normal = mul( float4( IN.Normal, 0.0 ), gObjectToViewMatrix );
	
    return output;
}