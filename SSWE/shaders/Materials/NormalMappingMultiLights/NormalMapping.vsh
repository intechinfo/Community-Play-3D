
float4x4 ModelViewProjectionMatrix;

struct VS_OUTPUT
{
    float4 Position            : POSITION0;
    float2 Texcoord            : TEXCOORD0;
    float3 Normal              : TEXCOORD1;
	float4 ObjectPosition	   : TEXCOORD2;
};

struct VS_INPUT
{
   float3 Position         : POSITION0;
   float3 Normal           : NORMAL;
   float2 Texcoord         : TEXCOORD0;
};

VS_OUTPUT vertexMain( in VS_INPUT IN ) 
{    
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.Position  = mul(float4(IN.Position, 1.0), ModelViewProjectionMatrix);
    OUT.Texcoord  = IN.Texcoord;
    OUT.Normal = IN.Normal;
	OUT.ObjectPosition = OUT.Position;
   
    return OUT;
}
