//Matrices
uniform float4x4 WorldMatrix;
uniform float4x4 ViewMatrix;
uniform float4x4 ReflectedViewMatrix;
uniform float4x4 ProjMatrix;
uniform float    ElapsedTime;
uniform float    WaveLength;
uniform float2   WindForce;

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos       : POSITION;	//Vertexposition
	float4 Reflected : TEXCOORD0;	//Texcoords Reflectionmap
	float4 Refracted : TEXCOORD1;	//Texcoords Refractionmap
	float2 Bump1     : TEXCOORD2;	//Texcoords NormalMap
	float2 Bump2     : TEXCOORD3;	//Texcoords NormalMap
	float2 Bump3     : TEXCOORD4;	//Texcoords NormalMap
	float3 PosWorld  : TEXCOORD5;   //PosWorld
};

VS_OUTPUT main (VS_INPUT input)
{
	VS_OUTPUT output;

	// default positions
    output.Pos = mul(mul(mul(input.Pos, WorldMatrix), ViewMatrix), ProjMatrix);
    
	// reflected position
    output.Reflected = mul(mul(mul(input.Pos, WorldMatrix), ReflectedViewMatrix), ProjMatrix);

	// refrected position
    output.Refracted = output.Pos;
	
	// texture coordinates of the normal map
	output.Bump1 = WindForce*ElapsedTime*0.001+input.Tex*WaveLength;
	output.Bump2 = WindForce*ElapsedTime*0.001+input.Tex*WaveLength*0.3;
	output.Bump3 = WindForce*ElapsedTime*0.001-input.Tex*WaveLength*0.66;
	
	// position in worldspace
	output.PosWorld = mul(input.Pos, WorldMatrix);
	return output;
}
