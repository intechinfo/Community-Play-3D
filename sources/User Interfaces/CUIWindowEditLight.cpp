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
    
    currentBrowse = -1;
}

CUIWindowEditLight::~CUIWindowEditLight() {
    
}

void CUIWindowEditLight::open(ISceneNode *node, stringw prefix) {
    nodeToEdit = (ILightSceneNode *)node;
    
    if (nodeToEdit == 0) {
        devices->getGUIEnvironment()->addMessageBox(L"Warning", 
                                                    L"No node to edit, maybe an intern error or your node is out \n\n"
                                                    L"Please try again");
    } else {
        nodeToEditPrefix.append(prefix);
        
        nodeToEdit->setDebugDataVisible(EDS_FULL);
        
        editWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(devices->getVideoDriver()->getScreenSize().Width/2-200, 
                                                                       100, devices->getVideoDriver()->getScreenSize().Width/2+300, 570),
                                                             false, L"Light Edition Window", 0, -1);
        tabCtrl = devices->getGUIEnvironment()->addTabControl(rect<int>(5, 20, 495, 420), editWindow, true, true, -1);
        generalTab = tabCtrl->addTab(L"General");
        advancedTab = tabCtrl->addTab(L"Advanced");
        lensFlareTab = tabCtrl->addTab(L"Lens Flare");
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
        
        //LENS FLARE TAB
        lensFlare = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 10, 110, 30), lensFlareTab, -1, L"Lens Flare");
        if (devices->getCoreData()->getLensFlareSceneNodes()->operator[](index)) {
            lensFlare->setChecked(true);
        } else {
            lensFlare->setChecked(false);
        }
        
        devices->getGUIEnvironment()->addStaticText(L"Strength : ", rect<s32>(10, 40, 80, 60), true, true, lensFlareTab, -1, true);
        lfStrength = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(80, 40, 220, 60), true, lensFlareTab, -1);
        
        //LENS FLARE TAB : SPHERE
        IGUIStaticText *sphereText = devices->getGUIEnvironment()->addStaticText(L"Sphere", rect<s32>(400, 70, 480, 140), true, true, lensFlareTab, -1, true);
        sphereText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
        sphereText->setDrawBackground(true);
        
        devices->getGUIEnvironment()->addStaticText(L"Scale : ", rect<s32>(10, 70, 60, 90), true, true, lensFlareTab, -1, true);
        
        sphereXYZ = devices->getGUIEnvironment()->addSpinBox(L"", rect<s32>(50, 70, 100, 90), true, lensFlareTab, -1);
        sphereXYZ->setRange(-10*pow(10, 20), 10*pow(10, 20));
        sphereXYZ->setStepSize(1);
        
        devices->getGUIEnvironment()->addStaticText(L"X : ", rect<s32>(10, 90, 30, 110), true, true, lensFlareTab, -1, true);
        sphereX = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(30, 90, 130, 110), true, lensFlareTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"Y : ", rect<s32>(140, 90, 160, 110), true, true, lensFlareTab, -1, true);
        sphereY = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(160, 90, 260, 110), true, lensFlareTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"Z : ", rect<s32>(270, 90, 290, 110), true, true, lensFlareTab, -1, true);
        sphereZ = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(290, 90, 390, 110), true, lensFlareTab, -1);
        
        devices->getGUIEnvironment()->addStaticText(L"Texture : ", rect<s32>(10, 120, 70, 140), true, true, lensFlareTab, -1, true);
        sphereTexture = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(70, 120, 310, 140), true, lensFlareTab, -1);
        sphereTextureBrowse = devices->getGUIEnvironment()->addButton(rect<s32>(320, 120, 390, 140), lensFlareTab, -1, L"Browse...", L"Browse...");
        
        //LENS FLARE TAB : BILLBOARD
        sphereText = devices->getGUIEnvironment()->addStaticText(L"BillBoard", rect<s32>(400, 160, 480, 230), true, true, lensFlareTab, -1, true);
        sphereText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
        
        devices->getGUIEnvironment()->addStaticText(L"Dimensions : ", rect<s32>(10, 160, 90, 180), true, true, lensFlareTab, -1, true);
        devices->getGUIEnvironment()->addStaticText(L"W : ", rect<s32>(10, 180, 30, 200), true, true, lensFlareTab, -1, true);
        bbW = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(30, 180, 130, 200), true, lensFlareTab, -1);
        devices->getGUIEnvironment()->addStaticText(L"H : ", rect<s32>(140, 180, 160, 200), true, true, lensFlareTab, -1, true);
        bbH = devices->getGUIEnvironment()->addEditBox(L"0", rect<s32>(160, 180, 260, 200), true, lensFlareTab, -1);
        bbWH = devices->getGUIEnvironment()->addSpinBox(L"", rect<s32>(260, 180, 310, 200), true, lensFlareTab, -1);
        bbWH->setRange(-10*pow(10, 20), 10*pow(10, 20));
        bbWH->setStepSize(1);
        
        devices->getGUIEnvironment()->addStaticText(L"Texture : ", rect<s32>(10, 210, 70, 230), true, true, lensFlareTab, -1, true);
        bbTexture = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(70, 210, 310, 230), true, lensFlareTab, -1);
        bbTextureBrowse = devices->getGUIEnvironment()->addButton(rect<s32>(320, 210, 390, 230), lensFlareTab, -1, L"Browse...", L"Browse...");
        
        //LENS FLARE TAB : LENS FLARE NODE
        sphereText = devices->getGUIEnvironment()->addStaticText(L"LF Node", rect<s32>(400, 250, 480, 330), true, true, lensFlareTab, -1, true);
        sphereText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
        
        devices->getGUIEnvironment()->addStaticText(L"Texture : ", rect<s32>(10, 250, 70, 270), true, true, lensFlareTab, -1, true);
        lfnTexture = devices->getGUIEnvironment()->addEditBox(L"", rect<s32>(70, 250, 310, 270), true, lensFlareTab, -1);
        lfnTextureBrowse = devices->getGUIEnvironment()->addButton(rect<s32>(320, 250, 390, 270), lensFlareTab, -1, L"Browse...", L"Browse...");
        
        devices->getGUIEnvironment()->addStaticText(L"Position : ", rect<s32>(10, 280, 80, 300), true, true, lensFlareTab, -1, true);
        lfnCursorPosition = devices->getGUIEnvironment()->addButton(rect<s32>(80, 280, 190, 300), lensFlareTab, -1, L"Cursor Position", L"To cursor absolute position");
        lfn0Position = devices->getGUIEnvironment()->addButton(rect<s32>(200, 280, 310, 300), lensFlareTab, -1, L"(0, 0, 0) Position", L"To vector (0, 0, 0) position");
        lfnArrowMeshPosition = devices->getGUIEnvironment()->addButton(rect<s32>(80, 310, 190, 330), lensFlareTab, -1, L"To Arrow Mesh", L"To Arrow Mesh Object Placement");
        
        //LENS FLARE TAB : CHECK IF CHECKBOX IS CHECKED
        if (!lensFlare->isChecked()) {
            core::list<IGUIElement *>::ConstIterator element = lensFlareTab->getChildren().begin();
            element++;
            for (; element != lensFlareTab->getChildren().end(); ++element) {
                (*element)->setEnabled(false);
            }
        }
        
        //WINDOW BUTTONS
        applyButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 430, 80, 460), editWindow, CXT_EDIT_LIGHT_WINDOW_EVENTS_APPLY_BUTTON, 
                                                              L"Apply", L"Apply the settings");
        closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(100, 430, 175, 460), editWindow, CXT_EDIT_LIGHT_WINDOW_EVENTS_CLOSE_BUTTON, 
                                                              L"Cancel", L"Close without effect");
        editWindow->getCloseButton()->remove();
    }
}

bool CUIWindowEditLight::OnEvent(const SEvent &event) {
    
    if (lensFlare->isChecked()) {
        if (devices->getCoreData()->getLfMeshNodes()->operator[](index)) {
            sphereX->setText(devices->getCore()->getStrNumber(devices->getCoreData()->getLfMeshNodes()->operator[](index)->getScale().X).c_str());
            sphereY->setText(devices->getCore()->getStrNumber(devices->getCoreData()->getLfMeshNodes()->operator[](index)->getScale().Y).c_str());
            sphereZ->setText(devices->getCore()->getStrNumber(devices->getCoreData()->getLfMeshNodes()->operator[](index)->getScale().Z).c_str());
        }
        if (devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index)) {
            bbW->setText(devices->getCore()->getStrNumber(devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index)->getSize().Width).c_str());
            bbH->setText(devices->getCore()->getStrNumber(devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index)->getSize().Height).c_str());
        }
    }
    
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
            
            if (event.GUIEvent.Caller == sphereTextureBrowse) {
                currentBrowse = 1;
                devices->createFileOpenDialog(L"Choose sphere texture");
            }
            if (event.GUIEvent.Caller == bbTextureBrowse) {
                currentBrowse = 2;
                devices->createFileOpenDialog(L"Choose billboard texture");
            }
            if (event.GUIEvent.Caller == lfnTextureBrowse) {
                currentBrowse = 3;
                devices->createFileOpenDialog(L"Choose Lens Flare texture");
            }
            
            if (event.GUIEvent.Caller == lfnCursorPosition) {
                IMeshSceneNode *lfNode = devices->getCoreData()->getLfMeshNodes()->operator[](index);
                //SET LENS FLARE NODE TO THE ABSOLUTE CURSOR POSITION
                matrix4 matr = devices->getCursor()->getAbsoluteTransformation();
                const matrix4 w2n(lfNode->getParent()->getAbsoluteTransformation(), matrix4::EM4CONST_INVERSE);
                matr = (w2n*matr);
                lfNode->setPosition(matr.getTranslation());
                lfNode->setRotation(matr.getRotationDegrees());
                lfNode->updateAbsolutePosition();
            }
            
            if (event.GUIEvent.Caller == lfn0Position) {
                IMeshSceneNode *lfNode = devices->getCoreData()->getLfMeshNodes()->operator[](index);
                lfNode->setPosition(vector3df(0));
            }
            
            if (event.GUIEvent.Caller == lfnArrowMeshPosition) {
                devices->getObjectPlacement()->setNodeToPlace(devices->getCoreData()->getLfMeshNodes()->operator[](index));
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
        
        if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
            //LENS FLARE CHECKBOX
            if (event.GUIEvent.Caller == lensFlare) {
                if (lensFlare->isChecked()) {
                    IMeshSceneNode* meshNode = devices->getSceneManager()->addSphereSceneNode(1, 16, devices->getSceneManager()->getRootSceneNode());
                    meshNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
                    meshNode->setMaterialFlag(EMF_LIGHTING, false);
                    meshNode->setScale(vector3d<f32>(0, 0, 0));
                    meshNode->setParent(nodeToEdit);
                    devices->getCoreData()->getLfMeshNodes()->operator[](index) = meshNode;
                    
                    IBillboardSceneNode* bill = devices->getSceneManager()->addBillboardSceneNode(meshNode);
                    bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
                    bill->setMaterialFlag(EMF_LIGHTING, false);
                    bill->setSize(dimension2d<f32>(0, 0));
                    bill->setParent(meshNode);
                    devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index) = bill;
                    
                    CLensFlareSceneNode* lensFlareNode = new CLensFlareSceneNode(meshNode, devices->getSceneManager());
                    lensFlareNode->setMaterialTexture(0, devices->getVideoDriver()->getTexture("shaders/Textures/LF/flare.png"));
                    lensFlareNode->setFalseOcclusion(true);
                    lensFlareNode->setParent(meshNode);
                    devices->getCoreData()->getLensFlareSceneNodes()->operator[](index) = lensFlareNode;
                    
                    core::list<IGUIElement *>::ConstIterator element = lensFlareTab->getChildren().begin();
                    element++;
                    for (; element != lensFlareTab->getChildren().end(); ++element) {
                        (*element)->setEnabled(true);
                    }
                    
                } else {
                    devices->getCoreData()->getLfMeshNodes()->operator[](index)->remove();
                    devices->getCoreData()->getLfMeshNodes()->operator[](index) = 0;
                    
                    devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index)->remove();
                    devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index) = 0;
                    
                    devices->getCoreData()->getLensFlareSceneNodes()->operator[](index)->remove();
                    devices->getCoreData()->getLensFlareSceneNodes()->operator[](index) = 0;
                    
                    core::list<IGUIElement *>::ConstIterator element = lensFlareTab->getChildren().begin();
                    element++;
                    for (; element != lensFlareTab->getChildren().end(); ++element) {
                        (*element)->setEnabled(false);
                    }
                }
            }
        }
        
        if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
            //LENS FLARE STRENGTH
            if (event.GUIEvent.Caller == lfStrength) {
                stringc strength_c = lfStrength->getText();
                f32 strength = devices->getCore()->getF32(strength_c.c_str());
                devices->getCoreData()->getLensFlareSceneNodes()->operator[](index)->setStrength(strength);
            }
            
            //LENS FLARE SPHERE SCALE
            if (event.GUIEvent.Caller == sphereX) {
                stringc scalex_c = sphereX->getText();
                f32 scalex = devices->getCore()->getF32(scalex_c.c_str());
                IMeshSceneNode *node = devices->getCoreData()->getLfMeshNodes()->operator[](index);
                node->setScale(vector3df(scalex, node->getScale().Y, node->getScale().Z));
            }
            
            if (event.GUIEvent.Caller == sphereY) {
                stringc scaley_c = sphereY->getText();
                f32 scaley = devices->getCore()->getF32(scaley_c.c_str());
                IMeshSceneNode *node = devices->getCoreData()->getLfMeshNodes()->operator[](index);
                node->setScale(vector3df(node->getScale().X, scaley, node->getScale().Z));
            }
            
            if (event.GUIEvent.Caller == sphereZ) {
                stringc scalez_c = sphereZ->getText();
                f32 scalez = devices->getCore()->getF32(scalez_c.c_str());
                IMeshSceneNode *node = devices->getCoreData()->getLfMeshNodes()->operator[](index);
                node->setScale(vector3df(node->getScale().X, node->getScale().Y, scalez));
            }
            
            //LENS FLARE DIMENSIONS BB
            if (event.GUIEvent.Caller == bbW) {
                stringc widthBB_c = bbW->getText();
                f32 widthBB = devices->getCore()->getF32(widthBB_c.c_str());
                IBillboardSceneNode *node = devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index);
                node->setSize(dimension2d<f32>(widthBB, node->getSize().Height));
            }
            
            if (event.GUIEvent.Caller == bbH) {
                stringc heightBB_c = bbH->getText();
                f32 heightBB = devices->getCore()->getF32(heightBB_c.c_str());
                IBillboardSceneNode *node = devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index);
                node->setSize(dimension2d<f32>(node->getSize().Width, heightBB));
            }
        }
        
        if (event.GUIEvent.EventType == EGET_SPINBOX_CHANGED) {
            if (event.GUIEvent.Caller == bbWH) {
                IBillboardSceneNode *bbNode = devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index);
                bbNode->setSize(dimension2d<f32>(bbNode->getSize().Width+bbWH->getValue(), bbNode->getSize().Height+bbWH->getValue()));
                bbWH->setValue(0);
            }
            
            if (event.GUIEvent.Caller == sphereXYZ) {
                IMeshSceneNode *sphereNode = devices->getCoreData()->getLfMeshNodes()->operator[](index);
                sphereNode->setScale(vector3df(sphereNode->getScale().X+sphereXYZ->getValue(), sphereNode->getScale().Y+sphereXYZ->getValue(), 
                                               sphereNode->getScale().Z+sphereXYZ->getValue()));
                sphereXYZ->setValue(0);
            }
        }
        
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            IGUIFileOpenDialog *dialog = ((IGUIFileOpenDialog *)event.GUIEvent.Caller);
            
            //LENS FLARE NODE
            if (currentBrowse == 1) {
                stringw path = dialog->getFileName();
                devices->getCoreData()->getLfMeshNodes()->operator[](index)->setMaterialTexture(0, devices->getVideoDriver()->getTexture(path.c_str()));
                sphereTexture->setText(path.c_str());
            }
            
            if (currentBrowse == 2) {
                stringw path = dialog->getFileName();
                devices->getCoreData()->getLfBillBoardSceneNodes()->operator[](index)->setMaterialTexture(0, devices->getVideoDriver()->getTexture(path.c_str()));
                bbTexture->setText(path.c_str());
            }
            
            if (currentBrowse == 3) {
                stringw path = dialog->getFileName();
                devices->getCoreData()->getLensFlareSceneNodes()->operator[](index)->setMaterialTexture(0, devices->getVideoDriver()->getTexture(path.c_str()));
                lfnTexture->setText(path.c_str());
            }
        }
    }
    
    return false;
}
