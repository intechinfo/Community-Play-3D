//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "stdafx.h"
#include "OculusRift.h"

const char *fragShader =
"struct VS_OUTPUT \n"
"{"
"	float4 Position		: POSITION0;"
"	float2 TexCoords	: TEXCOORD0;"
"	float3 LStart		: TEXCOORD1;"
"	float3 LEnd			: TEXCOORD2;"
"	float2 ScreenSize	: TEXCOORD3;"
"}; \n"
"float2 scale;\n"
"float2 scaleIn;\n"
"float2 lensCenter;\n"
"float4 hmdWarpParam;\n"
"sampler2D texid : register(s0);\n"
"\n"
"float4 pixelMain(VS_OUTPUT input) : COLOR0\n"
"{\n"
"  float2 uv = (input.TexCoords*2.0)-1.0;\n" // range from [0,1] to [-1,1]
"  float2 theta = (uv-lensCenter)*scaleIn;\n"
"  float rSq = theta.x*theta.x + theta.y*theta.y;\n"
"  float2 rvector = theta*(hmdWarpParam.x + hmdWarpParam.y*rSq + hmdWarpParam.z*rSq*rSq + hmdWarpParam.w*rSq*rSq*rSq);\n"
"  float2 tc = (lensCenter + scale * rvector);\n"
"  tc = (tc+1.0)/2.0;\n" // range from [-1,1] to [0,1]
"  if (any(bool2(clamp(tc, float2(0.0,0.0), float2(1.0,1.0))-tc)))\n"
"    return float4(0.0, 0.0, 0.0, 1.0);\n"
"  else\n"
"    return tex2D(texid, tc);\n"
"}\n";

#include <iostream>

void *createMonitor() {
	return static_cast< void* > (new COculusRoftMonitor);
}

COculusRoftMonitor::COculusRoftMonitor() {

}

COculusRoftMonitor::~COculusRoftMonitor() {

}

void COculusRoftMonitor::init() {
	monitorName = "Generic Monitor";

	_renderTexture = 0;
	_timer = renderer->getIrrlichtDevice()->getTimer();
	_pCamera = smgr->addCameraSceneNode();

	// Parameters from the Oculus Rift DK1
	_HMD.hResolution = 1280;
	_HMD.vResolution = 800;
	_HMD.hScreenSize = 0.14976;
	_HMD.vScreenSize = 0.0936;
	_HMD.interpupillaryDistance = 0.064;
	_HMD.lensSeparationDistance = 0.064;
	_HMD.eyeToScreenDistance = 0.041;
	_HMD.distortionK[0] = 1.0;
	_HMD.distortionK[1] = 0.22;
	_HMD.distortionK[2] = 0.24;
	_HMD.distortionK[3] = 0.0;

	_worldScale = 10.f;
  
	// Plane
	_planeVertices[0] = irr::video::S3DVertex(-1.0f, -1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 0.0f);
	_planeVertices[1] = irr::video::S3DVertex(-1.0f,  1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 0.0f, 1.0f);
	_planeVertices[2] = irr::video::S3DVertex( 1.0f,  1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 1.0f);
	_planeVertices[3] = irr::video::S3DVertex( 1.0f, -1.0f, 1.0f,1,1,0, irr::video::SColor(255,0,255,255), 1.0f, 0.0f);
	_planeIndices[0] = 0; _planeIndices[1] = 1; _planeIndices[2] = 2; _planeIndices[3] = 0; _planeIndices[4] = 2; _planeIndices[5] = 3;

	// Create shader material
	_renderMaterial.Wireframe = false;
	_renderMaterial.Lighting = false;
	_renderMaterial.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP;
	_renderMaterial.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP;

	renderer->getIrrlichtDevice()->getLogger()->setLogLevel(irr::ELL_INFORMATION);
	_renderMaterial.MaterialType = (irr::video::E_MATERIAL_TYPE)renderer->addPostProcessingEffectFromString(fragShader, 0, false);
	renderer->getIrrlichtDevice()->getLogger()->setLogLevel(irr::ELL_NONE);
	_distortionCB.scale[0] = 1.0f; _distortionCB.scale[1] = 1.0f;
	_distortionCB.scaleIn[0] = 1.0f; _distortionCB.scaleIn[1] = 1.0f;
	_distortionCB.lensCenter[0] = 0.0f;_distortionCB.lensCenter[1] = 0.0f;
	_distortionCB.hmdWarpParam[0] = 1.0f;_distortionCB.hmdWarpParam[1] = 0.0f;_distortionCB.hmdWarpParam[2] = 0.0f;_distortionCB.hmdWarpParam[3] = 0.0f;
	renderer->setPostProcessingRenderCallback(_renderMaterial.MaterialType, &_distortionCB);

	// Compute aspect ratio and FOV
	float aspect = _HMD.hResolution / (2.0f*_HMD.vResolution);

	// Fov is normally computed with:
	//   2*atan2(HMD.vScreenSize,2*HMD.eyeToScreenDistance)
	// But with lens distortion it is increased (see Oculus SDK Documentation)
	float r = -1.0f - (4.0f * (_HMD.hScreenSize/4.0f - _HMD.lensSeparationDistance/2.0f) / _HMD.hScreenSize);
	float distScale = (_HMD.distortionK[0] + _HMD.distortionK[1] * pow(r,2) + _HMD.distortionK[2] * pow(r,4) + _HMD.distortionK[3] * pow(r,6));
	float fov = 2.0f*atan2(_HMD.vScreenSize*distScale, 2.0f*_HMD.eyeToScreenDistance);

	// Compute camera projection matrices
	irr::core::matrix4 centerProjection = irr::core::matrix4().buildProjectionMatrixPerspectiveFovLH (fov, aspect, 1, 10000);
	float h = 4 * (_HMD.hScreenSize/4 - _HMD.interpupillaryDistance/2) / _HMD.hScreenSize;
	_projectionLeft = irr::core::matrix4().setTranslation(irr::core::vector3df(h, 0.0, 0.0)) * centerProjection;
	_projectionRight = irr::core::matrix4().setTranslation(irr::core::vector3df(-h, 0.0, 0.0)) * centerProjection;

	// Compute camera offset
	_eyeSeparation = _worldScale * _HMD.interpupillaryDistance/2.0f;

	// Compute Viewport
	_viewportLeft = irr::core::rect<irr::s32>(0, 0, _HMD.hResolution/2, _HMD.vResolution);
	_viewportRight = irr::core::rect<irr::s32>(_HMD.hResolution/2, 0, _HMD.hResolution, _HMD.vResolution);

	// Distortion shader parameters
	_lensShift = 4.0f * (_HMD.hScreenSize/4.0f - _HMD.lensSeparationDistance/2.0f) / _HMD.hScreenSize;

	_distortionCB.scale[0] = 1.0f/distScale;
	_distortionCB.scale[1] = 1.0f*aspect/distScale;
  
	_distortionCB.scaleIn[0] = 1.0f;
	_distortionCB.scaleIn[1] = 1.0f/aspect;
  
	_distortionCB.hmdWarpParam[0] = _HMD.distortionK[0];
	_distortionCB.hmdWarpParam[1] = _HMD.distortionK[1];
	_distortionCB.hmdWarpParam[2] = _HMD.distortionK[2];
	_distortionCB.hmdWarpParam[3] = _HMD.distortionK[3];

	// Create render target
	if (driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
	{
		if (_renderTexture != NULL) _renderTexture->drop();
		_renderTexture = driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(_HMD.hResolution*distScale/2.0f, _HMD.vResolution*distScale));
		_renderMaterial.setTexture(0, _renderTexture);
	}
	else {
	// Render to target not supported
		assert(0);
	}
}

void COculusRoftMonitor::setActiveCamera(irr::scene::ICameraSceneNode *camera) {
	monitorCamera = camera;
}

void COculusRoftMonitor::setSceneManager(irr::scene::ISceneManager *sceneManager) {
	smgr = sceneManager;
	driver = smgr->getVideoDriver();
}

void COculusRoftMonitor::drawScene() {
    
	/*if (renderRenderer) {
		renderer->setActiveSceneManager(smgr);
		renderer->update(renderFull);
    } else {
        smgr->drawAll();
    }*/
	using namespace irr;
	using namespace video;
	using namespace core;
	using namespace scene;

	ICameraSceneNode* camera = smgr->getActiveCamera();
	camera->OnAnimate(_timer->getTime());

	// Render Left
	//driver->setRenderTarget(_renderTexture, true, true, video::SColor(0,0,0,0));
  
	_pCamera->setProjectionMatrix(_projectionLeft);

	vector3df r = camera->getRotation();
	vector3df tx(-_eyeSeparation, 0.0,0.0);
	tx.rotateXZBy(-r.Y);
	tx.rotateYZBy(-r.X);
	tx.rotateXYBy(-r.Z);

	_pCamera->setPosition(camera->getPosition() + tx);
	_pCamera->setTarget(camera->getTarget() + tx);  

	smgr->setActiveCamera(_pCamera);
	//smgr->drawAll();

	//driver->setRenderTarget(0, false, false, video::SColor(0,100,100,100));
	driver->setViewPort(_viewportLeft);

	_distortionCB.lensCenter[0] = _lensShift;

	renderer->update(false, _renderTexture);
	driver->setMaterial(_renderMaterial);
	driver->drawIndexedTriangleList(_planeVertices, 4, _planeIndices, 2);
 
	// Render Right
	//driver->setRenderTarget(_renderTexture, true, true, video::SColor(0,0,0,0));
	_pCamera->setProjectionMatrix(_projectionRight);

	vector3df r2 = camera->getRotation();
	vector3df tx2(-_eyeSeparation, 0.0,0.0);
	tx.rotateXZBy(-r2.Y);
	tx.rotateYZBy(-r2.X);
	tx.rotateXYBy(-r2.Z);

	//_pCamera->setPosition(camera->getPosition() + tx2);
	_pCamera->setTarget(camera->getTarget() + tx2);  

	//smgr->drawAll();

	//driver->setRenderTarget(0, false, false, video::SColor(0,100,100,100));  
	driver->setViewPort(_viewportRight);

	_distortionCB.lensCenter[0] = -_lensShift;

	renderer->update(false);
	driver->setMaterial(_renderMaterial); 
	driver->drawIndexedTriangleList(_planeVertices, 4, _planeIndices, 2);

	smgr->setActiveCamera(camera);

	//driver->setViewPort(irr::core::rect<irr::s32>(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height));

}

void COculusRoftMonitor::drawGUI() {
	//driver->setRenderTarget(0);
	gui->drawAll();
}
