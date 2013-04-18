#include "CUINodeFactoryPlanarMapping.h"

CUINodeFactoryPlanarMapping::CUINodeFactoryPlanarMapping(CDevices *_devices) {
	devices = _devices;
	devices->getEventReceiver()->AddEventReceiver(this);

	IMeshManipulator *meshMan = devices->getSceneManager()->getMeshManipulator();
}
CUINodeFactoryPlanarMapping::~CUINodeFactoryPlanarMapping() {

}

void CUINodeFactoryPlanarMapping::open(ISceneNode *node, IMesh *mesh) {
	nodeToEdit = node;
	meshToEdit = mesh;

	IGUIEnvironment *gui = devices->getDevice()->getGUIEnvironment();
	//WINDOW
	window = gui->addWindow(rect<s32>(310, 190, 770, 410), false, 
						    stringw(stringw(L"Make Planar Texture Mapping : ") + stringw(node->getName())).c_str(),
							0, -1);
	//WINDOW ASSETS
	gui->addStaticText(L"Horizontal Resolution :", rect<s32>(10, 30, 150, 50), true, true, window, -1, false);
	horizontalResolution = gui->addEditBox(L"0", rect<s32>(150, 30, 320, 50), true, window, -1);

	gui->addStaticText(L"Vertical Resolution :", rect<s32>(10, 60, 150, 80), true, true, window, -1, false);
	verticalResolution = gui->addEditBox(L"0", rect<s32>(150, 60, 320, 80), true, window, -1);

	gui->addStaticText(L"Axis (X, Y or Z) :", rect<s32>(10, 90, 150, 110), true, true, window, -1, false);
	axis = gui->addComboBox(rect<s32>(150, 90, 320, 110), window, -1);
	axis->addItem(L"X");
	axis->addItem(L"Y");
	axis->addItem(L"Z");

	gui->addStaticText(L" Offset X : ", rect<s32>(10, 120, 80, 140), true, true, window, -1, false);
	offsetX = gui->addEditBox(L"0", rect<s32>(80, 120, 150, 140), true, window, -1);
	gui->addStaticText(L" Offset Y : ", rect<s32>(150, 120, 220, 140), true, true, window, -1, false);
	offsetY = gui->addEditBox(L"0", rect<s32>(220, 120, 290, 140), true, window, -1);
	gui->addStaticText(L" Offset Z : ", rect<s32>(290, 120, 360, 140), true, true, window, -1, false);
	offsetZ = gui->addEditBox(L"0", rect<s32>(360, 120, 430, 140), true, window, -1);

	generalPTM = gui->addCheckBox(false, rect<s32>(10, 150, 450, 170), window, -1, L"Apply General (value taken from Horizontal Resolution)");
	//BUTTONS
	accept = gui->addButton(rect<s32>(240, 180, 340, 210), window, -1, L"Ok", L"Apply to the mesh");
	cancel = gui->addButton(rect<s32>(350, 180, 450, 210), window, -1, L"Close", L"Close The Window");
}

bool CUINodeFactoryPlanarMapping::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == cancel) {
				devices->getEventReceiver()->RemoveEventReceiver(this);
				window->remove();
				delete this;
			}
			if (event.GUIEvent.Caller == accept) {
				messageBoxAccept = devices->addInformationDialog(L"Informaitons", L"It will apply all the same parameters and all the buffers\n"
																 L"Continue ?", 0x3, true, window);
			}
		}
		if (event.GUIEvent.EventType == EGET_MESSAGEBOX_OK) {
			if (event.GUIEvent.Caller == messageBoxAccept) {
				IMeshManipulator *meshMap = devices->getSceneManager()->getMeshManipulator();
				f32 hr = devices->getCore()->getF32(stringc(horizontalResolution->getText()).c_str());
				f32 vr = devices->getCore()->getF32(stringc(verticalResolution->getText()).c_str());
				u8 a = axis->getSelected();
				vector3df offset = devices->getCore()->getVector3df(stringw(offsetX->getText()),
																	stringw(offsetY->getText()),
																	stringw(offsetZ->getText()));
				bool general = generalPTM->isChecked();
				if (generalPTM->isChecked()) {
					meshMap->makePlanarTextureMapping(meshToEdit, hr);
				} else {
					for (u32 i=0; i < meshToEdit->getMeshBufferCount(); i++) {
						meshMap->makePlanarTextureMapping(meshToEdit->getMeshBuffer(i), hr, vr, a, offset);
					}
				}

				SPlanarTextureMapping sptm(hr, vr, a, offset, general);
				s32 i = devices->getCoreData()->isMeshPlanared(nodeToEdit);
				if (i != -1) {
					devices->getCoreData()->getPlanarTextureMappingValues()->operator[](i) = sptm;
				} else {
					devices->getCoreData()->getPlanarMeshes()->push_back(nodeToEdit);
					devices->getCoreData()->getPlanarTextureMappingValues()->push_back(sptm);
				}
			}
		}
	}

	return false;
}
