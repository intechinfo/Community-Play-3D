//Matrices
uniform mat4 WorldMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjMatrix;

varying vec3 WorldNormal;
varying  float Depth;

void main()
{
	// default positions
    gl_Position = ProjMatrix*(ViewMatrix*(WorldMatrix*gl_Vertex));
    
	// world normal
    vec4 normal = WorldMatrix*vec4(gl_Normal, 0.0);
    
    WorldNormal = 0.5*normal.rgb+0.5;
	Depth = gl_Position.z;
}
