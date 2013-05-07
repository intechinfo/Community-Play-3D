//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#include "CUITexturesManager.h"

CUITexturesManager::CUITexturesManager(CDevices *_devices) {
	devices = _devices;
	devices->getEventReceiver()->AddEventReceiver(this);

	driver = devices->getVideoDriver();
	gui = devices->getGUIEnvironment();

	window = gui->addWindow(rect<s32>(110, 90, 810, 530), false, L"Textures Manager", 0, -1);
	window->getMaximizeButton()->setVisible(true);
	window->getMinimizeButton()->setVisible(true);

	textures = gui->addListBox(rect<s32>(10, 30, 390, 430), window, -1, true);

	remove = gui->addButton(rect<s32>(400, 30, 500, 50), window, -1, L"Remove", L"Removes the texture");
	whoUse = gui->addButton(rect<s32>(400, 60, 500, 80), window, -1, L"Who Use ?", L"Checks all scene node to know who use the selected texture");
	edit = gui->addButton(rect<s32>(400, 90, 500, 110), window, -1, L"Edit...", L"Open texture editor");

	texturePreview = gui->addImage(rect<s32>(400, 130, 690, 430), window, -1, L"Preview");
	texturePreview->setScaleImage(true);
}

CUITexturesManager::~CUITexturesManager() {

}

array<ISceneNode *> CUITexturesManager::whoUseThisTexture(ITexture *tex) {
	array<ISceneNode *> nodes, nodeswu;
	devices->getSceneManager()->getSceneNodesFromType(ESNT_ANY, nodes);

	nodeswu.clear();

	//SEARCH FOR EACH NODE WHICH NODE USES THE TEXTURE
	for (u32 i=0; i < nodes.size(); i++) {
		ISceneNode *node = nodes[i];
		bool use = false;
		for (u32 j=0; j < node->getMaterialCount(); j++) {
			for (u32 k=0; k < irr::video::MATERIAL_MAX_TEXTURES; k++) {
				if (node->getMaterial(j).TextureLayer[k].Texture == tex) {
					use = true;
					break;
				}
			}
		}
		if (use) {
			nodeswu.push_back(node);
		}
	}

	return nodeswu;
}

void CUITexturesManager::removeTexture(ITexture *tex) {

	if (tex->isRenderTarget()) {
		devices->addErrorDialog(L"Cannot remove", L"This texture is a render target, you cannot remove", EMBF_OK);
		return;
	}

	array<ISceneNode *> nodes = whoUseThisTexture(tex);
	for (u32 i=0; i < nodes.size(); i++) {
		ISceneNode *node = nodes[i];
		for (u32 j=0; j < node->getMaterialCount(); j++) {
			for (u32 k=0; k < irr::video::MATERIAL_MAX_TEXTURES; k++) {
				if (node->getMaterial(j).TextureLayer[k].Texture == tex) {
					node->getMaterial(j).setTexture(k, 0);
				}
			}
		}
	}
	driver->removeTexture(tex);
	texturePreview->setImage(0);
}

bool CUITexturesManager::OnEvent(const SEvent &event) {

	if (event.EventType == EET_USER_EVENT) {
		//REACTIVATE WINOW
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == window->getReferenceCount()) {
				devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		//UPDATE LISTBOX
		s32 selected = textures->getSelected();
		textures->clear();
		for (u32 i=0; i < driver->getTextureCount(); i++) {
			stringw name = driver->getTextureByIndex(i)->getName().getPath().c_str();
			name.remove(devices->getWorkingDirectory().c_str());
			textures->addItem(name.c_str());
		}
		textures->setSelected(selected);

		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			//IF CLOSE WINDOW
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			//IF MINIMIZE
			if (event.GUIEvent.Caller == window->getMinimizeButton()) {
				devices->getEventReceiver()->AddMinimizedWindow(this, window);
			}
			//REMOVE SELECTED TEXTURE
			if (event.GUIEvent.Caller == remove) {
				ITexture *tex = driver->getTextureByIndex(textures->getSelected());
				//driver->removeTexture(tex);
				removeTexture(tex);
			}
			//GET WHO USE THE TEXTURE
			if (event.GUIEvent.Caller == whoUse) {
				//WHO USE ?
				array<ISceneNode *> nodes = whoUseThisTexture(driver->getTextureByIndex(textures->getSelected()));
				stringw nodesText = L"Nodes that use this texture : \n\n";
				for (u32 i=0; i < nodes.size(); i++) {
					if (nodes[i]->getName() == "") {
						nodesText += L"Unnamed node";
					} else {
						nodesText += nodes[i]->getName();
					}
					nodesText += L"\n";
				}
				if (driver->getTextureByIndex(textures->getSelected())->isRenderTarget()) {
					nodesText += L"Is a render target \n";
				}
				devices->addInformationDialog(L"Informations", nodesText.c_str(), EMBF_OK, true, 0);
			}
		}

		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			//NEW TEXTURE SELECTED
			if (event.GUIEvent.Caller == textures) {
				if (textures->getSelected() != -1) {
					texturePreview->setImage(driver->getTextureByIndex(textures->getSelected()));
				}
			}
		}
	}
	
	return false;
}
