//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#include "Stdafx.h"
#include "CSSWEUTOpenSCADDocument.h"

#include <thread>
#include <mutex>
std::mutex cssweutopenscaddocumentm;

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

COpenSCADDocument::COpenSCADDocument(cp3d::core::IDevices *_devices, stringc _workingDirectory) {
	/// Set datas
	devices = _devices;
	workingDirectory = _workingDirectory;
	currentObjectVersion = 0;

	openscadCode = L"";

	/// Create the code editor
	codeEditor = devices->createGUICodeEditor();
	((IGUIWindow*)codeEditor->getWindow())->getMaximizeButton()->setVisible(false);
	((IGUIWindow*)codeEditor->getWindow())->getCloseButton()->setVisible(false);
	codeEditor->setAutoSave(true);

	codeEditor->setTextData(&openscadCode);
	codeEditor->getWindow()->setRelativePosition(rect<s32>(codeEditor->getWindow()->getRelativePosition().UpperLeftCorner.X,
														   codeEditor->getWindow()->getRelativePosition().UpperLeftCorner.Y,
														   codeEditor->getWindow()->getRelativePosition().LowerRightCorner.X,
														   codeEditor->getWindow()->getRelativePosition().LowerRightCorner.Y + 30));


	/// Configure code editor
	codeEditor->clearKeyWords();
	codeEditor->addKeyword("difference", SColor(255, 150, 0, 150), false);
	codeEditor->addKeyword("union", SColor(255, 150, 0, 150), false);
	codeEditor->addKeyword("intersection", SColor(255, 150, 0, 150), false);
	codeEditor->addKeyword("render", SColor(255, 150, 0, 150), false);
	codeEditor->addKeyword("hull", SColor(255, 150, 0, 150), false);
	codeEditor->addKeyword("minkowski", SColor(255, 150, 0, 150), false);

	codeEditor->addKeyword("cube", SColor(255, 100, 40, 38), false);
	codeEditor->addKeyword("sphere", SColor(255, 100, 40, 38), false);
	codeEditor->addKeyword("surface", SColor(255, 100, 40, 38), false);
	codeEditor->addKeyword("cylinder", SColor(255, 100, 40, 38), false);
	codeEditor->addKeyword("polyhedron", SColor(255, 100, 40, 38), false);

	codeEditor->addKeyword("use", SColor(255, 50, 100, 38), true);
	codeEditor->addKeyword("import", SColor(255, 50, 100, 38), true);
	codeEditor->addKeyword("module", SColor(255, 50, 100, 38), false);

	codeEditor->addKeyword("rotate", SColor(255, 100, 100, 38), false);
	codeEditor->addKeyword("translate", SColor(255, 100, 100, 38), false);
	codeEditor->addKeyword("scale", SColor(255, 100, 100, 38), false);

	codeEditor->addCppKeywords();

	/// Create buttons
	buildButton = devices->getGUIEnvironment()->addButton(rect<s32>(5,
																	codeEditor->getWindow()->getRelativePosition().getHeight()-40,
																	105,
																	codeEditor->getWindow()->getRelativePosition().getHeight()-10),
														  codeEditor->getWindow(), -1, L"Build...", L"Build and add/update to the scene");
	closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(115,
																	codeEditor->getWindow()->getRelativePosition().getHeight()-40,
																	215,
																	codeEditor->getWindow()->getRelativePosition().getHeight()-10),
														  codeEditor->getWindow(), -1, L"Close", L"");

	stateTextElement = devices->getGUIEnvironment()->addStaticText(L"Ready.",
																   rect<s32>(230,
																			 codeEditor->getWindow()->getRelativePosition().getHeight()-40,
																			 codeEditor->getWindow()->getRelativePosition().getWidth()-10,
																			 codeEditor->getWindow()->getRelativePosition().getHeight()-10),
																   false, false, codeEditor->getWindow(), -1, false);
	stateTextElement->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_CENTER);

	/// Configure events
	devices->getEventReceiver()->AddEventReceiver(this);
}

COpenSCADDocument::~COpenSCADDocument() {

}

void COpenSCADDocument::buildObject() {
	/// Change state of the build
	stateTextElement->setText(L"Building...");
	buildButton->setEnabled(false);
	closeButton->setEnabled(false);

	stringc cur = stringc(currentObjectVersion);
	/// Create temporary file and then call openscad as builder
	openscadCode = openscadCode.replace("\r", "\n");
	openscadCode = openscadCode.replace("\t", "    ");
	openscadCode = openscadCode.replace("	", "    ");

	FILE *export_file = fopen(stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/temp/temp_openscad" + cur + ".scad").c_str(), "w");
	fprintf(export_file, "%s\n", stringc(openscadCode).c_str());
	fclose(export_file);
    #ifndef _IRR_OSX_PLATFORM_
	std::system(stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/openscad.exe -o "
		+ workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/" + "temp/test" + cur + ".stl "
		+ workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/" + "temp/temp_openscad" + cur + ".scad").c_str());
    #else
    stringc ppath = "\"";
    system(stringc(ppath + workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/OpenSCAD.app/Contents/MacOS/OpenSCAD\" -o "
                        + "\"" + workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/" + "temp/test" + cur + ".stl\" "
                        + "\"" + workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/" + "temp/temp_openscad" + cur + ".scad\"").c_str());
    #endif

	/// Add the object
	/// Lock the main thread to add the object to the scene
	#ifndef _IRR_LINUX_PLATFORM_
	std::lock_guard<std::mutex> lck(cssweutopenscaddocumentm);
	#endif
	IMesh *openscadMesh = devices->getSceneManager()->getMesh(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/temp/test" + cur + ".stl");
	if (openscadMesh) {
		IMeshSceneNode *openscadNode = devices->getSceneManager()->addMeshSceneNode(openscadMesh, 0, -1, vector3df(0),
																					vector3df(0), vector3df(1), false);
		openscadNode->setMaterialFlag(EMF_LIGHTING, false);
		openscadNode->setName("#object:new_openscad_node");
		devices->getCollisionManager()->setCollisionFromBoundingBox(openscadNode);
		devices->getCoreData()->addObjectNode(openscadNode, openscadMesh, "corner");
		devices->getXEffect()->addShadowToNode(openscadNode, devices->getXEffectFilterType(), cp3d::video::ESM_BOTH);
	}

	/// Remove useless files
	remove(stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/temp/temp_openscad" + cur + ".scad").c_str());
	remove(stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/temp/test" + cur + ".stl").c_str());

	/// Reset state to ready and currentObjectVersion++
	stateTextElement->setText(L"Ready.");
	buildButton->setEnabled(true);
	closeButton->setEnabled(true);
	currentObjectVersion++;
} /// The mutex is released here

bool COpenSCADDocument::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == buildButton) {
                #ifndef _IRR_LINUX_PLATFORM_
				std::thread t(&COpenSCADDocument::buildObject, *this);
				t.detach();
				#else
				buildObject();
				#endif
			}

			if (event.GUIEvent.Caller == closeButton) {
				codeEditor->close();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}
	}

	return false;
}
