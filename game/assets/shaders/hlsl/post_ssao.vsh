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
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------

VertexOutput VSMain( VertexInput IN )
{
    VertexOutput output;
   
    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
    output.TexCoord = IN.TexCoord;
	
    return output;
}