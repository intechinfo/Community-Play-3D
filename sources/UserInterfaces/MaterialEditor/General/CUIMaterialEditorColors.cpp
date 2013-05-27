//
//  CUIMaterialEditorColors.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//
#include "stdafx.h"
#include "CUIMaterialEditorColors.h"

CUIMaterialEditorColors::CUIMaterialEditorColors(CDevices *_devices, IGUIElement *_parent, ISceneNode *node) {
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
	gui->addStaticText(L"", rect<s32>(9, 16, 789, 487), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));

	gui->addStaticText(L"", rect<s32>(9, 16, 359, 27), true, true, parent, -1, false);
    IGUIStaticText *textureText = gui->addStaticText(L"General", rect<s32>(359, 6, 439, 36), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    gui->addStaticText(L"", rect<s32>(439, 16, 789, 27), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(9, 26, 19, 476), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(9, 476, 789, 487), true, true, parent, -1, false);

	//DIFFUSE
	gui->addStaticText(L"Diffuse :", rect<s32>(29, 36, 89, 56), true, true, parent, -1, false);
	diffuseColorPreview = gui->addStaticText(L"", rect<s32>(89, 36, 109, 56), true, true, parent, -1, false);

	gui->addStaticText(L"Red : ", rect<s32>(29, 56, 79, 76), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 0, 0));
	diffuseRed = gui->addEditBox(L"", rect<s32>(79, 56, 169, 76), true, parent, -1);
	diffuseRedS = gui->addScrollBar(true, rect<s32>(169, 56, 769, 75), parent, -1);
    diffuseRedS->setMin(0);
    diffuseRedS->setMax(255);
	gui->addStaticText(L"Green : ", rect<s32>(29, 76, 79, 96), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 255, 0));
	diffuseGreen = gui->addEditBox(L"", rect<s32>(79, 76, 169, 96), true, parent, -1);
	diffuseGreenS = gui->addScrollBar(true, rect<s32>(169, 76, 769, 95), parent, -1);
    diffuseGreenS->setMin(0);
    diffuseGreenS->setMax(255);
	gui->addStaticText(L"Blue : ", rect<s32>(29, 96, 79, 116), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 0, 255));
	diffuseBlue = gui->addEditBox(L"", rect<s32>(79, 96, 169, 116), true, parent, -1);
	diffuseBlueS = gui->addScrollBar(true, rect<s32>(169, 96, 769, 115), parent, -1);
    diffuseBlueS->setMin(0);
    diffuseBlueS->setMax(255);
	gui->addStaticText(L"Alpha : ", rect<s32>(29, 116, 79, 136), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 255, 255));
	diffuseAlpha = gui->addEditBox(L"", rect<s32>(79, 116, 169, 136), true, parent, -1);
	diffuseAlphaS = gui->addScrollBar(true, rect<s32>(169, 116, 769, 135), parent, -1);
    diffuseAlphaS->setMin(0);
    diffuseAlphaS->setMax(255);

	//EMISSIVE
	gui->addStaticText(L"Emissive :", rect<s32>(29, 146, 89, 166), true, true, parent, -1, false);
	emissiveColorPreview = gui->addStaticText(L"", rect<s32>(89, 146, 109, 166), true, true, parent, -1, false);

	gui->addStaticText(L"Red : ", rect<s32>(29, 166, 79, 186), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 0, 0));
	emissiveRed = gui->addEditBox(L"", rect<s32>(79, 166, 169, 186), true, parent, -1);
	emissiveRedS = gui->addScrollBar(true, rect<s32>(169, 166, 769, 185), parent, -1);
    emissiveRedS->setMin(0);
    emissiveRedS->setMax(255);
	gui->addStaticText(L"Green : ", rect<s32>(29, 186, 79, 206), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 255, 0));
	emissiveGreen = gui->addEditBox(L"", rect<s32>(79, 186, 169, 206), true, parent, -1);
	emissiveGreenS = gui->addScrollBar(true, rect<s32>(169, 186, 769, 205), parent, -1);
    emissiveGreenS->setMin(0);
    emissiveGreenS->setMax(255);
	gui->addStaticText(L"Blue : ", rect<s32>(29, 206, 79, 226), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 0, 255));
	emissiveBlue = gui->addEditBox(L"", rect<s32>(79, 206, 169, 226), true, parent, -1);
	emissiveBlueS = gui->addScrollBar(true, rect<s32>(169, 206, 769, 225), parent, -1);
    emissiveBlueS->setMin(0);
    emissiveBlueS->setMax(255);
	gui->addStaticText(L"Alpha : ", rect<s32>(29, 226, 79, 246), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 255, 255));
	emissiveAlpha = gui->addEditBox(L"", rect<s32>(79, 226, 169, 246), true, parent, -1);
	emissiveAlphaS = gui->addScrollBar(true, rect<s32>(169, 226, 769, 245), parent, -1);
    emissiveAlphaS->setMin(0);
    emissiveAlphaS->setMax(255);

	//SPECULAR
	gui->addStaticText(L"Specular :", rect<s32>(29, 256, 89, 276), true, true, parent, -1, false);
	specularColorPreview = gui->addStaticText(L"", rect<s32>(89, 256, 109, 276), true, true, parent, -1, false);

	gui->addStaticText(L"Red : ", rect<s32>(29, 276, 79, 296), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 0, 0));
	specularRed = gui->addEditBox(L"", rect<s32>(79, 276, 169, 296), true, parent, -1);
	specularRedS = gui->addScrollBar(true, rect<s32>(169, 276, 769, 295), parent, -1);
    specularRedS->setMin(0);
    specularRedS->setMax(255);
	gui->addStaticText(L"Green : ", rect<s32>(29, 296, 79, 316), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 255, 0));
	specularGreen = gui->addEditBox(L"", rect<s32>(79, 296, 169, 316), true, parent, -1);
	specularGreenS = gui->addScrollBar(true, rect<s32>(169, 296, 769, 315), parent, -1);
    specularGreenS->setMin(0);
    specularGreenS->setMax(255);
	gui->addStaticText(L"Blue : ", rect<s32>(29, 316, 79, 336), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 0, 255));
	specularBlue = gui->addEditBox(L"", rect<s32>(79, 316, 169, 336), true, parent, -1);
	specularBlueS = gui->addScrollBar(true, rect<s32>(169, 316, 769, 335), parent, -1);
    specularBlueS->setMin(0);
    specularBlueS->setMax(255);
	gui->addStaticText(L"Alpha : ", rect<s32>(29, 336, 79, 356), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 255, 255));
	specularAlpha = gui->addEditBox(L"", rect<s32>(79, 336, 169, 356), true, parent, -1);
	specularAlphaS = gui->addScrollBar(true, rect<s32>(169, 336, 769, 355), parent, -1);
    specularAlphaS->setMin(0);
    specularAlphaS->setMax(255);

	//AMBIANT
	gui->addStaticText(L"Ambiant :", rect<s32>(29, 366, 89, 386), true, true, parent, -1, false);
	ambiantColorPreview = gui->addStaticText(L"", rect<s32>(89, 366, 109, 386), true, true, parent, -1, false);

	gui->addStaticText(L"Red : ", rect<s32>(29, 386, 79, 406), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 0, 0));
	ambiantRed = gui->addEditBox(L"", rect<s32>(79, 386, 169, 406), true, parent, -1);
	ambiantRedS = gui->addScrollBar(true, rect<s32>(169, 386, 769, 405), parent, -1);
    ambiantRedS->setMin(0);
    ambiantRedS->setMax(255);
	gui->addStaticText(L"Green : ", rect<s32>(29, 406, 79, 426), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 255, 0));
	ambiantGreen = gui->addEditBox(L"", rect<s32>(79, 406, 169, 426), true, parent, -1);
	ambiantGreenS = gui->addScrollBar(true, rect<s32>(169, 406, 769, 425), parent, -1);
    ambiantGreenS->setMin(0);
    ambiantGreenS->setMax(255);
	gui->addStaticText(L"Blue : ", rect<s32>(29, 426, 79, 446), true, true, parent, -1, false)->setOverrideColor(SColor(200, 0, 0, 255));
	ambiantBlue = gui->addEditBox(L"", rect<s32>(79, 426, 169, 446), true, parent, -1);
	ambiantBlueS = gui->addScrollBar(true, rect<s32>(169, 426, 769, 445), parent, -1);
    ambiantBlueS->setMin(0);
    ambiantBlueS->setMax(255);
	gui->addStaticText(L"Alpha : ", rect<s32>(29, 446, 79, 466), true, true, parent, -1, false)->setOverrideColor(SColor(200, 255, 255, 255));
	ambiantAlpha = gui->addEditBox(L"", rect<s32>(79, 446, 169, 466), true, parent, -1);
	ambiantAlphaS = gui->addScrollBar(true, rect<s32>(169, 446, 769, 465), parent, -1);
    ambiantAlphaS->setMin(0);
    ambiantAlphaS->setMax(255);
	//---------------------------------------------------------------------------------------------------------
}
CUIMaterialEditorColors::~CUIMaterialEditorColors() {
	devices->getEventReceiver()->RemoveEventReceiver(this);
}
    
void CUIMaterialEditorColors::update(SMaterial *material) {
	//DIFFUSE
	diffuseColorPreview->setBackgroundColor(material->DiffuseColor);

	diffuseRed->setText(stringw(material->DiffuseColor.getRed()).c_str());
	diffuseRedS->setPos(material->DiffuseColor.getRed());

	diffuseGreen->setText(stringw(material->DiffuseColor.getGreen()).c_str());
	diffuseGreenS->setPos(material->DiffuseColor.getGreen());

	diffuseBlue->setText(stringw(material->DiffuseColor.getBlue()).c_str());
	diffuseBlueS->setPos(material->DiffuseColor.getBlue());

	diffuseAlpha->setText(stringw(material->DiffuseColor.getAlpha()).c_str());
	diffuseAlphaS->setPos(material->DiffuseColor.getAlpha());

	//EMISSIVE
	emissiveColorPreview->setBackgroundColor(material->EmissiveColor);

	emissiveRed->setText(stringw(material->EmissiveColor.getRed()).c_str());
	emissiveRedS->setPos(material->EmissiveColor.getRed());

	emissiveGreen->setText(stringw(material->EmissiveColor.getGreen()).c_str());
	emissiveGreenS->setPos(material->EmissiveColor.getGreen());

	emissiveBlue->setText(stringw(material->EmissiveColor.getBlue()).c_str());
	emissiveBlueS->setPos(material->EmissiveColor.getBlue());

	emissiveAlpha->setText(stringw(material->EmissiveColor.getAlpha()).c_str());
	emissiveAlphaS->setPos(material->EmissiveColor.getAlpha());

	//SPECULAR
	specularColorPreview->setBackgroundColor(material->SpecularColor);

	specularRed->setText(stringw(material->SpecularColor.getRed()).c_str());
	specularRedS->setPos(material->SpecularColor.getRed());

	specularGreen->setText(stringw(material->SpecularColor.getGreen()).c_str());
	specularGreenS->setPos(material->SpecularColor.getGreen());

	specularBlue->setText(stringw(material->SpecularColor.getBlue()).c_str());
	specularBlueS->setPos(material->SpecularColor.getBlue());

	specularAlpha->setText(stringw(material->SpecularColor.getAlpha()).c_str());
	specularAlphaS->setPos(material->SpecularColor.getAlpha());

	//AMBIANT
	ambiantColorPreview->setBackgroundColor(material->AmbientColor);

	ambiantRed->setText(stringw(material->AmbientColor.getRed()).c_str());
	ambiantRedS->setPos(material->AmbientColor.getRed());

	ambiantGreen->setText(stringw(material->AmbientColor.getGreen()).c_str());
	ambiantGreenS->setPos(material->AmbientColor.getGreen());

	ambiantBlue->setText(stringw(material->AmbientColor.getBlue()).c_str());
	ambiantBlueS->setPos(material->AmbientColor.getBlue());

	ambiantAlpha->setText(stringw(material->AmbientColor.getAlpha()).c_str());
	ambiantAlphaS->setPos(material->AmbientColor.getAlpha());

	//UPDATE MATERIAL POINTER
	materialToEdit = material;
}
    
bool CUIMaterialEditorColors::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			IGUIScrollBar *scbar = (IGUIScrollBar *)event.GUIEvent.Caller;
			//IF DIFFUSE
			if (scbar == diffuseRedS) {
				materialToEdit->DiffuseColor.setRed(scbar->getPos());
			}
			if (scbar == diffuseGreenS) {
				materialToEdit->DiffuseColor.setGreen(scbar->getPos());
			}
			if (scbar == diffuseBlueS) {
				materialToEdit->DiffuseColor.setBlue(scbar->getPos());
			}
			if (scbar == diffuseAlphaS) {
				materialToEdit->DiffuseColor.setAlpha(scbar->getPos());
			}
			//IF EMISSIVE
			if (scbar == emissiveRedS) {
				materialToEdit->EmissiveColor.setRed(scbar->getPos());
			}
			if (scbar == emissiveGreenS) {
				materialToEdit->EmissiveColor.setGreen(scbar->getPos());
			}
			if (scbar == emissiveBlueS) {
				materialToEdit->EmissiveColor.setBlue(scbar->getPos());
			}
			if (scbar == emissiveAlphaS) {
				materialToEdit->EmissiveColor.setAlpha(scbar->getPos());
			}
			//IF SPECULAR
			if (scbar == specularRedS) {
				materialToEdit->SpecularColor.setRed(scbar->getPos());
			}
			if (scbar == specularGreenS) {
				materialToEdit->SpecularColor.setGreen(scbar->getPos());
			}
			if (scbar == specularBlueS) {
				materialToEdit->SpecularColor.setBlue(scbar->getPos());
			}
			if (scbar == specularAlphaS) {
				materialToEdit->SpecularColor.setAlpha(scbar->getPos());
			}
			//IF AMBIANT
			if (scbar == ambiantRedS) {
				materialToEdit->AmbientColor.setRed(scbar->getPos());
			}
			if (scbar == ambiantGreenS) {
				materialToEdit->AmbientColor.setGreen(scbar->getPos());
			}
			if (scbar == ambiantBlueS) {
				materialToEdit->AmbientColor.setBlue(scbar->getPos());
			}
			if (scbar == ambiantAlphaS) {
				materialToEdit->AmbientColor.setAlpha(scbar->getPos());
			}
			//UPDATE GUI
			update(materialToEdit);
		}
	}

	return false;
}
