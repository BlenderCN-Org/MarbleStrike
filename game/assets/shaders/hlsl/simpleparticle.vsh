//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;

//--------------------------------------------------------------------------------------
// Output structure
//--------------------------------------------------------------------------------------
struct VertexInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD0;	
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;    
    float2 TexCoord : TEXCOORD1;    
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;

    output.Position = mul( float4( IN.Position, 1.0 ), gObjectToProjectionMatrix );
	        
    output.Color = IN.Color;
    output.TexCoord = IN.TexCoord;            
	
    return output;
}