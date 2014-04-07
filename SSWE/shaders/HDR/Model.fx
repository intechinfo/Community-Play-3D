//======================================================================
//
//	HDRSample
//
//		by MJP
//		09/20/08
//
//======================================================================
//
//	File:		LogLuv.fxh
//
//	Desc:		Contains functions for encoding and decoding the
//				the LogLuv format, based on Marco Salvi's "NAO32"
//				technique used for Heavenly Sword.  Mad props go to
//				Christer Ericson for posting an optimized version of
//				the encoding technique on his blog.
//				(http://realtimecollisiondetection.net/blog/?p=15).
//				Equally mad props go to remigius for helping me fix
//				the banding.
//
//======================================================================

// M matrix, for encoding
const static float3x3 M = float3x3(
    0.2209, 0.3390, 0.4184,
    0.1138, 0.6780, 0.7319,
    0.0102, 0.1130, 0.2969);

// Inverse M matrix, for decoding
const static float3x3 InverseM = float3x3(
	6.0013,	-2.700,	-1.7995,
	-1.332,	3.1029,	-5.7720,
	.3007,	-1.088,	5.6268);	

float4 LogLuvEncode(in float3 vRGB) 
{		 
    float4 vResult; 
    float3 Xp_Y_XYZp = mul(vRGB, M);
    Xp_Y_XYZp = max(Xp_Y_XYZp, float3(1e-6, 1e-6, 1e-6));
    vResult.xy = Xp_Y_XYZp.xy / Xp_Y_XYZp.z;
    float Le = 2 * log2(Xp_Y_XYZp.y) + 127;
    vResult.w = frac(Le);
    vResult.z = (Le - (floor(vResult.w*255.0f))/255.0f)/255.0f;
    return vResult;
}

float3 LogLuvDecode(in float4 vLogLuv)
{	
	float Le = vLogLuv.z * 255 + vLogLuv.w;
	float3 Xp_Y_XYZp;
	Xp_Y_XYZp.y = exp2((Le - 127) / 2);
	Xp_Y_XYZp.z = Xp_Y_XYZp.y / vLogLuv.y;
	Xp_Y_XYZp.x = vLogLuv.x * Xp_Y_XYZp.z;
	float3 vRGB = mul(Xp_Y_XYZp, InverseM);
	return max(vRGB, 0);
}



//======================================================================
//
//	HDRSample
//
//		by MJP
//		09/20/08
//
//======================================================================
//
//	File:		Model.fx
//
//	Desc:		Renders the scene model, applying a single directional
//				light source.
//
//======================================================================

// Camera position
float3 g_vCameraPositionWS;

// Lighting parameters
float3 g_vSunlightDirectionWS;
float3 g_vSunlightColor;
float g_fSunlightBrightness;

// Material parameters
float3 g_vDiffuseAlbedo;
float3 g_vSpecularAlbedo; 
float g_fSpecularPower;
float g_fReflectivity;

float g_fReflectionBrightness;

texture ReflectionMap;
samplerCUBE ReflectionSampler = sampler_state
{
    Texture = <ReflectionMap>;
    MinFilter = anisotropic;
    MagFilter = linear;
    MipFilter = linear;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
    MaxAnisotropy = 16;
};
sampler2D ReflectionMapSampler : register(s0);

// Calculated the contribution for a single light
float3 CalcLighting (	float3 vDiffuseAlbedo, 
						float3 vSpecularAlbedo, 
						float fSpecularPower, 
						float3 vLightColor, 
						float3 vNormal, 
						float3 vLightDir, 
						float3 vViewDir	)
{
	float3 R = normalize(reflect(-vLightDir, vNormal));
    
    // Calculate the raw lighting terms
    float fDiffuseReflectance = saturate(dot(vNormal, vLightDir));
    float fSpecularReflectance = saturate(dot(R, vViewDir));

	// Modulate the lighting terms based on the material colors, and the attenuation factor
    float3 vSpecular = vSpecularAlbedo * vLightColor * pow(fSpecularReflectance, fSpecularPower);
    float3 vDiffuse = vDiffuseAlbedo * vLightColor * fDiffuseReflectance;	

	// Lighting contribution is the sum of diffuse and specular terms
	return vDiffuse + vSpecular;
}			

// Pixel shader
float4 pixelMain (	in float3 in_vNormalWS		: TEXCOORD0,
					in float3 in_vPositionWS	: TEXCOORD1,
					uniform bool bEncodeLogLuv	) : COLOR0
{
	// Calculate the reflected view direction
	float3 vNormalWS = normalize(in_vNormalWS);
	float3 vViewDirWS = normalize(g_vCameraPositionWS - in_vPositionWS);
	float3 vReflectedDirWS = reflect(-vViewDirWS, vNormalWS);
	vReflectedDirWS.z *= -1;
	
	// Get the sunlight term
	float3 vColor = CalcLighting(	g_vDiffuseAlbedo,
									g_vSpecularAlbedo,
									g_fSpecularPower,
									g_vSunlightColor * g_fSunlightBrightness,
									vNormalWS,
									normalize(-g_vSunlightDirectionWS),
									vViewDirWS	);
									
	// Add in the reflection
	//float3 vReflection = texCUBE(ReflectionSampler, vReflectedDirWS);
	float3 vReflection = tex2D(ReflectionMapSampler, vReflectedDirWS.xy);
	vColor += vReflection * g_fReflectivity * g_fReflectionBrightness;																			
	
	// Encode to LogLuv?
	float4 vOutput;
	//if (bEncodeLogLuv)
		//vOutput = LogLuvEncode(vColor);
	//else
		vOutput = float4(vColor, 1.0f);							

	// return the color
	return vOutput;
}


technique Render
{
    pass p0
    {
		ZEnable = true;
		ZWriteEnable = true;
		AlphaBlendEnable = false;
		CullMode = CCW;
		AlphaTestEnable = false;
		StencilEnable = false;
    
        PixelShader = compile ps_2_0 pixelMain(false);
    }
}

technique RenderEncode
{
    pass p0
    {
		ZEnable = true;
		ZWriteEnable = true;
		AlphaBlendEnable = false;
		CullMode = CCW;
		AlphaTestEnable = false;
		StencilEnable = false;	
    
        PixelShader = compile ps_2_0 pixelMain(true);
    }
}				