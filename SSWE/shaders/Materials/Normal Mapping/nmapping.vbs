float4x4 matWorldInverse; 
float4x4 ModelViewMatrix;
float4x4 ModelViewProjectionMatrix;

float3 fvLightPosition1; 
float3 fvLightPosition2; 
float3 fvLightPosition3; 
float3 fvLightPosition4; 
 
float fLightStrength1; 
float fLightStrength2; 
float fLightStrength3; 
float fLightStrength4; 
 
struct VS_OUTPUT
{
   float4 Position            : POSITION0;
   float2 Texcoord            : TEXCOORD0;
   float3 ViewDirection      : TEXCOORD1; 
   float3 LightDirection1      : TEXCOORD2; 
   float3 LightDirection2      : TEXCOORD3; 
   float3 LightDirection3      : TEXCOORD4; 
   float3 LightDirection4      : TEXCOORD5; 
   float4 LightDistMultiplier  : TEXCOORD6;
};
    
float getLengthSQR (float3 vec) 
{ 
   return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z); 
}  

struct VS_INPUT
{
   float3 Position         : POSITION0;
   float3 Normal           : NORMAL;
   float2 Texcoord         : TEXCOORD0;
};

VS_OUTPUT vertexMain( in VS_INPUT IN ) 
{    
   VS_OUTPUT OUT = (VS_OUTPUT)0;

   // common output
   OUT.Position = mul(float4(IN.Position,1.0), ModelViewProjectionMatrix);
   OUT.Texcoord = IN.Texcoord;

   float4x4 LightTransform= ModelViewMatrix; 
   LightTransform= mul(matWorldInverse, LightTransform);

   float4 fvObjectPosition = mul(float4(IN.Position,1.0), ModelViewProjectionMatrix);
   float4 fvLightPos1 = mul(float4(fvLightPosition1,1.0), LightTransform); 
   float4 fvLightPos2 = mul(float4(fvLightPosition2,1.0), LightTransform); 
   float4 fvLightPos3 = mul(float4(fvLightPosition3,1.0), LightTransform); 
   float4 fvLightPos4 = mul(float4(fvLightPosition4,1.0), LightTransform); 

   float3 fvViewDirection  =  - fvObjectPosition.xyz; 

   float3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz); 
   float3 fvLightDirection2 = (fvLightPos2.xyz - fvObjectPosition.xyz); 
   float3 fvLightDirection3 = (fvLightPos3.xyz - fvObjectPosition.xyz); 
   float3 fvLightDirection4 = (fvLightPos4.xyz - fvObjectPosition.xyz); 

   OUT.LightDistMultiplier[0]=1.0/(getLengthSQR (fvLightDirection1)/(fLightStrength1*10000.0)); 
   OUT.LightDistMultiplier[1]=1.0/(getLengthSQR (fvLightDirection2)/(fLightStrength2*10000.0)); 
   OUT.LightDistMultiplier[2]=1.0/(getLengthSQR (fvLightDirection3)/(fLightStrength3*10000.0)); 
   OUT.LightDistMultiplier[3]=1.0/(getLengthSQR (fvLightDirection4)/(fLightStrength4*10000.0)); 

   float3 fvTangent   = -float3(abs(IN.Normal.y) + abs(IN.Normal.z), abs(IN.Normal.x), 0); 
   float3 fvBinormal   = cross(fvTangent,IN.Normal);
   float3 fvNormal     = mul(IN.Normal, ModelViewMatrix); 

   fvTangent         = mul( cross(fvBinormal, IN.Normal), ModelViewMatrix ); 
   fvBinormal         = mul( fvBinormal, ModelViewMatrix ); 

   OUT.ViewDirection.x  = dot( fvTangent, fvViewDirection ); 
   OUT.ViewDirection.y  = dot( fvBinormal, fvViewDirection ); 
   OUT.ViewDirection.z  = dot( fvNormal, fvViewDirection ); 

   OUT.LightDirection1.x  = dot( fvTangent, fvLightDirection1.xyz ); 
   OUT.LightDirection1.y  = dot( fvBinormal, fvLightDirection1.xyz ); 
   OUT.LightDirection1.z  = dot( fvNormal, fvLightDirection1.xyz ); 

   OUT.LightDirection2.x  = dot( fvTangent, fvLightDirection2.xyz ); 
   OUT.LightDirection2.y  = dot( fvBinormal, fvLightDirection2.xyz ); 
   OUT.LightDirection2.z  = dot( fvNormal, fvLightDirection2.xyz ); 

   OUT.LightDirection3.x  = dot( fvTangent, fvLightDirection3.xyz ); 
   OUT.LightDirection3.y  = dot( fvBinormal, fvLightDirection3.xyz ); 
   OUT.LightDirection3.z  = dot( fvNormal, fvLightDirection3.xyz ); 

   OUT.LightDirection4.x  = dot( fvTangent, fvLightDirection4.xyz ); 
   OUT.LightDirection4.y  = dot( fvBinormal, fvLightDirection4.xyz ); 
   OUT.LightDirection4.z  = dot( fvNormal, fvLightDirection4.xyz ); 

   return OUT;
}
