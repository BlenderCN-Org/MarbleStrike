//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4x4 gObjectToCameraMatrix;

//--------------------------------------------------------------------------------------
// Output structure
//--------------------------------------------------------------------------------------
struct VertexInput
{
	float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;	
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD1;    
	float FogFactor : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
VertexOutput VSMain( const VertexInput IN )
{
    VertexOutput output;
   
    output.Position = mul( float4( IN.Position, 1.0 ), gObjectToProjectionMatrix );
	        
    output.TexCoord = IN.TexCoord;    
	
	float fogEnd = 1000.0;
	float fogStart = 0.0;
	float4 cameraPosition = mul( float4( IN.Position, 1.0 ), gObjectToCameraMatrix );
	//output.FogFactor = saturate( ( fogEnd - -cameraPosition.z) / (fogEnd - fogStart));        
	float fogDensity = 0.001;
	output.FogFactor = 1.0 / ( pow( 2.71828, -cameraPosition.z * fogDensity ) );
	
    return output;
}