//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gProjectionMatrix;
float4x4 gViewMatrix;
float4x4 gTransformMatrix;

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
    float2 TexCoord : TEXCOORD1;    
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
    output.TexCoord = IN.TexCoord;            
	
    return output;
}