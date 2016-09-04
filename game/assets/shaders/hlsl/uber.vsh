//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gTransformMatrix;
uniform float4x4 gInverseTransposeObjectToWorldMatrix;
uniform float4x4 gObjectToProjectionLightMatrix;
uniform float4 gLightPosition;
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
	float3 LightDirection : TEXCOORD1;
	float3 CameraDirection : TEXCOORD2;
	float2 TexCoord : TEXCOORD3;
	float4 PosLight : TEXCOORD4;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Normal = ( mul( float4( IN.Normal, 0 ), gInverseTransposeObjectToWorldMatrix ) ).xyz;
	
	float4 worldPosition = mul( float4( IN.Position, 1 ), gTransformMatrix );
	output.LightDirection.xyz = gLightPosition.xyz - worldPosition.xyz;
	output.CameraDirection.xyz = gCameraPosition.xyz - worldPosition.xyz;
   
    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
    
    output.TexCoord = IN.TexCoord;

	output.PosLight = mul( float4( IN.Position, 1 ), gObjectToProjectionLightMatrix );	
		
    return output;
}