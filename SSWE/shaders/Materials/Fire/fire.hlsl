float4x4 wvp;
float time;

struct VS_OUTPUT
{
   float4 Pos       : POSITION;
   float2 TexCoord0 : TEXCOORD0;
   float2 TexCoord1 : TEXCOORD1;
   float2 TexCoord2 : TEXCOORD2;
   float2 TexCoord3 : TEXCOORD3;
};

VS_OUTPUT vertexMain (float4 pos: POSITION, 
                      float2 texCoord : TEXCOORD0)
{
   VS_OUTPUT Out;
   Out.Pos = mul(pos, wvp);

   float4 layer_speed = float4(0.2, 0.52, 0.1, 1.0);
   
   Out.TexCoord0 = texCoord;
   Out.TexCoord0.y += 0.2;

   Out.TexCoord1.x = texCoord.x;
   Out.TexCoord1.y = texCoord.y + layer_speed.x * time / 1000.0;

   Out.TexCoord2.x = texCoord.x;
   Out.TexCoord2.y = texCoord.y + layer_speed.y * time / 1000.0;

   Out.TexCoord3.x = texCoord.x;
   Out.TexCoord3.y = texCoord.y + layer_speed.z * time / 1000.0;

   return Out;
}

sampler fire_base       : register(s0);
sampler fire_distortion : register(s1);
sampler fire_opacity    : register(s2);

float4 bx2(float x)
{
   return 2.0f * x - 1.0f;
}

float4 pixelMain (float4 tc0 : TEXCOORD0, 
                  float4 tc1 : TEXCOORD1,
                  float4 tc2 : TEXCOORD2, 
                  float4 tc3 : TEXCOORD3) : COLOR
{
   float distortion_amount0  = 0.092;
   float distortion_amount1  = 0.092;
   float distortion_amount2  = 0.092;
   
   float4 height_attenuation = float4(0.3, 0.39, 0.0, 1.0);
   
   float4 noise0 = tex2D(fire_distortion, tc1);
   float4 noise1 = tex2D(fire_distortion, tc2);
   float4 noise2 = tex2D(fire_distortion, tc3);

   float4 noiseSum = bx2(noise0) * distortion_amount0 + 
          bx2(noise1) * distortion_amount1 + bx2(noise2) * distortion_amount2;
          
   float4 perturbedBaseCoords = tc0 + noiseSum * (tc0.y * 
         height_attenuation.x + height_attenuation.y);

   float4 base    = tex2D(fire_base, perturbedBaseCoords) * 2;
   float4 opacity = tex2D(fire_opacity, perturbedBaseCoords);

   return base * opacity;
}