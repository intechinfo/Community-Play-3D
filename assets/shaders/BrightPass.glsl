uniform sampler2D ColorMapSampler;

uniform float contrast;
uniform float multiplier;

void main()
{
	vec4 finalVal = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
	finalVal *= 2.0;
	finalVal = finalVal * finalVal * finalVal * multiplier;

	gl_FragColor = finalVal + contrast;
}