//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gObjectToWorldMatrix;
uniform float4x4 gObjectToProjectionLightMatrix;
uniform float4 gCameraPosition;

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
	float3 CameraDirection : TEXCOORD1;
	float2 TexCoord : TEXCOORD2;
	float4 PosLight : TEXCOORD3;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Normal = ( mul( float4( IN.Normal, 0 ), gObjectToWorldMatrix ) ).xyz;
	
	float4 worldPosition = mul( float4( IN.Position, 1 ), gObjectToWorldMatrix );	
	output.CameraDirection.xyz = gCameraPosition.xyz - worldPosition.xyz;
   
    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
    
    output.TexCoord = IN.TexCoord;

	output.PosLight = mul( float4( IN.Position, 1 ), gObjectToProjectionLightMatrix );	
		
    return output;
}