//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#if defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "Stdafx.h"
#include <DLLExport.h>
#include "CSSWEUltimateTool.h"

#include "SSWEUltimateTool/UserInterfaces/OpenSCAD/CSSWEUTOpenSCADDocument.h"
#include "SSWEUltimateTool/UserInterfaces/SoundViewer/CSSWEUTSoundViewer.h"

#include <thread>
#include <mutex>
std::mutex cssweultimatetoolm;

using namespace irr;
using namespace gui;
using namespace core;
using namespace scene;
using namespace video;

/// Create the extern method to return a new instance of the plugin (heritates ISSWELibPlugin)
/// The constructor must have any arguments and must be casted as void * (as explained in the DLLExport.h header
ISSWELibPlugin* createSSWELibPlugin() {
	return new CSSWEUltimateTool();
}

/// Constructor
CSSWEUltimateTool::CSSWEUltimateTool() {

}

/// Destructor
CSSWEUltimateTool::~CSSWEUltimateTool() {

}

/// Update method
/// SSWELibPlugin heritates IUpdate located in ISSWECore
/// This update method is called by CP3D automatically and is needed if
/// you want to update some treatments as you can do in a main() function.
void CSSWEUltimateTool::update() {
	//Update Quit Button Position On Window
	quitButton->setRelativePosition(position2di(quitButton->getRelativePosition().UpperLeftCorner.X,
												window->getRelativePosition().getHeight()-25)
								   );

	//Update Main Window Position On Screen
	window->setRelativePosition(rect<s32>(0,
										  73,
										  window->getRelativePosition().getWidth(),
										  devices->getVideoDriver()->getScreenSize().Height-25)
							   );

	//if (devices->getGUIEnvironment()->getFocus() == 0)
	//	devices->getGUIEnvironment()->getRootGUIElement()->bringToFront(window);
}

/// Close the plugin
/// In this function you can remove all the elements you created (gui, scene nodes, etc...)
/// And then the plugin will be destroyed for you by the ISSWELibPlugin using ISSWELibPlugin::close();
void CSSWEUltimateTool::close() {
	window->remove();

	ISSWELibPlugin::close();
}

/// Open the plugin
/// open is a good way to initialize all your gui elements, instances, etc. as below
/// Don't forget to call ISSWELibPlugin::open(); to initialise the events in ISSWELibPlugin
void CSSWEUltimateTool::open() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	//GUI ELEMENTS
	window = gui->addWindow(rect<s32>(40, 20, 122, 160), false, L"Ult Tools", 0, -1);
	window->getMaximizeButton()->setVisible(false);
	window->getMinimizeButton()->setVisible(false);
	window->getCloseButton()->setVisible(false);
	window->setDraggable(false);

	//Terrains
	this->addArea(L"Terrains");
	this->addButton(L"Create a new terrain", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/create_terrain.png"));
	this->addButton(L"Paint the selected terrain", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/paint_terrain.png"));
	this->addButton(L"Edit the selected terrain", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/edit_terrain.png"));

	//Meshes
	this->addArea(L"Meshes");
	addCornerButton = this->addButton(L"Add a corner mesh scene node", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/meshes_corner.png"));
	addCylinderButton = this->addButton(L"Add a corner mesh scene node", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/meshes_cylinder.png"));
	addSuzanneButton = this->addButton(L"Add a corner mesh scene node", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/meshes_suzanne.png"));

	//OpenSCAD
	this->addArea(L"OpenSCAD");
	createNewOpenSCADDocButton = this->addButton(L"Add a new ", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/openscad_new.png"));
	openscadButton = this->addButton(L"Add a new ", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/openscad_logo.png"));
	openscadOpenButton = this->addButton(L"Add a new object from an OpenSCAD file", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/openscad_open.png"));

	//Primitives
	this->addArea(L"Primitives");
	splitSelectedNodeButton = this->addButton(L"Split Mesh into different nodes", devices->getVideoDriver()->getTexture(workingDirectory
		+ "datas/SSWEUltimateTool/GUI/primitives_split.png"));

	this->addArea(L"Sound");
	soundViewerButton = this->addButton(L"Sound Viewer", devices->getVideoDriver()->getTexture(workingDirectory + "datas/SSWEUltimateTool/GUI/sound_viewer.png"));

	quitButton = gui->addButton(rect<s32>(0, 110, 80, 130), window, -1, L"Quit", L"Quit the Ultimate Tool");

	//FINISH PLUGIN
	ISSWELibPlugin::open();

}

void CSSWEUltimateTool::addArea(irr::core::stringw name) {
	u32 pos = getNewElementPosition();
	IGUIStaticText *t = devices->getGUIEnvironment()->addStaticText(name.c_str(), rect<s32>(2, 20, 78, 40), false, true, window, -1, true);
	t->setRelativePosition(position2di(2, pos));
	t->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
	t->setBackgroundColor(SColor(255, 32, 32, 32));
	buttonIsLeftElement = true;
}

IGUIButton *CSSWEUltimateTool::addButton(irr::core::stringw tooltipText, irr::video::ITexture *texture) {
	u32 pos = getNewElementPosition();
	IGUIButton *b = devices->getGUIEnvironment()->addButton(buttonIsLeftElement == true ? rect<s32>(2, 40, 38, 80) : rect<s32>(40, 40, 80, 80),
															window, -1, L"", tooltipText.c_str());
	b->setRelativePosition(position2di(b->getRelativePosition().UpperLeftCorner.X, (buttonIsLeftElement == true) ? pos : pos-45));
	b->setUseAlphaChannel(true);
	b->setScaleImage(true);
	b->setImage(texture);

	/// Switch test
	buttonIsLeftElement = !buttonIsLeftElement;

	return b;
}

u32 CSSWEUltimateTool::getNewElementPosition() {
	s32 pos = 25;
	for(IGUIElement *e : window->getChildren()) {
		if (e->getRelativePosition().LowerRightCorner.Y > pos)
			pos = e->getRelativePosition().LowerRightCorner.Y;
	}
	pos+=5; /// Add an offset
	return (u32)pos;
}

void CSSWEUltimateTool::runOpenSCAD() {
	std::string path = "\"";
    #ifndef _IRR_OSX_PLATFORM_
	path += stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/openscad.exe\"").c_str();
    #else
    path += stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/OpenSCAD.app/Contents/MacOS/OpenSCAD\"").c_str();
    #endif
	system(path.c_str());
}

void CSSWEUltimateTool::buildOpenSCADFromFile() {
	stringc uniqName = openScadFileDialog->getFileName();
	uniqName.remove("/");
	uniqName.remove(":");
	uniqName.remove(".");
	devices->getDevice()->getLogger()->log(uniqName.c_str());

    #ifndef _IRR_OSX_PLATFORM_
	std::system(stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/openscad.exe -o "
		+ workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/" + "temp/test" + uniqName + ".stl "
		+ openScadFileDialog->getFileName()).c_str());
    #else
    stringc ppath = "\"";
    system(stringc(ppath + workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/OpenSCAD.app/Contents/MacOS/OpenSCAD\" -o "
                    + "\"" + workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/" + "temp/test" + uniqName + ".stl\" "
                    + openScadFileDialog->getFileName()).c_str());
    #endif

	/// Add the object
	/// Lock the main thread to add the object to the scene
	std::lock_guard<std::mutex> lck(cssweultimatetoolm);
	IMesh *openscadMesh = devices->getSceneManager()->getMesh(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/temp/test" + uniqName + ".stl");
	if (openscadMesh) {
		IMeshSceneNode *openscadNode = devices->getSceneManager()->addMeshSceneNode(openscadMesh, 0, -1, vector3df(0),
																					vector3df(0), vector3df(1), false);
		openscadNode->setMaterialFlag(EMF_LIGHTING, false);
		openscadNode->setName("#object:new_openscad_node");
		devices->getCollisionManager()->setCollisionFromBoundingBox(openscadNode);
		devices->getCoreData()->addObjectNode(openscadNode, openscadMesh, "corner");
		devices->getXEffect()->addShadowToNode(openscadNode, devices->getXEffectFilterType(), ESM_BOTH);
	}

	remove(stringc(workingDirectory + "datas/SSWEUltimateTool/OpenSCAD/test" + uniqName + ".stl").c_str());
} /// The mutex is released here

bool CSSWEUltimateTool::OnEvent(const SEvent &event) {
	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			//MESHES PART
			if (event.GUIEvent.Caller == addSuzanneButton) {
				/// Add Blender's Suzanne model
				IMeshSceneNode *suzanne = devices->getSceneManager()->addMeshSceneNode(devices->getSceneManager()->getMesh(workingDirectory +
																					   "datas/SSWEUltimateTool/Models/suzanne.obj"),
																					   0, -1, vector3df(0), vector3df(0), vector3df(20), false);
				suzanne->setMaterialFlag(EMF_LIGHTING, false);
				suzanne->setName("#object:new_suzanne_node");
				devices->getCollisionManager()->setCollisionFromBoundingBox(suzanne);
				devices->getCoreData()->addObjectNode(suzanne, suzanne->getMesh(), stringw(workingDirectory +
																				   "datas/SSWEUltimateTool/Models/suzanne.obj")
																				   .remove(devices->getWorkingDirectory()));
				devices->getXEffect()->addShadowToNode(suzanne, devices->getXEffectFilterType(), ESM_BOTH);
			} else
			if (event.GUIEvent.Caller == addCylinderButton) {
				/// Add a cylnder model
				IMesh *cylinderMesh = devices->getSceneManager()->getGeometryCreator()->createCylinderMesh(10, 100, 200);
				IMeshSceneNode *cylinderNode = devices->getSceneManager()->addMeshSceneNode(cylinderMesh, 0, -1, vector3df(0),
																							vector3df(0), vector3df(1), false);
				cylinderNode->setMaterialFlag(EMF_LIGHTING, false);
				cylinderNode->setName("#object:new_cylinder_node");
				devices->getCollisionManager()->setCollisionFromBoundingBox(cylinderNode);
				devices->getCoreData()->addObjectNode(cylinderNode, cylinderNode->getMesh(), "cylinder");
				devices->getXEffect()->addShadowToNode(cylinderNode, devices->getXEffectFilterType(), ESM_BOTH);
			} else
			if (event.GUIEvent.Caller == addCornerButton) {
				/// Add a Corner model
				IMesh *cornerMesh = devices->getSceneManager()->getGeometryCreator()->createConeMesh(10, 100, 200);
				IMeshSceneNode *cornerNode = devices->getSceneManager()->addMeshSceneNode(cornerMesh, 0, -1, vector3df(0),
																						  vector3df(0), vector3df(1), false);
				cornerNode->setMaterialFlag(EMF_LIGHTING, false);
				cornerNode->setName("#object:new_corner_node");
				devices->getCollisionManager()->setCollisionFromBoundingBox(cornerNode);
				devices->getCoreData()->addObjectNode(cornerNode, cornerNode->getMesh(), "corner");
				devices->getXEffect()->addShadowToNode(cornerNode, devices->getXEffectFilterType(), ESM_BOTH);
			} else

			//OPENSCAD
			if (event.GUIEvent.Caller == createNewOpenSCADDocButton) {
				COpenSCADDocument *document = new COpenSCADDocument(devices, workingDirectory);
			} else
			if (event.GUIEvent.Caller == openscadButton) {
                #ifndef _IRR_LINUX_PLATFORM_
				std::thread t(&CSSWEUltimateTool::runOpenSCAD, *this);
				t.detach();
				#else
				runOpenSCAD();
				#endif
			} else
			if (event.GUIEvent.Caller == openscadOpenButton) {
				openScadFileDialog = devices->createFileOpenDialog(L"Choose .scad file", CGUIFileSelector::EFST_OPEN_DIALOG, 0, true);
			} else

			//PRIMITIVES
			if (event.GUIEvent.Caller == splitSelectedNodeButton) {
				/*ISData *data = devices->getSelectedData();
				if (data) {
					ISceneNode *dataNode = data->getNode();
					if (data->getType() == ESNT_OCTREE || data->getType() == ESNT_MESH) {
						for (u32 i=0; i < dataNode->getMaterialCount(); i++) {
							/// Set up mesh
							SMesh *mesh = new SMesh();
							SMeshBuffer *meshBuffer = new SMeshBuffer();
							IMeshBuffer *orBuffer = ((IMeshSceneNode*)dataNode)->getMesh()->getMeshBuffer(i);

							meshBuffer->Vertices.set_used(orBuffer->getVertexCount());
							/// Copy vertices here
							meshBuffer->Indices.set_used(orBuffer->getIndexCount());
							/// Copy indices here

							meshBuffer->Material = orBuffer->getMaterial();
							meshBuffer->BoundingBox = orBuffer->getBoundingBox();
							meshBuffer->MappingHint_Index = orBuffer->getHardwareMappingHint_Index();
							meshBuffer->MappingHint_Vertex = orBuffer->getHardwareMappingHint_Vertex();

							mesh->addMeshBuffer(meshBuffer);

							/// Set up scene node
							IMeshSceneNode *node = devices->getSceneManager()->addMeshSceneNode(mesh, dataNode->getParent(), -1);
							if (!node)
								continue;
							node->setScale(dataNode->getScale());
							node->setPosition(dataNode->getPosition());
							node->setRotation(dataNode->getPosition());
							node->setName(stringc(dataNode->getName()).c_str());
							devices->getCoreData()->addTerrainNode(node, mesh, "");
						}

					}
					//devices->getCoreData()->removeSceneNode(data->getNode(), devices->getXEffect());
				}*/
			} else

			if (event.GUIEvent.Caller == soundViewerButton) {
				CSoundViewer *soundViewer = new CSoundViewer(devices, workingDirectory);
			} else

			//QUIT BUTTON
			if (event.GUIEvent.Caller == quitButton) {
				this->close();
			}
		} else

		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == openScadFileDialog) {
                #ifndef _IRR_LINUX_PLATFORM_
				std::thread t(&CSSWEUltimateTool::buildOpenSCADFromFile, *this);
				t.detach();
				#else
				buildOpenSCADFromFile();
				#endif
			}
		}
	}

	return false;
}
