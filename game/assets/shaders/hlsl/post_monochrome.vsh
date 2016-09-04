//--------------------------------------------------------------------------------------
// GLOBAL VARIABLES
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------

VS_OUTPUT VSMain( 
float4 vPos : POSITION,
float2 vTexCoord : TEXCOORD0
)
{
    VS_OUTPUT output;
   
    output.Position = mul( vPos, gObjectToProjectionMatrix );
    output.TexCoord = vTexCoord;
	
    return output;
}