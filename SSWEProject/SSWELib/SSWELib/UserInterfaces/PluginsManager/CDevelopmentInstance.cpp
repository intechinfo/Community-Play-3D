//
//  CDevelopmentInstance.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/10/2014.
//
//

#include "stdafx.h"
#include "CDevelopmentInstance.h"
#include <irrbullet.h>

CDevelopmentPluginManager::CDevelopmentPluginManager(CDevices *devices)
: cp3d::core::IDevelomentInstance(devices)
{
    this->devices = devices;
	playingForDevelopment = false;
}

CDevelopmentPluginManager::~CDevelopmentPluginManager() {
    
}

void CDevelopmentPluginManager::run(void) {
    /// Create scene manager
    smgrForDevelopment = devices->getSceneManager()->createNewSceneManager(false);
    
    /// Clone nodes
    ISceneNode *rootNode = devices->getSceneManager()->getRootSceneNode();
    core::list<ISceneNode *>::ConstIterator nodes = rootNode->getChildren().begin();
    for (; nodes != rootNode->getChildren().end(); ++nodes) {
        smgrForDevelopment->getRootSceneNode()->addChild((*nodes)->clone());
    }
    
    smgrForDevelopment->addCubeSceneNode();
    
    /// Clone the effect handler
    effectFormDevelopment = devices->getXEffect()->clone(smgrForDevelopment);
    effectFormDevelopment->setActiveSceneManager(smgrForDevelopment);
    
    /// Set active camera
    SKeyMap keyMap[9];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_KEY_Z;
    keyMap[1].Action = EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = KEY_KEY_S;
    keyMap[2].Action = EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = KEY_KEY_Q;
    keyMap[3].Action = EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = KEY_KEY_D;
    keyMap[4].Action = EKA_JUMP_UP;
    keyMap[4].KeyCode = KEY_SPACE;
    keyMap[5].Action = EKA_MOVE_FORWARD;
    keyMap[5].KeyCode = KEY_UP;
    keyMap[6].Action = EKA_MOVE_BACKWARD;
    keyMap[6].KeyCode = KEY_DOWN;
    keyMap[7].Action = EKA_STRAFE_LEFT;
    keyMap[7].KeyCode = KEY_LEFT;
    keyMap[8].Action = EKA_STRAFE_RIGHT;
    keyMap[8].KeyCode = KEY_RIGHT;
    
    ICameraSceneNode *camera_fps = smgrForDevelopment->addCameraSceneNodeFPS(0, 200.0f, 0.09f, -1, keyMap, 5, true, 0.3f, false, true);
    camera_fps->setTarget(vector3df(0.f, 5.f, 0.f));
    camera_fps->setFarValue(42000.0f);
    camera_fps->setName("developmentPlugin:FPScamera");
    camera_fps->setID(-1);
    camera_fps->setPosition(devices->getMayaCamera()->getPosition());
    smgrForDevelopment->setActiveCamera(camera_fps);
    
    //devices->getCollisionManager()->getFPSCameraSettings()->setSettings(smgrForDevelopment, devices->getCollisionManager()->getMetaTriangleSelectors(), camera_fps);
    
    /// Create bullet world
    bworldForDevelopment = createIrrBulletWorld(devices->getDevice(), true, false);
    bworldForDevelopment->setGravity(devices->getBulletWorld()->getGravity());
    
    /// Configure devices
    devices->setRenderEffectForDevelopment(effectFormDevelopment);
    
    playingForDevelopment = true;
}

void CDevelopmentPluginManager::stop(void) {
    delete effectFormDevelopment;
    smgrForDevelopment->drop();
    
    devices->getSceneManager()->setActiveCamera(devices->getMayaCamera());
    
    devices->setDontRenderEffectForDevelopment();
    
    playingForDevelopment = false;
}

void CDevelopmentPluginManager::update() {
    //smgrForDevelopment->drawAll();
    effectFormDevelopment->setActiveSceneManager(smgrForDevelopment);
}
