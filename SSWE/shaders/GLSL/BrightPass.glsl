uniform sampler2D ColorMapSampler;

uniform float contrast;
uniform float multiplier;

void main()
{
	vec4 finalVal = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	finalVal *= 2.0 * multiplier;
	finalVal = finalVal * finalVal * finalVal;

	gl_FragColor = finalVal;
}