//
//  CUIMaterialEditorGeneral.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//
#include "stdafx.h"
#include "CUIMaterialEditorTextures.h"

CUIMaterialEditorTextures::CUIMaterialEditorTextures(CDevices *_devices, IGUIElement *_parent, ISceneNode *node) {
    //-------------------------------------------------------------------------------------------------------
	//DEVICES DATAS
    devices = _devices;
    
    driver = devices->getVideoDriver();
    smgr = devices->getSceneManager();
    gui = devices->getGUIEnvironment();
    
    nodeToEdit = node;
    parent = _parent;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    materialToEdit = 0;
    //---------------------------------------------------------------------------------------------------------
    
    //---------------------------------------------------------------------------------------------------------
	//GENERAL GUI ELEMENTS
    gui->addStaticText(L"", rect<s32>(9, 16, 789, 307), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));
    
    gui->addStaticText(L"", rect<s32>(9, 16, 359, 27), true, true, parent, -1, false);
    IGUIStaticText *textureText = gui->addStaticText(L"General", rect<s32>(359, 6, 439, 36), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    gui->addStaticText(L"", rect<s32>(439, 16, 789, 27), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(9, 26, 19, 296), true, true, parent, -1, false);
    
    gui->addStaticText(L"Texture 1 :", rect<s32>(29, 36, 109, 56), true, true, parent, -1, false);
    gtex1 = gui->addEditBox(devices->getCore()->getTexturePath(node->getMaterial(0).TextureLayer[0].Texture).c_str(), 
                            rect<s32>(109, 36, 289, 56), true, parent, -1);
    gltex1 = gui->addButton(rect<s32>(289, 36, 329, 56), parent, -1, L"...", L"Browse Texture...");
    gdtex1 = gui->addButton(rect<s32>(329, 36, 399, 56), parent, -1, L"Remove", L"Remove Texture From Material");
	gmntex1 = gui->addButton(rect<s32>(399, 36, 499, 56), parent, -1, L"Make Normal", L"Create a normal map texture from this");

    gui->addStaticText(L"Texture 2 :", rect<s32>(29, 66, 109, 86), true, true, parent, -1, false);
    gtex2 = gui->addEditBox(devices->getCore()->getTexturePath(node->getMaterial(0).TextureLayer[1].Texture).c_str(), 
                            rect<s32>(109, 66, 289, 86), true, parent, -1);
    gltex2 = gui->addButton(rect<s32>(289, 66, 329, 86), parent, -1, L"...", L"Browse Texture...");
    gdtex2 = gui->addButton(rect<s32>(329, 66, 399, 86), parent, -1, L"Remove", L"Remove Texture From Material");
	gmntex2 = gui->addButton(rect<s32>(399, 66, 499, 86), parent, -1, L"Make Normal", L"Create a normal map texture from this");
    
    gui->addStaticText(L"Texture 3 :", rect<s32>(29, 96, 109, 116), true, true, parent, -1, false);
    gtex3 = gui->addEditBox(devices->getCore()->getTexturePath(node->getMaterial(0).TextureLayer[2].Texture).c_str(), 
                            rect<s32>(109, 96, 289, 116), true, parent, -1);
    gltex3 = gui->addButton(rect<s32>(289, 96, 329, 116), parent, -1, L"...", L"Browse Texture...");
    gdtex3 = gui->addButton(rect<s32>(329, 96, 399, 116), parent, -1, L"Remove", L"Remove Texture From Material");
	gmntex3 = gui->addButton(rect<s32>(399, 96, 499, 116), parent, -1, L"Make Normal", L"Create a normal map texture from this");
    
    gui->addStaticText(L"Texture 4 :", rect<s32>(29, 126, 109, 146), true, true, parent, -1, false);
    gtex4 = gui->addEditBox(devices->getCore()->getTexturePath(node->getMaterial(0).TextureLayer[3].Texture).c_str(), 
                            rect<s32>(109, 126, 289, 146), true, parent, -1);
    gltex4 = gui->addButton(rect<s32>(289, 126, 329, 146), parent, -1, L"...", L"Browse Texture...");
    gdtex4 = gui->addButton(rect<s32>(329, 126, 399, 146), parent, -1, L"Remove", L"Remove Texture From Material");
	gmntex4 = gui->addButton(rect<s32>(399, 126, 499, 146), parent, -1, L"Make Normal", L"Create a normal map texture from this");
    
    gui->addStaticText(L"", rect<s32>(779, 26, 789, 296), true, true, parent, -1, false);
    
    gitex1 = gui->addImage(rect<s32>(129, 156, 259, 286), parent, -1, L"Material Texture 1");
    gitex1->setImage(node->getMaterial(0).TextureLayer[0].Texture);
    gitex1->setScaleImage(true);
    gitex2 = gui->addImage(rect<s32>(269, 156, 399, 286), parent, -1, L"Material Texture 2");
    gitex2->setImage(node->getMaterial(0).TextureLayer[1].Texture);
    gitex2->setScaleImage(true);
    gitex3 = gui->addImage(rect<s32>(409, 156, 539, 286), parent, -1, L"Material Texture 3");
    gitex3->setImage(node->getMaterial(0).TextureLayer[2].Texture);
    gitex3->setScaleImage(true);
    gitex4 = gui->addImage(rect<s32>(549, 156, 679, 286), parent, -1, L"Material Texture 4");
    gitex4->setImage(node->getMaterial(0).TextureLayer[3].Texture);
    gitex4->setScaleImage(true);
    
    gui->addStaticText(L"", rect<s32>(9, 296, 789, 307), true, true, parent, -1, false);
    //---------------------------------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------------------------------
	//ADVANCED GUI ELEMENTS
    
    gui->addStaticText(L"", rect<s32>(9, 326, 789, 667), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));
    
    gui->addStaticText(L"", rect<s32>(9, 326, 359, 337), true, true, parent, -1, false);
    textureText = gui->addStaticText(L"Advanced", rect<s32>(359, 316, 439, 346), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    gui->addStaticText(L"", rect<s32>(439, 326, 789, 337), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(9, 336, 19, 656), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(779, 336, 789, 656), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(9, 656, 789, 667), true, true, parent, -1, false);
    
    (layer1 = gui->addButton(rect<s32>(129, 356, 259, 386), parent, -1, L"Layer1", L"Select Layer 1"))->setIsPushButton();
    layer1->setPressedImage(driver->getTexture(stringc(devices->getWorkingDirectory().c_str()) + stringc("GUI/buttonpushed.png")));
    (layer2 = gui->addButton(rect<s32>(269, 356, 399, 386), parent, -1, L"Layer2", L"Select Layer 2"))->setIsPushButton();
    layer2->setPressedImage(driver->getTexture(stringc(devices->getWorkingDirectory().c_str()) + stringc("GUI/buttonpushed.png")));
    (layer3 = gui->addButton(rect<s32>(409, 356, 539, 386), parent, -1, L"Layer3", L"Select Layer 3"))->setIsPushButton();
    layer3->setPressedImage(driver->getTexture(stringc(devices->getWorkingDirectory().c_str()) + stringc("GUI/buttonpushed.png")));
    (layer4 = gui->addButton(rect<s32>(549, 356, 679, 386), parent, -1, L"Layer4", L"Select Layer 4"))->setIsPushButton();
    layer4->setPressedImage(driver->getTexture(stringc(devices->getWorkingDirectory().c_str()) + stringc("GUI/buttonpushed.png")));
    layer1->setPressed(true);
    
    gui->addStaticText(L"Texture Wrap U", rect<s32>(29, 416, 139, 436), true, true, parent, -1, false)->setOverrideColor(SColor(255, 255, 127, 0));
    textureWrapUChoice = gui->addComboBox(rect<s32>(149, 416, 539, 436), parent, -1);
    for (u32 i=0; i < 9; i++) {
        stringw name = aTextureClampNames[i];
        name.make_upper();
        textureWrapUChoice->addItem(name.c_str());
    }
    
    gui->addStaticText(L"Texture Wrap V", rect<s32>(29, 436, 139, 456), true, true, parent, -1, false)->setOverrideColor(SColor(255, 0, 127, 255));
    textureWrapVChoice = gui->addComboBox(rect<s32>(149, 436, 539, 456), parent, -1);
    for (u32 i=0; i < 9; i++) {
        stringw name = aTextureClampNames[i];
        name.make_upper();
        textureWrapVChoice->addItem(name.c_str());
    }
    
    gui->addStaticText(L"Anti Aliasing : ", rect<s32>(29, 466, 149, 486), true, true, parent, -1, false)->setOverrideColor(SColor(255, 127, 255, 0));
    anisotropicFilterChoice = gui->addComboBox(rect<s32>(149, 466, 229, 486), parent, -1);
    for (u32 i=0; i < 8; i++) {
        anisotropicFilterChoice->addItem(stringw(i+1).c_str());
    }
    
    gui->addStaticText(L"LOD Bias 0-200 : ", rect<s32>(29, 486, 149, 506), true, true, parent, -1, false)->setOverrideColor(SColor(255, 0, 255, 127));
    lodBiasChoice = gui->addComboBox(rect<s32>(149, 486, 229, 506), parent, -1);
    for (u32 i=0; i < 200; i++) {
        lodBiasChoice->addItem(stringw(i+1).c_str());
    }
    
    gui->addStaticText(L"Load Template : ", rect<s32>(29, 546, 149, 566), true, true, parent, -1, false)->setOverrideColor(SColor(255, 127, 255, 127));
    loadTemplatePath = gui->addEditBox(L"", rect<s32>(149, 546, 429, 566), true, parent, -1);
    browseLoadTemplate = gui->addButton(rect<s32>(429, 546, 469, 566), parent, -1, L"...", L"Browse For Template");
    saveTemplate = gui->addButton(rect<s32>(29, 566, 149, 586), parent, -1, L"Save Template...", L"Save Template As...");
    
    layerPreview = gui->addImage(rect<s32>(559, 406, 769, 606), parent, -1, L"Layer Preview");
    layerPreview->setScaleImage(true);
    regenerateBipMapsLevels = gui->addButton(rect<s32>(559, 616, 769, 636), parent, -1, L"Regenerate Mip Map Levels", L"");
    //---------------------------------------------------------------------------------------------------------
    
	editBoxValue = 0;
	enterValueWindow = 0;
    okEnterValue = 0;
	cancelEnterValue = 0;
}

CUIMaterialEditorTextures::~CUIMaterialEditorTextures() {
    devices->getEventReceiver()->RemoveEventReceiver(this);
}

void CUIMaterialEditorTextures::update(SMaterial *material) {
    gtex1->setText(devices->getCore()->getTexturePath(material->getTexture(0)).c_str());
    gtex2->setText(devices->getCore()->getTexturePath(material->getTexture(1)).c_str());
    gtex3->setText(devices->getCore()->getTexturePath(material->getTexture(2)).c_str());
    gtex4->setText(devices->getCore()->getTexturePath(material->getTexture(3)).c_str());
    
    gitex1->setImage(material->getTexture(0));
    gitex2->setImage(material->getTexture(1));
    gitex3->setImage(material->getTexture(2));
    gitex4->setImage(material->getTexture(3));
    
    textureWrapUChoice->setSelected(material->TextureLayer[getSelectedLayer()].TextureWrapU);
    textureWrapVChoice->setSelected(material->TextureLayer[getSelectedLayer()].TextureWrapV);
    
    anisotropicFilterChoice->setSelected(material->TextureLayer[getSelectedLayer()].AnisotropicFilter);
    lodBiasChoice->setSelected(material->TextureLayer[getSelectedLayer()].LODBias);
    
    layerPreview->setImage(material->TextureLayer[getSelectedLayer()].Texture);
    
    if (materialToEdit) {
        materialToEdit->Wireframe = false;
    }
    materialToEdit = material;
    materialToEdit->Wireframe = true;
}

void CUIMaterialEditorTextures::addEnterValue(stringw title) {
	enterValueWindow = gui->addWindow(rect<s32>(120, 150, 370, 240), true, title.c_str(), parent, -1);
	enterValueWindow->getCloseButton()->remove();

	gui->addStaticText(L"Value :", rect<s32>(10, 30, 60, 50), true, true, enterValueWindow, -1, false);
    editBoxValue = gui->addEditBox(L"9.0", rect<s32>(60, 30, 240, 50), true, enterValueWindow, -1);

	okEnterValue = gui->addButton(rect<s32>(70, 60, 150, 80), enterValueWindow, -1, L"Ok", L"");
	cancelEnterValue = gui->addButton(rect<s32>(160, 60, 240, 80), enterValueWindow, -1, L"Cancel", L"");
}

void CUIMaterialEditorTextures::setTexture(u32 id, stringw file_path, IGUIImage *image, IGUIEditBox *editBox) {
    ITexture *tex1 = driver->getTexture(file_path.c_str());
    if (tex1) {
        materialToEdit->setTexture(id, tex1);
        image->setImage(tex1);
        editBox->setText(file_path.c_str());
    } else {
        devices->addErrorDialog(L"Error when loaded texture", L"Cannot load the texture...", EMBF_OK);
    }
}

u32 CUIMaterialEditorTextures::getSelectedLayer() {
    u32 selected = 0;
    if (layer1->isPressed()) {
        selected = 0;
    }
    if (layer2->isPressed()) {
        selected = 1;
    }
    if (layer3->isPressed()) {
        selected = 2;
    }
    if (layer4->isPressed()) {
        selected = 3;
    }
    
    return selected;
}

bool CUIMaterialEditorTextures::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        //IF BUTTON CLICKED
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            IGUIButton *button = (IGUIButton *)event.GUIEvent.Caller;
            //---------------------------------------------------------------------------------------------------------
            //GENERAL
            //DELETE
            if (button == gdtex1) {
                materialToEdit->setTexture(0, 0);
                gitex1->setImage(0);
				gtex1->setText(L"");
            }
            if (button == gdtex2) {
                materialToEdit->setTexture(1, 0);
                gitex2->setImage(0);
				gtex2->setText(L"");
            }
            if (button == gdtex3) {
                materialToEdit->setTexture(2, 0);
                gitex3->setImage(0);
				gtex3->setText(L"");
            }
            if (button == gdtex4) {
                materialToEdit->setTexture(3, 0);
                gitex4->setImage(0);
				gtex4->setText(L"");
            }
            
            //LOAD TEXTURE
            if (button == gltex1) {
				gfodtex1 = devices->createFileOpenDialog(L"Choose your texture", CGUIFileSelector::EFST_OPEN_DIALOG, parent);
            }
            if (button == gltex2) {
                gfodtex2 = devices->createFileOpenDialog(L"Choose your texture", CGUIFileSelector::EFST_OPEN_DIALOG, parent);
            }
            if (button == gltex3) {
                gfodtex3 = devices->createFileOpenDialog(L"Choose your texture", CGUIFileSelector::EFST_OPEN_DIALOG, parent);
            }
            if (button == gltex4) {
                gfodtex4 = devices->createFileOpenDialog(L"Choose your texture", CGUIFileSelector::EFST_OPEN_DIALOG, parent);
            }
            //MAKE NORMAL
			if (button == gmntex1) {
				addEnterValue(L"Enter Value For Normal Map 1");
				currentNormalMap = 1;
			}
			if (button == gmntex2) {
				addEnterValue(L"Enter Value For Normal Map 2");
				currentNormalMap = 2;
			}
			if (button == gmntex3) {
				addEnterValue(L"Enter Value For Normal Map 3");
				currentNormalMap = 3;
			}
			if (button == gmntex4) {
				addEnterValue(L"Enter Value For Normal Map 4");
				currentNormalMap = 4;
			}
            //ADVANCED
			//ADD 
			if (button == okEnterValue) {
				ITexture *textureToNormalMap;
				if (currentNormalMap == 1) {
					textureToNormalMap = materialToEdit->TextureLayer[0].Texture;
				}
				if (currentNormalMap == 2) {
					textureToNormalMap = materialToEdit->TextureLayer[1].Texture;
				}
				if (currentNormalMap == 3) {
					textureToNormalMap = materialToEdit->TextureLayer[2].Texture;
				}
				if (currentNormalMap == 4) {
					textureToNormalMap = materialToEdit->TextureLayer[3].Texture;
				}
				driver->makeNormalMapTexture(textureToNormalMap, devices->getCore()->getF32(stringc(editBoxValue->getText()).c_str()));
				enterValueWindow->remove();
				enterValueWindow = 0;
			}
			if (button == cancelEnterValue) {
				enterValueWindow->remove();
				enterValueWindow = 0;
			}
			
            //LAYERS
            if (button == layer1 || button == layer2 || button == layer3 || button == layer4) {
                layer1->setPressed(false);
                layer2->setPressed(false);
                layer3->setPressed(false);
                layer4->setPressed(false);
                button->setPressed(true);
                update(materialToEdit);
            }
            
            //OTHER BUTTONS
            if (button == regenerateBipMapsLevels) {
                materialToEdit->TextureLayer[getSelectedLayer()].Texture->regenerateMipMapLevels();
            }
            if (button == browseLoadTemplate) {
                loadTemplateFileOpenDialog = devices->createFileOpenDialog(L"Choose the template", CGUIFileSelector::EFST_OPEN_DIALOG, parent);
            }
            if (button == saveTemplate) {
                saveTemplateFileOpenDialog = devices->createFileOpenDialog(L"Save template as...", CGUIFileSelector::EFST_SAVE_DIALOG, parent);
            }
            //---------------------------------------------------------------------------------------------------------
        }
        
        //COMBO BOX CHANGED
        if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
            IGUIComboBox *combBox = (IGUIComboBox *)event.GUIEvent.Caller;
            if (combBox == textureWrapUChoice) {
                materialToEdit->TextureLayer[getSelectedLayer()].TextureWrapU = (E_TEXTURE_CLAMP)textureWrapUChoice->getSelected();
            }
            if (combBox == textureWrapVChoice) {
                materialToEdit->TextureLayer[getSelectedLayer()].TextureWrapV = (E_TEXTURE_CLAMP)textureWrapVChoice->getSelected();
            }
            if (combBox == anisotropicFilterChoice) {
                materialToEdit->TextureLayer[getSelectedLayer()].AnisotropicFilter = anisotropicFilterChoice->getSelected();
            }
            if (combBox == lodBiasChoice) {
                materialToEdit->TextureLayer[getSelectedLayer()].LODBias = lodBiasChoice->getSelected();
            }
        }
        
        //IF FILE SELECTED
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
            IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
            if (dialog == gfodtex1) {
                setTexture(0, dialog->getFileName(), gitex1, gtex1);
            }
            if (dialog == gfodtex2) {
                setTexture(1, dialog->getFileName(), gitex2, gtex2);
            }
            if (dialog == gfodtex3) {
                setTexture(2, dialog->getFileName(), gitex3, gtex3);
            }
            if (dialog == gfodtex4) {
                setTexture(3, dialog->getFileName(), gitex4, gtex4);
            }
        }
    }
    
    return false;
}
