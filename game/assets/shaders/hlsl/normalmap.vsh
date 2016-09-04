//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
uniform float4x4 gObjectToProjectionMatrix;
uniform float4x4 gObjectToWorldMatrix;
uniform float4 gLightPosition;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;    	
	float2 TexCoord : TEXCOORD0;	
	float3 LightDirectionTangent : TEXCOORD1;
};

struct VS_INPUT
{
	float4 vPos : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexCoord : TEXCOORD0;
	float3 vTangent : TANGENT;
};

//--------------------------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain( VS_INPUT IN )
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;
    
	float3 normal = mul( float4( IN.vNormal, 0 ), gObjectToWorldMatrix ).xyz;
	float3 tangent = mul( float4( IN.vTangent, 0 ), gObjectToWorldMatrix ).xyz;
	float3 binormal = cross( normal, tangent );
	
	float4 vertexViewPosition = mul( IN.vPos, gObjectToWorldMatrix );
	float3 lightDirection = gLightPosition.xyz - vertexViewPosition.xyz;
		
	OUT.LightDirectionTangent.x = dot( lightDirection, tangent );
	OUT.LightDirectionTangent.y = dot( lightDirection, binormal );
	OUT.LightDirectionTangent.z = dot( lightDirection, normal );
	
    OUT.Position = mul( IN.vPos, gObjectToProjectionMatrix );
    
	OUT.TexCoord = IN.vTexCoord;
	
    return OUT;
}