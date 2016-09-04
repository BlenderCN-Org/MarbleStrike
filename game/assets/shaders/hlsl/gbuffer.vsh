//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4x4 gInverseTransposeObjectToWorldMatrix;

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
    float4 HPosition : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
   
    float4 hPosition = mul( float4( IN.Position, 1.0 ), gObjectToProjectionMatrix );
    output.Position = hPosition;
    output.Normal = mul( float4( IN.Normal, 0.0 ), gInverseTransposeObjectToWorldMatrix );
    output.HPosition = hPosition;
	
    return output;
}