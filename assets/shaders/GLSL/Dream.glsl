
uniform sampler2D sceneTex; // 0

uniform float contrast;
uniform float noise;

void main ()
{
  vec2 uv = gl_TexCoord[0].xy;
  vec4 c = texture2D(sceneTex, uv);
  
  c += texture2D(sceneTex, uv+0.001+noise);
  c += texture2D(sceneTex, uv+0.003+noise);
  c += texture2D(sceneTex, uv+0.005+noise);
  c += texture2D(sceneTex, uv+0.007+noise);
  c += texture2D(sceneTex, uv+0.009+noise);
  c += texture2D(sceneTex, uv+0.011+noise);

  c += texture2D(sceneTex, uv-0.001+noise);
  c += texture2D(sceneTex, uv-0.003+noise);
  c += texture2D(sceneTex, uv-0.005+noise);
  c += texture2D(sceneTex, uv-0.007+noise);
  c += texture2D(sceneTex, uv-0.009+noise);
  c += texture2D(sceneTex, uv-0.011+noise);

  c.rgb = vec3((c.r+c.g+c.b)/3.0);
  c = c / (9.5 + contrast);
  gl_FragColor = c;
}			
