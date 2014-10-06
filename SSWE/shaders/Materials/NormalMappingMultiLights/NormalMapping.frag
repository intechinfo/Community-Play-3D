#define __CP3D__MAX_LIGHTS__ 8

uniform int  numLights;
uniform vec4 fvAmbient;
uniform vec4 fvLightColor[__CP3D__MAX_LIGHTS__];

uniform float fSpecularPower;
uniform float fSpecularStrength;
uniform float fBumpStrength;

uniform sampler2D baseMap;
uniform sampler2D bumpMap;

varying vec2  Texcoord;
varying vec3  ViewDirection;
varying vec3  LightDirection[__CP3D__MAX_LIGHTS__];
varying float LightDistMultiplier[__CP3D__MAX_LIGHTS__];

void main( void )
{
    vec4  fvBaseColor      = texture2D( baseMap, Texcoord );
    vec3  fvNormal         = texture2D( bumpMap, Texcoord ).yxz;
    vec4  fvTotalAmbient   = fvAmbient * fvBaseColor;
    fvNormal.xy *= 2.0;
    fvNormal.xy -= 1.0;
    fvNormal = (vec3(0.0,0.0,1.0) - fvNormal) * fBumpStrength + fvNormal;
    fvNormal = normalize(fvNormal);
    vec3  fvViewDirection = normalize( ViewDirection );
    
    vec4 fvTotalDiffuse  = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 fvTotalSpecular = vec4(0.0, 0.0, 0.0, 0.0);
    
    for (int i=0; i < numLights; i++) {
        vec3  fvLightDirection = normalize( LightDirection[i] );
        float fNDotL           = max(dot(fvNormal, fvLightDirection),0.0)-0.1;
        vec3  fvReflection     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection );
        float fRDotV           = max( 0.0, dot( fvReflection, fvViewDirection ) );
        fvTotalDiffuse  += fvLightColor[i] * fNDotL * fvBaseColor * LightDistMultiplier[i];
        fvTotalSpecular += fNDotL * fvLightColor[i] * ( pow( fRDotV, fSpecularPower ) ) * LightDistMultiplier[i];
    }
    
    vec4 color = ( fvTotalAmbient + fvTotalDiffuse + (fvTotalSpecular * fSpecularStrength));
    if(color.r>1.0) { color.gb += color.r - 1.0; }
    if(color.g>1.0) { color.rb += color.g - 1.0; }
    if(color.b>1.0) { color.rg += color.b - 1.0; }
    gl_FragColor = color;
}

