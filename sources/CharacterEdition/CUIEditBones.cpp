//
//  CUICharacterWindow.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#include "CUIEditBones.h"

CUIEditBones::CUIEditBones(CDevices *_devices, IAnimatedMeshSceneNode *_node) {
	devices = _devices;
	driver = devices->getVideoDriver();
	smgr = devices->getSceneManager();
	gui = devices->getGUIEnvironment();

	node = _node;

	node->animateJoints(true);

	lastBoneSceneNode = 0;

	bonesCloseButton = 0;
	bonesListBox = 0;
	bonesWindow = 0;

	arrayFrames.clear();
}

CUIEditBones::~CUIEditBones() {

}

void CUIEditBones::open() {
	//MAIN WINDOW
	mainWindow = gui->addWindow(rect<s32>(10, 80, 430, 590), false, L"Frames & Animations", 0, -1);
	mainWindow->getCloseButton()->remove();
	gui->addStaticText(L"Frames : ", rect<s32>(10, 40, 370, 60), true, true, mainWindow, -1, true);
	addFrame = gui->addButton(rect<s32>(370, 40, 390, 60), mainWindow, -1, L"+", L"Add Frame");
	removeFrame = gui->addButton(rect<s32>(390, 40, 410, 60), mainWindow, -1, L"-", L"Remove Selected Frame");
	frames = gui->addListBox(rect<s32>(10, 60, 410, 270), mainWindow, -1, true);

	previewFrame = gui->addButton(rect<s32>(10, 280, 410, 300), mainWindow, -1, L"Preview", L"Preview the frame");

	gui->addStaticText(L"Animations : ", rect<s32>(10, 310, 370, 330), true, true, mainWindow, -1, true);
	addAnimation = gui->addButton(rect<s32>(370, 310, 390, 330), mainWindow, -1, L"+", L"Add Animation");
	removeAnimation = gui->addButton(rect<s32>(390, 310, 410, 330), mainWindow, -1, L"-", L"Remove Selected Animation");

	//animations = gui->addListBox(rect<s32>(10, 330, 410, 500), mainWindow, -1, true);
	animations = gui->addTreeView(rect<s32>(10, 330, 410, 500), mainWindow, -1, true, true, true);
	rootAnimations = animations->getRoot()->addChildBack(L"Animations", L"ANIM");
	rootAnimations->setExpanded(true);

	removeAnimation->setEnabled(false);
	addAnimation->setEnabled(false);
	animations->setEnabled(false);

	//BONES WINDOW
	bonesWindow = gui->addWindow(rect<s32>(driver->getScreenSize().Width-410, 75,
										   driver->getScreenSize().Width, driver->getScreenSize().Height),
										   false, L"Bones", 0, -1);
	bonesWindow->getCloseButton()->remove();
	bonesWindow->setDrawTitlebar(false);
	bonesWindow->setDraggable(false);

	//BONES LIST BOX
	gui->addStaticText(L"Bones : ", rect<s32>(10, 50, 400, 70), false, true, bonesWindow, -1, false);
	bonesListBox = gui->addListBox(rect<s32>(10, 70, 400, 280), bonesWindow, -1, true);
	for (u32 i=0; i < node->getJointCount(); i++) {
		bonesListBox->addItem(stringw(node->getJointNode(i)->getBoneName()).c_str());
	}

	//ANIMATION VALUES (SENS, TYPE, NAME, ETC.)
	gui->addStaticText(L"Name :", rect<s32>(10, 320, 140, 340), true, true, bonesWindow, -1, true);
	nameeb = gui->addEditBox(L"", rect<s32>(140, 320, 400, 340), true, bonesWindow, -1);

	gui->addStaticText(L"Type :", rect<s32>(10, 340, 140, 360), true, true, bonesWindow, -1, true);
	typecb = gui->addComboBox(rect<s32>(140, 340, 400, 360), bonesWindow, -1);
	typecb->addItem(L"Rotation");
	typecb->addItem(L"Translation");
	typecb->addItem(L"Scale");

	gui->addStaticText(L"Sens :", rect<s32>(10, 360, 140, 380), true, true, bonesWindow, -1, true);
	senscb = gui->addComboBox(rect<s32>(140, 360, 400, 380), bonesWindow, -1);
	senscb->addItem(L"Positive");
	senscb->addItem(L"Negative");
	senscb->addItem(L"Unknown");

	gui->addStaticText(L"Time To Start :", rect<s32>(10, 380, 140, 400), true, true, bonesWindow, -1, true);
	timeToStartcb = gui->addEditBox(L"", rect<s32>(140, 380, 400, 400), true, bonesWindow, -1);

	gui->addStaticText(L"Time To Complete :", rect<s32>(10, 400, 140, 420), true, true, bonesWindow, -1, true);
	timeToCompletecb = gui->addEditBox(L"", rect<s32>(140, 400, 400, 420), true, bonesWindow, -1);

	//PREDECESSORS
	gui->addStaticText(L"Predecessors :", rect<s32>(10, 430, 260, 450), false, true, bonesWindow, -1, false);
	editBehavior = gui->addButton(rect<s32>(10, 290, 400, 310), bonesWindow, -1, L"Edit Behavior", L"Edit Behavior");
	addPredecessor = gui->addButton(rect<s32>(360, 430, 380, 450), bonesWindow, -1, L"+", L"Add Predecessor");
	removePredecessor = gui->addButton(rect<s32>(380, 430, 400, 450), bonesWindow, -1, L"-", L"Remove Predecessor");
	predecessors = gui->addListBox(rect<s32>(10, 450, 400, 530), bonesWindow, -1, true);

	//INFORMATIONS
	startInfo = gui->addStaticText(L"Predecessors :", rect<s32>(10, 540, 400, 560), true, true, bonesWindow, -1, true);
	endInfo = gui->addStaticText(L"Predecessors :", rect<s32>(10, 560, 400, 580), true, true, bonesWindow, -1, true);

	//PREVIEW
	preview = gui->addButton(rect<s32>(10, 590, 400, 610), bonesWindow, -1, L"Preview", L"Preview the animation");

	devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, false);

	//CLOSE BUTTON
	bonesCloseButton = gui->addButton(rect<s32>(5, bonesWindow->getRelativePosition().getHeight()-40,
												80, bonesWindow->getRelativePosition().getHeight() - 10), 
									  bonesWindow, -1, L"Finish", L"Finish");

	menu = gui->addMenu();
	menu->addItem(L"Bones Edition", -1, false, false, false, false);
	int i = 1;

	menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);
	menu->addItem(L"View", -1, true, true);
	menu->addItem(L"Help", -1, true, true);

	filem = menu->getSubMenu(i++);
	filem->addItem(L"Open...", 1);
	filem->addItem(L"Save...", 2);
	filem->addSeparator();
	filem->addItem(L"Quit", 3);

	editm = menu->getSubMenu(i++);
	editm->addItem(L"Edit Node...", 1);
	editm->addItem(L"Edit Selected Bone Node", 2);

	bonesm = menu->getSubMenu(i++);
	bonesm->addItem(L"Preview Frame", 1);
	bonesm->addItem(L"Preview All Frames", 2);

	helpm = menu->getSubMenu(i++);
	helpm->addItem(L"How to ?", 1);

}

void CUIEditBones::close() {
	devices->getObjectPlacement()->setNodeToPlace(0);
	devices->getObjectPlacement()->setArrowVisible(false);

	if (bonesWindow) {
		//bonesCloseButton->remove();
		//bonesListBox->remove();
		menu->remove();
		menu = 0;
		
		mainWindow->remove();
		frames = 0;
		animations = 0;
		mainWindow = 0;

		bonesWindow->remove();
		bonesWindow = 0;
		bonesListBox = 0;
		bonesCloseButton = 0;
	}
}

bool CUIEditBones::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT || event.EventType == EET_KEY_INPUT_EVENT) {
		if (frames && bonesListBox) {
			if (arrayFrames.size() != 0 && frames->getSelected() != -1 && bonesListBox->getSelected() != -1) {

				/*arrayFrames[frames->getSelected()]->getBonesInformations()->operator[](bonesListBox->getSelected()).setNewRotation(node->getJointNode(bonesListBox->getSelected())->getRotation());

				stringw startInfosw = "Start Info : ";
				startInfosw += devices->getCore()->getStrVector3df(arrayFrames[frames->getSelected()]->getBonesInformations()->operator[](bonesListBox->getSelected()).getRotation());
				startInfo->setText(startInfosw.c_str());
				stringw endInfosw = "End Info   : ";
				endInfosw += devices->getCore()->getStrVector3df(arrayFrames[frames->getSelected()]->getBonesInformations()->operator[](bonesListBox->getSelected()).getNewRotation());
				endInfo->setText(endInfosw.c_str());*/
			}
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
            IGUIListBox *listBox = (IGUIListBox *)event.GUIEvent.Caller;

			if (listBox == bonesListBox) {
				if (lastBoneSceneNode) {
					lastBoneSceneNode->updateAbsolutePosition();
					lastBoneSceneNode->updateAbsolutePositionOfAllChildren();
				}
				CCoreObjectPlacement *cobj = devices->getObjectPlacement();
				cobj->setNodeToPlace(node->getJointNode(bonesListBox->getSelected()));
				cobj->setArrowVisible(true);
				node->getJointNode(bonesListBox->getSelected())->setDebugDataVisible(EDS_FULL);
				lastBoneSceneNode = node->getJointNode(bonesListBox->getSelected());

				node->setDebugDataVisible(EDS_SKELETON);
			}

			if (listBox == frames) {
				selectedFrame = arrayFrames[frames->getSelected()];
				rootAnimations->clearChilds();
				for (u32 i=0; i < arrayFrames[frames->getSelected()]->getAnimations()->size(); i++) {
					CAnimation *anim = arrayFrames[frames->getSelected()]->getAnimations()->operator[](i);
					IGUITreeViewNode *newAnimation = rootAnimations->addChildBack(anim->getName().c_str(), L"ANIM", -1, -1, anim);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_TREEVIEW_NODE_SELECT) {
			if (event.GUIEvent.Caller == animations) {
				IGUITreeViewNode *selectedAnim = animations->getSelected();
				if (selectedAnim->getParent() == rootAnimations) {
					selectedAnimation = (CAnimation *)animations->getSelected();
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			if (event.GUIEvent.Caller == filem) {
				u32 id = filem->getItemCommandId(filem->getSelectedItem());

				if (id == 3) {
					SEvent nevent;
					nevent.EventType = EET_GUI_EVENT;
					nevent.GUIEvent.EventType = EGET_BUTTON_CLICKED;
					nevent.GUIEvent.Caller = bonesCloseButton;
					nevent.GUIEvent.Element = bonesCloseButton;
					devices->getEventReceiver()->OnEvent(nevent);
				}
			}

			if (event.GUIEvent.Caller == editm) {
				u32 id = editm->getItemCommandId(editm->getSelectedItem());

				if (id == 1) {
					CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
					editNode->open(node, "#object");
				}

				if (id == 2) {
					CUIWindowEditNode *editNode = new CUIWindowEditNode(devices);
					editNode->open(node->getJointNode(bonesListBox->getSelected()), "#object");
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			//FRAMES
			if (event.GUIEvent.Caller == addFrame) {
				CFrame *fr = new CFrame(node);
				fr->setName(stringw(stringw("Frame ") + stringw(arrayFrames.size())).c_str());
				arrayFrames.push_back(fr);
				frames->addItem(fr->getName().c_str());
				removeAnimation->setEnabled(true);
				addAnimation->setEnabled(true);
				animations->setEnabled(true);
			}

			if (event.GUIEvent.Caller == removeFrame) {
				if (frames->getSelected() != -1) {
					delete arrayFrames[frames->getSelected()];
					arrayFrames.erase(frames->getSelected());
					frames->removeItem(frames->getSelected());
				}
				if (frames->getItemCount() == 0) {
					devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, false);
					bonesCloseButton->setEnabled(true);
					removeAnimation->setEnabled(false);
					addAnimation->setEnabled(false);
					animations->setEnabled(false);
				}
			}

			//ANIMATIONS
			if (event.GUIEvent.Caller == addAnimation) {
				if (frames->getSelected() != -1) {
					CAnimation *anim = arrayFrames[frames->getSelected()]->createNewAnimation();
					anim->setName(stringw(stringw("Animation ") + stringw(arrayFrames[frames->getSelected()]->getAnimations()->size())).c_str());
					rootAnimations->addChildBack(anim->getName().c_str(), L"ANIM", -1, -1, anim);
					devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, true);
				} else {
					devices->addInformationDialog(L"Informations", "Please choose a frame before", EMBF_OK, true, 0);
				}
			}

			if (event.GUIEvent.Caller == removeAnimation) {
				IGUITreeViewNode *selectedAnim = animations->getSelected();
				//IF REMOVE ENTIRE ANIMATION
				if (selectedAnim->getParent() == rootAnimations) {
					CAnimation *anim = (CAnimation *)selectedAnim->getData();
					arrayFrames[frames->getSelected()]->removeAnimation(anim);
					rootAnimations->deleteChild(selectedAnim);
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller == nameeb) {
				CFrame *frame = arrayFrames[frames->getSelected()];
				//frame->getBonesInformations()->operator[](bonesListBox->getSelected()).setName(nameeb->getText());
			}
		}

		if (event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED) {
			if (event.GUIEvent.Caller == typecb) {
				if (typecb->getSelected() != EMAT_ROTATION) {
					senscb->setEnabled(false);
					senscb->setSelected(EMAS_UNKNOWN);
				} else {
					senscb->setEnabled(true);
					senscb->setSelected(EMAS_NEGATIVE);
				}
			}
		}
	}

	return false;
}
