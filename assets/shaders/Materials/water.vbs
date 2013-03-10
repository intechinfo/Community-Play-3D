uniform mat4 mWorld;
uniform vec4 CamPos;
uniform float Time;
uniform float sinWave;
 
varying vec4 waterPos;
varying vec3 MultiVar;
varying vec3 worldView;
varying vec2 texCoords;
 
void main() {
    vec4 position=ftransform();
    gl_Position=position;
    waterPos=gl_Vertex*mWorld;
 
    MultiVar.y=Time/10000.0;
    
    if(sinWave>0.0) {
        MultiVar.x=(sin((position.x/3.0)+(Time*10.0/10000.0)))+(cos((position.z/3.0)+(Time*10.0/10000.0)));
        gl_Position.y+=MultiVar.x;
    }
    
    MultiVar.z=CamPos.y;
    worldView=CamPos.xyz-(mWorld*gl_Vertex).xyz;
 
    texCoords=(gl_MultiTexCoord0.xy+0.5)/2.0+vec2(MultiVar.y, MultiVar.y);
    texCoords.x=texCoords.x+sin(MultiVar.x*5.0)*(5.0/1000.0);
    texCoords.y=texCoords.y+cos(MultiVar.x*5.0)*(5.0/1000.0);
 
}
