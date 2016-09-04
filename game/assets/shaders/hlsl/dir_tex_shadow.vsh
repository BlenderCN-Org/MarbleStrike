//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gObjectToWorldMatrix;

uniform float4x4 gObjectToProjectionLightMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VertexInput
{
    float3 Position : POSITION;    
    float2 TexCoord : TEXCOORD0;  
	float3 Normal : NORMAL;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
	float2 TexCoord : TEXCOORD3;
	float4 PosLight : TEXCOORD4;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Normal = ( mul( float4( IN.Normal, 0 ), gObjectToWorldMatrix ) ).xyz;
   
    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
    
    output.TexCoord = IN.TexCoord;

	output.PosLight = mul( float4( IN.Position, 1 ), gObjectToProjectionLightMatrix );	
		
    return output;
}