varying vec2 Texcoord;
varying vec4 Pos;
void main( void )
{
    gl_Position = ftransform();
    Pos=gl_Vertex;
    Texcoord    = gl_MultiTexCoord0.xy;
    
}
