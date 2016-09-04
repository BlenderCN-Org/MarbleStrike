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
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;    
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------

VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
   
    float4 hPosition = mul( float4( IN.Position, 1.0 ), gObjectToProjectionMatrix );
    output.Position = hPosition;
    output.TexCoord = float2( hPosition.xy / hPosition.w );    
	
    return output;
}