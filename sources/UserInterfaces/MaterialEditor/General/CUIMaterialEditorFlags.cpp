//
//  CUIMaterialEditorColors.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#include "CUIMaterialEditorFlags.h"

CUIMaterialEditorFlags::CUIMaterialEditorFlags(CDevices *_devices, IGUIElement *_parent, ISceneNode *node) {
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
	//FILTERS
	gui->addStaticText(L"", rect<s32>(9, 16, 789, 112), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));
    
    gui->addStaticText(L"", rect<s32>(9, 16, 359, 27), true, true, parent, -1, false);
    IGUIStaticText *textureText = gui->addStaticText(L"Filters", rect<s32>(359, 6, 439, 36), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    gui->addStaticText(L"", rect<s32>(439, 16, 789, 27), true, true, parent, -1, false);

    gui->addStaticText(L"", rect<s32>(9, 26, 19, 104), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(780, 26, 789, 105), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(9, 104, 789, 112), true, true, parent, -1, false);

	anisotropicFilter = gui->addCheckBox(false, rect<s32>(26, 43, 166, 63), parent, -1, L"Anisotropic Filter");
	bilinearFilter = gui->addCheckBox(false, rect<s32>(176, 43, 316, 63), parent, -1, L"Bilinear Filter");
	trilinearFilter = gui->addCheckBox(false, rect<s32>(326, 43, 466, 63), parent, -1, L"Trilinear Filter");
	antialiasingFilter = gui->addCheckBox(false, rect<s32>(26, 73, 166, 93), parent, -1, L"Anti Aliasing");
	
	//CULLING
	gui->addStaticText(L"", rect<s32>(9, 143, 786, 202), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));
    
    gui->addStaticText(L"", rect<s32>(9, 143, 356, 154), true, true, parent, -1, false);
    textureText = gui->addStaticText(L"Culling", rect<s32>(356, 133, 436, 163), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    gui->addStaticText(L"", rect<s32>(436, 143, 786, 154), true, true, parent, -1, false);

    gui->addStaticText(L"", rect<s32>(9, 153, 19, 193), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(776, 153, 786, 193), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(9, 193, 786, 202), true, true, parent, -1, false);

	backFaceCulling = gui->addCheckBox(false, rect<s32>(26, 163, 166, 183), parent, -1, L"Back Face Culling");
	frontFaceCulling = gui->addCheckBox(false, rect<s32>(186, 163, 326, 183), parent, -1, L"Front Face Culling");

	//OTHERS
	gui->addStaticText(L"", rect<s32>(9, 223, 786, 321), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));
    
    gui->addStaticText(L"", rect<s32>(6, 223, 356, 234), true, true, parent, -1, false);
    textureText = gui->addStaticText(L"Others", rect<s32>(356, 213, 436, 243), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    gui->addStaticText(L"", rect<s32>(436, 223, 786, 234), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(8, 233, 19, 313), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(776, 233, 786, 313), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(8, 312, 786, 321), true, true, parent, -1, false);

	colorMask = gui->addCheckBox(false, rect<s32>(26, 253, 166, 273), parent, -1, L"Color Mask");
	colorMaterial = gui->addCheckBox(false, rect<s32>(186, 253, 326, 273), parent, -1, L"Color Material");
	textureWrap = gui->addCheckBox(false, rect<s32>(336, 253, 476, 273), parent, -1, L"Texture Wrap");

	zbuffer = gui->addCheckBox(false, rect<s32>(26, 283, 166, 303), parent, -1, L"ZBuffer");
	zwriteEnable = gui->addCheckBox(false, rect<s32>(186, 283, 326, 303), parent, -1, L"ZWrite Enable");

	gui->addStaticText(L"", rect<s32>(9, 353, 788, 452), true, true, parent, -1, false)->setBackgroundColor(SColor(255, 50, 50, 50));
    
    gui->addStaticText(L"", rect<s32>(6, 353, 356, 364), true, true, parent, -1, false);
    textureText = gui->addStaticText(L"Params", rect<s32>(356, 343, 436, 373), true, true, parent, -1, true);
    textureText->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    textureText->setBackgroundColor(SColor(255, 50, 50, 50));
    textureText->setOverrideColor(SColor(255, 255, 255, 255));
    gui->addStaticText(L"", rect<s32>(436, 353, 786, 364), true, true, parent, -1, false);
    gui->addStaticText(L"", rect<s32>(6, 363, 16, 443), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(776, 363, 786, 443), true, true, parent, -1, false);
	gui->addStaticText(L"", rect<s32>(6, 443, 788, 452), true, true, parent, -1, false);

	gui->addStaticText(L"Material Type Parameter 1", rect<s32>(26, 383, 206, 403), true, true, parent, -1, false);
	materialTypeParam1 = gui->addEditBox(L"", rect<s32>(206, 383, 346, 403), true, parent, -1);
	gui->addStaticText(L"Material Type Parameter 2", rect<s32>(26, 403, 206, 423), true, true, parent, -1, false);
	materialTypeParam2 = gui->addEditBox(L"", rect<s32>(206, 403, 346, 423), true, parent, -1);
	//---------------------------------------------------------------------------------------------------------
}
CUIMaterialEditorFlags::~CUIMaterialEditorFlags() {
	devices->getEventReceiver()->RemoveEventReceiver(this);
}

void CUIMaterialEditorFlags::update(SMaterial *material) {
	anisotropicFilter->setChecked(material->getFlag(EMF_ANISOTROPIC_FILTER));
	bilinearFilter->setChecked(material->getFlag(EMF_BILINEAR_FILTER));
	trilinearFilter->setChecked(material->getFlag(EMF_TRILINEAR_FILTER));
	antialiasingFilter->setChecked(material->getFlag(EMF_ANTI_ALIASING));

	backFaceCulling->setChecked(material->getFlag(EMF_BACK_FACE_CULLING));
	frontFaceCulling->setChecked(material->getFlag(EMF_FRONT_FACE_CULLING));

	colorMask->setChecked(material->getFlag(EMF_COLOR_MASK));
	colorMaterial->setChecked(material->getFlag(EMF_COLOR_MATERIAL));
	textureWrap->setChecked(material->getFlag(EMF_TEXTURE_WRAP));
	zbuffer->setChecked(material->getFlag(EMF_ZBUFFER));
	zwriteEnable->setChecked(material->getFlag(EMF_ZWRITE_ENABLE));

	materialTypeParam1->setText(devices->getCore()->getStrNumber(material->MaterialTypeParam).c_str());
	materialTypeParam2->setText(devices->getCore()->getStrNumber(material->MaterialTypeParam2).c_str());

	//UPDATE MATERIAL POINTER
	materialToEdit = material;
}
    
bool CUIMaterialEditorFlags::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED) {
			IGUICheckBox *chb = (IGUICheckBox *)event.GUIEvent.Caller;
			//FILTERS
			if (chb == anisotropicFilter) {
				nodeToEdit->setMaterialFlag(EMF_ANISOTROPIC_FILTER, chb->isChecked());
			}
			if (chb == bilinearFilter) {
				nodeToEdit->setMaterialFlag(EMF_BILINEAR_FILTER, chb->isChecked());
			}
			if (chb == trilinearFilter) {
				nodeToEdit->setMaterialFlag(EMF_TRILINEAR_FILTER, chb->isChecked());
			}
			if(chb == antialiasingFilter) {
				nodeToEdit->setMaterialFlag(EMF_ANTI_ALIASING, chb->isChecked());
			}
			//CULLING
			if (chb == backFaceCulling) {
				nodeToEdit->setMaterialFlag(EMF_BACK_FACE_CULLING, chb->isChecked());
			}
			if(chb == frontFaceCulling) {
				nodeToEdit->setMaterialFlag(EMF_FRONT_FACE_CULLING, chb->isChecked());
			}
			//OTHERS
			if (chb == colorMask) {
				nodeToEdit->setMaterialFlag(EMF_COLOR_MASK, chb->isChecked());
			}
			if(chb == colorMaterial) {
				nodeToEdit->setMaterialFlag(EMF_COLOR_MATERIAL, chb->isChecked());
			}
			if(chb == textureWrap) {
				nodeToEdit->setMaterialFlag(EMF_TEXTURE_WRAP, chb->isChecked());
			}
			if(chb == zbuffer) {
				nodeToEdit->setMaterialFlag(EMF_ZBUFFER, chb->isChecked());
			}
			if(chb == zwriteEnable) {
				nodeToEdit->setMaterialFlag(EMF_ZWRITE_ENABLE, chb->isChecked());
			}
			//UPDATE
			update(materialToEdit);
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller == materialTypeParam1) {
				materialToEdit->MaterialTypeParam = devices->getCore()->getF32(stringc(materialTypeParam1->getText()).c_str());
			}
			if (event.GUIEvent.Caller == materialTypeParam2) {
				materialToEdit->MaterialTypeParam2 = devices->getCore()->getF32(stringc(materialTypeParam2->getText()).c_str());
			}
		}

	}

	return false;
}
