//
//  CUIWindowEditNode.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 29/09/12.
//
//

#include "CUIWindowEditLight.h"

CUIWindowEditLight::CUIWindowEditLight(CDevices *_devices, s32 _index) {
    devices = _devices;
    devices->getEventReceiver()->AddEventReceiver(this);
    
    index = _index;
    
    nodeToEdit = 0;
    nodeToEditPrefix = L"";
}

CUIWindowEditLight::~CUIWindowEditLight() {
    
}

void CUIWindowEditLight::open(ISceneNode *node, stringw prefix) {
    nodeToEdit = (ILightSceneNode *) node;
    
    if (nodeToEdit == 0) {
        devices->getGUIEnvironment()->addMessageBox(L"Warning", 
                                                    L"No node to edit, maybe an intern error or your node is out \n\n"
                                                    L"Please try again");
    } else {
        nodeToEditPrefix.append(prefix);
        
        nodeToEdit->setDebugDataVisible(EDS_FULL);
        
        editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-200, 
                                                                       100, devices->getVideoDriver()->getScreenSize().Width/2+200, 570),
                                                             false, L"Light Edition Window", 0, -1);
        tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(5, 20, 395, 420), editWindow, true, true, -1);
        generalTab = tabCtrl->addTab(L"General");
        advancedTab = tabCtrl->addTab(L"Advanced");
        tabCtrl->setActiveTab(generalTab);
        
        //NAME
        stringw nodeName = nodeToEdit->getName();
        devices->getGUIEnvironment()->addStaticText(L"Name : ", rect<s32>(10, 5, 60, 25), true, true, generalTab, -1, true);
        ebNodeName = devices->getGUIEnvironment()->addEditBox(nodeName.c_str(), rect<s32>(70, 5, 370, 25), true, generalTab, -1);
        
        //POSITION
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 30, 175, 50), generalTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_GENERAL_POSITION, 
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
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 90, 175, 110), generalTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_GENERAL_TARGET, 
                                                L"Cursor Position", L"Set target to the cursor position");
        devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(10, 110, 30, 130), true, true, generalTab, -1, true);
        ebNodeTargetX = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRotation().X).c_str(), 
                                                                   rect<s32>(30, 110, 130, 130), true, generalTab, -10);
        devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(130, 110, 150, 130), true, true, generalTab, -1, true);
        ebNodeTargetY = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRotation().Y).c_str(), 
                                                                   rect<s32>(150, 110, 250, 130), true, generalTab, -10);
        devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(250, 110, 270, 130), true, true, generalTab, -1, true);
        ebNodeTargetZ = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRotation().Z).c_str(), 
                                                                   rect<s32>(270, 110, 370, 130), true, generalTab, -10);
        
        //RADIUS
        devices->getGUIEnvironment()->addStaticText(L"Radius : ", rect<s32>(10, 140, 70, 160), true, true, generalTab, -1, true);
        ebNodeRadius = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getRadius()).c_str(),
                                                                rect<s32>(75, 140, 175, 160), true, generalTab, -10);
        
        //ADVANCED RESOLUTION
        devices->getGUIEnvironment()->addStaticText(L"Resolution : ", rect<s32>(10, 5, 90, 25), true, true, advancedTab, -1, true);
        resolutionComboBox = devices->getGUIEnvironment()->addComboBox(rect<s32>(95, 5, 195, 25), advancedTab, 
                                                                       CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_RESOLUTION);
        resolutionComboBox->addItem(L"128");
        resolutionComboBox->addItem(L"256");
        resolutionComboBox->addItem(L"512");
        resolutionComboBox->addItem(L"1024");
        resolutionComboBox->addItem(L"2048");
        resolutionComboBox->addItem(L"4096");
        
        //ADVANCED DIFFUSE COLOR
        devices->getGUIEnvironment()->addStaticText(L"Diffuse :", rect<s32>(10, 30, 70, 50), true, true, advancedTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 30, 175, 50), advancedTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_DIFFUSE_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 30, 205, 50), true, true, advancedTab, -1, true);
        ebNodeDiffuseColorA = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().DiffuseColor.a).c_str(), 
                                                                rect<s32>(210, 30, 310, 50), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 50, 30, 70), true, true, advancedTab, -1, true);
        ebNodeDiffuseColorR = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().DiffuseColor.r).c_str(), 
                                                                   rect<s32>(30, 50, 130, 70), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 50, 150, 70), true, true, advancedTab, -1, true);
        ebNodeDiffuseColorG = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().DiffuseColor.g).c_str(), 
                                                                   rect<s32>(150, 50, 250, 70), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 50, 270, 70), true, true, advancedTab, -1, true);
        ebNodeDiffuseColorB = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().DiffuseColor.r).c_str(), 
                                                                   rect<s32>(270, 50, 370, 70), true, advancedTab, -1);
        //ADVANCED AMBIANT COLOR
        devices->getGUIEnvironment()->addStaticText(L"Ambiant :", rect<s32>(10, 80, 70, 100), true, true, advancedTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 80, 175, 100), advancedTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_DIFFUSE_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 80, 205, 100), true, true, advancedTab, -1, true);
        ebNodeAmbiantColorA = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().AmbientColor.a).c_str(), 
                                                                       rect<s32>(210, 80, 310, 100), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 100, 30, 120), true, true, advancedTab, -1, true);
        ebNodeAmbiantColorR = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().AmbientColor.r).c_str(), 
                                                                       rect<s32>(30, 100, 130, 120), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 100, 150, 120), true, true, advancedTab, -1, true);
        ebNodeAmbiantColorG = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().AmbientColor.g).c_str(), 
                                                                       rect<s32>(150, 100, 250, 120), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 100, 270, 120), true, true, advancedTab, -1, true);
        ebNodeAmbiantColorB = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().AmbientColor.r).c_str(), 
                                                                       rect<s32>(270, 100, 370, 120), true, advancedTab, -1);
        //ADVANCED SPECULAR COLOR
        devices->getGUIEnvironment()->addStaticText(L"Specular :", rect<s32>(10, 130, 70, 150), true, true, advancedTab, -1, true);
        devices->getGUIEnvironment()->addButton(rect<s32>(75, 130, 175, 150), advancedTab, CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_DIFFUSE_COLOR, 
                                                L"Edit Color", L"Open Select Color Window");
        devices->getGUIEnvironment()->addStaticText(L"Al : ", rect<s32>(185, 130, 205, 150), true, true, advancedTab, -1, true);
        ebNodeSpecularColorA = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().SpecularColor.a).c_str(), 
                                                                       rect<s32>(210, 130, 310, 150), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"R : ", rect<s32>(10, 150, 30, 170), true, true, advancedTab, -1, true);
        ebNodeSpecularColorR = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().SpecularColor.r).c_str(), 
                                                                       rect<s32>(30, 150, 130, 170), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"G : ", rect<s32>(130, 150, 150, 170), true, true, advancedTab, -1, true);
        ebNodeSpecularColorG = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().SpecularColor.g).c_str(), 
                                                                       rect<s32>(150, 150, 250, 170), true, advancedTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"B : ", rect<s32>(250, 150, 270, 170), true, true, advancedTab, -1, true);
        ebNodeSpecularColorB = devices->getGUIEnvironment()->addEditBox(devices->getCore()->getStrNumber(nodeToEdit->getLightData().SpecularColor.r).c_str(), 
                                                                       rect<s32>(270, 150, 370, 170), true, advancedTab, -1);
        
        //WINDOW BUTTONS
        applyButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 430, 80, 460), editWindow, CXT_EDIT_LIGHT_WINDOW_EVENTS_APPLY_BUTTON, 
                                                              L"Apply", L"Apply the settings");
        closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(100, 430, 175, 460), editWindow, CXT_EDIT_LIGHT_WINDOW_EVENTS_CLOSE_BUTTON, 
                                                              L"Cancel", L"Close without effect");
        editWindow->getCloseButton()->remove();
    }
}

bool CUIWindowEditLight::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                    
                case CXT_EDIT_LIGHT_WINDOW_EVENTS_GENERAL_POSITION:
                    ebNodePositionX->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().X).c_str());
                    ebNodePositionY->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().Y).c_str());
                    ebNodePositionZ->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().Z).c_str());
                    break;
                    
                case CXT_EDIT_LIGHT_WINDOW_EVENTS_GENERAL_TARGET:
                    ebNodeTargetX->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().X).c_str());
                    ebNodeTargetY->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().Y).c_str());
                    ebNodeTargetZ->setText(devices->getCore()->getStrNumber(devices->getCursorPosition().Z).c_str());
                    break;
                    
                case CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_DIFFUSE_COLOR:
                    devices->getGUIEnvironment()->addColorSelectDialog(L"Color Edition", false, 0, -1);
                    break;
                    
                case CXT_EDIT_LIGHT_WINDOW_EVENTS_APPLY_BUTTON:
                    //GENERAL
                    nodeToEdit->setName(ebNodeName->getText());
                    nodeToEdit->setPosition(devices->getCore()->getVector3df(devices->getCore()->convertToString(ebNodePositionX->getText()), 
                                                                             devices->getCore()->convertToString(ebNodePositionY->getText()), 
                                                                             devices->getCore()->convertToString(ebNodePositionZ->getText())));
                    nodeToEdit->setRotation(devices->getCore()->getVector3df(devices->getCore()->convertToString(ebNodeTargetX->getText()), 
                                                                             devices->getCore()->convertToString(ebNodeTargetY->getText()), 
                                                                             devices->getCore()->convertToString(ebNodeTargetZ->getText())));
                    nodeToEdit->setRadius(devices->getCore()->getF32(devices->getCore()->convertToString(ebNodeRadius->getText())));
                    
                    //ADVANCED DIFFUSE
                    nodeToEdit->getLightData().DiffuseColor.r = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeDiffuseColorR->getText()));
                    nodeToEdit->getLightData().DiffuseColor.g = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeDiffuseColorG->getText()));
                    nodeToEdit->getLightData().DiffuseColor.b = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeDiffuseColorB->getText()));
                    nodeToEdit->getLightData().DiffuseColor.a = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeDiffuseColorA->getText()));
                    //ADVANCED AMBIANT
                    nodeToEdit->getLightData().AmbientColor.r = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeAmbiantColorR->getText()));
                    nodeToEdit->getLightData().AmbientColor.g = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeAmbiantColorG->getText()));
                    nodeToEdit->getLightData().AmbientColor.b = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeAmbiantColorB->getText()));
                    nodeToEdit->getLightData().AmbientColor.a = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeAmbiantColorA->getText()));
                    //ADVANCED SPECULAR
                    nodeToEdit->getLightData().SpecularColor.r = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeSpecularColorR->getText()));
                    nodeToEdit->getLightData().SpecularColor.g = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeSpecularColorG->getText()));
                    nodeToEdit->getLightData().SpecularColor.b = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeSpecularColorB->getText()));
                    nodeToEdit->getLightData().SpecularColor.a = devices->getCore()->getF32(
                                                                devices->getCore()->convertToString(ebNodeSpecularColorA->getText()));
                    
                    break;
                    
                case CXT_EDIT_LIGHT_WINDOW_EVENTS_CLOSE_BUTTON:
                    nodeToEdit->setDebugDataVisible(EDS_OFF);
                    editWindow->remove();
                    devices->getEventReceiver()->RemoveEventReceiver(this);
                    delete this;
                    break;
                    
                default:
                    break;
            }
        }
        
        if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
            if (event.GUIEvent.Caller->getID() == CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_RESOLUTION) {
                
                stringc resolutionw = resolutionComboBox->getItem(resolutionComboBox->getSelected());
                u32 resolution = devices->getCore()->getU32(resolutionw.c_str());
                devices->getXEffect()->getShadowLight(index).setShadowMapResolution(resolution);
                
                if (resolutionw == "4096") {
                    devices->addWarningDialog(L"Warning", 
                                              L"This quality of shadows can make the World Editor CRASHING !\n\n"
                                              L"I cannot make sure the render will be successful...",
                                              EMBF_OK);
                }
            }
        }
    }
    
    return false;
}
