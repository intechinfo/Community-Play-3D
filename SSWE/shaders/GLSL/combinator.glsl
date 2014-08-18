uniform sampler2D ColorMapSampler;
uniform sampler2D ScreenMapSampler;
uniform sampler2D DepthMapSampler;

void main() 
{
    vec4 cms;
    vec4 dms = texture2D(DepthMapSampler, gl_TexCoord[0].xy);
    if (dms.r == 1.0 && dms.g == 1.0)
        cms = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);
    else
        cms = texture2D(ColorMapSampler, gl_TexCoord[0].xy);
    
    gl_FragColor = cms;
}