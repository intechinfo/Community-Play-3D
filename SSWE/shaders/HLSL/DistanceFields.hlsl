sampler2D ScreenMapSampler : register(s1);
sampler2D DepthMapSampler : register(s2);
sampler2D UserMapSampler : register(s3);

// Fast noise thanks to Ysaneya
float4 randomizer4(const float4 x)
{
    float4 z = fmod(x, float4(5612.0, 5612.0, 5612.0, 5612.0));
    float tVal = 3.1415927 * 2.0;
    z = fmod(z, float4(tVal, tVal, tVal, tVal));
    return frac(cos(z) * float4(56812.5453, 56812.5453, 56812.5453, 56812.5453));
}

#define A 1.0
#define B 57.0
#define C 113.0

float cnoise4(const in float3 xx)
{
	const float3 ABC = float3(A, B, C);
	const float4 A3 = float4(0, B, C, C+B);
	const float4 A4 = float4(A, A+B, C+A, C+A+B);

    float3 x = fmod(xx + float3(32768.0, 32768.0, 32768.0), float3(65536.0, 65536.0, 65536.0));
    float3 ix = floor(x);
    float3 fx = frac(x);
    float3 wx = fx * fx * (float3(3.0, 3.0, 3.0) - 2.0 * fx);
    float nn = dot(ix, ABC);

    float4 N1 = nn + A3;
    float4 N2 = nn + A4;
    float4 R1 = randomizer4(N1);
    float4 R2 = randomizer4(N2);
    float4 R = lerp(R1, R2, wx.x);
    float re = lerp(lerp(R.x, R.y, wx.y), lerp(R.z, R.w, wx.y), wx.z);

    return 1.0 - 2.0 * re;
}

#define TEST_EPISILON 0.01

float planeNoise(float3 mpoint)
{
	return cnoise4(mpoint / 5.0) * 20.0 + cnoise4(mpoint) * 5.0 +  + cnoise4(mpoint * 5.0);
}

float getDistanceForPlaneY(float3 mpoint, float offset)
{
	float3 tempmpoint = float3(mpoint.x, mpoint.y + planeNoise(float3(mpoint.x / 25.0, 10.0, mpoint.z / 25.0)), mpoint.z);
	return max(abs(tempmpoint.y - offset), 0.0);
}


float getDistanceToXPipe(float3 mpoint, float2 offset, float radius)
{
	float2 tempmpoint = float2(mpoint.y - offset.x + cnoise4(float3(mpoint.x / 10.0, 0.5, 0.5)) * 5.0, mpoint.z - offset.y);
	return max(length(tempmpoint) - radius, 0.0);
}

float getDistanceToSphere(float3 mpoint, float3 pos, float radius)
{
	return max(length(mpoint - pos) - radius, 0.0);
}

float getDistanceFormpoint(float3 mpoint)
{
	//return getDistanceToSphere(mpoint, float3(0.0, 0.0, 50.0), 5.0);
	//return getDistanceToXPipe(mpoint, float2(-20.0, 50.0), 5.0);
	return getDistanceForPlaneY(mpoint, -40.0);
}

float4 pixelMain
(
	float2 TexCoords : TEXCOORD0,
	float3 LStart : TEXCOORD1,
	float3 LEnd : TEXCOORD2
) : COLOR0
{
	float3 lVec = LEnd - LStart;
	float3 lDir = normalize(lVec);
	float lLength = length(lVec);

	float4 finalColour = float4(0.0, 0.0, 0.0, 1.0);
	
	float distTravelled = 0;
	bool distHit = false;
	
	float cFac = 0.0;
	float farValue = 500.0;
	
	while(distTravelled < farValue && !distHit)
	{
		float3 currentWPos = LStart + lDir * distTravelled;
		
		float currentDist = getDistanceFormpoint(currentWPos);
		
		cFac += 0.02;
		
		if(currentDist < TEST_EPISILON)
		{
			finalColour = float4(1.0, 1.0, 1.0, 1.0);
			distHit = true;
		}
		else
		{
			distTravelled += currentDist;
		}
	}
	
	finalColour = float4(cFac, 0.0, 0.0, 1.0);

	return finalColour;
}