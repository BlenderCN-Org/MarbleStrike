//--------------------------------------------------------------------------------------
// View
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;
float4x4 gObjectToWorldMatrix;
float4x4 gBoneMatrix[16];

//--------------------------------------------------------------------------------------
// Light
//--------------------------------------------------------------------------------------
float4 gLightPosition;
float4x4 gTexAdjust;
float4x4 gObjectToLightProjection;

//--------------------------------------------------------------------------------------
// Output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 Normal : TEXCOORD0;
	float3 LightDirectionNormalized : TEXCOORD1;
	float2 TexCoord : TEXCOORD2;
	float2 Depth : TEXCOORD3;
	float4 ShadowTexCoord : TEXCOORD4;
};

//--------------------------------------------------------------------------------------
// Vertex shader
//--------------------------------------------------------------------------------------

VS_OUTPUT VSMain( 
float4 vPos : POSITION,
float3 vNormal : NORMAL,
float2 vTexCoord : TEXCOORD0,
float4 vBoneIndex : TEXCOORD1,
float4 vBoneWeight : TEXCOORD2
)
{
    VS_OUTPUT output;
    
    float4x4 boneMatrix = gBoneMatrix[vBoneIndex.x];	
	float4x4 currentBoneMatrix = boneMatrix * vBoneWeight.x;	
	float4 skinPosition = mul( vPos, currentBoneMatrix );
    
    float3 normalBoneMatrix = ( mul( float4( vNormal, 0 ), currentBoneMatrix ) ).xyz;
    output.Normal = ( mul( float4( normalBoneMatrix, 0 ), gObjectToWorldMatrix ) ).xyz;
	
	float4 vertexViewPosition = mul( skinPosition, gObjectToWorldMatrix );

	float3 lightDirection = gLightPosition.xyz - vertexViewPosition.xyz;
	output.LightDirectionNormalized.xyz = normalize( lightDirection );
   
    output.Position = mul( skinPosition, gObjectToProjectionMatrix  );
    
    output.TexCoord = vTexCoord;
    
    float4 position = mul( skinPosition, gObjectToLightProjection );
        
	output.Depth.x = position.z;
	output.Depth.y = position.w;
		
	output.ShadowTexCoord = mul( position, gTexAdjust );
	
    return output;
}