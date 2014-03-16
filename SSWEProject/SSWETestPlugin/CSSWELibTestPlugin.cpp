//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "stdafx.h"
#include "CSSWELibTestPlugin.h"

using namespace irr;
using namespace gui;
using namespace core;

void* createSSWELibPlugin() {
	return static_cast< void* > (new CSSWELibTestPlugin);
}

CSSWELibTestPlugin::CSSWELibTestPlugin() {

}

CSSWELibTestPlugin::~CSSWELibTestPlugin() {

}

void CSSWELibTestPlugin::update() {

}

void CSSWELibTestPlugin::close() {
	window->remove();

	ISSWELibPlugin::close();
}

void CSSWELibTestPlugin::open() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(630, 230, 1330, 650), false, L"SSWE Web Exporter !", 0, -1);
	window->getMaximizeButton()->setVisible(false);
	window->getMinimizeButton()->setVisible(true);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	//TOP
	gui->addStaticText(L"Engine : ", rect<s32>(10, 30, 70, 50), true, true, window, -1, true);
	engines = gui->addComboBox(rect<s32>(70, 30, 220, 50), window, -1);
	engines->addItem(L"Babylon.js");

	gui->addStaticText(L"To Folder : ", rect<s32>(240, 30, 320, 50), true, true, window, -1, true);
	toFolder = gui->addEditBox(stringw(devices->getWorkingDirectory() + "WebExport/").c_str(), rect<s32>(320, 30, 580, 50), true, window, -1);
	browseExportFolder = gui->addButton(rect<s32>(580, 30, 680, 50), window, -1, L"Browse...", L"Browse for folder to export");

	//MODELS
	gui->addStaticText(L"Models & Parameters : ", rect<s32>(10, 80, 200, 100), true, true, window, -1, true);
	exportStaticModels = gui->addCheckBox(false, rect<s32>(10, 100, 200, 120), window, -1, L"Export Static Models");
	exportAnimatedModels = gui->addCheckBox(false, rect<s32>(10, 120, 200, 140), window, -1, L"Export Animated Models");
	exportCustomMaterials = gui->addCheckBox(false, rect<s32>(10, 140, 200, 160), window, -1, L"Export Custom Materials");
	exportEffectFilters = gui->addCheckBox(false, rect<s32>(10, 160, 200, 180), window, -1, L"Export Effect Filters");

	exportAnimators = gui->addCheckBox(false, rect<s32>(10, 200, 200, 220), window, -1, L"Export Animators");
	exportTextures = gui->addCheckBox(false, rect<s32>(10, 220, 200, 240), window, -1, L"Export Textures");
	exportAnimations = gui->addCheckBox(false, rect<s32>(10, 240, 150, 260), window, -1, L"Export Animations");
	editExportAnimations = gui->addButton(rect<s32>(150, 240, 200, 260), window, -1, L"Edit", L"Edit animations list");

	exportLuaScripts = gui->addCheckBox(false, rect<s32>(10, 280, 200, 300), window, -1, L"Export Lua Scripts");

	//DESIGN
	gui->addStaticText(L"Design : ", rect<s32>(230, 80, 420, 100), true, true, window, -1, true);
	createHTMLFile = gui->addCheckBox(false, rect<s32>(230, 100, 420, 120), window, -1, L"Create HTML File");
	makeJSMinimized = gui->addCheckBox(false, rect<s32>(230, 120, 420, 140), window, -1, L"Make JS Minimized");

	//BUTTONS
	exportbtn = gui->addButton(rect<s32>(590, 380, 690, 410), window, -1, L"Export !", L"Export with the selected options");
	exportInformations = gui->addButton(rect<s32>(670, 360, 690, 380), window, -1, L"?", L"What will I export ?");

	//SEPARATORS
	gui->addStaticText(L"", rect<s32>(10, 60, 690, 70), true, true, window, -1, true);
	gui->addStaticText(L"", rect<s32>(210, 70, 220, 410), true, true, window, -1, true);

	//DATAS
	ISSWELibPlugin::open();

}

bool CSSWELibTestPlugin::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window)
				this->close();
		}
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == window->getMaximizeButton()) {
				rect<s32> windowScreenCenter = devices->getCore()->getScreenCenterRectFromRect(rect<s32>(630, 230, 1330, 650));
				devices->getCore()->maximizeWindow(window, windowScreenCenter);
			}

			//BUTTONS
			if (event.GUIEvent.Caller == exportInformations) {
				devices->addInformationDialog(L"Informations",
					L"SSWE Web Exporter !\n\n"
					L"3D Rendering using Babylon.js\n"
					L"Lua execution via LUA.VM.js\n",
					EMBF_OK, true, 0);
			}
			if (event.GUIEvent.Caller == browseExportFolder) {
				toFolderFileOpenDialog = devices->createFileOpenDialog(L"Folder to export web project", CGUIFileSelector::EFST_OPEN_DIALOG, window, true);
				//((CGUIFileSelector *)toFolderFileOpenDialog)->addFileFilter(L"Folder", L"", 0);
			}
		}

		//FILE SELECTED
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == toFolderFileOpenDialog) {
				toFolder->setText(toFolderFileOpenDialog->getFileName());
			}
		}
	}

	return false;
}
