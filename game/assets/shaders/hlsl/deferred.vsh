//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VertexInput
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 FrustumCorners : TEXCOORD1;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 FrustumCorners : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------

VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
   
    output.Position = mul( float4( IN.Position, 1.0 ), gObjectToProjectionMatrix );
    output.TexCoord = IN.TexCoord;
    output.FrustumCorners = IN.FrustumCorners;
	
    return output;
}