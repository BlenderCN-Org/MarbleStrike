//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gProjectionMatrix;
float4x4 gViewMatrix;
float4x4 gTransformMatrix;
float4 gLightPosition;

//--------------------------------------------------------------------------------------
// Output structure
//--------------------------------------------------------------------------------------
struct VertexInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;	
	float2 TexCoord : TEXCOORD0;	
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    float3 LightDirection : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
   
    float4 objectToWorldPosition = mul( float4( IN.Position, 1.0 ), gTransformMatrix );
	float4 worldToCameraPosition = mul( objectToWorldPosition, gViewMatrix );

    output.Position = mul( worldToCameraPosition, gProjectionMatrix );
        
    output.Color = IN.Color;
    output.Normal = ( mul( float4( IN.Normal, 0.0 ), gTransformMatrix ) ).xyz;
    output.TexCoord = IN.TexCoord;
            
    float4 worldPosition = mul( gTransformMatrix, float4( IN.Position, 1.0 ) );
    float3 lightDirection = gLightPosition.xyz - worldPosition.xyz;
	output.LightDirection = lightDirection;
	
    return output;
}