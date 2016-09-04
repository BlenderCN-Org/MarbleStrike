//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4 gOffset;

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

#ifdef D3D9_API
	output.Position += float4( -gOffset.x, gOffset.y, 0, 0 );
#endif

    output.TexCoord = IN.TexCoord;
	
    return output;
}