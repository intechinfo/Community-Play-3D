
uniform sampler2D DepthMapSampler;
uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

uniform vec2 lightPositionOnScreen;
		
//float decay=0.96815;
//float exposure=0.2;
//float density=0.26;
//float weight=0.58767;

void main()
{
	float decay=0.96815;
	float exposure=0.034;//0.0034;
	float density=01.84;
	float weight=5.65;
	int NUM_SAMPLES = 100;

	vec2 textCoo = gl_TexCoord[0].xy;
	
 	vec2 deltaTextCoord = vec2( textCoo - lightPositionOnScreen.xy );
 	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
 	
 	float illuminationDecay = 1.0;

	vec4 color = texture2D(DepthMapSampler, textCoo)*0.4;
	
 	for(int i=0; i < NUM_SAMPLES ; i++)

  	{

    			textCoo -= deltaTextCoord;

    			vec4 sample = texture2D(DepthMapSampler, textCoo )*0.4;
    			
    			sample *= illuminationDecay * weight;
    			
    			color += sample;
    			
    			illuminationDecay *= decay;
 	}

 	//gl_FragColor *= exposure;
 	//gl_FragColor += texture2D(ColorMapSampler, textCoo.xy);
 	vec4 realColor = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
 	gl_FragColor = ((vec4((vec3(color.r,color.g,color.b) * exposure),1))+(realColor*(1.5-0.4)));
	//gl_FragColor = color;
}
