//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gTransformMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VertexInput
{
    float3 Position : POSITION;    
	float3 Normal : NORMAL;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
	float3 WorldPosition : TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Normal = ( mul( float4( IN.Normal, 0 ), gInverseTransposeObjectToWorldMatrix ) ).xyz;
	
	float4 worldPosition = mul( float4( IN.Position, 1 ), gTransformMatrix );	   
	output.WorldPosition = worldPosition.xyz;

    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
        		
    return output;
}