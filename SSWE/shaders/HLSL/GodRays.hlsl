
sampler2D ColorMapSampler : register(s0);
sampler2D UserMapSampler : register(s3);

float2 lightPositionOnScreen;
		
//float decay=0.96815;
//float exposure=0.2;
//float density=0.26;
//float weight=0.58767;

float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR0
{
	float decay=0.96815;
	float exposure=0.2;
	float density=0.26;
	float weight=0.58767;

	int NUM_SAMPLES = 100;
	
	float2 tc = texCoord;
 	float2 deltaTexCoord = (tc - lightPositionOnScreen.xy);  
 	deltaTexCoord *= 1.0f / NUM_SAMPLES * density;  
 	
 	float illuminationDecay = 1.0;

	float4 color =tex2D(UserMapSampler, tc)*0.4;
	
 	for(int i=0; i < NUM_SAMPLES ; i++)
  	{
		tc -= deltaTexCoord;
		float4 sample = tex2D(UserMapSampler, tc)*0.4;
		sample *= illuminationDecay * weight;  
		color += sample;
		illuminationDecay *= decay;  
 	}

 	//gl_FragColor *= exposure;
 	//gl_FragColor += texture2D(ColorMapSampler, textCoo.xy);
 	float4 realColor = tex2D(ColorMapSampler, texCoord.xy);
 	//gl_FragColor = ((vec4((vec3(color.r,color.g,color.b) * exposure),1))+(realColor*(1.5-0.4)));
	//gl_FragColor = color;
	//return ((float4((float3(color.r,color.g,color.b) * exposure),1))+(realColor*(1.5-0.4)));
	//return realColor + float3(color.r, color.g, color.b) * exposure));
	//return color + realColor;
	//return float4( float3(color.r, color.g, color.b) * exposure, 1) + realColor;
	//return float4( color * exposure, 1);
	//return realColor + float4(color.r, color.g, color.b, 1);

	return ((float4((float3(color.r,color.g,color.b) * exposure),1))+(realColor*(1.5-0.4)));
}
