//Matrices
uniform mat4  WorldMatrix;
uniform mat4  ViewMatrix;
uniform mat4  ReflectedViewMatrix;
uniform mat4  ProjMatrix;
uniform float ElapsedTime;
uniform float WaveLength;
uniform vec2  WindForce;

varying vec4 Reflected;
varying vec4 Refracted;
varying vec2 Bump1;
varying vec2 Bump2;
varying vec2 Bump3;
varying vec4 PosWorld;

void main()
{
	// default positions
    gl_Position = ProjMatrix*(ViewMatrix*(WorldMatrix*gl_Vertex));
    
	// reflected position
    Reflected = ProjMatrix*(ReflectedViewMatrix*(WorldMatrix*gl_Vertex));

	// refrected position
    Refracted = gl_Position;
	
	// texture coordinates of the normal map
	Bump1 = WindForce*ElapsedTime*0.001+gl_Vertex.xy*WaveLength;
	Bump2 = WindForce*ElapsedTime*0.001+gl_Vertex.xy*WaveLength*0.3;
	Bump3 = WindForce*ElapsedTime*0.001-gl_Vertex.xy*WaveLength*0.66;
	
	// position in worldspace
	PosWorld = WorldMatrix*gl_Vertex;
}
