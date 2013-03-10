#ifndef H_XEFFECTS_SHADERS
#define H_XEFFECTS_SHADERS

enum E_SHADER_EXTENSION
{
	ESE_GLSL,
	ESE_HLSL,

	ESE_COUNT
};

const char* LIGHT_MODULATE_P[ESE_COUNT] = {"uniform sampler2D ColorMapSampler;\n"
"uniform sampler2D ScreenMapSampler;\n"
""
"void main() "
"{		"
"	vec4 finalCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);\n"
"	vec4 lightCol = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);\n"
""
"	gl_FragColor = finalCol * lightCol;\n"
"}"
,
"sampler2D ColorMapSampler : register(s0);\n"
"sampler2D ScreenMapSampler : register(s1);\n"
""
"float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0"
"{		"
"	float4 finalCol = tex2D(ColorMapSampler, TexCoords);\n"
"	float4 lightCol = tex2D(ScreenMapSampler, TexCoords);\n"
""
"	return finalCol * lightCol;\n"
"}"};


const char* SHADOW_PASS_1P[ESE_COUNT] = {"void main() "
"{"
"	vec4 vInfo = gl_TexCoord[0];\n"
"	float depth = vInfo.z / vInfo.x;\n"
"   gl_FragColor = vec4(depth, depth * depth, 0.0, 0.0);\n"
"}"
,
"float4 pixelMain(float4 ClipPos: TEXCOORD0) : COLOR0"
"{"
"	float depth = ClipPos.z / ClipPos.x;\n"
"	return float4(depth, depth * depth, 0.0, 0.0);\n"
"}"};

const char* SHADOW_PASS_1PT[ESE_COUNT] = {"uniform sampler2D ColorMapSampler;\n"
""
"void main() "
"{"
"	vec4 vInfo = gl_TexCoord[0];\n"
""
"	float depth = vInfo.z / vInfo.x;\n"
""
"	float alpha = texture2D(ColorMapSampler, gl_TexCoord[1].xy).a;\n"
""
"    gl_FragColor = vec4(depth, depth * depth, 0.0, alpha);\n"
"}"
,
"sampler2D ColorMapSampler : register(s0);\n"
""
"float4 pixelMain(float4 Color: TEXCOORD0, float2 Texcoords: TEXCOORD1) : COLOR0"
"{"
"	float depth = Color.z / Color.w;\n"
"	"
"	float alpha = tex2D(ColorMapSampler, Texcoords).a;\n"
"	"
"	return float4(depth, depth * depth, 0.0, alpha);\n"
"}"};

const char* SHADOW_PASS_1V[ESE_COUNT] = {"uniform mat4 mWorldViewProj;\n"
"uniform float MaxD;\n"
""
"void main()"
"{"
"	vec4 tPos = mWorldViewProj * gl_Vertex;\n"
"	gl_Position = tPos;\n"
"	gl_TexCoord[0] = vec4(MaxD, tPos.y, tPos.z, tPos.w);\n"
""
"	gl_TexCoord[1].xy = gl_MultiTexCoord0.xy;\n"
"}"
,
"float4x4 mWorldViewProj;\n"
"float MaxD;\n"
""
"struct VS_OUTPUT "
"{"
"	float4 Position: POSITION0;\n"
"	float4 ClipPos: TEXCOORD0;\n"
"	float2 Texcoords: TEXCOORD1;\n"
"	float4 VColor: TEXCOORD2;\n"
"};\n"
""
"VS_OUTPUT vertexMain(float3 Position : POSITION0, float2 Texcoords : TEXCOORD0, float4 vColor : COLOR0)"
"{"
"	VS_OUTPUT  OUT;\n"
"	float4 hpos = mul(float4(Position.x, Position.y, Position.z, 1.0), mWorldViewProj);\n"
"   OUT.ClipPos = hpos;\n"
"	OUT.ClipPos.x = MaxD;\n"
"   OUT.Position = hpos;\n"
"	OUT.Texcoords = Texcoords;\n"
"	OUT.VColor = vColor;\n"
"	return(OUT);\n"
"}"};

const char* SHADOW_PASS_2P[ESE_COUNT] = {"uniform sampler2D ShadowMapSampler;\n"
"uniform vec4 LightColour;\n"
"varying float lightVal;\n"
""
"\n##ifdef VSM\n"
"float testShadow(vec2 texCoords, vec2 offset, float RealDist)\n"
"{\n"
"	vec4 shadTexCol = texture2D(ShadowMapSampler, texCoords + offset);\n"
""
"	float lit_factor = (RealDist <= shadTexCol.x) ? 1.0 : 0.0;\n"
""
"	float E_x2 = shadTexCol.y;\n"
"	float Ex_2 = shadTexCol.x * shadTexCol.x;\n"
"	float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);\n"
"	float m_d = (shadTexCol.x - RealDist);\n"
"	float p = variance / (variance + m_d * m_d);\n"
""
"	return (1.0 - max(lit_factor, p)) / float(SAMPLE_AMOUNT);\n"
"}\n"
"##else\n"
"float testShadow(vec2 smTexCoord, vec2 offset, float realDistance)"
"{"
"	vec4 texDepth = texture2D(ShadowMapSampler, vec2( smTexCoord + offset));\n"
"	float extractedDistance = texDepth.r;\n"
"	"
"	return (extractedDistance <= realDistance) ? (1.0  / float(SAMPLE_AMOUNT)) : 0.0;\n"
"}\n"
"##endif\n"
"\n"
"vec2 offsetArray[16];\n"
"\n"
"void main() \n"
"{"
"	vec4 SMPos = gl_TexCoord[0];\n"
"	vec4 MVar = gl_TexCoord[1];\n"
""
"	offsetArray[0] = vec2(0.0, 0.0);\n"
"	offsetArray[1] = vec2(0.0, 1.0);\n"
"	offsetArray[2] = vec2(1.0, 1.0);\n"
"	offsetArray[3] = vec2(-1.0, -1.0);\n"
"	offsetArray[4] = vec2(-2.0, 0.0);\n"
"	offsetArray[5] = vec2(0.0, -2.0);\n"
"	offsetArray[6] = vec2(2.0, -2.0);\n"
"	offsetArray[7] = vec2(-2.0, 2.0);\n"
"	offsetArray[8] = vec2(3.0, 0.0);\n"
"	offsetArray[9] = vec2(0.0, 3.0);\n"
"	offsetArray[10] = vec2(3.0, 3.0);\n"
"	offsetArray[11] = vec2(-3.0, -3.0);\n"
"	offsetArray[12] = vec2(-4.0, 0.0);\n"
"	offsetArray[13] = vec2(0.0, -4.0);\n"
"	offsetArray[14] = vec2(4.0, -4.0);\n"
"	offsetArray[15] = vec2(-4.0, 4.0);\n"
""
"    SMPos.xy  = SMPos.xy / SMPos.w / 2.0 + vec2(0.5, 0.5);\n"
""
"	vec4 finalCol = vec4(0.0, 0.0, 0.0, 0.0);\n"
""
"	// If this point is within the light's frustum.\n"
"##ifdef ROUND_SPOTLIGHTS\n"
"	float lengthToCenter = length(SMPos.xy - vec2(0.5, 0.5));\n"
"	if(SMPos.z - 0.01 > 0.0 && SMPos.z + 0.01 < MVar.z)\n"
"##else\n"
"	vec2 clampedSMPos = clamp(SMPos.xy, vec2(0.0, 0.0), vec2(1.0, 1.0));\n"
"	if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar.z)\n"
"##endif\n"
"	{"
"		float lightFactor = 1.0;\n"
"		float realDist = MVar.x / MVar.z - 0.002;\n"
"	"
"		for(int i = 0;i < SAMPLE_AMOUNT; i++)"
"			lightFactor -= testShadow(SMPos.xy, offsetArray[i] * MVar.w, realDist);\n"
""
"		// Multiply with diffuse.\n"
"##ifdef ROUND_SPOTLIGHTS\n"
"		finalCol = LightColour * lightFactor * MVar.y * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);\n"
"##else\n"
"		finalCol = LightColour * lightFactor * MVar.y;\n"
"##endif\n"
"	}"
""
"	gl_FragColor = finalCol;\n"
"}"
,
"sampler2D ShadowMapSampler : register(s0);\n"
"float4 LightColour;\n"
"\n"
"##ifdef VSM\n"
"float calcShadow(float2 texCoords, float2 offset, float RealDist)"
"{"
"	float4 shadTexCol = tex2D(ShadowMapSampler, texCoords + offset);\n"
""
"	float lit_factor = (RealDist <= shadTexCol.x);\n"
""
"	float E_x2 = shadTexCol.y;\n"
"	float Ex_2 = shadTexCol.x * shadTexCol.x;\n"
"	float variance = min(max(E_x2 - Ex_2, 0.00001) + 0.000001, 1.0);\n"
"	float m_d = (shadTexCol.x - RealDist);\n"
"	float p = variance / (variance + m_d * m_d);\n"
"	  "
"	return (1.0 - max(lit_factor, p)) / SAMPLE_AMOUNT;\n"
"}\n"
"##else\n"
"float calcShadow(float2 texCoords, float2 offset, float RealDist)"
"{"
"   float4 shadTexCol = tex2D(ShadowMapSampler,texCoords + offset);\n"
"   float extractedDistance = shadTexCol.r;\n"
"      "
"   return (extractedDistance <= RealDist ? (1.0  / SAMPLE_AMOUNT) : 0.0);\n"
"}\n"
"##endif\n"
""
"float4 pixelMain"
"("
"   float4 SMPos       : TEXCOORD0,"
"   float4 MVar        : TEXCOORD1,"
"   float2 TexCoords    : TEXCOORD2"
") : COLOR0"
"{"
"	const float2 offsetArray[16] = "
"	{"
"		float2(0.0, 0.0),"
"		float2(0.0, 1.0),"
"		float2(1.0, -1.0),"
"		float2(-1.0, 1.0),"
"		float2(-2.0, 0.0),"
"		float2(0.0, -2.0),"
"		float2(-2.0, -2.0),"
"		float2(2.0, 2.0),"
"		float2(3.0, 0.0),"
"		float2(0.0, 3.0),"
"		float2(3.0, -3.0),"
"		float2(-3.0, 3.0),"
"		float2(-4.0, 0.0),"
"		float2(0.0, -4.0),"
"		float2(-4.0, -4.0),"
"		float2(4.0, 4.0)"
"	};\n"
""
"	SMPos.xy = SMPos.xy / SMPos.w + float2(0.5, 0.5);\n"
""
"	float4 finalCol = float4(0.0, 0.0, 0.0, 0.0);\n"
""
"	// If this point is within the light's frustum.\n"
"##ifdef ROUND_SPOTLIGHTS\n"
"	float lengthToCenter = length(SMPos.xy - float2(0.5, 0.5));\n"
"	if(lengthToCenter < 0.5 && SMPos.z > 0.0 && SMPos.z < MVar[3])\n"
"##else\n"
"	float2 clampedSMPos = saturate(SMPos.xy);\n"
"	if(clampedSMPos.x == SMPos.x && clampedSMPos.y == SMPos.y && SMPos.z > 0.0 && SMPos.z < MVar[3])\n"
"##endif\n"
"	{"
"		float lightFactor = 1.0;\n"
"		float realDistance = MVar[0] / MVar[3] - 0.005;\n"
"	"
"		for(int i = 0;i < SAMPLE_AMOUNT; ++i)"
"			lightFactor -= calcShadow(SMPos.xy, offsetArray[i] * MVar[2], realDistance);\n"
""
"		// Multiply with diffuse.\n"
"##ifdef ROUND_SPOTLIGHTS\n"
"		finalCol = LightColour * lightFactor * MVar[1] * clamp(5.0 - 10.0 * lengthToCenter, 0.0, 1.0);\n"
"##else\n"
"		finalCol = LightColour * lightFactor * MVar[1];\n"
"##endif\n"
"	}"
"	"
"	return finalCol;\n"
"}"};

const char* SHADOW_PASS_2V[ESE_COUNT] = {"struct VS_OUTPUT "
"{"
"	vec4 Position;\n"
"	vec4 ShadowMapSamplingPos;\n"
"	vec4 MVar;\n"
"};\n"
""
"uniform float MaxD, MAPRES;\n"
"uniform vec3 LightPos;\n"
"uniform mat4 mWorldViewProj;\n"
"uniform mat4 mWorldViewProj2;\n"
""
"VS_OUTPUT vertexMain( in vec3 Position) "
"{"
"	VS_OUTPUT OUT;\n"
""
"	OUT.Position = (mWorldViewProj * vec4(Position.x, Position.y, Position.z, 1.0));\n"
"	OUT.ShadowMapSamplingPos = (mWorldViewProj2 * vec4(Position.x, Position.y, Position.z, 1.0));\n"
""
"	vec3 lightDir = normalize(LightPos - Position);\n"
"	"
"	OUT.MVar.x = OUT.ShadowMapSamplingPos.z;\n"
"	OUT.MVar.y = dot(normalize(gl_Normal.xyz), lightDir);\n"
"	OUT.MVar.z = MaxD;\n"
"	OUT.MVar.w = 1.0 / MAPRES;\n"
""
"	return OUT;\n"
"}"
""
"void main() "
"{"
"	VS_OUTPUT vOut = vertexMain(gl_Vertex.xyz);\n"
""
"	gl_Position = vOut.Position;\n"
"	gl_TexCoord[0] = vOut.ShadowMapSamplingPos;\n"
"	gl_TexCoord[1] = vOut.MVar;\n"
"}"
,
"float4x4 mWorldViewProj;\n"
"float4x4 mWorldViewProj2;\n"
"float3 LightPos;\n"
"float ShadDark;\n"
"float MaxD;\n"
"float EnableLighting;\n"
"float MAPRES;\n"
""
"struct VS_OUTPUT "
"{"
"	float4 Position				: POSITION0;\n"
"	float4 ShadowMapSamplingPos : TEXCOORD0; "
"	float4 MVar        			: TEXCOORD1;\n"
"	float2 TexCoords            : TEXCOORD2;\n"
"};\n"
""
"VS_OUTPUT vertexMain( "
"   	float3 Position	: POSITION0,"
"	float2 TexCoords : TEXCOORD0,"
"	float2 TexCoords2 : TEXCOORD1,"
"	float3 Normal : NORMAL"
"  )"
"{"
"	VS_OUTPUT  OUT;\n"
"    OUT.Position = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj);\n"
"	float4 SMPos = mul(float4(Position.x,Position.y,Position.z,1.0), mWorldViewProj2);\n"
"	SMPos.xy = float2(SMPos.x, -SMPos.y) / 2.0;\n"
"	"
"	OUT.ShadowMapSamplingPos = SMPos;\n"
"		"
"	float3 LightDir = normalize(LightPos - Position.xyz);\n"
"	"
"	OUT.MVar = float4(SMPos.z, dot(Normal, LightDir), 1.0 / MAPRES, MaxD);\n"
"	OUT.TexCoords = TexCoords;\n"
"	"
"	return(OUT);\n"
"}"};


const char* SIMPLE_P[ESE_COUNT] = {"uniform sampler2D ColorMapSampler;\n"
""
"void main() "
"{		"
"	vec4 finalCol = texture2D(ColorMapSampler, gl_TexCoord[0].xy);\n"
"	gl_FragColor = finalCol;\n"
"}"
,
"sampler2D ColorMapSampler : register(s0);\n"
""
"float4 pixelMain(float2 TexCoords : TEXCOORD0) : COLOR0"
"{		"
"	float4 finalCol = tex2D(ColorMapSampler, TexCoords);\n"
"	return finalCol;\n"
"}"};


const char* WHITE_WASH_P[ESE_COUNT] = {"uniform sampler2D ColorMapSampler;\n"
""
"void main() "
"{"
"	float alpha = texture2D(ColorMapSampler, gl_TexCoord[1].xy).a;\n"
""
"    gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);\n"
"}"
,
"sampler2D ColorMapSampler : register(s0);\n"
""
"float4 pixelMain(float4 Color: TEXCOORD0, float2 Texcoords: TEXCOORD1) : COLOR0"
"{"
"	float alpha = tex2D(ColorMapSampler, Texcoords).a;\n"
""
"	return float4(1.0, 1.0, 1.0, alpha);\n"
"}"};


const char* WHITE_WASH_P_ADD[ESE_COUNT] = {"uniform sampler2D ColorMapSampler;\n"
"float luminance(vec3 color)"
"{"
"	return clamp(color.r * 0.3 + color.g * 0.59 + color.b * 0.11, 0.0, 1.0);\n"
"}"
"void main() "
"{"
"	vec4 diffuseTex = texture2D(ColorMapSampler, gl_TexCoord[1].xy);\n"
"	//diffuseTex *= gl_TexCoord[2];\n"
""
"    gl_FragColor = vec4(1.0, 1.0, 1.0, luminance(diffuseTex.rgb));\n"
"}"
,
"sampler2D ColorMapSampler : register(s0);\n"
""
"float luminance(float3 color)"
"{"
"	return clamp(color.r * 0.3 + color.g * 0.59 + color.b * 0.11, 0.0, 1.0);\n"
"}"
""
"float4 pixelMain(float4 Color : TEXCOORD0, float2 Texcoords : TEXCOORD1, float4 VColor : TEXCOORD2) : COLOR0"
"{"
"	float4 diffuseTex = tex2D(ColorMapSampler, Texcoords);\n"
"	diffuseTex *= VColor;\n"
""
"	return float4(1.0, 1.0, 1.0, luminance(diffuseTex.rgb));\n"
"}"};

const char* SCREEN_QUAD_V[ESE_COUNT] = {"uniform float screenX, screenY; \n"
"uniform vec3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; \n"
"uniform vec3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; \n"
"void main() \n" 
"{\n" 
"	gl_Position = vec4(gl_Vertex.x, gl_Vertex.y, 0.0, 1.0); \n"
"	vec2 tCoords; \n" 
"	tCoords.x = 0.5 * (1.0 + gl_Vertex.x); \n" 
"	tCoords.y = 0.5 * (1.0 + gl_Vertex.y); \n" 
"	gl_TexCoord[0].xy = tCoords.xy; \n" 
"	tCoords.y = 1.0 - tCoords.y; \n" 
"	vec3 tLStart = mix(LineStarts0, LineStarts1, tCoords.x); \n" 
"	vec3 bLStart = mix(LineStarts2, LineStarts3, tCoords.x); \n" 
"	gl_TexCoord[1].xyz = mix(tLStart, bLStart, tCoords.y); \n" 
"	vec3 tLEnd = mix(LineEnds0, LineEnds1, tCoords.x); \n" 
"	vec3 bLEnd = mix(LineEnds2, LineEnds3, tCoords.x); \n" 
"	gl_TexCoord[2].xyz = mix(tLEnd, bLEnd, tCoords.y); \n" 
"	gl_TexCoord[3].xy = vec2(screenX, screenY); \n"
"}"
, 
"float screenX, screenY; \n"
"float3 LineStarts0, LineStarts1, LineStarts2, LineStarts3; \n"
"float3 LineEnds0, LineEnds1, LineEnds2, LineEnds3; \n"
"struct VS_OUTPUT \n"
"{"
"	float4 Position		: POSITION0;"
"	float2 TexCoords	: TEXCOORD0;"
"	float3 LStart		: TEXCOORD1;"
"	float3 LEnd			: TEXCOORD2;"
"	float2 ScreenSize	: TEXCOORD3;"
"}; \n"
"VS_OUTPUT vertexMain(float3 Position : POSITION0) \n"
"{ \n"
"	VS_OUTPUT OUT; \n"
"   OUT.Position = float4(Position.x,Position.y, 0.0, 1.0); \n"
"	OUT.TexCoords.x = 0.5 * (1.0 + Position.x + (1.0 / screenX)); \n" 
"	OUT.TexCoords.y = 1.0 - 0.5 * (1.0 + Position.y - (1.0 / screenY)); \n" 
"	float3 tLStart = lerp(LineStarts0, LineStarts1, OUT.TexCoords.x); \n" 
"	float3 bLStart = lerp(LineStarts2, LineStarts3, OUT.TexCoords.x); \n" 
"	OUT.LStart = lerp(tLStart, bLStart, OUT.TexCoords.y); \n" 
"	float3 tLEnd = lerp(LineEnds0, LineEnds1, OUT.TexCoords.x); \n" 
"	float3 bLEnd = lerp(LineEnds2, LineEnds3, OUT.TexCoords.x); \n" 
"	OUT.LEnd = lerp(tLEnd, bLEnd, OUT.TexCoords.y); \n" 
"	OUT.ScreenSize = float2(screenX, screenY); \n"
"	return(OUT); \n"
"} \n"};


const char* VSM_BLUR_P[ESE_COUNT] = {"uniform sampler2D ColorMapSampler;\n"
"\n"
"vec2 offsetArray[5];\n"
"\n"
"void main() \n"
"{\n"
"\n"
"##ifdef VERTICAL_VSM_BLUR\n"
"	offsetArray[0] = vec2(0.0, 0.0);\n"
"	offsetArray[1] = vec2(0.0, -1.5 / gl_TexCoord[3].y);\n"
"	offsetArray[2] = vec2(0.0, 1.5 / gl_TexCoord[3].y);\n"
"	offsetArray[3] = vec2(0.0, -2.5 / gl_TexCoord[3].y);\n"
"	offsetArray[4] = vec2(0.0, 2.5 / gl_TexCoord[3].y);\n"
"##else\n"
"	offsetArray[0] = vec2(0.0, 0.0);\n"
"	offsetArray[1] = vec2(-1.5 / gl_TexCoord[3].x, 0.0);\n"
"	offsetArray[2] = vec2(1.5 / gl_TexCoord[3].x, 0.0);\n"
"	offsetArray[3] = vec2(-2.5 / gl_TexCoord[3].x, 0.0);\n"
"	offsetArray[4] = vec2(2.5 / gl_TexCoord[3].x, 0.0);\n"
"##endif\n"
"\n"
"	vec4 BlurCol = vec4(0.0, 0.0, 0.0, 0.0);\n"
"\n"
"	for(int i = 0;i < 5;++i)\n"
"		BlurCol += texture2D(ColorMapSampler, clamp(gl_TexCoord[0].xy + offsetArray[i], vec2(0.001, 0.001), vec2(0.999, 0.999)));\n"
"\n"
"	gl_FragColor = BlurCol / 5.0;\n"
"}\n"
,
"sampler2D ColorMapSampler : register(s0);\n"
"\n"
"float4 pixelMain ( float4 Texcoords : TEXCOORD0, float2 ScreenSize : TEXCOORD3 ) : COLOR0\n"
"{\n"
"	float2 offsetArray[5];\n"
"##ifdef VERTICAL_VSM_BLUR\n"
"	offsetArray[0] = float2(0, 0);\n"
"	offsetArray[1] = float2(0, 1.5 / ScreenSize.y);\n"
"	offsetArray[2] = float2(0, -1.5 / ScreenSize.y);\n"
"	offsetArray[3] = float2(0, 2.5 / ScreenSize.y);\n"
"	offsetArray[4] = float2(0, -2.5 / ScreenSize.y);\n"
"##else\n"
"	offsetArray[0] = float2(0, 0);\n"
"	offsetArray[1] = float2(1.5 / ScreenSize.x, 0);\n"
"	offsetArray[2] = float2(-1.5 / ScreenSize.x, 0);\n"
"	offsetArray[3] = float2(2.5 / ScreenSize.x, 0);\n"
"	offsetArray[4] = float2(-2.5 / ScreenSize.x, 0);\n"
"##endif\n"
"\n"
"	float4 finalVal = float4(0.0, 0.0, 0.0, 0.0);\n"
"\n"
"	for(int i = 0;i < 5;++i)\n"
"		finalVal += tex2D(ColorMapSampler, clamp(Texcoords.xy + offsetArray[i], float2(0.001, 0.001), float2(0.999, 0.999)));\n"
"\n"
"	return finalVal / 5.0;\n"
"}\n"};

#endif
