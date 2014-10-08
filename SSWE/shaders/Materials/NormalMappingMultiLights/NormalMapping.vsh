
float4x4 ModelViewProjectionMatrix;

struct VS_OUTPUT
{
   float4 Position            : POSITION0;
   float2 Texcoord            : TEXCOORD0;
   float3 Normal			  : TEXCOORD1;
   float4 ObjectPosition	  : TEXCOORD2;
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
   OUT.Position = mul(float4(IN.Position,1.0), ModelViewProjectionMatrix);
   OUT.Texcoord = IN.Texcoord;
   OUT.Normal   = IN.Normal;
   OUT.ObjectPosition = OUT.Position;

   return OUT;
   
   /*float4x4 LightTransform= ModelViewMatrix; 
   LightTransform= mul(matWorldInverse, LightTransform);
   float4 fvObjectPosition = mul(float4(IN.Position,1.0), ModelViewProjectionMatrix);
   float4 fvLightPos1 = mul(float4(fvLightPosition1,1.0), LightTransform); 

   float3 fvViewDirection  =  - fvObjectPosition.xyz; 
   float3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz); 
   
   OUT.LightDistMultiplier=1.0/(getLengthSQR (fvLightDirection1)/(fLightStrength1*10000.0)); 

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
   OUT.LightDirection1.z  = dot( fvNormal, fvLightDirection1.xyz );*/
   
   return OUT;
}
