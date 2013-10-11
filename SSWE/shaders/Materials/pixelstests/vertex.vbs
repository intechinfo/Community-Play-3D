float4x4 matWorldViewProjection;
float4x4 matWorld;
float tileSize;
float4 viewPos;
 
struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal :   NORMAL0; 
};
 
struct VS_OUTPUT 
{
   float4 Position :        POSITION0;
   float2 TexcoordX :       TEXCOORD0;
   float2 TexcoordY :       TEXCOORD1;
   float2 TexcoordZ :       TEXCOORD2;
   float3 viewDirection   : TEXCOORD3;
   float3x3 tangentSpace :  TEXCOORD4;
};
 
VS_OUTPUT vertexMain( VS_INPUT Input )
{
   VS_OUTPUT Output;
 
   Output.Position         = mul( Input.Position, matWorldViewProjection );
   float4 realPos          = mul( Input.Position,matWorld);
   
   Output.viewDirection = (realPos-viewPos).xyz;
   
   Output.TexcoordX         = realPos.zy/tileSize;
   Output.TexcoordY         = realPos.xz/tileSize;
   Output.TexcoordZ         = realPos.xy/tileSize;
   
   float3 worldNormal = Input.Normal;
   float3 n = worldNormal;
   n*=n;
   
   float3 xtan,ytan,ztan;
   float3 xbin,ybin,zbin;
   
   xtan = float3(0,0,1);
   xbin = float3(0,1,0);
   
   ytan = float3(1,0,0);
   ybin = float3(0,0,1);
   
   ztan = float3(1,0,0);
   zbin = float3(0,1,0);
   
   float3 worldBinormal = xbin*n.x+ybin*n.y+zbin*n.z;
   float3 worldTangent = xtan*n.x+ytan*n.y+ztan*n.z;
   
   worldNormal = mul(matWorld,worldNormal);
   worldBinormal = mul(matWorld,worldBinormal);
   worldTangent = mul(matWorld,worldTangent);
   
   Output.tangentSpace[0]   = worldTangent;
   Output.tangentSpace[1]   = worldBinormal; 
   Output.tangentSpace[2]   = worldNormal;
      
   return( Output );
   
}
