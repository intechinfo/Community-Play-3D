#include "CWaterSurface.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;

CWaterSurface::CWaterSurface(irr::scene::ISceneManager* smgr, irr::video::ITexture *target, irr::scene::IMesh *mesh,
							 bool SinWave, bool Refraction, irr::core::stringw packagePath,
							 irr::core::dimension2du RTTSize, irr::scene::ISceneNode* parent, irr::s32 id, irr::video::E_MATERIAL_TYPE mat)
							 : ISceneNode(parent, smgr, id), refRTTSize(RTTSize)
{

    SceneManager = smgr;

    if (!Parent)
        setParent(SceneManager->getRootSceneNode());
    
    driver = SceneManager->getVideoDriver();
    
	refRTT = driver->addRenderTargetTexture(refRTTSize, "refRTT", ECF_A8R8G8B8);
    refCam = SceneManager->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0), -1, false);
	originRTT = target;
    
	if (!mesh) {
		waterMesh = SceneManager->addHillPlaneMesh("waterMesh", dimension2df(25, 25), dimension2du(25, 25));
		SceneManager->getMeshManipulator()->makePlanarTextureMapping(waterMesh, 0.05f);
	} else {
		waterMesh = mesh;
		waterNode = SceneManager->addMeshSceneNode(waterMesh);
	}
    
    waterNode = SceneManager->addMeshSceneNode(waterMesh, this);
    waterNode->getMaterial(0).MaterialType = mat;
    waterNode->getMaterial(0).setTexture(0, refRTT);
    waterNode->getMaterial(0).setTexture(1, driver->getTexture("shaders/Textures/Water/waterNormal.png"));
    waterNode->getMaterial(0).setTexture(2, driver->getTexture("shaders/Textures/Water/waterDUDV.png"));
    
    setPosition(vector3df(0, 0, 0));
    setSinWaveEnabled(SinWave);
    setRefractionEnabled(Refraction);

	drawScene = true;

    this->drop();
}

CWaterSurface::~CWaterSurface() {
    if (refCam)    { refCam->remove();    }
    if (waterNode) { waterNode->remove(); }
}

void CWaterSurface::OnRegisterSceneNode() {
    updateAbsolutePosition();
    waterNode->updateAbsolutePosition();

    // Used as seaLevel in shader callback
    waterNode->getMaterial(0).MaterialTypeParam2 = waterNode->getAbsolutePosition().Y;

    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void CWaterSurface::setOriginRTT(irr::video::ITexture *tex) {
	originRTT = tex;
}

void CWaterSurface::OnAnimate(irr::u32 timeMs) {
    if (IsVisible) {
        setVisible(false);
        
        ICameraSceneNode* tempCam = SceneManager->getActiveCamera();
        // Needed for manual drawing
        // tempCam->OnRegisterSceneNode();
        // tempCam->OnAnimate(timeMs);
        // tempCam->render();
        
        SceneManager->setActiveCamera(refCam);
        refCam->setNearValue(tempCam->getNearValue());
        refCam->setFarValue (tempCam->getFarValue());
        refCam->setFOV      (tempCam->getFOV());
        
        vector3df tempPosition = tempCam->getAbsolutePosition();
        vector3df tempTarget   = tempCam->getTarget();
        
        f32 seaLevel = 0;
        if (waterNode)
            seaLevel = waterNode->getPosition().Y;
        
        if(tempPosition.Y >= seaLevel) {
            refCam->setPosition(vector3df(tempPosition.X, 2.0f * seaLevel - tempPosition.Y, tempPosition.Z));
            
            tempTarget -= tempPosition;
            tempTarget.normalize();
            tempTarget.Y *= -1.0f;
            tempTarget   *= 1000000.0f;

            refCam->setTarget(tempPosition + tempTarget);
            
            waterNode->setMaterialFlag(EMF_BACK_FACE_CULLING, true);
        } else {
            refCam->setPosition(tempPosition);
            
            tempTarget -= tempPosition;
            tempTarget.normalize();
            tempTarget *= 1000000.0f;

            refCam->setTarget(tempPosition + tempTarget);
            
            waterNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
        }
        
		if (drawScene) {
			driver->setRenderTarget(refRTT, true, true, SColor(0,0,0,0));
			driver->setClipPlane(0, plane3df(0.0f, seaLevel - CLIP_PLANE_OFFSET, 0.0f, 0.0f, 1.0f, 0.0f), true);
			SceneManager->drawAll();
		}

        driver->setRenderTarget(originRTT, true, true, SColor(0,0,0,0));
        driver->enableClipPlane(0, false);
        
        SceneManager->setActiveCamera(tempCam);
        
        setVisible(true);
    }
    
    ISceneNode::OnAnimate(timeMs);
}

void CWaterSurface::render() {
    
}

const irr::core::aabbox3df& CWaterSurface::getBoundingBox() const {
    return waterNode->getBoundingBox();
}

const bool CWaterSurface::isSinWaveEnabled() {
    // Hijacked: Used as bool for sinus waves in shader callback
    if (waterNode->getMaterial(0).Shininess > 0.0f)
        return true;
    else
        return false;
}

const bool CWaterSurface::isRefractionEnabled() {
    // Hijacked: Used as bool for refraction in shader callback
    if (waterNode->getMaterial(0).Thickness > 0.0f)
        return true;
    else
        return false;
}

void CWaterSurface::setSinWaveEnabled(bool Enabled) {
    if (Enabled)
        waterNode->getMaterial(0).Shininess = 1.0f;
    else
        waterNode->getMaterial(0).Shininess = -1.0f;
}

void CWaterSurface::setRefractionEnabled(bool Enabled) {
    if (Enabled)
        waterNode->getMaterial(0).Thickness = 1.0f;
    else
        waterNode->getMaterial(0).Thickness = -1.0f;
}
