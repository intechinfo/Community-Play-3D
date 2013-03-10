const static float4 vPurple = float4(0.7f, 0.2f, 0.9f, 1.0f);
const static float4 vOrange = float4(0.7f, 0.4f, 0.2f, 1.0f);
const static float fThreshold = 0.1f;

float4 LensFlarePS (    in float2 in_vTexCoord    : TEXCOORD0,
 uniform int NumSamples,
 uniform float4 vTint,
 uniform float fTexScale,
 uniform float fBlurScale)    : COLOR0
{
 // The flare should appear on the opposite side of the screen as the
 // source of the light, so first we mirror the texture coordinate.
 // Then we normalize so we can apply a scaling factor.
 float2 vMirrorCoord = float2(1.0f, 1.0f) - in_vTexCoord;
 float2 vNormalizedCoord = vMirrorCoord * 2.0f - 1.0f;
 vNormalizedCoord *= fTexScale;

 // We'll blur towards the center of screen, and also away from it.

 float2 vTowardCenter = normalize(-vNormalizedCoord);
 float2 fBlurDist = fBlurScale * NumSamples;
 float2 vStartPoint = vNormalizedCoord + ((vTowardCenter / g_vSourceDimensions) * fBlurDist);
 float2 vStep = -(vTowardCenter / g_vSourceDimensions) * 2 * fBlurDist;

 // Do the blur and sum the samples
 float4 vSum = 0;
 float2 vSamplePos = vStartPoint;
 for (int i = 0; i < NumSamples; i++)
 {
 float2 vSampleTexCoord = vSamplePos * 0.5f + 0.5f;

 // Don't add in samples past texture border
 if (vSampleTexCoord.x >= 0 && vSampleTexCoord.x <= 1.0f
 && vSampleTexCoord.y >=0 && vSampleTexCoord.y <= 1.0f)
 {
 float4 vSample = tex2D(PointSampler0, vSampleTexCoord);
 vSum +=  max(0, vSample - fThreshold) * vTint;
 }

 vSamplePos += vStep;
 }

 return vSum / NumSamples;
}

float4 CombinePS (in float2 in_vTexCoord    : TEXCOORD0) : COLOR0
{
 float4 vColor = tex2D(PointSampler0, in_vTexCoord);
 vColor += tex2D(PointSampler1, in_vTexCoord);
 vColor += tex2D(PointSampler2, in_vTexCoord);
 return vColor;
}

technique LensFlareFirstPass
{
 pass p0
 {
 VertexShader = compile vs_3_0 PostProcessVS();
 PixelShader = compile ps_3_0 LensFlarePS(12, vOrange, 2.00f, 0.15f);

 ZEnable = false;
 ZWriteEnable = false;
 AlphaBlendEnable = false;
 AlphaTestEnable = false;
 StencilEnable = false;
 }
}

technique LensFlareSecondPass
{
 pass p0
 {
 VertexShader = compile vs_3_0 PostProcessVS();
 PixelShader = compile ps_3_0 LensFlarePS(12, vPurple, 0.5f, 0.1f);

 ZEnable = false;
 ZWriteEnable = false;
 AlphaBlendEnable = false;
 AlphaTestEnable = false;
 StencilEnable = false;
 }
}
