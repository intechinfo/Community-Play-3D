/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CGrassPainter.h"

const char *pixelGrass = {
"uniform float4x4 worldViewProj;\n"
"uniform float4x4 mWorld;\n"
"uniform float3 eyePositionO;\n"
"uniform float3 LightPos;\n"
"uniform float time; //Runs from 0 to 10 per second.\n"
"\n"
"void vertexMain(\n"
"\n"
"	float4 position	: POSITION,\n"
"	float3 normal	: NORMAL,\n"
"	\n"
"	float2 texCoord	 : TEXCOORD0,\n"
"	float3 tangent : TEXCOORD1,\n"
"\n"
"	float4 vertColor : COLOR, //In this implementation a per vertex lightmap is used\n"
"\n"
"	out float4 oVertColor      	: COLOR,\n"
"	out float2 oTexCoord	  	: TEXCOORD0,\n"
"	out float2 oTexCoord2	  	: TEXCOORD1,\n"
"	out float4 oPosition	   	: POSITION,\n"
"	out float3 oEyeDirTan      	: TEXCOORD2,\n"
"	out float4 oPositionViewProj	: TEXCOORD3\n"
"	##ifdef PPLighting\n"
"		,out float3 oLightDir	: TEXCOORD4,\n"
"		out float3 oNormal	: TEXCOORD5\n"
"	##endif\n"
")\n"
"{\n"
"	oPosition = mul(position, worldViewProj); //oPosition must be output to satisy pipeline.\n"
"	oPositionViewProj = oPosition;\n"
"\n"
"	oTexCoord = texCoord;\n"
"	oTexCoord2 = float2((texCoord.x+time*0.2)/2,(texCoord.y+time*0.2)/2); // offset second texture coordinate\n"
"										// according to time for wind texture\n"
"\n"
"	oVertColor = vertColor;\n"
"	\n"
"	float3 posWorld = mul(position, mWorld);\n"
"	float3 eyeDirO = normalize(-(eyePositionO-posWorld)) ; //eye vector in object space\n"
"	\n"
"	##ifdef PPLighting\n"
"	oLightDir = normalize(LightPos-posWorld);\n"
"	oNormal = normal;\n"
"	##endif\n"
"	\n"
"	// Using mesh-defined tangents. (Uncomment section to calculate them ourselves.)\n"
"	tangent = float3(abs(normal.y) + abs(normal.z),abs(normal.x),0);\n"
"	\n"
"	float3 binormal = cross(tangent,normal);\n"
"	float3x3 TBNMatrix = float3x3(tangent,binormal,normal); \n"
"\n"
"	oEyeDirTan = normalize(mul(eyeDirO,TBNMatrix)); // eye vector in tangent space\n"
"}\n"
"\n"
"#define MAX_RAYDEPTH D_RAY_DEPTH //Number of iterations.\n"
"\n"
"#define PLANE_NUM 16.0 //Number of grass slice grid planes per unit in tangent space.\n"
"\n"
"#define PLANE_NUM_INV (1.0/PLANE_NUM)\n"
"#define PLANE_NUM_INV_DIV2 (PLANE_NUM_INV/2)\n"
"\n"
"#define GRASS_SLICE_NUM 8 // Number of grass slices in texture grassblades.\n"
"\n"
"#define GRASS_SLICE_NUM_INV (1.0/GRASS_SLICE_NUM)\n"
"#define GRASS_SLICE_NUM_INV_DIV2 (GRASS_SLICE_NUM_INV/2)\n"
"\n"
"#define GRASSDEPTH GRASS_SLICE_NUM_INV //Depth set to inverse of number of grass slices so no stretching occurs.\n"
"\n"
"#define TC1_TO_TC2_RATIO 8 //Ratio of texture coordinate set 1 to texture coordinate set 2, used for the animation lookup.\n"
"\n"
"#define PREMULT (GRASS_SLICE_NUM_INV*PLANE_NUM) //Saves a multiply in the shader.\n"
"\n"
"#define AVERAGE_COLOR float4(0.32156,0.513725,0.0941176,1.0) //Used to fill remaining opacity, can be replaced by a texture lookup.\n"
"\n"
"sampler2D grassblades : register(s0);\n"
"sampler2D ground : register(s1);\n"
"sampler2D windnoise : register(s2);\n"
"\n"
"void  pixelMain(\n"
"\n"
"	in float4 vertColor      : COLOR,\n"
"	\n"
"	in float2 texCoord       	: TEXCOORD0,\n"
"	in float2 texCoord2      	: TEXCOORD1,\n"
"	in float3 eyeDirTan      	: TEXCOORD2,\n"
"	in float4 positionViewProj  : TEXCOORD3,\n"
"	##ifdef PPLighting\n"
"	in float3 LightDir	: TEXCOORD4,\n"
"	in float3 Normal	: TEXCOORD5,\n"
"	##endif\n"
"	\n"
"	out float4 color	: COLOR\n"
"	,out float depth		: DEPTH\n"
"\n"
"\n"
")\n"
"{	\n"
"	\n"
"	//Initialize increments/decrements and per fragment constants\n"
"	color = float4(0.0,0.0,0.0,0.0);\n"
"\n"
"	float2 plane_offset = float2(0.0,0.0);					\n"
"	float3 rayEntry = float3(texCoord.xy,0.0);\n"
"	float zOffset = 0.0;\n"
"	bool zFlag = 1;\n"
"\n"
"\n"
"	//The signs of eyeDirTan determines if we increment or decrement along the tangent space axis\n"
"	//plane_correct, planemod and pre_dir_correct are used to avoid unneccessary if-conditions. \n"
"\n"
" 	float2 signf = float2(sign(eyeDirTan.x),sign(eyeDirTan.y));	\n"
" 	float2 plane_correct = float2((signf.x+1)*GRASS_SLICE_NUM_INV_DIV2,\n"
" 	                              (signf.y+1)*GRASS_SLICE_NUM_INV_DIV2);\n"
" 	float2 planemod = float2(floor(rayEntry.x*PLANE_NUM)/PLANE_NUM,\n"
" 	                         floor(rayEntry.y*PLANE_NUM)/PLANE_NUM);\n"
"	\n"
"	\n"
"\n"
"	float2 pre_dir_correct = float2((signf.x+1)*PLANE_NUM_INV_DIV2,\n"
"	                                (signf.y+1)*PLANE_NUM_INV_DIV2);\n"
"									\n"
"	int hitcount;\n"
"	\n"
"	float2 orthoLookup; //Will contain texture lookup coordinates for grassblades texture.\n"
	"	for(hitcount =0; hitcount < MAX_RAYDEPTH ; hitcount++) // %([MAX_RAYDEPTH]+1) speeds up compilation.\n"
"									 // It may prove to be faster to early exit this loop\n"
"										 // depending on the hardware used.\n"
"	{\n"
"\n"
"		//Calculate positions of the intersections with the next grid planes on the u,v tangent space axis independently.\n"
"\n"
	"		float2 dir_correct = float2(signf.x*plane_offset.x+pre_dir_correct.x,\n"
	"		                            signf.y*plane_offset.y+pre_dir_correct.y);			\n"
	"	float2 distance = float2((planemod.x + dir_correct.x - rayEntry.x)/(eyeDirTan.x),\n"
	"	                         (planemod.y + dir_correct.y - rayEntry.y)/(eyeDirTan.y));\n"
 	"				\n"
 	"	float3 rayHitpointX = rayEntry + eyeDirTan *distance.x;   \n"
  	"	float3 rayHitpointY = rayEntry + eyeDirTan *distance.y;\n"
	"	\n"
	"	//Check if we hit the ground. If so, calculate the intersection and look up the ground texture and blend colors.\n"
	"\n"
  	"	if ((rayHitpointX.z <= -GRASSDEPTH)&& (rayHitpointY.z <= -GRASSDEPTH)) 	\n"
  	"	{\n"
  	"		float distanceZ = (-GRASSDEPTH)/eyeDirTan.z; // rayEntry.z is 0.0 anyway \n"
	"\n"
  	"		float3 rayHitpointZ = rayEntry + eyeDirTan *distanceZ;\n"
	"		float2 orthoLookupZ = float2(rayHitpointZ.x,rayHitpointZ.y);\n"
	"					\n"
  	"		color = (color)+((1.0-color.w) * tex2D(ground,orthoLookupZ));\n"
  	"		if(zFlag ==1) \n"
	"		zOffset = distanceZ; // write the distance from rayEntry to intersection\n"
  	"		zFlag = 0; //Early exit here if faster.		\n"
  	"	}  \n"
  	"	else\n"
 	"	{\n"
 	"		\n"
 	"		\n"
	"\n"
 	"		//check if we hit a u or v plane, calculate lookup accordingly with wind shear displacement.\n"
	"		if(distance.x <= distance.y)\n"
 	"		{\n"
 	"			float4 windX = (tex2D(windnoise,texCoord2+rayHitpointX.xy/TC1_TO_TC2_RATIO)-0.5)/2;\n"
	"			\n"
	"			float lookupX = -(rayHitpointX.z+(planemod.x+signf.x*plane_offset.x)*PREMULT)-plane_correct.x;\n"
	"			orthoLookup=float2(rayHitpointX.y+windX.x*(GRASSDEPTH+rayHitpointX.z),lookupX); \n"
	"			\n"
	"			plane_offset.x += PLANE_NUM_INV; // increment/decrement to next grid plane on u axis\n"
	"			if(zFlag==1) zOffset = distance.x;\n"
	"		}\n"
	"		else {\n"
	"			float4 windY = (tex2D(windnoise,texCoord2+rayHitpointY.xy/TC1_TO_TC2_RATIO)-0.5)/2;\n"
 	"		\n"
	"			float lookupY = -(rayHitpointY.z+(planemod.y+signf.y*plane_offset.y)*PREMULT)-plane_correct.y;\n"
	"			orthoLookup = float2(rayHitpointY.x+windY.y*(GRASSDEPTH+rayHitpointY.z) ,lookupY);\n"
 	"		\n"
	"			plane_offset.y += PLANE_NUM_INV;  // increment/decrement to next grid plane on v axis\n"
	"			if(zFlag==1) zOffset = distance.y;\n"
	"				\n"
  	"		}\n"
  	"		 \n"
 		"		color += (1.0-color.w)*tex2D(grassblades,orthoLookup);\n"
 	"\n"
 		"		if(color.w >= 0.49)\n"
		"	{zFlag = 0;}	//Early exit here if faster.\n"
  		"}\n"
	"}	\n"
	"\n"
		"color += (1.0-color.w)*tex2D(ground,orthoLookup); 	//Fill remaining transparency in case there is some left. Can be replaced by a texture lookup\n"
		"									//into a fully opaque grass slice using orthoLookup.\n"
		"\n"
		"\n"
		"//color.xyz *= (vertColor.xyz);	 //Modulate with per vertex lightmap,as an alternative, modulate with N*L for dynamic lighting.\n"
	"\n"
"	##ifdef PPLighting\n"
"		color.xyz *= dot(LightDir,Normal);\n"
"	##endif\n"
"	 \n"
	"    //zOffset is along eye direction, transform and add to vertex position to get correct z-value.\n"
	"   // BlindSide NOTE: Commented this section out as it causes a bug.\n"
	" positionViewProj += mul(eyeDirTan.xzy*zOffset,worldViewProj); \n"
	"\n"
	"   //Divide by homogenous part.\n"
	"  depth = positionViewProj.z / positionViewProj.w;\n"
	"\n"
	" //color.xyz = depth;\n"
"}\n"
"\n"
};

CGrassPainter::CGrassPainter(CDevices *_devices, ITerrainSceneNode * _node) {
	devices = _devices;
	node = _node;

	currentQuality = 3;
	currentScale = 10;
	currentDistance = 10;

	buffer = new SMeshBuffer();
	S3DVertex vertices[4];
	vertices[0] = S3DVertex(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 1.0f, 1.0f);
	vertices[1] = S3DVertex(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 1.0f, 0.0f);
	vertices[2] = S3DVertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 0.0f, 0.0f);
	vertices[3] = S3DVertex(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, SColor(0, 255, 255, 255), 0.0f, 1.0f);
	u16 indices[6] = { 0, 1, 2, 0, 2, 3 };
	buffer->append(vertices, 4, indices, 6);

	mesh = new SMesh();
	mesh->addMeshBuffer(buffer);

	devices->getDevice()->getLogger()->setLogLevel(ELL_INFORMATION);
	grassShaderCallback = new CGrassShaderCallback(devices->getDevice(), vector3df(0, 500, 0));
	CShaderPreprocessor shaderPreprocessor(devices->getVideoDriver());
	shaderPreprocessor.addShaderDefine("PPLighting");
	shaderPreprocessor.addShaderDefine("D_RAY_DEPTH", "1");
	material = devices->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterial(
												shaderPreprocessor.ppShader(pixelGrass).c_str(), "vertexMain", EVST_VS_3_0,
												shaderPreprocessor.ppShader(pixelGrass).c_str(), "pixelMain", EPST_PS_3_0,
												grassShaderCallback, video::EMT_SOLID);
	devices->getDevice()->getLogger()->setLogLevel(ELL_NONE);

	plPreview = devices->getSceneManager()->addHillPlaneMesh(stringc(stringc("plPreview") + stringc(currentQuality)).c_str(), 
															 dimension2df(currentQuality, currentQuality),
															 dimension2du(currentQuality, currentQuality));
	plPreviewNode = devices->getSceneManager()->addMeshSceneNode(plPreview);
	plPreviewNode->setMaterialFlag(EMF_LIGHTING, false);
}

CGrassPainter::~CGrassPainter() {
	buffer->drop();
	mesh->drop();

	plPreview->drop();
	plPreviewNode->remove();
}

void CGrassPainter::setCurrentQuality(s32 _currentQuality) {
	currentQuality = _currentQuality;
	plPreviewNode->remove();
	plPreview = devices->getSceneManager()->addHillPlaneMesh(stringc(stringc("plPreview") + stringc(currentQuality)).c_str(), 
															 dimension2df(currentQuality, currentQuality),
															 dimension2du(currentQuality, currentQuality));
	plPreviewNode = devices->getSceneManager()->addMeshSceneNode(plPreview);
}

void CGrassPainter::setCurrentScale(s32 _currentScale) {
	currentScale = _currentScale;
	plPreviewNode->setScale(vector3df(currentScale/100));
}

void CGrassPainter::drawPreview(vector3df pos) {
	plPreviewNode->setPosition(pos);
}

void CGrassPainter::paint(vector3df pos, s32 scale, s32 quality, s32 distance, bool remove) {
	bool canPaint = true;
	f32 scalef = scale;
	f32 currentDistancef = currentDistance;
	f32 qualityf = quality;
	f32 distancef = distance;
	f32 space =	distancef/10.f;

	core::list<SGrassData *>::Iterator grassData = grassList.begin();
	for (; grassData != grassList.end(); ++grassData) {
		ISceneNode *gdnode = (*grassData)->node;

		if (gdnode->getPosition().getDistanceFrom(pos) < space) {
			canPaint = false;
			if (remove) {
				devices->getXEffect()->removeShadowFromNode((*grassData)->node);
				devices->getXEffect()->removeNodeFromDepthPass((*grassData)->node);
				(*grassData)->node->remove();
				delete *grassData;
				grassList.erase(grassData);
				break;
			}
		}
	}

	if (!remove && textures.size() == 0) {
		if (canPaint) {
			SGrassData *gdata = new SGrassData(0, pos, scale, quality);

			IAnimatedMesh *pl = devices->getSceneManager()->addHillPlaneMesh(stringc(stringc("pl") + stringc(quality)).c_str(), 
																			 dimension2df(quality, quality), dimension2du(quality, quality));
			IMeshSceneNode *gnode = devices->getSceneManager()->addMeshSceneNode(pl);

			if (gnode) {
				gnode->setMaterialFlag(EMF_LIGHTING, false);
				gnode->setMaterialType((E_MATERIAL_TYPE)material);
				gnode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

				//ITexture * texture = textures[textureIndex];
				//gnode->setMaterialTexture(0, texture);
				gnode->setMaterialTexture(0, devices->getVideoDriver()->getTexture("m_grassblades.png"));
				gnode->setMaterialTexture(1, devices->getVideoDriver()->getTexture("m_grass_ground.png"));
				gnode->setMaterialTexture(2, devices->getVideoDriver()->getTexture("windnoise.png"));

				gnode->setScale(vector3df(scalef/100.f));
				gnode->setPosition(vector3df(pos.X, pos.Y, pos.Z));
				gnode->setRotation(vector3df(180, 0, 0));

				for (u32 j=0; j < gnode->getMesh()->getMeshBufferCount(); j++)
					gnode->getMesh()->getMeshBuffer(j)->recalculateBoundingBox();

				devices->getXEffect()->addShadowToNode(gnode, devices->getXEffectFilterType(), ESM_EXCLUDE);
				gdata->node = gnode;
				grassList.push_back(gdata);
			}
		}
	}
}
