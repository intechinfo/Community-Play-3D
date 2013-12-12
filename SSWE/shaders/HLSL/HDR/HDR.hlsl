sampler2D img1 : register(s0);
sampler2D img2 : register(s1);
float threshold;

float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR0
{
    float4 clr1 =  tex2D(img1, texCoord);
    float4 clr2 =  tex2D(img2, texCoord);
    float avrg1 = (clr1.r + clr1.g + clr1.b)/3;
    float avrg2 = (clr2.r + clr2.g + clr2.b)/3;
    float diff1 = abs(avrg1 - threshold);
    diff1 *= (1/(max(1-threshold, threshold)));
    float diff2 = abs(avrg2 - threshold);
    diff2 *= (1/(max(1-threshold, threshold)));

    return (clr1*(1-diff1+diff2) + clr2*(1-diff2+diff1))/2;
}
