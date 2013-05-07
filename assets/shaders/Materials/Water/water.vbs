float4x4 mWorldViewProj;
float4x4 mWorld;
float4   CamPos;
float    Time;
float    sinWave;

float wavesHeight;


struct VS_OUTPUT
{
  float4 Position   : POSITION0;
  float4 waterpos   : TEXCOORD0;
  float3 WorldView  : TEXCOORD1;
  float2 TexCoords  : TEXCOORD2;
  float3 MultiVar   : TEXCOORD3;
};
VS_OUTPUT vertexMain( float3 Position  : POSITION0,
                      float2 TexCoords : TEXCOORD0 )
{
  VS_OUTPUT OUT = (VS_OUTPUT)0;
  OUT.Position = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj);
  OUT.waterpos = float4(Position.x,Position.y,Position.z,1.0);
  OUT.MultiVar.y = Time / 10000.0;
  if (sinWave > 0.0)
  {
    OUT.MultiVar.x = (sin((Position.x/3.0) + (Time * 10.0 / 10000.0))) +
                     (cos((Position.z/3.0) + (Time * 10.0 / 10000.0)));
    OUT.Position.y += OUT.MultiVar.x * wavesHeight;
  }
  
  OUT.MultiVar.z = CamPos.y;
  float4 PosW = mul(float4(Position.x,Position.y,Position.z,1.0), mWorld);
  OUT.WorldView = CamPos.xyz - PosW.xyz;
  OUT.TexCoords = TexCoords + float2(OUT.MultiVar.y, OUT.MultiVar.y);
  return(OUT);
}
