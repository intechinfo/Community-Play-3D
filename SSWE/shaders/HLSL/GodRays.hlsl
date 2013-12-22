
sampler2D ColorMapSampler : register(s0);
sampler2D DepthMapSampler : register(s2);

float2 lightPositionOnScreen;
		
//float decay=0.96815;
//float exposure=0.2;
//float density=0.26;
//float weight=0.58767;

struct PS_INPUT
{
   float2 textCoo : TEXCOORD0;
};

float4 pixelMain(PS_INPUT Input) : COLOR0
{
	float decay=0.96815;
	float exposure=0.034;//0.0034;
	float density=01.84;
	float weight=5.65;
	int NUM_SAMPLES = 100;

	float2 textCoo = Input.textCoo.xy;
	
 	float2 deltaTextCoord = float2( textCoo - lightPositionOnScreen.xy );
 	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
 	
 	float illuminationDecay = 1.0;

	float4 color = tex2D(DepthMapSampler, textCoo)*0.4;
	
 	for(int i=0; i < NUM_SAMPLES ; i++)

  	{

    			textCoo -= deltaTextCoord;

    			float4 sample = tex2D(DepthMapSampler, textCoo )*0.4;
    			
    			sample *= illuminationDecay * weight;
    			
    			color += sample;
    			
    			illuminationDecay *= decay;
 	}

 	//gl_FragColor *= exposure;
 	//gl_FragColor += texture2D(ColorMapSampler, textCoo.xy);
 	float4 realColor = tex2D(ColorMapSampler, Input.textCoo.xy);
 	//gl_FragColor = ((vec4((vec3(color.r,color.g,color.b) * exposure),1))+(realColor*(1.5-0.4)));
	//gl_FragColor = color;
	return ( ((float4((float3(color.r,color.g,color.b) * exposure),1))+(realColor*(1.5-0.4))) );
}
