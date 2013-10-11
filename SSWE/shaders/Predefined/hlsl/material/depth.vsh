//Matrices
float4x4 WorldMatrix;
float4x4 ViewMatrix;
float4x4 ProjMatrix;

struct VS_INPUT
{
	float4 Pos    : POSITION0;
    float3 Normal : NORMAL0;
	float2 Tex    : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos         : POSITION;	// Vertexposition
    float3 WorldNormal : TEXCOORD0;	// Normal
    float  Depth       : TEXCOORD1;	// Depth
};

VS_OUTPUT main (VS_INPUT input)
{
	VS_OUTPUT output;

	// default positions
    output.Pos = mul(mul(mul(input.Pos, WorldMatrix), ViewMatrix), ProjMatrix);
    
	// world normal
    output.WorldNormal = 0.5*mul(input.Normal, WorldMatrix)+0.5;
	
	// depth
	output.Depth = output.Pos.z;
	
	return output;
}
