//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 gObjectToProjectionMatrix;

//--------------------------------------------------------------------------------------
// Output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex shader
//--------------------------------------------------------------------------------------

VS_OUTPUT VSMain( float4 vPos : POSITION )
{
    VS_OUTPUT output;
   
    output.Position = mul( vPos, gObjectToProjectionMatrix );
	
    return output;
}