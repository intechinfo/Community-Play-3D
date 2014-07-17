//
//  CUIWindowEditNode.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 29/09/12.
//
//
#include "stdafx.h"
#include "CUIWindowEditNode.h"

#include "../../../SSWERenders/Renders/Materials/CNormalMappingMaterial.h"

#include <irrbullet.h>

CUIWindowEditNode::CUIWindowEditNode(CDevices *_devices) {
    devices = _devices;
    
    nodeToEdit = 0;
    nodeToEditPrefix = L"";
    currentBrowse = 0;
    
    editWindow = 0;
    
    isWindowed = true;
}

CUIWindowEditNode::~CUIWindowEditNode() {

}

void CUIWindowEditNode::open(ISceneNode *node, stringw prefix, bool modal) {
    if (!node)
		return;

    nodeToEdit = node;
    
    if (nodeToEdit == 0) {
        devices->addErrorDialog(L"Error", L"No node to edit, maybe an intern error or your node is out \n"
                                L"Please try again", EMBF_OK);
        devices->getEventReceiver()->RemoveEventReceiver(this);
        delete this;
    } else {
        editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-200, 
                                                                   100, devices->getVideoDriver()->getScreenSize().Width/2+220, 700),
															 modal, stringw(stringw("Node Edition Window : ") + stringw(node->getName())).c_str(), 0, -1);
		editWindow->getMinimizeButton()->setVisible(!modal);
        open(node, prefix);
    }
}

void CUIWindowEditNode::open(ISceneNode *node, stringw prefix) {
    if (!node)
		return;
    
    nodeToEdit = node;
    
    if (nodeToEdit == 0) {
        devices->addErrorDialog(L"Error", L"No node to edit, maybe an intern error or your node is out \n"
                                L"Please try again", EMBF_OK);
        devices->getEventReceiver()->RemoveEventReceiver(this);
        delete this;
    } else {

        nodeToEditPrefix.append(prefix);
        nodeToEdit->setDebugDataVisible(EDS_BBOX_ALL);
        
        //WINDOW
        if (!editWindow) {
            editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-200, 
                                                                           100, devices->getVideoDriver()->getScreenSize().Width/2+220, 700),
                                                                 false, stringw(stringw("Node Edition Window : ") + stringw(node->getName())).c_str(), 0, -1);
			editWindow->getMinimizeButton()->setVisible(true);
        }
        editWindow->getMaximizeButton()->setVisible(true);
        
        //WINDOW BUTTONS
        applyButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 560, 80, 590), editWindow, CXT_EDIT_WINDOW_EVENTS_APPLY_BUTTON, 
                                                              L"Apply", L"Apply the settings");
        closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(100, 560, 175, 590), editWindow, CXT_EDIT_WINDOW_EVENTS_CLOSE_BUTTON, 
                                                              L"Close", L"Close without effect");
        //editWindow->getCloseButton()->remove();
        
        tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(2, 20, 418, 550), editWindow, true, true, -1);
        generalTab = tabCtrl->addTab(L"General");
        flagsTab = tabCtrl->addTab(L"Flags & Materials");
        generalFlagsTab = tabCtrl->addTab(L"Global Flags");
        advancedTab = tabCtrl->addTab(L"Height Map");
		physicsTab = tabCtrl->addTab(L"Physics");
        animatedTab = tabCtrl->addTab(L"Animated");
        
        tabCtrl->setActiveTab(generalTab);
        
        //NAME
        stringw nodeName = nodeToEdit->getName();
        devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(10, 5, 60, 25), true, true, generalTab, -1, true);
        ebNodeName = devices->getGUIEnvironment()->addEditBox(nodeName.c_str(), rect<s32>(70, 5, 370, 25), true, generalTab, -1);
        
        //POSITION
       cursorPositionbtn =  devices->getGUIEnvironment()->addButton(rect<s32>(75, 30, 175, 50), generalTab, -1, 
                                                L"Cursor Position", L"Place the selected node to the cursor position");
        devices->getGUIEnvironment()->addStaticText(L"Position : ", rect<s32>(10, 30, 70, 50), true, true, generalTab, -1, true);
        devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(10, 50, 30, 70), true, true, generalTab, -1, true);
        ebNodePositionX = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getPosition().X).c_str(), 
                                                                   rect<s32>(30, 50, 130, 70), true, generalTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(130, 50, 150, 70), true, true, generalTab, -1, true);
        ebNodePositionY = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getPosition().Y).c_str(), 
                                                                   rect<s32>(150, 50, 250, 70), true, generalTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(250, 50, 270, 70), true, true, generalTab, -1, true);
        ebNodePositionZ = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getPosition().Z).c_str(), 
                                                                   rect<s32>(270, 50, 370, 70), true, generalTab, -1);
        
        //ROTATION
        devices->getGUIEnvironment()->addStaticText(L"Rotation : ", rect<s32>(10, 90, 70, 110), true, true, generalTab, -1, true);
        devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(10, 110, 30, 130), true, true, generalTab, -1, true);
        ebNodeRotationX = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRotation().X).c_str(), 
                                                                   rect<s32>(30, 110, 130, 130), true, generalTab, -10);
        devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(130, 110, 150, 130), true, true, generalTab, -1, true);
        ebNodeRotationY = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRotation().Y).c_str(), 
                                                                   rect<s32>(150, 110, 250, 130), true, generalTab, -10);
        devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(250, 110, 270, 130), true, true, generalTab, -1, true);
        ebNodeRotationZ = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRotation().Z).c_str(), 
                                                                   rect<s32>(270, 110, 370, 130), true, generalTab, -10);
        
        //SCALE
		if (nodeToEdit->getType() != ESNT_BILLBOARD) {
			devices->getGUIEnvironment()->addStaticText(L"Scale : ", rect<s32>(10, 150, 70, 170), true, true, generalTab, -1, true);
			devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(10, 170, 30, 190), true, true, generalTab, -1, true);
			ebNodeScaleX = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getScale().X).c_str(), 
																	rect<s32>(30, 170, 130, 190), true, generalTab, -1);
			devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(130, 170, 150, 190), true, true, generalTab, -1, true);
			ebNodeScaleY = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getScale().Y).c_str(), 
																	rect<s32>(150, 170, 250, 190), true, generalTab, -1);
			devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(250, 170, 270, 130), true, true, generalTab, -1, true);
			ebNodeScaleZ = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getScale().Z).c_str(), 
																	rect<s32>(270, 170, 370, 190), true, generalTab, -1);
		} else {
			IBillboardSceneNode *billNode = (IBillboardSceneNode *)nodeToEdit;
			devices->getGUIEnvironment()->addStaticText(L"Size : ", rect<s32>(10, 150, 70, 170), true, true, generalTab, -1, true);
			devices->getGUIEnvironment()->addStaticText(L"W : ", rect<s32>(10, 170, 30, 190), true, true, generalTab, -1, true);
			ebNodeScaleX = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(billNode->getSize().Width).c_str(), 
																	rect<s32>(30, 170, 130, 190), true, generalTab, -1);
			devices->getGUIEnvironment()->addStaticText(L"H : ", rect<s32>(130, 170, 150, 190), true, true, generalTab, -1, true);
			ebNodeScaleY = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(billNode->getSize().Height).c_str(), 
																	rect<s32>(150, 170, 250, 190), true, generalTab, -1);
		}
        
        //TEXTURE LAYERS
        devices->getGUIEnvironment()->addStaticText(L"Index 1 : ", rect<s32>(10, 200, 70, 220), false, true, generalTab, -1, false);
        devices->getGUIEnvironment()->addStaticText(L"Path : ", rect<s32>(10, 220, 50, 240), true, true, generalTab, -1, true);
        ebTextureLayerPath1 = devices->getGUIEnvironment()->addEditBox(L"Empty", rect<s32>(50, 220, 300, 240), true, generalTab, -1);
        browseButton1 = devices->getGUIEnvironment()->addButton(rect<s32>(305, 220, 360, 240), generalTab, 
                                                                CXT_EDIT_WINDOW_EVENTS_TEXLAYER_1, L"Browse...", L"Browse for file");
        
        devices->getGUIEnvironment()->addStaticText(L"Index 2 : ", rect<s32>(10, 240, 70, 260), false, true, generalTab, -1, false);
        devices->getGUIEnvironment()->addStaticText(L"Path : ", rect<s32>(10, 260, 50, 280), true, true, generalTab, -1, true);
        ebTextureLayerPath2 = devices->getGUIEnvironment()->addEditBox(L"Empty", rect<s32>(50, 260, 300, 280), true, generalTab, -10);
        browseButton2 = devices->getGUIEnvironment()->addButton(rect<s32>(305, 260, 360, 280), generalTab, 
                                                                CXT_EDIT_WINDOW_EVENTS_TEXLAYER_2, L"Browse...", L"Browse for file");
        
        devices->getGUIEnvironment()->addStaticText(L"Index 3 : ", rect<s32>(10, 280, 70, 300), false, true, generalTab, -1, false);
        devices->getGUIEnvironment()->addStaticText(L"Path : ", rect<s32>(10, 300, 50, 320), true, true, generalTab, -1, true);
        ebTextureLayerPath3 = devices->getGUIEnvironment()->addEditBox(L"Empty", rect<s32>(50, 300, 300, 320), true, generalTab, -10);
        browseButton3 = devices->getGUIEnvironment()->addButton(rect<s32>(305, 300, 360, 320), generalTab, 
                                                                CXT_EDIT_WINDOW_EVENTS_TEXLAYER_3, L"Browse...", L"Browse for file");
        
        devices->getGUIEnvironment()->addStaticText(L"Index 4 : ", rect<s32>(10, 320, 70, 340), false, true, generalTab, -1, false);
        devices->getGUIEnvironment()->addStaticText(L"Path : ", rect<s32>(10, 340, 50, 360), true, true, generalTab, -1, true);
        ebTextureLayerPath4 = devices->getGUIEnvironment()->addEditBox(L"Empty", rect<s32>(50, 340, 300, 360), true, generalTab, -10);
        browseButton4 = devices->getGUIEnvironment()->addButton(rect<s32>(305, 340, 360, 360), generalTab, 
                                                                CXT_EDIT_WINDOW_EVENTS_TEXLAYER_4, L"Browse...", L"Browse for file");
        
        generalLighting = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 370, 250, 390), generalTab, 
                                                                    CXT_EDIT_WINDOW_EVENTS_GENERAL_LIGHTING, L"Set All The Materials Lighting");
        
        generalVisible = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->isVisible(), rect<s32>(260, 370, 350, 390), generalTab, 
                                                                    CXT_EDIT_WINDOW_EVENTS_GENERAL_VISIBLE, L"Visible");
        
        devices->getGUIEnvironment()->addStaticText(L"Mat Type :", rect<s32>(10, 400, 80, 420), false, true, generalTab, -1, false);
        generalMaterialCB = devices->getGUIEnvironment()->addComboBox(rect<s32>(90, 400, 390, 420), generalTab, CXT_EDIT_WINDOW_EVENTS_GENERAL_MATERIAL_TYPE);
        createMaterialTypesComboBox(generalMaterialCB);
        
        devices->getGUIEnvironment()->addStaticText(L"Shadows :", rect<s32>(10, 440, 80, 460), false, true, generalTab, -1, false);
        shadowed = devices->getGUIEnvironment()->addComboBox(rect<s32>(90, 440, 390, 460), generalTab, CXT_EDIT_WINDOW_EVENTS_GENERAL_SHADOWS);
        shadowed->addItem(L"CAST");
        shadowed->addItem(L"RECEIVE");
        shadowed->addItem(L"BOTH (CAST & RECEIVE)");
        shadowed->addItem(L"EXCLUDE");
		shadowed->addItem(L"NO SHADOWS");
        E_SHADOW_MODE shadowMode = devices->getXEffect()->getNodeShadowMode(nodeToEdit, devices->getXEffectFilterType());
        switch (shadowMode) {
            case ESM_CAST:
                shadowed->setSelected(0);
                break;
            case ESM_RECEIVE:
                shadowed->setSelected(1);
                break;
            case ESM_BOTH:
                shadowed->setSelected(2);
                break;
            case ESM_EXCLUDE:
                shadowed->setSelected(3);
                break;
                
            default:
                break;
        }
		if (!devices->getXEffect()->isNodeShadowed(nodeToEdit)) {
			shadowed->setSelected(4);
		}
        
        nodeToDepthPass = devices->getGUIEnvironment()->addCheckBox(devices->getXEffect()->isDepthPassed(nodeToEdit), rect<s32>(10, 470, 150, 490), generalTab, 
                                                                    CXT_EDIT_WINDOW_EVENTS_GENERAL_DEPTH_PASS, L"Depth pass...");
        excludeLightingCalc = devices->getGUIEnvironment()->addCheckBox(devices->getXEffect()->isLightScatteringPassed(nodeToEdit),
                                                                        rect<s32>(160, 470, 420, 490), generalTab, 
                                                                        CXT_EDIT_WINDOW_EVENTS_GENERAL_EXCL_LIGHT_CALC, L"Light Scattering Pass");
        
        //ADVANCED TERRAIN
        devices->getGUIEnvironment()->addStaticText(L"Patch Size : ", rect<s32>(10, 5, 90, 25), true, true, advancedTab, -1, true);
        patchSize = devices->getGUIEnvironment()->addComboBox(rect<s32>(95, 5, 200, 25), advancedTab, CXT_EDIT_WINDOW_EVENTS_PATCH_SIZE);
        patchSize->addItem(L"ETPS_9");
        patchSize->addItem(L"ETPS_17");
        patchSize->addItem(L"ETPS_33");
        patchSize->addItem(L"ETPS_65");
        patchSize->addItem(L"ETPS_129");
        
        //ADVANCED MAX LOD
        devices->getGUIEnvironment()->addStaticText(L"Max LOD : ", rect<s32>(10, 35, 80, 55), true, true, advancedTab, -1, true);
        maxLOD = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(23.f).c_str(), rect<s32>(85, 35, 200, 55), 
                                                          true, advancedTab, -1);
        
        devices->getGUIEnvironment()->addStaticText(L"*Not implanted yet", rect<s32>(10, 100, 350, 120), false, false, advancedTab, -1, true);
        
        if (nodeToEdit->getType() != ESNT_TERRAIN) {
            patchSize->setEnabled(false);
            maxLOD->setEnabled(false);
            advancedTab->setTextColor(SColor(255, 50, 0, 0));
        }
        
        if (nodeToEdit->getType() != ESNT_ANIMATED_MESH) {
            animatedTab->setTextColor(SColor(255, 50, 0, 0));
        }
        
        //FLAGS AND MATERIALS
        //FLAGS LIGHTING
        lighting = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).Lighting, rect<s32>(10, 5, 180, 25),
                                                             flagsTab, CXT_EDIT_WINDOW_EVENTS_LIGHTING, L"Set Node Lighting");
        
        //MATERIALS
        materialsBar = devices->getGUIEnvironment()->addScrollBar(true, rect<s32>(10, 30, 390, 50), flagsTab, CXT_EDIT_WINDOW_EVENTS_MATERIALS);
        materialsBar->setMin(0);
        materialsBar->setLargeStep(1);
        materialsBar->setSmallStep(1);
        materialsBar->setMax(nodeToEdit->getMaterialCount()-1);
        materialsBar->setPos(0);
        
        materialNumber = devices->getGUIEnvironment()->addStaticText(L"Material Number : 0", rect<s32>(10, 60, 390, 80), true, true, flagsTab, -1, true);
        
        //TEXTURE 1
        devices->getGUIEnvironment()->addStaticText(L"Texture 1 :", rect<s32>(10, 90, 80, 110), false, true, flagsTab, -1, false);
        mTexture1 = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(80, 90, 290, 110), true, flagsTab, -1);
        devices->getGUIEnvironment()->addButton(rect<s32>(300, 90, 390, 110), flagsTab, CXT_EDIT_WINDOW_EVENTS_SELECT_1, L"Select", L"Select Texture 1");
        //TEXTURE 2
        devices->getGUIEnvironment()->addStaticText(L"Texture 2 :", rect<s32>(10, 120, 80, 140), false, true, flagsTab, -1, false);
        mTexture2 = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(80, 120, 290, 140), true, flagsTab, -1);
        devices->getGUIEnvironment()->addButton(rect<s32>(300, 120, 390, 140), flagsTab, CXT_EDIT_WINDOW_EVENTS_SELECT_2, L"Select", L"Select Texture 2");
        //TEXTURE 3
        devices->getGUIEnvironment()->addStaticText(L"Texture 3 :", rect<s32>(10, 150, 80, 170), false, true, flagsTab, -1, false);
        mTexture3 = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(80, 150, 290, 170), true, flagsTab, -1);
        devices->getGUIEnvironment()->addButton(rect<s32>(300, 150, 390, 170), flagsTab, CXT_EDIT_WINDOW_EVENTS_SELECT_3, L"Select", L"Select Texture 3");
        //TEXTURE 4
        devices->getGUIEnvironment()->addStaticText(L"Texture 4 :", rect<s32>(10, 180, 80, 200), false, true, flagsTab, -1, false);
        mTexture4 = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(80, 180, 290, 200), true, flagsTab, -1);
        devices->getGUIEnvironment()->addButton(rect<s32>(300, 180, 390, 200), flagsTab, CXT_EDIT_WINDOW_EVENTS_SELECT_4, L"Select", L"Select Texture 4");
        
        //ADVANCED DIFFUSE COLOR
        devices->getGUIEnvironment()->addStaticText(L"Diffuse :", rect<s32>(10, 210, 70, 230), true, true, flagsTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 210, 175, 230), flagsTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 210, 210, 230), true, true, flagsTab, -1, true);
        mNodeDiffuseColorA = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(210, 210, 310, 230), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_DIFFUSE_A);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 230, 30, 250), true, true, flagsTab, -1, true);
        mNodeDiffuseColorR = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(30, 230, 130, 250), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_DIFFUSE_R);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 230, 150, 250), true, true, flagsTab, -1, true);
        mNodeDiffuseColorG = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(150, 230, 250, 250), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_DIFFUSE_G);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 230, 270, 250), true, true, flagsTab, -1, true);
        mNodeDiffuseColorB = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(270, 230, 370, 250), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_DIFFUSE_B);
        
        //ADVANCED AMBIANT COLOR
        devices->getGUIEnvironment()->addStaticText(L"Ambiant :", rect<s32>(10, 260, 70, 280), true, true, flagsTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 260, 175, 280), flagsTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 260, 205, 280), true, true, flagsTab, -1, true);
        mNodeAmbiantColorA = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(210, 260, 310, 280), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_AMBIANT_A);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 280, 30, 300), true, true, flagsTab, -1, true);
        mNodeAmbiantColorR = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(30, 280, 130, 300), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_AMBIANT_R);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 280, 150, 300), true, true, flagsTab, -1, true);
        mNodeAmbiantColorG = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(150, 280, 250, 300), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_AMBIANT_G);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 280, 270, 300), true, true, flagsTab, -1, true);
        mNodeAmbiantColorB = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(270, 280, 370, 300), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_AMBIANT_B);
        
        //ADVANCED SPECULAR COLOR
        devices->getGUIEnvironment()->addStaticText(L"Specular :", rect<s32>(10, 310, 70, 330), true, true, flagsTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 310, 175, 330), flagsTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 310, 205, 330), true, true, flagsTab, -1, true);
        mNodeSpecularColorA = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(210, 310, 310, 330), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_SPECULAR_A);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 330, 30, 350), true, true, flagsTab, -1, true);
        mNodeSpecularColorR = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(30, 330, 130, 350), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_SPECULAR_R);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 330, 150, 350), true, true, flagsTab, -1, true);
        mNodeSpecularColorG = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(150, 330, 250, 350), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_SPECULAR_G);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 330, 270, 350), true, true, flagsTab, -1, true);
        mNodeSpecularColorB = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(270, 330, 370, 350), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_SPECULAR_B);
        
        //ADVANCED EMISSIVE COLOR
        devices->getGUIEnvironment()->addStaticText(L"Emissive :", rect<s32>(10, 360, 70, 380), true, true, flagsTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 360, 175, 380), flagsTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 360, 205, 380), true, true, flagsTab, -1, true);
        mNodeEmissiveColorA = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(210, 360, 310, 380), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_EMISSIVE_A);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 380, 30, 400), true, true, flagsTab, -1, true);
        mNodeEmissiveColorR = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(30, 380, 130, 400), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_EMISSIVE_R);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 380, 150, 400), true, true, flagsTab, -1, true);
        mNodeEmissiveColorG = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(150, 380, 250, 400), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_EMISSIVE_G);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 380, 270, 400), true, true, flagsTab, -1, true);
        mNodeEmissiveColorB = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(270, 380, 370, 400), true, flagsTab, CXT_EDIT_WINDOW_EVENTS_EMISSIVE_B);
        
        materialType = devices->getGUIEnvironment()->addComboBox(rect<s32>(10, 410, 390, 430), flagsTab, CXT_EDIT_WINDOW_EVENTS_MATERIAL_TYPE);
        createMaterialTypesComboBox(materialType);
        
        anisotropicFilter = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_ANISOTROPIC_FILTER), rect<s32>(10, 440, 140, 460),
                                                             flagsTab, CXT_EDIT_WINDOW_EVENTS_ANISOTROPIC_FILTER, L"Anisotropic Filter");
        bilinearFilter = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_BILINEAR_FILTER), rect<s32>(150, 440, 250, 460),
                                                                      flagsTab, CXT_EDIT_WINDOW_EVENTS_BILINEAR_FILTER, L"Bilinear Filter");
        trilinearFilter = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_TRILINEAR_FILTER), rect<s32>(270, 440, 380, 460),
                                                                      flagsTab, CXT_EDIT_WINDOW_EVENTS_TRILINEAR_FILTER, L"Trilinear Filter");
        
        antiAliasing = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_ANTI_ALIASING), rect<s32>(10, 470, 110, 490),
                                                                 flagsTab, CXT_EDIT_WINDOW_EVENTS_ANTI_ALIASING, L"Anti Aliasing");
        
        backFaceCulling = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_BACK_FACE_CULLING), rect<s32>(10, 500, 140, 520),
                                                                 flagsTab, CXT_EDIT_WINDOW_EVENTS_BACK_FACE_CULLING, L"Back Face Culling");
        frontFaceCulling = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_FRONT_FACE_CULLING), rect<s32>(150, 500, 380, 520),
                                                                 flagsTab, CXT_EDIT_WINDOW_EVENTS_FRONT_FACE_CULLING, L"Front Face Culling");
        
        colorMask = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_COLOR_MASK), rect<s32>(10, 530, 110, 550),
                                                                    flagsTab, CXT_EDIT_WINDOW_EVENTS_COLOR_MASK, L"Color Mask");
        colorMaterial = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_COLOR_MATERIAL), rect<s32>(150, 530, 380, 550),
                                                                     flagsTab, CXT_EDIT_WINDOW_EVENTS_COLOR_MATERIAL, L"Color Material");
        
        textureWrap = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_TEXTURE_WRAP), rect<s32>(10, 560, 130, 580),
                                                              flagsTab, CXT_EDIT_WINDOW_EVENTS_TEXTURE_WRAP, L"Texture Wrap");
        zbuffer = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_ZBUFFER), rect<s32>(150, 560, 250, 580),
                                                                  flagsTab, CXT_EDIT_WINDOW_EVENTS_ZBUFFER, L"ZBuffer");
        zwriteEnable = devices->getGUIEnvironment()->addCheckBox(nodeToEdit->getMaterial(0).getFlag(EMF_ZWRITE_ENABLE), rect<s32>(270, 560, 380, 580),
                                                              flagsTab, CXT_EDIT_WINDOW_EVENTS_ZWRITE_ENABLE, L"ZWrite Enable");
        
        //SETTING UP SPIN BOX FOR CUSTOM RESOLUTION AND MANAGE FLAGS VIEW
        ravelSpin = devices->getGUIEnvironment()->addSpinBox(L"", rect<s32>(flagsTab->getRelativePosition().getWidth()-15, 0, 
                                                                            flagsTab->getRelativePosition().getWidth(),
                                                                            flagsTab->getRelativePosition().getHeight()), true, flagsTab, -1);
        core::list<IGUIElement *>::ConstIterator element = flagsTab->getChildren().getLast();
        *element--;
        totalSpacing = (*element)->getRelativePosition().UpperLeftCorner.Y - flagsTab->getRelativePosition().getHeight();
        int numberOfPages = totalSpacing / flagsTab->getRelativePosition().getHeight();
        ravelSpin->setRange(0, numberOfPages+1);
        ravelSpin->setValue(ravelSpin->getMax());
        rsCurrentPos = ravelSpin->getValue();
        
        //SETTING UP GLOBAL FLAGS
        gAnisotropicFilter = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(9, 16, 149, 36), generalFlagsTab, -1, L"Anisotropic Filter");
        gBilinearFilter = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(149, 16, 259, 36), generalFlagsTab, -1, L"Bilinear Filter");
        gTrilinearFilter = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(259, 16, 379, 36), generalFlagsTab, -1, L"Trilinear Filter");
        
        gAntiAliasing = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(9, 46, 129, 66), generalFlagsTab, -1, L"Anti Aliasing");
        
        gBFCulling = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(9, 76, 149, 96), generalFlagsTab, -1, L"Back Face Culling");
        gFFCulling = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(149, 76, 289, 96), generalFlagsTab, -1, L"Front Face Culling");
        
        gColorMask = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(9, 106, 149, 126), generalFlagsTab, -1, L"Color Mask");
        gColorMaterial = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(149, 106, 289, 126), generalFlagsTab, -1, L"Color Material");
        
        gTextureWrap = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(9, 136, 149, 156), generalFlagsTab, -1, L"Texture Wrap");
        gZBuffer = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(149, 136, 259, 156), generalFlagsTab, -1, L"ZBuffer");
        gZWriteEnable = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(259, 136, 399, 156), generalFlagsTab, -1, L"ZWrite Enable");

		//SETTING PHYSICS
		SData *sdatat = (SData*)devices->getCoreData()->getISDataOfSceneNode(nodeToEdit);

		if (sdatat) {
			penablePhysics = devices->getGUIEnvironment()->addCheckBox(sdatat->isPhysicEnabled(), rect<s32>(9, 23, 193, 43), physicsTab, -1, L"Enable Physics");
			devices->getGUIEnvironment()->addStaticText(L"Type :", rect<s32>(9, 53, 63, 73), true, true, physicsTab, -1, true);
			pBodyType = devices->getGUIEnvironment()->addComboBox(rect<s32>(63, 53, 233, 73), physicsTab, -1);
			pBodyType->addItem(L"None");
			pBodyType->addItem(L"Rigid Body");
			pBodyType->addItem(L"Liquid Body");
			pBodyType->addItem(L"Soft Body");
			pBodyType->setSelected(sdatat->getBodyType());
			lastPhysicBodyType = sdatat->getBodyType();

			devices->getGUIEnvironment()->addStaticText(L"Mass :", rect<s32>(9, 93, 63, 113), true, true, physicsTab, -1, true);
			pMasseb = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(63, 93, 233, 113), true, physicsTab, -1);
			if (sdatat->getBodyType() == ISData::EIPT_LIQUID_BODY) {
				pMasseb->setEnabled(false);
			} else if (sdatat->getBodyType() == ISData::EIPT_NONE) {
				pMasseb->setEnabled(false);
			} else {
				pMasseb->setText(stringw(((ICollisionShape*)sdatat->getBodyPtr())->getMass()).c_str());
			}
			pEditBody = devices->getGUIEnvironment()->addButton(rect<s32>(9, 123, 113, 143), physicsTab, -1, L"Edit...", L"Edit the shape...");
		} else {
			for (IGUIElement *element : physicsTab->getChildren())
				element->setEnabled(false);

			physicsTab->setTextColor(SColor(255, 50, 0, 0));
		}
        
		//SETTING UP ANIMATED
		drawAnimations = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(19, 16, 159, 36), animatedTab, -1, L"Draw Animations");
		
		devices->getGUIEnvironment()->addStaticText(L"Saved Animations :", rect<s32>(19, 46, 149, 66), true, true, animatedTab, -1, true);
		savedAnimationsPath = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(149, 46, 309, 66), true, animatedTab, -1);
		savedAnimationsPath->setEnabled(false);
		browseSavedAnimations = devices->getGUIEnvironment()->addButton(rect<s32>(309, 46, 409, 66), animatedTab, -1, 
																		L"Browse...", L"Browse...");

		devices->getGUIEnvironment()->addStaticText(L"Choose Animation :", rect<s32>(19, 76, 149, 96), true, true, animatedTab, -1, true);
		chooseSavedAnimation = devices->getGUIEnvironment()->addComboBox(rect<s32>(149, 76, 409, 96), animatedTab, -1);

		if (nodeToEdit->getType() == ESNT_ANIMATED_MESH) {
			u32 i = devices->getCoreData()->getObjectNodeIndice(nodeToEdit);
			if (i != -1) {
				drawAnimations->setChecked(true);
				for (u32 ai = 0; ai < devices->getCoreData()->getObjectsData()->operator[](i).getActions()->size(); ai++) {
					chooseSavedAnimation->addItem(devices->getCoreData()->getObjectsData()->operator[](i).getActions()->operator[](ai)->getName().c_str());
				}
			} else {
				drawAnimations->setChecked(false);
			}
		}

		if (nodeToEdit->getType() != ESNT_ANIMATED_MESH) {
			core::list<IGUIElement *>::ConstIterator animatedElements = animatedTab->getChildren().begin();
			for (; animatedElements != animatedTab->getChildren().end(); ++animatedElements) {
				IGUIElement *element = (IGUIElement *)*animatedElements;
				element->setEnabled(false);
			}
		}

        //APPLY MATERIAL 0 DEFAULT VALUES
		lastMaterial = 0;
		lastMaterialType = nodeToEdit->getMaterial(0).MaterialType;

        setMaterialTextures();

		devices->getEventReceiver()->AddEventReceiver(this, editWindow, this);
    }
}

void CUIWindowEditNode::activateCloseButtons(bool activateCloseButton, bool activateWindowCloseButton) {
	if (editWindow) {
		closeButton->setVisible(activateCloseButton);
		editWindow->getCloseButton()->setVisible(activateWindowCloseButton);
	}
}

void CUIWindowEditNode::setMaterialTextures() {
	stringw material = L"Material Number : ";
    material += materialsBar->getPos();
    materialNumber->setText(material.c_str());

	nodeToEdit->getMaterial(lastMaterial).MaterialType = (E_MATERIAL_TYPE)lastMaterialType;
	lastMaterial = materialsBar->getPos();
	lastMaterialType = nodeToEdit->getMaterial(materialsBar->getPos()).MaterialType;
	nodeToEdit->getMaterial(materialsBar->getPos()).MaterialType = (E_MATERIAL_TYPE)devices->getXEffect()->getSelectionMaterial();

    //LIGHTING
    if (nodeToEdit->getMaterial(materialsBar->getPos()).Lighting == true) {
        lighting->setChecked(true);
    } else {
        lighting->setChecked(false);
    }
    
    //TEXTURE 1
    stringw texture1 = L"";
    if (nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[0].Texture) {
        texture1 += nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[0].Texture->getName().getPath().c_str();
        texture1.remove(devices->getWorkingDirectory().c_str());
    } else {
        texture1 += "Empty";
    }
    mTexture1->setText(texture1.c_str());
    //TEXTURE 2
    stringw texture2 = L"";
    if (nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[1].Texture) {
        texture2 += nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[1].Texture->getName().getPath().c_str();
        texture2.remove(devices->getWorkingDirectory().c_str());
    } else {
        texture2 += "Empty";
    }
    mTexture2->setText(texture2.c_str());
    //TEXTURE 3
    stringw texture3 = L"";
    if (nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[2].Texture) {
        texture3 += nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[2].Texture->getName().getPath().c_str();
        texture3.remove(devices->getWorkingDirectory().c_str());
    } else {
        texture3 += "Empty";
    }
    mTexture3->setText(texture3.c_str());
    //TEXTURE 4
    stringw texture4 = L"";
    if (nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[3].Texture) {
        texture4 += nodeToEdit->getMaterial(materialsBar->getPos()).TextureLayer[3].Texture->getName().getPath().c_str();
        texture4.remove(devices->getWorkingDirectory().c_str());
    } else {
        texture4 += "Empty";
    }
    mTexture4->setText(texture4.c_str());
    
    mNodeDiffuseColorA->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.getAlpha()).c_str());
    mNodeDiffuseColorR->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.getRed()).c_str());
    mNodeDiffuseColorG->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.getGreen()).c_str());
    mNodeDiffuseColorB->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.getBlue()).c_str());
    
    mNodeAmbiantColorA->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.getAlpha()).c_str());
    mNodeAmbiantColorR->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.getRed()).c_str());
    mNodeAmbiantColorG->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.getGreen()).c_str());
    mNodeAmbiantColorB->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.getBlue()).c_str());
    
    mNodeSpecularColorA->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.getAlpha()).c_str());
    mNodeSpecularColorR->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.getRed()).c_str());
    mNodeSpecularColorG->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.getGreen()).c_str());
    mNodeSpecularColorB->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.getBlue()).c_str());
    
    mNodeEmissiveColorA->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.getAlpha()).c_str());
    mNodeEmissiveColorR->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.getRed()).c_str());
    mNodeEmissiveColorG->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.getGreen()).c_str());
    mNodeEmissiveColorB->setText(devices->getCore()->getStrNumber(nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.getBlue()).c_str());
    
    /*nodeToEdit->setMaterialFlag(EMF_WIREFRAME, false);
    nodeToEdit->getMaterial(materialsBar->getPos()).Wireframe = true;
    
    if (nodeToEdit->getMaterialCount() == 1) {
        nodeToEdit->getMaterial(0).Wireframe = false;
    }*/
    
    materialType->setSelected(getMaterialTypeCB(nodeToEdit->getMaterial(materialsBar->getPos()).MaterialType));
    
    anisotropicFilter->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_ANISOTROPIC_FILTER));
    bilinearFilter->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_BILINEAR_FILTER));
    trilinearFilter->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_TRILINEAR_FILTER));
    
    antiAliasing->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_ANTI_ALIASING));
    
    backFaceCulling->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_BACK_FACE_CULLING));
    frontFaceCulling->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_FRONT_FACE_CULLING));
    
    colorMask->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_COLOR_MASK));
    colorMaterial->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_COLOR_MATERIAL));
    
    textureWrap->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_ZBUFFER));
    zbuffer->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_ZBUFFER));
    zwriteEnable->setChecked(nodeToEdit->getMaterial(materialsBar->getPos()).getFlag(EMF_ZWRITE_ENABLE));
}

void CUIWindowEditNode::createMaterialTypesComboBox(IGUIElement *element) {
    IGUIComboBox *comboBox = (IGUIComboBox *)element;
    
    /*comboBox->addItem(L"SOLID");
    comboBox->addItem(L"SOLID_2_LAYER");
    comboBox->addItem(L"LIGHTMAP");
    comboBox->addItem(L"LIGHTMAP_ADD");
    comboBox->addItem(L"LIGHTMAP_M2");
    comboBox->addItem(L"LIGHTMAP_M4");
    comboBox->addItem(L"LIGHTMAP_LIGHTING");
    comboBox->addItem(L"LIGHTMAP_LIGHTING_M2");
    comboBox->addItem(L"LIGHTMAP_LIGHTING_M4");
    comboBox->addItem(L"DETAIL_MAP");
    comboBox->addItem(L"SPHERE_MAP");
    comboBox->addItem(L"REFLECTION_2_LAYER");
    comboBox->addItem(L"TRANSPARENT_ADD_COLOR");
    comboBox->addItem(L"TRANSPARENT_ALPHA_CHANNEL");
    comboBox->addItem(L"TRANSPARENT_ALPHA_CHANNEL_REF");
    comboBox->addItem(L"TRANSPARENT_VERTEX_ALPHA");
    comboBox->addItem(L"TRANSPARENT_REFLECTION_2_LAYER");
    comboBox->addItem(L"NORMAL_MAP_SOLID");
    comboBox->addItem(L"NORMAL_MAP_TRANSPARENT_ADD_COLOR");
    comboBox->addItem(L"NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA");
    comboBox->addItem(L"PARALLAX_MAP_SOLID");
    comboBox->addItem(L"PARALLAX_MAP_TRANSPARENT_ADD_COLOR");
    comboBox->addItem(L"PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA");
    comboBox->addItem(L"ONETEXTURE_BLEND");
    comboBox->addItem(L"FORCE_32BIT");*/

	for (u32 i=0; i < devices->getCore()->getNumberOfBuildInMaterialTypes(); i++) {
		stringw name = irr::video::sBuiltInMaterialTypeNames[i];
		name.make_upper();
		comboBox->addItem(name.c_str());
	}
    
    // > 25
    for (u32 i=0; i < devices->getCoreData()->getShaderCallbacks()->size(); i++) {
        stringw name = devices->getCoreData()->getShaderCallbacks()->operator[](i)->getName().c_str();
        comboBox->addItem(name.c_str());
    }
}

s32 CUIWindowEditNode::getMaterialTypeCB(E_MATERIAL_TYPE type) {
    s32 pos = 0;
    
    //WE CAN CHANGE IT !
    switch (type) {
        case EMT_SOLID: pos=0; break;
        case EMT_SOLID_2_LAYER: pos=1; break;
        case EMT_LIGHTMAP: pos=2; break;
        case EMT_LIGHTMAP_ADD: pos=3; break;
        case EMT_LIGHTMAP_M2: pos=4; break;
        case EMT_LIGHTMAP_M4: pos=5; break;
        case EMT_LIGHTMAP_LIGHTING: pos=6; break;
        case EMT_LIGHTMAP_LIGHTING_M2: pos=7; break;
        case EMT_LIGHTMAP_LIGHTING_M4: pos=8; break;
        case EMT_DETAIL_MAP: pos=9; break;
        case EMT_SPHERE_MAP: pos=10; break;
        case EMT_REFLECTION_2_LAYER: pos=11; break;
        case EMT_TRANSPARENT_ADD_COLOR: pos=12; break;
        case EMT_TRANSPARENT_ALPHA_CHANNEL: pos=13; break;
        case EMT_TRANSPARENT_ALPHA_CHANNEL_REF: pos=14; break;
        case EMT_TRANSPARENT_VERTEX_ALPHA: pos=15; break;
        case EMT_TRANSPARENT_REFLECTION_2_LAYER: pos=16; break;
        case EMT_NORMAL_MAP_SOLID: pos=17; break;
        case EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR: pos=18; break;
        case EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA: pos=19; break;
        case EMT_PARALLAX_MAP_SOLID: pos=20; break;
        case EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR: pos=21; break;
        case EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA: pos=22; break;
        case EMT_ONETEXTURE_BLEND: pos=23; break;
        case EMT_FORCE_32BIT: pos=24; break;
            
        default:
            break;
    }
    
    return pos;
}

E_MATERIAL_TYPE CUIWindowEditNode::getMaterialType(s32 pos) {
    E_MATERIAL_TYPE type = EMT_SOLID;
    
    //IGUIComboBox *comboBox = (IGUIComboBox *)element;
    
    //WE CAN CHANGE IT !
    switch (pos) {
        case 0: type = EMT_SOLID; break;
        case 1: type = EMT_SOLID_2_LAYER; break;
        case 2: type = EMT_LIGHTMAP; break;
        case 3: type = EMT_LIGHTMAP_ADD; break;
        case 4: type = EMT_LIGHTMAP_M2; break;
        case 5: type = EMT_LIGHTMAP_M4; break;
        case 6: type = EMT_LIGHTMAP_LIGHTING; break;
        case 7: type = EMT_LIGHTMAP_LIGHTING_M2; break;
        case 8: type = EMT_LIGHTMAP_LIGHTING_M4; break;
        case 9: type = EMT_DETAIL_MAP; break;
        case 10: type = EMT_SPHERE_MAP; break;
        case 11: type = EMT_REFLECTION_2_LAYER; break;
        case 12: type = EMT_TRANSPARENT_ADD_COLOR; break;
        case 13: type = EMT_TRANSPARENT_ALPHA_CHANNEL; break;
        case 14: type = EMT_TRANSPARENT_ALPHA_CHANNEL_REF; break;
        case 15: type = EMT_TRANSPARENT_VERTEX_ALPHA; break;
        case 16: type = EMT_TRANSPARENT_REFLECTION_2_LAYER; break;
        case 17: type = EMT_NORMAL_MAP_SOLID; break;
        case 18: type = EMT_NORMAL_MAP_TRANSPARENT_ADD_COLOR; break;
        case 19: type = EMT_NORMAL_MAP_TRANSPARENT_VERTEX_ALPHA; break;
        case 20: type = EMT_PARALLAX_MAP_SOLID; break;
        case 21: type = EMT_PARALLAX_MAP_TRANSPARENT_ADD_COLOR; break;
        case 22: type = EMT_PARALLAX_MAP_TRANSPARENT_VERTEX_ALPHA; break;
        case 23: type = EMT_ONETEXTURE_BLEND; break;
        case 24: type = EMT_FORCE_32BIT; break;
            
        default:
            break;
    }
    
    return type;
}

void CUIWindowEditNode::update() {
	bool canChange = true;

	IGUIElement *element = devices->getGUIEnvironment()->getFocus();
	while (element != 0) {
		if (element == editWindow) {
			canChange = false;
			break;
		}
		element = element->getParent();
	}

	if (canChange) {
		CCore *core = devices->getCore();
		//POSITION
		if (core->getF32(ebNodePositionX->getText()) != nodeToEdit->getPosition().X) {
			ebNodePositionX->setText(stringw(nodeToEdit->getPosition().X).c_str());
		}
		if (core->getF32(ebNodePositionY->getText()) != nodeToEdit->getPosition().Y) {
			ebNodePositionY->setText(stringw(nodeToEdit->getPosition().Y).c_str());
		}
		if (core->getF32(ebNodePositionZ->getText()) != nodeToEdit->getPosition().Z) {
			ebNodePositionZ->setText(stringw(nodeToEdit->getPosition().Z).c_str());
		}

		//ROTATION
		if (core->getF32(ebNodeRotationX->getText()) != nodeToEdit->getRotation().X) {
			ebNodeRotationX->setText(stringw(nodeToEdit->getRotation().X).c_str());
		}
		if (core->getF32(ebNodeRotationY->getText()) != nodeToEdit->getRotation().Y) {
			ebNodeRotationY->setText(stringw(nodeToEdit->getRotation().Y).c_str());
		}
		if (core->getF32(ebNodeRotationZ->getText()) != nodeToEdit->getRotation().Z) {
			ebNodeRotationZ->setText(stringw(nodeToEdit->getRotation().Z).c_str());
		}

		//SCALE
		if (nodeToEdit->getType() != ESNT_BILLBOARD) {
			if (core->getF32(ebNodeScaleX->getText()) != nodeToEdit->getScale().X) {
				ebNodeScaleX->setText(stringw(nodeToEdit->getScale().X).c_str());
			}
			if (core->getF32(ebNodeScaleY->getText()) != nodeToEdit->getScale().Y) {
				ebNodeScaleY->setText(stringw(nodeToEdit->getScale().Y).c_str());
			}
			if (core->getF32(ebNodeScaleZ->getText()) != nodeToEdit->getScale().Z) {
				ebNodeScaleZ->setText(stringw(nodeToEdit->getScale().Z).c_str());
			}
		}
	}

}

void CUIWindowEditNode::applyToMeshBuffers(u32 id, ITexture *tex) {
	if (nodeToEdit->getType() == ESNT_MESH || nodeToEdit->getType() == ESNT_OCTREE) {
		for (u32 i=0; i < ((IMeshSceneNode*)nodeToEdit)->getMesh()->getMeshBufferCount(); i++)
			((IMeshSceneNode*)nodeToEdit)->getMesh()->getMeshBuffer(i)->getMaterial().setTexture(id, tex);
	}
}

bool CUIWindowEditNode::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			//RESET COMBO BOXES FOR MATERIALS
			IGUIElement *element = devices->getGUIEnvironment()->getFocus();

			bool isWindow = false;
			while (element != 0) {
				if (element == editWindow) {
					isWindow = true;
					break;
				}
				element = element->getParent();
			}

			if (isWindow) {
				generalMaterialCB->clear();
				materialType->clear();
				this->createMaterialTypesComboBox(generalMaterialCB);
				this->createMaterialTypesComboBox(materialType);
			}

			//GET SELECTED MATERIAL
			/*if (devices->getGUIEnvironment()->getFocus() == 0 && !devices->isCtrlPushed() && !devices->isShiftPushed()) {
				ISceneCollisionManager *collisionManager = devices->getSceneManager()->getSceneCollisionManager();
				ICursorControl *cursorCtrl = devices->getDevice()->getCursorControl();

				core::line3d<f32> ray = collisionManager->getRayFromScreenCoordinates(cursorCtrl->getPosition());
				vector3df intersection;
				triangle3df hitTriangle;
				ISceneNode *selectedMouseNode = collisionManager->getSceneNodeAndCollisionPointFromRay(ray, intersection, hitTriangle, 0, 0);

				if (selectedMouseNode == nodeToEdit) {
					IMesh *mnode;
					if (nodeToEdit->getType() == ESNT_MESH || nodeToEdit->getType() == ESNT_OCTREE) {
						mnode = ((IMeshSceneNode*)nodeToEdit)->getMesh();
					} else if (nodeToEdit->getType() == ESNT_ANIMATED_MESH) {
						mnode = ((IAnimatedMeshSceneNode*)nodeToEdit)->getMesh();
					}
					u32 nearestBuffer = 0;
					f32 distanceMin = -1.0f;

					for (u32 i=0; i < mnode->getMeshBufferCount(); i++) {
						mnode->getMeshBuffer(i)->recalculateBoundingBox();
						S3DVertex *vertices = (S3DVertex *)mnode->getMeshBuffer(i)->getVertices();
						for (u32 j=0; j < mnode->getMeshBuffer(i)->getVertexCount(); j++) {
							if (distanceMin == -1) {
								distanceMin = hitTriangle.pointA.getDistanceFrom(vertices[j].Pos);
							} else {
								f32 distanceFromA = hitTriangle.pointA.getDistanceFrom(vertices[j].Pos);
								f32 distanceFromB = hitTriangle.pointB.getDistanceFrom(vertices[j].Pos);
								f32 distanceFromC = hitTriangle.pointC.getDistanceFrom(vertices[j].Pos);
								if (distanceFromA < distanceMin) {
									nearestBuffer = i;
									distanceMin = distanceFromA;
								}
								if (distanceFromB < distanceMin) {
									nearestBuffer = i;
									distanceMin = distanceFromB;
								}
								if (distanceFromC < distanceMin) {
									nearestBuffer = i;
									distanceMin = distanceFromC;
								}
							}
						}
					}
					materialsBar->setPos(nearestBuffer);
					setMaterialTextures();
				}
			}*/
		}
	}

    if (event.EventType == EET_KEY_INPUT_EVENT) {
        if (!event.KeyInput.PressedDown) {
            if (event.KeyInput.Key == KEY_LEFT) {
                if (!devices->isEditBoxEntered()) {
                    if (materialsBar->getPos() > 0) {
                        materialsBar->setPos(materialsBar->getPos() - 1);
                        
                        setMaterialTextures();
                    }
                }
            }
            
            if (event.KeyInput.Key == KEY_RIGHT) {
                if (!devices->isEditBoxEntered()) {
                    if (materialsBar->getPos() < nodeToEdit->getMaterialCount()) {
                        materialsBar->setPos(materialsBar->getPos() + 1);
                        
                        setMaterialTextures();
                    }
                }
            }
            
            if (event.KeyInput.Key == KEY_RETURN) {
                nodeToEdit->setMaterialFlag(EMF_WIREFRAME, false);
				nodeToEdit->getMaterial(lastMaterial).MaterialType = (E_MATERIAL_TYPE)lastMaterialType;
            }
        }
    }
    
    if (event.EventType == EET_GUI_EVENT) {
        
        IGUIElement *selectedElement = event.GUIEvent.Caller;
        bool treat = false;
        while (selectedElement != 0 && selectedElement->getParent() != devices->getGUIEnvironment()->getRootGUIElement()) {
            if (selectedElement->getParent() == editWindow) {
                treat = true;
                break;
            }
            selectedElement = selectedElement->getParent();
        }

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == editWindow) {
				SEvent ev;
				ev.EventType = EET_GUI_EVENT;
				ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
				ev.GUIEvent.Caller = closeButton;
				ev.GUIEvent.Element = closeButton;
				OnEvent(ev);
			}
		}

        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
			if (event.GUIEvent.Caller == editWindow->getMinimizeButton()) {
				//devices->getEventReceiver()->AddMinimizedWindow(this, editWindow);
			}

            if (event.GUIEvent.Caller == editWindow->getMaximizeButton()) {
                
                if (isWindowed) {
                    editWindow->setRelativePosition(rect<s32>(devices->getVideoDriver()->getScreenSize().Width-420, 75, 
                                                              devices->getVideoDriver()->getScreenSize().Width, 
                                                              devices->getVideoDriver()->getScreenSize().Height-20));
                    
                    tabCtrl->setRelativePosition(rect<int>(5, 20, 415, editWindow->getRelativePosition().getHeight()-50));
                    
                    int numberOfPages = totalSpacing / flagsTab->getRelativePosition().getHeight();
                    ravelSpin->setRange(0, numberOfPages+1);
                    ravelSpin->setValue(ravelSpin->getMax());
                    rsCurrentPos = ravelSpin->getValue();
                    ravelSpin->setRelativePosition(rect<s32>(flagsTab->getRelativePosition().getWidth()-15, 0, 
                                                             flagsTab->getRelativePosition().getWidth(),
                                                             flagsTab->getRelativePosition().getHeight()));
                    
                    applyButton->setRelativePosition(rect<s32>(5, editWindow->getRelativePosition().getHeight()-40, 
                                                               80, editWindow->getRelativePosition().getHeight()-10));
                    closeButton->setRelativePosition(rect<s32>(100, editWindow->getRelativePosition().getHeight()-40, 
                                                               175, editWindow->getRelativePosition().getHeight()-10));
                    
                    editWindow->setDraggable(false);
                    editWindow->setDrawTitlebar(false);
                } else {
                    editWindow->setRelativePosition(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-200, 
                                                              100, devices->getVideoDriver()->getScreenSize().Width/2+200, 670));
                    
                    tabCtrl->setRelativePosition(rect<int>(2, 20, 418, 520));
                    
                    int numberOfPages = totalSpacing / flagsTab->getRelativePosition().getHeight();
                    ravelSpin->setRange(0, numberOfPages+1);
                    ravelSpin->setValue(ravelSpin->getMax());
                    rsCurrentPos = ravelSpin->getValue();
                    ravelSpin->setRelativePosition(rect<s32>(flagsTab->getRelativePosition().getWidth()-15, 0, 
                                                             flagsTab->getRelativePosition().getWidth(),
                                                             flagsTab->getRelativePosition().getHeight()));
                    
                    applyButton->setRelativePosition(rect<s32>(5, 530, 80, 560));
                    closeButton->setRelativePosition(rect<s32>(100, 530, 175, 560));
                    
                    editWindow->setDraggable(true);
                    editWindow->setDrawTitlebar(true);
                }
                
                isWindowed = !isWindowed;
            }

			if (event.GUIEvent.Caller == closeButton) {
				if (nodeToEdit) {
					nodeToEdit->setDebugDataVisible(EDS_OFF);
					nodeToEdit->setMaterialFlag(EMF_WIREFRAME, false);
				}
				nodeToEdit->getMaterial(lastMaterial).MaterialType = (E_MATERIAL_TYPE)lastMaterialType;
                editWindow->remove();
                devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
			}

			if (event.GUIEvent.Caller == applyButton) {
				//GENERAL
                nodeToEdit->setName(ebNodeName->getText());
                nodeToEdit->setPosition(devices->getCore()->getVector3df(devices->getCore()->convertToString(ebNodePositionX->getText()), 
                                                                            devices->getCore()->convertToString(ebNodePositionY->getText()), 
                                                                            devices->getCore()->convertToString(ebNodePositionZ->getText())));
                nodeToEdit->setRotation(devices->getCore()->getVector3df(devices->getCore()->convertToString(ebNodeRotationX->getText()), 
                                                                            devices->getCore()->convertToString(ebNodeRotationY->getText()), 
                                                                            devices->getCore()->convertToString(ebNodeRotationZ->getText())));
				if (nodeToEdit->getType() != ESNT_BILLBOARD) {
					nodeToEdit->setScale(devices->getCore()->getVector3df(devices->getCore()->convertToString(ebNodeScaleX->getText()), 
																			devices->getCore()->convertToString(ebNodeScaleY->getText()), 
																			devices->getCore()->convertToString(ebNodeScaleZ->getText())));
				} else {
					IBillboardSceneNode *billNode = (IBillboardSceneNode *)nodeToEdit;
					billNode->setSize(devices->getCore()->getDimensionF32(devices->getCore()->convertToString(ebNodeScaleX->getText()), 
																			devices->getCore()->convertToString(ebNodeScaleY->getText())));
					nodeToEdit->setScale(devices->getCore()->getVector3df(devices->getCore()->convertToString(ebNodeScaleX->getText()), 
																			devices->getCore()->convertToString(ebNodeScaleY->getText()), 
																			devices->getCore()->convertToString("1")));
				}
                if (!stringw(ebTextureLayerPath1->getText()).equals_ignore_case(L"Empty")) {
                nodeToEdit->setMaterialTexture(0, 
                            devices->getVideoDriver()->getTexture(stringc(ebTextureLayerPath1->getText()).c_str()));
                }
                if (!stringw(ebTextureLayerPath2->getText()).equals_ignore_case(L"Empty")) {
                nodeToEdit->setMaterialTexture(1, 
                            devices->getVideoDriver()->getTexture(stringc(ebTextureLayerPath2->getText()).c_str()));
				applyToMeshBuffers(1, devices->getVideoDriver()->getTexture(stringc(ebTextureLayerPath2->getText()).c_str()));
                }
                if (!stringw(ebTextureLayerPath3->getText()).equals_ignore_case(L"Empty")) {
                nodeToEdit->setMaterialTexture(2, 
                            devices->getVideoDriver()->getTexture(stringc(ebTextureLayerPath3->getText()).c_str()));
                }
                if (!stringw(ebTextureLayerPath4->getText()).equals_ignore_case(L"Empty")) {
                nodeToEdit->setMaterialTexture(3, 
                            devices->getVideoDriver()->getTexture(stringc(ebTextureLayerPath4->getText()).c_str()));
                }

                if (nodeToEdit->getType() == ESNT_TERRAIN) {
					devices->getSceneManager()->getMeshManipulator()->recalculateNormals(((ITerrainSceneNode*)nodeToEdit)->getMesh(), true, true);
					devices->getSceneManager()->getMeshManipulator()->recalculateTangents(((ITerrainSceneNode*)nodeToEdit)->getMesh(), true, true, true);
					nodeToEdit->setPosition(nodeToEdit->getPosition());
					((ITerrainSceneNode *)nodeToEdit)->getMesh()->setDirty();
                }

				if (nodeToEdit->getType() == ESNT_TERRAIN || nodeToEdit->getType() == ESNT_OCTREE || nodeToEdit->getType() == ESNT_MESH)
					devices->getCollisionManager()->setCollisionToAnOctTreeNode(nodeToEdit);
				if (nodeToEdit->getType() == ESNT_ANIMATED_MESH)
					devices->getCollisionManager()->setCollisionToAnAnimatedNode(nodeToEdit);
			}

			if (event.GUIEvent.Caller == cursorPositionbtn) {
				ebNodePositionX->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().X).c_str());
                ebNodePositionY->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().Y).c_str());
                ebNodePositionZ->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().Z).c_str());
			}
            
            if (treat) {
            
                s32 id = event.GUIEvent.Caller->getID();
                switch (id) {
                        
                    case CXT_EDIT_WINDOW_EVENTS_TEXLAYER_1:
                        //devices->getGUIEnvironment()->addFileOpenDialog(L"Choose the fist texture layer");
                        devices->createFileOpenDialog(L"Choose the first texture layer", 0);
                        currentBrowse = 1;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_TEXLAYER_2:
                        devices->createFileOpenDialog(L"Choose the second texture layer", 0);
                        currentBrowse = 2;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_TEXLAYER_3:
                        devices->createFileOpenDialog(L"Choose the third texture layer", 0);
                        currentBrowse = 3;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_TEXLAYER_4:
                        devices->createFileOpenDialog(L"Choose the fourth texture layer", 0);
                        currentBrowse = 4;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SELECT_1:
                        devices->createFileOpenDialog(L"Choose Texture 1 for this material", 0);
                        currentBrowse = 11;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SELECT_2:
                        devices->createFileOpenDialog(L"Choose Texture 2 for this material", 0);
                        currentBrowse = 12;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SELECT_3:
                        devices->createFileOpenDialog(L"Choose Texture 3 for this material", 0);
                        currentBrowse = 13;
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SELECT_4:
                        devices->createFileOpenDialog(L"Choose Texture 4 for this material", 0);
                        currentBrowse = 14;
                        break;
                        
                    default:
                        break;
                }
            }
			if (event.GUIEvent.Caller == browseSavedAnimations) {
				browseSavedAnimationDialog = devices->createFileOpenDialog(L"Choose the animation file",
																		   CGUIFileSelector::EFST_OPEN_DIALOG,
																		   devices->getGUIEnvironment()->getRootGUIElement());
			}
        }
        
        //EDIT BOXES
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            if (treat) {
                s32 id = event.GUIEvent.Caller->getID();
                switch (id) {
                    //DIFFUSE COLOR -----------------------------------------------
                    case CXT_EDIT_WINDOW_EVENTS_DIFFUSE_A: {
                        stringc alpha = "";
                        alpha += mNodeDiffuseColorA->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.setAlpha(devices->getCore()->getU32(alpha.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_DIFFUSE_R: {
                        stringc red = "";
                        red += mNodeDiffuseColorR->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.setRed(devices->getCore()->getU32(red.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_DIFFUSE_G: {
                        stringc green = "";
                        green += mNodeDiffuseColorG->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.setGreen(devices->getCore()->getU32(green.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_DIFFUSE_B: {
                        stringc blue = "";
                        blue += mNodeDiffuseColorB->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).DiffuseColor.setBlue(devices->getCore()->getU32(blue.c_str()));
                    }
                        break;
                        
                    //AMBIANT COLOR -----------------------------------------------
                    case CXT_EDIT_WINDOW_EVENTS_AMBIANT_A: {
                        stringc alpha = "";
                        alpha += mNodeAmbiantColorA->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.setAlpha(devices->getCore()->getU32(alpha.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_AMBIANT_R: {
                        stringc red = "";
                        red += mNodeAmbiantColorR->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.setRed(devices->getCore()->getU32(red.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_AMBIANT_G: {
                        stringc green = "";
                        green += mNodeAmbiantColorG->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.setGreen(devices->getCore()->getU32(green.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_AMBIANT_B: {
                        stringc blue = "";
                        blue += mNodeAmbiantColorB->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).AmbientColor.setBlue(devices->getCore()->getU32(blue.c_str()));
                    }
                        break;
                        
                    //SPECULAR COLOR -----------------------------------------------
                    case CXT_EDIT_WINDOW_EVENTS_SPECULAR_A: {
                        stringc alpha = "";
                        alpha += mNodeSpecularColorA->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.setAlpha(devices->getCore()->getU32(alpha.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SPECULAR_R: {
                        stringc red = "";
                        red += mNodeSpecularColorR->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.setRed(devices->getCore()->getU32(red.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SPECULAR_G: {
                        stringc green = "";
                        green += mNodeSpecularColorG->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.setGreen(devices->getCore()->getU32(green.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_SPECULAR_B: {
                        stringc blue = "";
                        blue += mNodeSpecularColorB->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).SpecularColor.setBlue(devices->getCore()->getU32(blue.c_str()));
                    }
                        break;
                        
                    //EMISSIVE COLOR -----------------------------------------------
                    case CXT_EDIT_WINDOW_EVENTS_EMISSIVE_A: {
                        stringc alpha = "";
                        alpha += mNodeEmissiveColorA->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.setAlpha(devices->getCore()->getU32(alpha.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_EMISSIVE_R: {
                        stringc red = "";
                        red += mNodeEmissiveColorR->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.setRed(devices->getCore()->getU32(red.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_EMISSIVE_G: {
                        stringc green = "";
                        green += mNodeEmissiveColorG->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.setGreen(devices->getCore()->getU32(green.c_str()));
                    }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_EMISSIVE_B: {
                        stringc blue = "";
                        blue += mNodeEmissiveColorB->getText();
                        nodeToEdit->getMaterial(materialsBar->getPos()).EmissiveColor.setBlue(devices->getCore()->getU32(blue.c_str()));
                    }
                        break;
                        
                    default:
                        break;
                }
            }
			if (event.GUIEvent.Caller == pMasseb) {
				SData *sdatat = (SData*)devices->getCoreData()->getISDataOfSceneNode(nodeToEdit);
				f32 newMass = devices->getCore()->getF32(pMasseb->getText());
				if (sdatat->getBodyType() == ISData::EIPT_RIGID_BODY) {
					devices->getBulletWorld()->removeCollisionObject((IRigidBody*)sdatat->getBodyPtr(), false);
					ICollisionShape *newShape = 0;

					if (nodeToEdit->getType() == ESNT_MESH || nodeToEdit->getType() == ESNT_OCTREE) {
						newShape = new IBvhTriangleMeshShape(nodeToEdit, sdatat->getMesh(), newMass);
					} else if (nodeToEdit->getType() == ESNT_CUBE) {
						newShape = new IBoxShape(nodeToEdit, newMass, false);
					} else if (nodeToEdit->getType() == ESNT_SPHERE) {
						newShape = new ISphereShape(nodeToEdit, newMass, false);
					}

					if (newShape) {
						newShape->setNewMass(newMass);
						IRigidBody *rbody = devices->getBulletWorld()->addRigidBody(newShape);
						rbody->forceActivationState(EAS_DISABLE_DEACTIVATION);
						rbody->setActivationState(EAS_DISABLE_DEACTIVATION);
						sdatat->setPBodyPtr(rbody);
					}
				}
				if (sdatat->getBodyType() == ISData::EIPT_SOFT_BODY) {
					devices->getBulletWorld()->removeCollisionObject((ISoftBody*)sdatat->getBodyPtr(), false);

				}
			}
        }
        
        //CHECKBOXES
        if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
            if (treat) {
                s32 id = event.GUIEvent.Caller->getID();
                switch (id) {
                        
                    //SET NODE LIGHTING OR NOT
                    case CXT_EDIT_WINDOW_EVENTS_GENERAL_LIGHTING:
                        nodeToEdit->setMaterialFlag(EMF_LIGHTING, generalLighting->isChecked());
                        generalLighting->setChecked(generalLighting->isChecked());
                        break;
                        
                    //SET NODE VISIBLE OR UNVISIBLE
                    case CXT_EDIT_WINDOW_EVENTS_GENERAL_VISIBLE:
                        nodeToEdit->setVisible(generalVisible->isChecked());
                        generalVisible->setChecked(nodeToEdit->isVisible());
                        break;
                        
                    //ADD OR REMOVE NODE FROM DEPTH PASS
                    case CXT_EDIT_WINDOW_EVENTS_GENERAL_DEPTH_PASS:
                        if (nodeToDepthPass->isChecked()) {
                            devices->getXEffect()->addNodeToDepthPass(nodeToEdit);
                        } else {
                            devices->getXEffect()->removeNodeFromDepthPass(nodeToEdit);
                        }
                        break;
                        
                    //EXCLUDE NODE FROM LIGHTING CALCULATION
                    case CXT_EDIT_WINDOW_EVENTS_GENERAL_EXCL_LIGHT_CALC: {
                        /*if (excludeLightingCalc->isChecked()) {
                            devices->getXEffect()->excludeNodeFromLightingCalculations(nodeToEdit);
                        } else {
                            devices->getXEffect()->addShadowToNode(nodeToEdit, devices->getXEffectFilterType(), ESM_RECEIVE);
                        }*/
						EffectHandler *effect = devices->getXEffect();
						excludeLightingCalc->isChecked() ? effect->addNodeToLightScatteringPass(nodeToEdit) : effect->removeNodeFromLightScatteringPass(nodeToEdit);
					}
                        break;
                        
                    //LIGHTING
                    case CXT_EDIT_WINDOW_EVENTS_LIGHTING:
                        nodeToEdit->getMaterial(materialsBar->getPos()).Lighting = lighting->isChecked();
                    break;
                        
                    //FILTERS
                    case CXT_EDIT_WINDOW_EVENTS_ANISOTROPIC_FILTER:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_ANISOTROPIC_FILTER, anisotropicFilter->isChecked());
                        break;
                    case CXT_EDIT_WINDOW_EVENTS_BILINEAR_FILTER:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_BILINEAR_FILTER, bilinearFilter->isChecked());
                        break;
                    case CXT_EDIT_WINDOW_EVENTS_TRILINEAR_FILTER:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_TRILINEAR_FILTER, trilinearFilter->isChecked());
                        break;
                        
                    //ANTI ALIASING
                    case CXT_EDIT_WINDOW_EVENTS_ANTI_ALIASING:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_ANTI_ALIASING, antiAliasing->isChecked());
                        break;
                        
                    //CULLING
                    case CXT_EDIT_WINDOW_EVENTS_BACK_FACE_CULLING:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_BACK_FACE_CULLING, backFaceCulling->isChecked());
                        break;
                    case CXT_EDIT_WINDOW_EVENTS_FRONT_FACE_CULLING:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_FRONT_FACE_CULLING, frontFaceCulling->isChecked());
                        break;
                        
                    //COLOR
                    case CXT_EDIT_WINDOW_EVENTS_COLOR_MASK:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_COLOR_MASK, colorMask->isChecked());
                        break;
                    case CXT_EDIT_WINDOW_EVENTS_COLOR_MATERIAL:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_COLOR_MATERIAL, colorMaterial->isChecked());
                        break;
                    
                    //WRITING
                    case CXT_EDIT_WINDOW_EVENTS_TEXTURE_WRAP:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_TEXTURE_WRAP, textureWrap->isChecked());
                        break;
                    case CXT_EDIT_WINDOW_EVENTS_ZBUFFER:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_ZBUFFER, zbuffer->isChecked());
                        break;
                    case CXT_EDIT_WINDOW_EVENTS_ZWRITE_ENABLE:
                        nodeToEdit->getMaterial(materialsBar->getPos()).setFlag(EMF_ZWRITE_ENABLE, zwriteEnable->isChecked());
                        break;
                        
                    default:
                        break;
                }
            }

			//DRAW ANIMATIONS
			if (event.GUIEvent.Caller == drawAnimations) {
				IAnimatedMeshSceneNode *animatedNode = reinterpret_cast<IAnimatedMeshSceneNode *>(nodeToEdit);
				if (drawAnimations->isChecked()) {
					animatedNode->setFrameLoop(animatedNode->getStartFrame(), animatedNode->getEndFrame());
					animatedNode->setAnimationSpeed(25.f);
				} else {
					animatedNode->setFrameLoop(0, 0);
				}
			}
            
            //GLOBAL FILTERS
            if (event.GUIEvent.Caller == gAnisotropicFilter) {
                nodeToEdit->setMaterialFlag(EMF_ANISOTROPIC_FILTER, gAnisotropicFilter->isChecked());
            }
            if (event.GUIEvent.Caller == gBilinearFilter) {
                nodeToEdit->setMaterialFlag(EMF_BILINEAR_FILTER, gBilinearFilter->isChecked());
            }
            if (event.GUIEvent.Caller == gTrilinearFilter) {
                nodeToEdit->setMaterialFlag(EMF_TRILINEAR_FILTER, gTrilinearFilter->isChecked());
            }
            
            //GLOBAL ANTIALIASING
            if (event.GUIEvent.Caller == gAntiAliasing) {
                nodeToEdit->setMaterialFlag(EMF_ANTI_ALIASING, gAntiAliasing->isChecked());
            }
            
            //CLOBAL CULLING
            if (event.GUIEvent.Caller == gBFCulling) {
                nodeToEdit->setMaterialFlag(EMF_BACK_FACE_CULLING, gBFCulling->isChecked());
            }
            if (event.GUIEvent.Caller == gFFCulling) {
                nodeToEdit->setMaterialFlag(EMF_FRONT_FACE_CULLING, gFFCulling->isChecked());
            }
            
            //GLOBAL COLOR
            if (event.GUIEvent.Caller == gColorMask) {
                nodeToEdit->setMaterialFlag(EMF_COLOR_MASK, gColorMask->isChecked());
            }
            if (event.GUIEvent.Caller == gColorMaterial) {
                nodeToEdit->setMaterialFlag(EMF_COLOR_MATERIAL, gColorMaterial->isChecked());
            }
            
            //GLOBAL TEXTURE
            if (event.GUIEvent.Caller == gTextureWrap) {
                nodeToEdit->setMaterialFlag(EMF_TEXTURE_WRAP, gTextureWrap->isChecked());
            }
            if (event.GUIEvent.Caller == gZBuffer) {
                nodeToEdit->setMaterialFlag(EMF_ZBUFFER, gZBuffer->isChecked());
            }
            if (event.GUIEvent.Caller == gZWriteEnable) {
                nodeToEdit->setMaterialFlag(EMF_ZWRITE_ENABLE, gZWriteEnable->isChecked());
            }

			//PHYSICS
			if (event.GUIEvent.Caller == penablePhysics) {
				SData *sdatat = (SData*)devices->getCoreData()->getISDataOfSceneNode(nodeToEdit);
				sdatat->setEnablePhysics(penablePhysics->isChecked());
				if (!penablePhysics->isChecked()) {
					if (sdatat->getBodyType() == ISData::EIPT_LIQUID_BODY)
						devices->getBulletWorld()->removeLiquidBody((ILiquidBody*)sdatat->getBodyPtr());
					else
						devices->getBulletWorld()->removeCollisionObject((ICollisionObject*)sdatat->getBodyPtr(), false);
					sdatat->setBodyType(ISData::EIPT_NONE);
					sdatat->setPBodyPtr(0);
					pBodyType->setSelected(0);
				}
			}
        }
        
        if (event.GUIEvent.EventType == EGET_SPINBOX_CHANGED) {
            if (event.GUIEvent.Caller == ravelSpin) {
                core::list<IGUIElement *>::ConstIterator elements = flagsTab->getChildren().begin();
                for (; elements != flagsTab->getChildren().end(); ++elements) {
                    if ((*elements) != ravelSpin) {
                        
                        if (rsCurrentPos > ravelSpin->getValue()) {
                            int diff = ravelSpin->getValue();
                            if (diff == 0) {
                                diff = 1;
                            }
                            (*elements)->setRelativePosition(position2di((*elements)->getRelativePosition().UpperLeftCorner.X,
                                                                         (*elements)->getRelativePosition().UpperLeftCorner.Y-
                                                                         (diff*flagsTab->getRelativePosition().getHeight())
                                                                         ));
                        } else {
                            int diff = ravelSpin->getValue();
                            if (diff == ravelSpin->getMax() && rsCurrentPos == ravelSpin->getMax()) {
                                diff = 0;
                            }
                            (*elements)->setRelativePosition(position2di((*elements)->getRelativePosition().UpperLeftCorner.X,
                                                                         (*elements)->getRelativePosition().UpperLeftCorner.Y+
                                                                         (diff*flagsTab->getRelativePosition().getHeight())
                                                                         ));
                        }
                    }
                }
                rsCurrentPos = ravelSpin->getValue();
                //std::cout << "Current Position of the cursor : " << rsCurrentPos << std::endl; //OK FOR VERIFYING SPINBOX POSITION
            }
        }
        
        if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
            if (treat) {
                s32 id = event.GUIEvent.Caller->getID();
                switch (id) {
                    //MATERIAL CHANGED
                    case CXT_EDIT_WINDOW_EVENTS_MATERIALS: {
                        setMaterialTextures();
                    }
                        break;
                        
                    default:
                        break;
                }
            }
        }
        
        //COMBOX BOX CHANGED
        if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
            if (treat) {
                s32 id = event.GUIEvent.Caller->getID();
                switch (id) {
                    case CXT_EDIT_WINDOW_EVENTS_PATCH_SIZE:
                        ((ITerrainSceneNode *)nodeToEdit)->setLODOfPatch(0, 0);
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_GENERAL_MATERIAL_TYPE:
                        if (generalMaterialCB->getSelected() <= devices->getCore()->getNumberOfBuildInMaterialTypes()-1) {
							#ifdef _IRR_OSX_PLATFORM_
                            if (generalMaterialCB->getSelected() == EMT_NORMAL_MAP_SOLID)
                                nodeToEdit->setMaterialType((E_MATERIAL_TYPE)devices->getNormalMappingMaterial()->getMaterialSolid());
                            else
							#endif
                            nodeToEdit->setMaterialType(getMaterialType(generalMaterialCB->getSelected()));
							lastMaterialType = getMaterialType(generalMaterialCB->getSelected());
                        } else {
                            nodeToEdit->setMaterialType((E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](generalMaterialCB->getSelected() - devices->getCore()->getNumberOfBuildInMaterialTypes())->getMaterial());
							lastMaterialType = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](generalMaterialCB->getSelected() - devices->getCore()->getNumberOfBuildInMaterialTypes())->getMaterial();
                        }

                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_MATERIAL_TYPE:
                        //nodeToEdit->getMaterial(materialsBar->getPos()).MaterialType = getMaterialType(materialType->getSelected());
                        if (materialType->getSelected() <= devices->getCore()->getNumberOfBuildInMaterialTypes()-1) {
                            nodeToEdit->getMaterial(materialsBar->getPos()).MaterialType = getMaterialType(materialType->getSelected());
							lastMaterialType = getMaterialType(materialType->getSelected());
                        } else {
                            nodeToEdit->getMaterial(materialsBar->getPos()).MaterialType = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](materialType->getSelected() - devices->getCore()->getNumberOfBuildInMaterialTypes())->getMaterial();
							lastMaterialType = (E_MATERIAL_TYPE)devices->getCoreData()->getShaderCallbacks()->operator[](materialType->getSelected() - devices->getCore()->getNumberOfBuildInMaterialTypes())->getMaterial();
                        }
                        break;
                        
                    case CXT_EDIT_WINDOW_EVENTS_GENERAL_SHADOWS:
                        devices->getXEffect()->removeShadowFromNode(nodeToEdit);
                        switch (shadowed->getSelected()) {
                            case 0:
                                devices->getXEffect()->addShadowToNode(nodeToEdit, devices->getXEffectFilterType(), ESM_CAST);
                                break;
                            case 1:
                                devices->getXEffect()->addShadowToNode(nodeToEdit, devices->getXEffectFilterType(), ESM_RECEIVE);
                                break;
                            case 2:
                                devices->getXEffect()->addShadowToNode(nodeToEdit, devices->getXEffectFilterType(), ESM_BOTH);
                                break;
                            case 3:
                                devices->getXEffect()->addShadowToNode(nodeToEdit, devices->getXEffectFilterType(), ESM_EXCLUDE);
                                break;
                                
                            default:
                                break;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
			if (event.GUIEvent.Caller == chooseSavedAnimation) {
				if (drawAnimations->isChecked()) {
					IAnimatedMeshSceneNode *animatedNode = (IAnimatedMeshSceneNode *)nodeToEdit;
					u32 i = devices->getCoreData()->getObjectNodeIndice(nodeToEdit);
					if (i != -1) {
						/*animatedNode->setFrameLoop(devices->getCoreData()->getObjectsData()->operator[](i).getActions()->operator[](chooseSavedAnimation->getSelected())->getStart(),
												   devices->getCoreData()->getObjectsData()->operator[](i).getActions()->operator[](chooseSavedAnimation->getSelected())->getEnd());
						animatedNode->setAnimationSpeed(devices->getCoreData()->getObjectsData()->operator[](i).getActions()->operator[](chooseSavedAnimation->getSelected())->getAnimSpeed());*/
						devices->getAnimationController()->applyAnimationToModel(animatedNode, chooseSavedAnimation->getSelected());
					}
				}
			}
			//PHYSICS
			if (event.GUIEvent.Caller == pBodyType) {
				pMasseb->setEnabled(true);
				SData *sdatat = (SData*)devices->getCoreData()->getISDataOfSceneNode(nodeToEdit);
				if (sdatat->getBodyType() != ISData::EIPT_NONE) {
					//REMOVE HERE
					if (sdatat->getType() == ISData::EIPT_RIGID_BODY) {
						devices->getBulletWorld()->removeCollisionObject((IRigidBody*)sdatat->getBodyPtr(), false);
					} else if (sdatat->getType() == ISData::EIPT_SOFT_BODY) {
						devices->getBulletWorld()->removeCollisionObject((ISoftBody*)sdatat->getBodyPtr(), false);
					} else if (sdatat->getType() == ISData::EIPT_LIQUID_BODY) {
						devices->getBulletWorld()->removeLiquidBody((ILiquidBody*)sdatat->getBodyPtr());
					}
				}
				if (pBodyType->getSelected() == ISData::EIPT_RIGID_BODY) {
					ICollisionShape *shape = 0;

					if (nodeToEdit->getType() == ESNT_MESH || nodeToEdit->getType() == ESNT_OCTREE) {
						shape = new IBvhTriangleMeshShape(nodeToEdit, ((IMeshSceneNode*)nodeToEdit)->getMesh(), 0.f);
					} else if (nodeToEdit->getType() == ESNT_CUBE || nodeToEdit->getType() == ESNT_ANIMATED_MESH) {
						shape = new IBoxShape(nodeToEdit, 0.f, false);
					} else if (nodeToEdit->getType() == ESNT_SPHERE) {
						shape = new ISphereShape(nodeToEdit, 0.f, false);
					} else if (nodeToEdit->getType() == ESNT_TERRAIN) {
						//NEEDS FIXES
						IMesh *mesh = ((ITerrainSceneNode*)nodeToEdit)->getMesh();
						CDynamicMeshBuffer *mb = new CDynamicMeshBuffer(EVT_2TCOORDS, EIT_32BIT);
						((ITerrainSceneNode*)nodeToEdit)->getMeshBufferForLOD(*mb);
						shape = new IBvhTriangleMeshShape((ITerrainSceneNode*)nodeToEdit, ((ITerrainSceneNode*)nodeToEdit)->getMesh(), 0.f);
					}

					if (shape != 0) {
						IRigidBody *rbody = devices->getBulletWorld()->addRigidBody(shape);
						rbody->forceActivationState(EAS_DISABLE_DEACTIVATION);
						rbody->setActivationState(EAS_DISABLE_DEACTIVATION);

						sdatat->setBodyType(ISData::EIPT_RIGID_BODY);
						sdatat->setPBodyPtr(rbody);
					}
				}
				if (pBodyType->getSelected() == ISData::EIPT_LIQUID_BODY) {
					aabbox3d<f32> box = nodeToEdit->getBoundingBox();
					ILiquidBody *lbody = devices->getBulletWorld()->addLiquidBody(vector3df(-5000,0,5000), 
																				  aabbox3df(0, -10000, 0, 10000, 0, 10000),
																				  500.0f, 200.0f);

					lbody->setCurrentDirection(vector3df(0,0,0));
					lbody->setGlobalWaveChangeIncrement(0.01f);
					lbody->setGlobalWaveUpdateFrequency(1.0f);
					lbody->setMaxGlobalWaveHeight(4.0f);
					lbody->setMinGlobalWaveHeight(-1.0f);
					lbody->setLocalWaveValues(10,1,0.5f);
					lbody->setInfinite(true);
					lbody->setInfiniteDepth(true);
					lbody->setLiquidDensity(0.1f);

					sdatat->setBodyType(ISData::EIPT_LIQUID_BODY);
					sdatat->setPBodyPtr(lbody);
					pMasseb->setEnabled(false);
				}
				if (pBodyType->getSelected() == ISData::EIPT_SOFT_BODY) {
					if (nodeToEdit->getType() == ESNT_MESH || nodeToEdit->getType() == ESNT_OCTREE || nodeToEdit->getType() == ESNT_CUBE) {
						ISoftBody *sbody = devices->getBulletWorld()->addSoftBody((IMeshSceneNode*)nodeToEdit);
						sdatat->setBodyType(ISData::EIPT_SOFT_BODY);
						sdatat->setPBodyPtr(sbody);
					}
				}
				if (pBodyType->getSelected() == ISData::EIPT_NONE) {
					sdatat->setBodyType(ISData::EIPT_NONE);
					sdatat->setPBodyPtr(0);
				}
			}
        }
        
        //TEXTURE LAYER SELECTED
        if (event.GUIEvent.EventType == EGET_FILE_CHOOSE_DIALOG_CANCELLED) {
            currentBrowse = 0;
        }
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
			if (dialog == browseSavedAnimationDialog) {
				//DELETE OBJECT ANIMATIONS
				array<CAction *> actions;
				chooseSavedAnimation->clear();
				IrrXMLReader *xmlReader = createIrrXMLReader(stringc(dialog->getFileName()).c_str());
				if (xmlReader) {
					xmlReader->read();
					stringc element = xmlReader->getNodeName();
					if (element != "rootAnim") {
						do {
							xmlReader->read();
							element = xmlReader->getNodeName();
						} while (element != "rootAnim");
						do {
							xmlReader->read();
							element = xmlReader->getNodeName();
							if (element == "action") {
								CAction *action = new CAction();
								action->setStart(xmlReader->getAttributeValueAsInt("start"));
								action->setEnd(xmlReader->getAttributeValueAsInt("end"));
								action->setName(xmlReader->getAttributeValue("name"));
								action->setAnimSpeed(xmlReader->getAttributeValueAsFloat("speed"));
								action->setAnimationPath(xmlReader->getAttributeValue("animPath"));
								actions.push_back(action);
								chooseSavedAnimation->addItem(stringw(action->getName().c_str()).c_str());
							}
						} while (element != "rootAnim");
					}
				}
				delete xmlReader;

				if (nodeToEditPrefix == "#object") {
					u32 i = devices->getCoreData()->getObjectNodeIndice(nodeToEdit);
					if (i != -1) {
						devices->getCoreData()->getObjectsData()->operator[](i).setActions(&actions);
					}
				}

				stringw pathSavedAnimation = browseSavedAnimationDialog->getFileName();
				pathSavedAnimation.remove(devices->getWorkingDirectory().c_str());
				savedAnimationsPath->setText(pathSavedAnimation.c_str());
			}
            
            switch (currentBrowse) {
                case 1:
                    ebTextureLayerPath1->setText(dialog->getFileName());
                    break;
                    
                case 2:
                    ebTextureLayerPath2->setText(dialog->getFileName());
                    break;
                    
                case 3:
                    ebTextureLayerPath3->setText(dialog->getFileName());
                    break;
                    
                case 4:
                    ebTextureLayerPath3->setText(dialog->getFileName());
                    break;
                    
                case 11: {
                    ITexture *texture;
                    stringw texture_c = "";
                    texture_c += dialog->getFileName();
                    texture = devices->getVideoDriver()->getTexture(texture_c.c_str());
                    if (texture) {
                        mTexture1->setText(texture_c.c_str());
                        nodeToEdit->getMaterial(materialsBar->getPos()).setTexture(0, texture);
                    } else {
                        devices->addErrorDialog(L"Load Texture Error", 
                                                L"Cannot load the texture, maybe a problem ?\n\n"
                                                L"Verify your texture or contact me...", EMBF_OK);
                    }
                }
                    break;
                    
                case 12: {
                    ITexture *texture;
                    stringw texture_c = "";
                    texture_c += dialog->getFileName();
                    texture = devices->getVideoDriver()->getTexture(texture_c.c_str());
                    if (texture) {
                        mTexture2->setText(texture_c.c_str());
                        nodeToEdit->getMaterial(materialsBar->getPos()).setTexture(1, texture);
                    } else {
                        devices->addErrorDialog(L"Load Texture Error", 
                                                L"Cannot load the texture, maybe a problem ?\n\n"
                                                L"Verify your texture or contact me...", EMBF_OK);
                    }
                }
                    break;
                    
                case 13: {
                    ITexture *texture;
                    stringw texture_c = "";
                    texture_c += dialog->getFileName();
                    texture = devices->getVideoDriver()->getTexture(texture_c.c_str());
                    if (texture) {
                        mTexture3->setText(texture_c.c_str());
                        nodeToEdit->getMaterial(materialsBar->getPos()).setTexture(2, texture);
                    } else {
                        devices->addErrorDialog(L"Load Texture Error", 
                                                L"Cannot load the texture, maybe a problem ?\n\n"
                                                L"Verify your texture or contact me...", EMBF_OK);
                    }
                }
                    break;
                    
                case 14: {
                    ITexture *texture;
                    stringw texture_c = "";
                    texture_c += dialog->getFileName();
                    texture = devices->getVideoDriver()->getTexture(texture_c.c_str());
                    if (texture) {
                        mTexture4->setText(texture_c.c_str());
                        nodeToEdit->getMaterial(materialsBar->getPos()).setTexture(3, texture);
                    } else {
                        devices->addErrorDialog(L"Load Texture Error", 
                                                L"Cannot load the texture, maybe a problem ?\n\n"
                                                L"Verify your texture or contact me...", EMBF_OK);
                    }
                }
                    break;
                    
                default:
                    break;
            }
            
            currentBrowse = 0;
        }
    }
    
    return false;
}
