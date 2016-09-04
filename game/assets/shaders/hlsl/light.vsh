//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4x4 gInverseTransposeObjectToWorldMatrix;
float4x4 gObjectToWorldMatrix;
float4x4 gObjectToCameraMatrix;
float4 gLightPosition;
float4x4 gTexAdjust;
float4x4 gObjectToLightProjection;

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
	float2 TexCoord : TEXCOORD2;
	float2 Depth : TEXCOORD3;
	float4 ShadowTexCoord : TEXCOORD4;
	float CameraDepth : TEXCOORD5;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
    
    output.Normal = ( mul( float4( IN.Normal, 0 ), gInverseTransposeObjectToWorldMatrix ) ).xyz;
	
	float4 worldPosition = mul( float4( IN.Position, 1 ), gObjectToWorldMatrix );

	float3 lightDirection = gLightPosition.xyz - worldPosition.xyz;
	output.LightDirection.xyz = lightDirection;
   
    output.Position = mul( float4( IN.Position, 1 ), gObjectToProjectionMatrix );
    
    output.TexCoord = IN.TexCoord;
    
    float4 position = mul( float4( IN.Position, 1 ), gObjectToLightProjection );    
        
	output.Depth.x = position.z;
	output.Depth.y = position.w;
		
	output.ShadowTexCoord = mul( position, gTexAdjust );
	
	float4 vertexCameraPosition = mul( float4( IN.Position, 1 ), gObjectToCameraMatrix );
	output.CameraDepth = -vertexCameraPosition.z;  
	
    return output;
}