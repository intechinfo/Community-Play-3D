struct VertexToFragment
{

	float4 ProjCoord:TEXCOORD0;
	float4 Position : POSITION;
	float3 LStart:TEXCOORD1;
	float3 LEnd:TEXCOORD2;
};

sampler2D Tex1 : register(s0);
sampler2D Tex2 : register(s2);
sampler2D NormalTex : register(s3);

float3 sunDirection;
float2 bumpDelta;

float4 pixelMain(VertexToFragment IN) : COLOR0
{

	float4 WaterColor=float4(0.2f,0.8f,1.0f,1.0f);
	float4 SpecularColor=float4(0.6f,0.6f,0.6f,0.6f);
	float SpecularExponent=32.0f;

	float3 lVec=IN.LEnd-IN.LStart;
	float3 lDir=normalize(lVec);
	float lLength=length(lVec);
	float currentDepth=tex2D(Tex2,IN.ProjCoord.xy).a*lLength;
	float3 currentWPos=IN.LStart+currentDepth*lDir;

	float4 screenColor=tex2D(Tex1,IN.ProjCoord.xy);
	float3 planeNormal=float3(0.0f,IN.LStart.y<0.0f ? -1.0f : 1.0f,0.0f);
	float planeDP=dot(planeNormal,lDir);
	if(IN.LStart.y < 0.0f)
		screenColor*=WaterColor;
	float4 finalColour=screenColor;
	if(planeDP < 0.0)
	{
		float planeDepth = -dot(planeNormal, IN.LStart) / planeDP;

		if(planeDepth < currentDepth && planeDepth < lLength)
		{
			float3 planePos = IN.LStart + lDir * planeDepth;
		
			float heightDiff = sin(planePos.x / 20.0) + sin(planePos.z / 20.0);
			planePos.y += heightDiff;
			float4 normalMap = tex2D(NormalTex, (planePos.xz / 50.0)+bumpDelta);
			float3 lightNormal = normalMap.xyz - float3(0.5, 0.5, 0.5);
			lightNormal = normalize(lightNormal);
			float2 refCoords = clamp(IN.ProjCoord.xy + lightNormal.xy / 20.0, float2(0.01, 0.01), float2(0.99, 0.99));
			float refDepth = tex2D(Tex2,IN.ProjCoord.xy).a * lLength;
			
			finalColour = WaterColor;

			if(refDepth - 0.5f > planeDepth)
				finalColour *= tex2D(Tex1, refCoords);
			else
				finalColour *= screenColor;

			
			float diffuse = dot(lightNormal, sunDirection);
			float3 Reflect = normalize(2.0 * diffuse * lightNormal - sunDirection);
			float4 specular = pow(saturate(dot(Reflect, lDir)), SpecularExponent) * SpecularColor;
			finalColour += specular;

			float4 foamColour = float4(0.8, 0.8, 0.8, 0.8)*5;			
			float foamFac = clamp(((currentDepth - planeDepth - 3.0) / 5.0), 0.0, 1.0);
			finalColour = lerp(foamColour, finalColour, foamFac);

			float shoreFac = clamp((currentDepth - planeDepth) / 20.0, 0.0, 1.0);

			finalColour = lerp(screenColor, finalColour, shoreFac);
			
			float fadeFac = clamp(1.0 - (planeDepth - lLength / 10.0) / lLength, 0.0, 1.0);
			finalColour = lerp(screenColor, finalColour, fadeFac);
		}
	}
	
	//Color0=finalColour;//(tex2D(Tex1,IN.ProjCoord.xy)*0.9f + tex2D(Tex1,IN.ProjCoord.xy)*0.0f);
	return finalColour;
}