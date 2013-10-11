uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;

uniform float screenColCoef; //0.9
uniform float bloomColCoef; //0.5

float luminance(vec4 c)
{
	return dot( c, vec4(0.3, 0.59, 0.11, 0.0) );
}

void main() 
{		
	vec4 bloomCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	vec4 screenCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);

	gl_FragColor = ((screenCol*screenColCoef) + (bloomCol*bloomColCoef));
}