//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gObjectToWorldMatrix;
uniform float4x4 gObjectToProjectionLightMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VertexInput
{
    float3 Position : POSITION;    
    float2 TexCoord : TEXCOORD0;  
	float3 Normal : NORMAL;
	float4 Color : COLOR;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
	float4 VColor : TEXCOORD1;
	float2 TexCoord : TEXCOORD2;
	float4 PosLight : TEXCOORD3;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Normal = ( mul( float4( IN.Normal, 0 ), gInverseTransposeObjectToWorldMatrix ) ).xyz;
	
    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
    
    output.TexCoord = IN.TexCoord;

	output.VColor = IN.Color;

	output.PosLight = mul( float4( IN.Position, 1 ), gObjectToProjectionLightMatrix );	
		
    return output;
}