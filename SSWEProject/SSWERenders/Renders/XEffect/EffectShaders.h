#ifndef H_XEFFECTS_SHADERS
#define H_XEFFECTS_SHADERS

#pragma once

enum E_SHADER_EXTENSION
{
	ESE_GLSL=0,
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
"\n"
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
"float4 pixelMain(float4 ClipPos: TEXCOORD0) : COLOR0\n"
"{\n"
"	float depth = ClipPos.z / ClipPos.x;\n"
"	return float4(depth, depth * depth, 0.0, 0.0);\n"
"}"};

const char* SHADOW_PASS_1P_2T[ESE_COUNT] = {
"uniform float maxSSAODepthPass;\n"
"void main() "
"{"
"	vec4 vInfo = gl_TexCoord[0];\n"
"	float depth = vInfo.z / vInfo.x;\n"
"	float ssaoDepth = vInfo.z / maxSSAODepthPass;\n"
"   gl_FragData[0] = vec4(depth, depth * depth, 0.0, 0.0);\n"
"	gl_FragData[1] = vec4(ssaoDepth, ssaoDepth * ssaoDepth, 0.0, 0.0);\n"
"}"
,
"struct PixelOutput {\n"
"	float4 DepthPassColor : COLOR0;\n"
"	float4 SSAOColor : COLOR1;\n"
"};\n"
"\n"
"float maxSSAODepthPass;\n"
"\n"
"PixelOutput pixelMain(float4 ClipPos: TEXCOORD0)\n"
"{\n"
"	PixelOutput output = (PixelOutput)0;\n"
"	float depth = ClipPos.z / ClipPos.x;\n"
"	float ssaoDepth = ClipPos.z / maxSSAODepthPass;\n"
"	output.DepthPassColor = float4(depth, depth * depth, 0.0, 0.0);\n"
"	output.SSAOColor = float4(ssaoDepth, ssaoDepth * ssaoDepth, 0.0, 0.0);\n"
"	return output;\n"
"}"
};

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
"	float depth = Color.z / Color.x;\n"
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
"}"
};

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
"\n"
"sampler2D ShadowMapSampler : register(s0);\n"
"##ifdef _CP3D_COMPUTE_FOG_\n"
"sampler2D FogMapSampler : register(s1);\n"
"##endif\n"
"\n"
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
"	}\n"
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

const char* SELECTION_PASS_V[ESE_COUNT] = {
"void main() {\n"
"   gl_Position = ftransform();\n"
"   gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
"}\n"
"\n"
,
"float4x4 worldViewProj;\n"
"\n"
"struct VertexShaderOutput {\n"
"    float4 Position : POSITION0;\n"
"    float2 TexCoords : TEXCOORD0;\n"
"};\n"
"\n"
"struct VertexShaderInput {\n"
"    float4 Position : POSITION0;\n"
"    float2 TexCoords : TEXCOORD0;\n"
"};\n"
"\n"
"VertexShaderOutput vertexMain(VertexShaderInput input) {\n"
"    VertexShaderOutput output = (VertexShaderOutput)0;\n"
"\n"
"    float4 Position = mul(input.Position, worldViewProj);\n"
"\n"
"    output.Position = Position;\n"
"    output.TexCoords = input.TexCoords;\n"
"\n"
"    return output;\n"
"}\n"
};

const char* SELECTION_PASS_P[ESE_COUNT] = {
"uniform sampler2D diffuse;\n"
"\n"
"float getAverage(vec4 color) {\n"
"    return (color.r + color.g + color.b) / 3.0;\n"
"}\n"
"void main() {\n"
"    vec4 color = texture2D(diffuse, gl_TexCoord[0].xy);\n"
"    float average = getAverage(color);\n"
"    if (average >= 0.2) {\n"
"        color = (vec4(136, 84, 245, 255) / 255.0) + (color * -average);\n"
"    }\n"
"    gl_FragColor = color;\n"
"}\n"
,
"sampler2D diffuse : register(s0);\n"
"\n"
"struct VertexShaderOutput {\n"
"    float4 Position : POSITION0;\n"
"    float2 TexCoords : TEXCOORD0;\n"
"};\n"
"\n"
"struct SAverage {\n"
"    float4 color;\n"
"    float average;\n"
"};\n"
"\n"
"SAverage getAverageAt(float2 TexCoords) {\n"
"    SAverage av = (SAverage)0;    av.color = tex2D(diffuse, TexCoords);\n"
"    av.average = (av.color.r + av.color.g + av.color.b) / 3.0;\n"
"    return av;\n"
"}\n"
"float4 pixelMain(VertexShaderOutput input) : COLOR0 {\n"
"    SAverage av = getAverageAt(input.TexCoords);\n"
"\n"
"    float4 color;\n"
"    if (av.average < 0.2) {\n"
"        color = av.color;\n"
"    } else {\n"
"        float r = 136;\n"
"        float g = 84;\n"
"        float b = 245;\n"
"        float a = 255;\n"
"        color = (float4(r, g, b, a) / 255) + (av.color * -av.average);\n"
"    }\n"
"\n"
"    return color;\n"
"}\n"
};

const char *DEPTH_OF_FIELD_P[ESE_COUNT] = {
"uniform sampler2D ScreenMapSampler;\n"
"uniform sampler2D ColorMapSampler;\n"
"uniform sampler2D DepthMapSampler;\n"
"\n"
"float getDepth(vec2 coords) {\n"
"    vec4 texDepth = texture2D(DepthMapSampler, coords);\n"
"    return texDepth.r;\n"
"}\n"
"\n"
"void main(void) {\n"
"    vec4 sharp = texture2D(ScreenMapSampler, gl_TexCoord[0].xy);\n"
"    vec4 blur  = texture2D(ColorMapSampler, gl_TexCoord[0].xy);\n"
"    float dist   = clamp(getDepth(gl_TexCoord[0].xy)*10.0, 0.0, 1.0);\n"
"    float factor = 0.0;\n"
"\n"
"    if (dist < 0.05)\n"
"        factor = 1.0;\n"
"    else if(dist < 0.1)\n"
"        factor = 20.0 *(0.1 - dist);\n"
"    else if(dist < 0.5)\n"
"        factor=0.0;\n"
"    else\n"
"        factor = 2.0 *(dist - 0.5);\n"
"\n"
"    factor = clamp(factor, 0.0, 0.90);\n"
"    gl_FragColor = mix(sharp, blur, factor);\n"
"}\n"
,
"sampler2D SceneBuffer : register(s1);\n"
"sampler2D BlurBuffer : register(s0);\n"
"sampler2D DistanceBuffer : register(s2);\n"
"\n"
"float getDepth(float2 coords) {\n"
"	float4 texDepth = tex2D(DistanceBuffer, coords);\n"
"	return texDepth.r;\n"
"}\n"
"\n"
"float4 pixelMain(float2 texCoord: TEXCOORD0) : COLOR0 {\n"
"	float4 sharp = tex2D(SceneBuffer, texCoord);\n"
"	float4 blur  = tex2D(BlurBuffer, texCoord);\n"
"	float dist   = saturate(getDepth(texCoord)*10.0);\n"
"	float factor = 0.0;\n"
"	\n"
"	if (dist < 0.05)\n"
"		factor = 1.0;\n"
"	else if(dist < 0.1)\n"
"		factor = 20.0 *(0.1 - dist);\n"
"	else if(dist < 0.5)\n"
"		factor=0;\n"
"	else\n"
"		factor = 2.0 *(dist - 0.5);\n"
"	\n"
"	factor = clamp(factor, 0.0, 0.90);\n"
"	return lerp(sharp, blur, factor);\n"
"}\n"
};

const char *DEPTH_NORMAL_SCATTERING_V[ESE_COUNT] = {
"##ifdef _CP3D_DEPTH_PASS_\n"
"uniform float CP3DDepthPassMaxD;\n"
"##endif\n"
""
"##ifdef _CP3D_NORMAL_PASS_\n"
"uniform mat4 mWorldView;\n"
"##endif\n"
""
"void main(void)\n"
"{\n"
"	gl_Position = ftransform();\n"
"	gl_TexCoord[0] = gl_MultiTexCoord0;\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"   gl_TexCoord[1] = gl_Position;\n"
"	gl_TexCoord[1].x = CP3DDepthPassMaxD;\n"
"	gl_TexCoord[2] = gl_TexCoord[0];\n"
"	gl_TexCoord[3] = gl_Color;\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"   gl_TexCoord[5] = mWorldView * vec4(gl_Normal.xyz, 0.0);\n"
"   gl_TexCoord[6] = mWorldView * vec4(gl_MultiTexCoord1.xyz, 0.0);\n"
"   gl_TexCoord[7].xyz = cross(gl_TexCoord[5].xyz, gl_TexCoord[6].xyz);\n"
"   gl_TexCoord[4] = mWorldView * gl_Position;\n"
"	##endif\n"
""
"}\n"
,
"float4x4 mWorldViewProj;\n"
""
"##ifdef _CP3D_DEPTH_PASS_\n"
"float CP3DDepthPassMaxD;\n"
"##endif\n"
""
"##ifdef _CP3D_NORMAL_PASS_\n"
"float4x4 mWorldView;\n"
"##endif\n"
""
"struct VS_INPUT\n"
"{\n"
"	float4 Position : POSITION0;\n"
"	float4 TexCoords : TEXCOORD0;\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"	float4 DepthColor : COLOR0;\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"   float3 NormalNormal : NORMAL;\n"
"   float3 NormalTangent : TANGENT0;\n"
"	##endif\n"
""
"};\n"
""
"struct VS_OUTPUT\n"
"{\n"
"	float4 Position: POSITION0;\n"
"	float4 TexCoords : TEXCOORD0;\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"	float4 DepthClipPos: TEXCOORD1;\n"
"	float2 DepthTexcoords: TEXCOORD2;\n"
"	float4 DepthColor: TEXCOORD3;\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"   float3 NormalViewPos : TEXCOORD4;\n"
"   float3 NormalNormal : TEXCOORD5;\n"
"   float3 NormalTangent : TEXCOORD6;\n"
"   float3 NormalBiNormal : TEXCOORD7;\n"
"	##endif\n"
""
"};\n"
""
"VS_OUTPUT vertexMain(VS_INPUT input)\n"
"{\n"
"	VS_OUTPUT  OUT;\n"
"	OUT.Position = mul(input.Position, mWorldViewProj);\n"
"	OUT.TexCoords = input.TexCoords;\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"   OUT.DepthClipPos = OUT.Position;\n"
"	OUT.DepthClipPos.x = CP3DDepthPassMaxD;\n"
"	OUT.DepthTexcoords = input.TexCoords.xy;\n"
"	OUT.DepthColor = input.DepthColor;\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"   OUT.NormalNormal = mul(mWorldView, float4(input.NormalNormal,0)).xyz;\n"
"   OUT.NormalTangent = mul(mWorldView, float4(input.NormalTangent,0)).xyz;\n"
"   OUT.NormalBiNormal = cross(OUT.NormalNormal, OUT.NormalTangent);\n"
"   OUT.NormalViewPos = mul(mWorldView, input.Position).xyz;\n"
"	##endif\n"
""
"	return(OUT);\n"
"}\n"
};

const char *DEPTH_NORMAL_SCATTERING_P[ESE_COUNT] = {
"uniform sampler2D DiffuseMapSampler;\n"
""
"##ifdef _CP3D_NORMAL_PASS_\n"
"uniform sampler2D NormalMapSampler;\n"
"uniform float CP3DNormalPassFarDistance;\n"
"##endif\n"
""
"##ifdef _CP3D_SCATTERING_PASS_\n"
"uniform int CP3DScatteringPassRenderNormal;"
"##endif\n"
""
"void main(void)"
"{"
"	vec4 diffuseColor = texture2D(DiffuseMapSampler, gl_TexCoord[0].xy);\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"	float CP3DDepthPassDepth = gl_TexCoord[1].z / gl_TexCoord[1].x;\n"
"	CP3DCOLOR0 = vec4(CP3DDepthPassDepth, CP3DDepthPassDepth * CP3DDepthPassDepth, 0.0, 0.0);\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"	CP3DCOLOR1 = vec4(0.0, 0.0, 0.0, 0.0);\n"
"   vec3 CP3DNormalPassTexNormal = (texture2D(NormalMapSampler, gl_TexCoord[0].xy).xyz-0.5)*2.0;\n"
"   mat3 CP3DNormalPassNormalRotation = mat3(gl_TexCoord[6].xy, gl_TexCoord[6].z,\n"
"                                            gl_TexCoord[7].xy, gl_TexCoord[7].z,\n"
"                                            gl_TexCoord[5].xy, gl_TexCoord[5].z);\n"
"   CP3DCOLOR1.rgb = normalize(CP3DNormalPassTexNormal * CP3DNormalPassNormalRotation);\n"
"   CP3DCOLOR1.a = length(gl_TexCoord[4]) / CP3DNormalPassFarDistance;\n"
"	##endif\n"
""
"	##ifdef _CP3D_SCATTERING_PASS_\n"
"	if (CP3DScatteringPassRenderNormal == 1)\n"
"		CP3DCOLOR2 = diffuseColor;\n"
"	else\n"
"		CP3DCOLOR2 = vec4(0.0, 0.0, 0.0, 0.0);\n"
"	##endif\n"
""
"}"
,
"sampler2D DiffuseMapSampler : register(s0);\n"
""
"##ifdef _CP3D_NORMAL_PASS_\n"
"sampler2D NormalMapSampler : register(s1);\n"
"float CP3DNormalPassFarDistance;\n"
"##endif\n"
""
"##ifdef _CP3D_SCATTERING_PASS_\n"
"int CP3DScatteringPassRenderNormal;"
"##endif\n"
""
"struct PixelOutput {\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"	float4 DepthPassColor : CP3DCOLOR0;\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"	float4 NormalPassColor : CP3DCOLOR1;\n"
"	##endif\n"
""
"	##ifdef _CP3D_SCATTERING_PASS_\n"
"	float4 ScatteringPassColor : CP3DCOLOR2;\n"
"	##endif\n"
"};\n"
""
"struct VS_OUTPUT\n"
"{\n"
"	float4 Position: POSITION0;\n"
"	float4 TexCoords : TEXCOORD0;\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"	float4 ClipPos: TEXCOORD1;\n"
"	float2 Texcoords: TEXCOORD2;\n"
"	float4 VColor: TEXCOORD3;\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"   float3 NormalViewPos : TEXCOORD4;\n"
"   float3 NormalNormal : TEXCOORD5;\n"
"   float3 NormalTangent : TEXCOORD6;\n"
"   float3 NormalBiNormal : TEXCOORD7;\n"
"	##endif\n"
""
"};\n"
""
"PixelOutput pixelMain(VS_OUTPUT input)"
"{"
"	PixelOutput OUT;\n"
"	float4 diffuseColor = tex2D(DiffuseMapSampler, input.TexCoords.xy);\n"
""
"	##ifdef _CP3D_DEPTH_PASS_\n"
"	float CP3DDepthPassDepth = input.ClipPos.z / input.ClipPos.x;\n"
"	OUT.DepthPassColor = float4(CP3DDepthPassDepth, CP3DDepthPassDepth * CP3DDepthPassDepth, 0.0, 0.0);\n"
"	##endif\n"
""
"	##ifdef _CP3D_NORMAL_PASS_\n"
"	OUT.NormalPassColor = float4(0.0, 0.0, 0.0, 0.0);\n"
"   float3 CP3DNormalPassTexNormal = (tex2D(NormalMapSampler, input.TexCoords)-0.5)*2;\n"
"   float3x3 CP3DNormalPassNormalRotation = float3x3(input.NormalTangent, input.NormalBiNormal, input.NormalNormal);\n"
"   OUT.NormalPassColor.rgb = normalize(mul(CP3DNormalPassTexNormal, CP3DNormalPassNormalRotation));\n"
"   OUT.NormalPassColor.a = length(input.NormalViewPos) / CP3DNormalPassFarDistance;\n"
"	##endif\n"
""
"	##ifdef _CP3D_SCATTERING_PASS_\n"
"	if (CP3DScatteringPassRenderNormal == 1)\n"
"		OUT.ScatteringPassColor = diffuseColor;\n"
"	else\n"
"		OUT.ScatteringPassColor = float4(0.0, 0.0, 0.0, 0.0);\n"
"	##endif\n"
""
"	return(OUT);\n"
"}"
};

#endif
