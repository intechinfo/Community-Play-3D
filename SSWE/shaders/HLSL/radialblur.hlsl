uniform sampler2D SceneBuffer : register(s0);

uniform float SampleDist;
uniform float SampleStrength;

float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR0
{
	float samples[10] =	{
		-0.08,
		-0.05,
		-0.03,
		-0.02,
		-0.01,
		 0.01,
		 0.02,
		 0.03,
		 0.05,
		 0.08
	}; 
	 
	// Vector from pixel to the center of the screen
	float2 dir = 0.5-texCoord;

	// Distance from pixel to the center (distant pixels have stronger effect)
	float dist = sqrt(dir.x*dir.x+dir.y*dir.y);

	// Now that we have dist, we can normlize vector
	dir = normalize(dir);

	// Save the color to be used later
	float4 color = tex2D(SceneBuffer, texCoord);

	// Average the pixels going along the vector
	float4 sum = color;
	for (int i=0; i<10; i++)
	{
		sum += tex2D(SceneBuffer, texCoord+dir*samples[i]*SampleDist);
	}
	sum /= 11.0;

	// Calculate amount of blur based on
	// distance and a strength parameter
	// We need 0 <= t <= 1
	float t = clamp(dist*SampleStrength, 0.0, 1.0);

	//Blend the original color with the averaged pixels
	return lerp(color, sum, t);
}
