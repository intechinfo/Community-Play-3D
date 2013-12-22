//
//  CUICharacterWindow.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 10/10/12.
//
//

#include "stdafx.h"
#include "CUIEditBones.h"

CUIEditBones::CUIEditBones(CDevices *_devices, IAnimatedMeshSceneNode *_node) {
	//DEVICES
	devices = _devices;
	driver = devices->getVideoDriver();
	smgr = devices->getSceneManager();
	gui = devices->getGUIEnvironment();

	//EFFECT
	effect = new EffectHandler(devices->getDevice(), devices->getDevice()->getVideoModeList()->getDesktopResolution(), true, true, true);
	effect->setActiveSceneManager(smgr);
	effect->addShadowToNode(_node, devices->getXEffectFilterType(), ESM_BOTH);

	//NODES
	node = _node;

	node->animateJoints(true);

	lastBoneSceneNode = 0;

	bonesCloseButton = 0;
	bonesListBox = 0;
	bonesWindow = 0;

	//GUI
	selectedAnimation = 0;
	selectedFrame = 0;
	selectedManualAction = 0;

	arrayFrames.clear();

	updateView = false;

	//INIT SPHERES
	/*if (node) {
		for (u32 i=0; i < node->getJointCount(); i++) {
			ISceneNode *snode = smgr->addSphereSceneNode(5.f, 16, node->getJointNode(i), -1, vector3df(0), vector3df(0), vector3df(3.6*0.02));
			snode->setMaterialFlag(EMF_ZBUFFER, false);
			snode->setMaterialFlag(EMF_ZWRITE_ENABLE, true);
		}
		node->setDebugDataVisible(EDS_SKELETON ^ irr::scene::EDS_BBOX_ALL);
	}*/
}

CUIEditBones::~CUIEditBones() {
	for (u32 i=0; i < arrayFrames.size(); i++) {
		delete arrayFrames[i];
	}
	arrayFrames.clear();
	delete effect;
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
	animations = gui->addListBox(rect<s32>(10, 330, 410, 500), mainWindow, -1, true);

	removeAnimation->setEnabled(false);
	addAnimation->setEnabled(false);
	animations->setEnabled(false);

	//BONES WINDOW
	bonesWindow = gui->addWindow(rect<s32>(driver->getScreenSize().Width-410, 75,
										   driver->getScreenSize().Width, driver->getScreenSize().Height),
										   false, L"Actions & Bones", 0, -1);
	bonesWindow->getCloseButton()->remove();
	//bonesWindow->setDrawTitlebar(false);
	bonesWindow->setDraggable(false);

	//BONES LIST BOX
	gui->addStaticText(L"Bones : ", rect<s32>(10, 50, 400, 70), false, true, bonesWindow, -1, false);
	bonesListBox = gui->addListBox(rect<s32>(10, 70, 400, 180), bonesWindow, -1, true);
	for (u32 i=0; i < node->getJointCount(); i++) {
		bonesListBox->addItem(stringw(node->getJointNode(i)->getBoneName()).c_str());
	}

	gui->addStaticText(L"Manual Actions : ", rect<s32>(10, 180, 360, 200), false, true, bonesWindow, -1, false);
	manualActionslb = gui->addListBox(rect<s32>(10, 200, 400, 280), bonesWindow, -1, true);
	addManualAction = gui->addButton(rect<s32>(360, 180, 380, 200), bonesWindow, -1, L"+", L"Add Manual Actions");
	removeManualActions = gui->addButton(rect<s32>(380, 180, 400, 200), bonesWindow, -1, L"-", L"Remove Manual Action");

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

	gui->addStaticText(L"Time To Do : (ms)", rect<s32>(10, 400, 140, 420), true, true, bonesWindow, -1, true);
	timeToCompletecb = gui->addEditBox(L"", rect<s32>(140, 400, 400, 420), true, bonesWindow, -1);

	//PREDECESSORS
	gui->addStaticText(L"Predecessors :", rect<s32>(10, 430, 260, 450), false, true, bonesWindow, -1, false);
	editBehavior = gui->addButton(rect<s32>(10, 290, 400, 310), bonesWindow, -1, L"Edit Behavior", L"Edit Behavior");
	addPredecessor = gui->addButton(rect<s32>(360, 430, 380, 450), bonesWindow, -1, L"+", L"Add Predecessor");
	removePredecessor = gui->addButton(rect<s32>(380, 430, 400, 450), bonesWindow, -1, L"-", L"Remove Predecessor");
	predecessors = gui->addListBox(rect<s32>(10, 450, 400, 530), bonesWindow, -1, true);

	//INFORMATIONS
	startInfo = gui->addStaticText(L"Start : ", rect<s32>(10, 540, 400, 560), true, true, bonesWindow, -1, true);
	endInfo = gui->addStaticText(L"End   : ", rect<s32>(10, 560, 400, 580), true, true, bonesWindow, -1, true);

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

void CUIEditBones::update() {

	bool updateXEffect = devices->isXEffectDrawable();
	devices->setXEffectDrawable(false);
	while (updateView) {
		devices->reupdate();
	}
	devices->setXEffectDrawable(updateXEffect);
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

void CUIEditBones::resetBonesWindow() {
	manualActionslb->clear();
	predecessors->clear();

	nameeb->setText(L"");
	timeToCompletecb->setText(L"");
	timeToStartcb->setText(L"");

	if (frames->getSelected() != -1 && animations->getSelected() != -1 && bonesListBox->getSelected() != -1) {
		manualActionslb->clear();
		u32 maSize = selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->size();
		for (u32 i=0; i < maSize; i++) {
			CManualAction *ma = selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->operator[](i);
			manualActionslb->addItem(ma->getName().c_str());
		}
		if (maSize > 0) {
			manualActionslb->setSelected(0);
		}
	}
}

void CUIEditBones::previewManualAction() {
	if (selectedManualAction) {
		ISceneNodeAnimator *ac;
		if (selectedManualAction->getType() == EMAT_ROTATION) {
			vector3df rotend = selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getNewRotation();
			vector3df rotbase = selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getRotation();
			ac = new CRotationAnimator(rotbase, rotend, selectedManualAction->getTimeToComplete(),
																selectedManualAction->getSens() == EMAS_POSITIVE, 
																devices->getDevice()->getTimer()->getTime());
		}
		if (ac) {
			node->getJointNode(bonesListBox->getSelected())->addAnimator(ac);
			ac->drop();
		}
	}
}

void CUIEditBones::previewEntireFrame() {
	if (selectedFrame) {
		for (u32 i=0; i < selectedFrame->getAnimations()->size(); i++) {
			CAnimation *an = selectedFrame->getAnimations()->operator[](i);
			for (u32 j=0; j < an->getBonesInformations()->size(); j++) {
				SBoneInformations bi = an->getBonesInformations()->operator[](j);
				for (u32 k=0; k < bi.getManualActions()->size(); k++) {
					CManualAction *ma = bi.getManualActions()->operator[](k);
					ISceneNodeAnimator *ac;
					if (ma->getType() == EMAT_ROTATION) {
						vector3df rotend = bi.getNewRotation();
						vector3df rotbase = bi.getRotation();
						ac = new CRotationAnimator(rotbase, rotend, ma->getTimeToComplete(),
												   ma->getSens() == EMAS_POSITIVE, 
												   devices->getDevice()->getTimer()->getTime());
					}
					if (ac) {
						bi.getNode()->addAnimator(ac);
						ac->drop();
					}
				}
			}
		}
	}
}

bool CUIEditBones::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT || event.EventType == EET_KEY_INPUT_EVENT) {
		if (selectedFrame && selectedAnimation && bonesListBox && manualActionslb) {
			if (bonesListBox->getSelected() != -1 && manualActionslb->getSelected() != -1) {
				CManualAction *ma = selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->operator[](manualActionslb->getSelected());
				stringw infosStart, infosEnd;
				switch (ma->getType()) {
					case EMAT_ROTATION:
						selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).setNewRotation(node->getJointNode(bonesListBox->getSelected())->getRotation());
						infosStart = devices->getCore()->getStrVector3df(selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getRotation());
						infosEnd = devices->getCore()->getStrVector3df(selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getNewRotation());
						break;
					case EMAT_TRANSLATION:
						selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).setNewPosition(node->getJointNode(bonesListBox->getSelected())->getPosition());
						infosStart = devices->getCore()->getStrVector3df(selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getPosition());
						infosEnd = devices->getCore()->getStrVector3df(selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getNewPosition());
						break;
					case EMAT_SCALE:
						selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).setNewScale(node->getJointNode(bonesListBox->getSelected())->getScale());
						infosStart = devices->getCore()->getStrVector3df(selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getScale());
						infosEnd = devices->getCore()->getStrVector3df(selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getNewScale());
						break;
					default:
						break;
				}

				stringw startInfosw = "Start : ";
				startInfosw += infosStart;
				startInfo->setText(startInfosw.c_str());
				stringw endInfosw = "End   : ";
				endInfosw += infosEnd;
				endInfo->setText(endInfosw.c_str());
			}
		}
		if (lastBoneSceneNode) {
			lastBoneSceneNode->updateAbsolutePosition();
			lastBoneSceneNode->updateAbsolutePositionOfAllChildren();
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
				node->getJointNode(bonesListBox->getSelected())->updateAbsolutePosition();
				cobj->setNodeToPlace(node->getJointNode(bonesListBox->getSelected()));
				cobj->setArrowVisible(true);
				node->getJointNode(bonesListBox->getSelected())->setDebugDataVisible(EDS_FULL);
				lastBoneSceneNode = node->getJointNode(bonesListBox->getSelected());

				node->setDebugDataVisible(EDS_SKELETON);

				resetBonesWindow();
			}

			if (listBox == frames) {
				if (frames->getSelected() != -1) {
					selectedFrame = arrayFrames[frames->getSelected()];
					animations->clear();
					for (u32 i=0; i < arrayFrames[frames->getSelected()]->getAnimations()->size(); i++) {
						CAnimation *anim = arrayFrames[frames->getSelected()]->getAnimations()->operator[](i);
						animations->addItem(anim->getName().c_str());
						if (i == 0) {
							animations->setSelected(i);
							selectedAnimation = anim;
						}
					}
					resetBonesWindow();
				}
			}

			if (listBox == manualActionslb) {
				if (manualActionslb->getSelected() != -1 && frames->getSelected() != -1 && animations->getSelected() != -1 && bonesListBox->getSelected() != -1) {
					selectedManualAction = selectedAnimation->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->operator[](manualActionslb->getSelected());
					nameeb->setText(selectedManualAction->getName().c_str());
					typecb->setSelected((E_MANUAL_ACTION_TYPE)selectedManualAction->getType());
					senscb->setSelected((E_MANUAL_ACTION_SENS)selectedManualAction->getSens());
					timeToStartcb->setText(stringw(selectedManualAction->getTimeToStart()).c_str());
					timeToCompletecb->setText(stringw(selectedManualAction->getTimeToComplete()).c_str());
					
					if (manualActionslb->getSelected() != -1 && frames->getSelected() != -1 && animations->getSelected() != -1) {
						predecessors->clear();
						for (u32 i=0; i < selectedManualAction->getPredecessors()->size(); i++) {
							predecessors->addItem(selectedManualAction->getPredecessors()->operator[](i)->getName().c_str());
						}
					}
				}
			}
		}

		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == animations) {
				if (animations->getSelected() != -1) {
					selectedAnimation = (CAnimation *)arrayFrames[frames->getSelected()]->getAnimations()->operator[](animations->getSelected());
					//SET FIRST MANUAL ACTION SELECTED
					bonesListBox->setEnabled(true);
					bonesCloseButton->setEnabled(true);
					addManualAction->setEnabled(true);
					devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, true);
				} else {
					selectedAnimation = 0;
					devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, false);
					bonesCloseButton->setEnabled(true);
				}
				resetBonesWindow();
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
					updateView = false;
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
					animations->clear();
				}
				if (frames->getItemCount() == 0) {
					devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, false);
					bonesCloseButton->setEnabled(true);
					removeAnimation->setEnabled(false);
					addAnimation->setEnabled(false);
					animations->setEnabled(false);
				}
				selectedFrame = 0;
				resetBonesWindow();
			}

			//ANIMATIONS
			if (event.GUIEvent.Caller == addAnimation) {
				if (frames->getSelected() != -1) {
					CAnimation *anim = arrayFrames[frames->getSelected()]->createNewAnimation();
					anim->setName(stringw(stringw("Animation ") + stringw(arrayFrames[frames->getSelected()]->getAnimations()->size())).c_str());
					animations->addItem(anim->getName().c_str());
				} else {
					devices->addInformationDialog(L"Informations", "Please choose a frame before", EMBF_OK, true, 0);
				}
			}

			if (event.GUIEvent.Caller == removeAnimation) {
				//IF REMOVE ENTIRE ANIMATION
				if (animations->getSelected() != -1 && frames->getSelected() != -1) {
					CAnimation *anim = arrayFrames[frames->getSelected()]->getAnimations()->operator[](animations->getSelected());
					arrayFrames[frames->getSelected()]->removeAnimation(anim);
					animations->removeItem(animations->getSelected());
				}
				resetBonesWindow();
			}

			//MANUAL ACTIONS
			if (event.GUIEvent.Caller == addManualAction) {
				if (bonesListBox->getSelected() != -1 && selectedFrame && animations->getSelected() != -1 && selectedAnimation) {
					CFrame *frame = arrayFrames[frames->getSelected()];
					CAnimation *anim = frame->getAnimations()->operator[](animations->getSelected());
					CManualAction *ma = anim->addManualAction();
					anim->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->push_back(ma);
					manualActionslb->addItem(ma->getName().c_str());
					devices->getCore()->deactiveChildrenOfGUIElement(bonesWindow, true);
				}
			}

			if (event.GUIEvent.Caller == removeManualActions) {
				if (bonesListBox->getSelected() != -1 && frames->getSelected() != -1 && selectedAnimation) {
					CFrame *frame = arrayFrames[frames->getSelected()];
					CAnimation *anim = frame->getAnimations()->operator[](animations->getSelected());
					delete anim->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->operator[](manualActionslb->getSelected());
					anim->getBonesInformations()->operator[](bonesListBox->getSelected()).getManualActions()->erase(manualActionslb->getSelected());
					manualActionslb->removeItem(manualActionslb->getSelected());
					resetBonesWindow();
				}
			}

			//PRECEDESSORS
			if (event.GUIEvent.Caller == addPredecessor) {
				if (selectedFrame && selectedAnimation && selectedManualAction) {
					predecessorsWindow = gui->addWindow(rect<s32>(330, 120, 750, 490), true, L"Choose predecessor", 0, -1);
					gui->addStaticText(L"Predecessors : ", rect<s32>(10, 20, 410, 40), true, true, predecessorsWindow, -1, true);
					predecessorstv = gui->addTreeView(rect<s32>(10, 40, 410, 320), predecessorsWindow, -1, true, true, false);
					acceptPredecessors = gui->addButton(rect<s32>(200, 330, 300, 360), predecessorsWindow, -1, L"Accept", L"Accept");
					cancelPredecessors = gui->addButton(rect<s32>(310, 330, 410, 360), predecessorsWindow, -1, L"Cancel", L"Cancel");

					rootPredecessors = predecessorstv->getRoot();
					rootPredecessors = rootPredecessors->addChildBack(L"Confoguration", L"CONGIF");
					rootPredecessors->setExpanded(true);
					for (u32 i=0; i < arrayFrames.size(); i++) {
						IGUITreeViewNode *cframe;
						cframe = rootPredecessors->addChildBack(arrayFrames[i]->getName().c_str(), 0, -1, -1, &arrayFrames[i], 0);
						cframe->setExpanded(true);
						for (u32 animi = 0; animi < arrayFrames[i]->getAnimations()->size(); animi++) {
							IGUITreeViewNode *canim;
							CAnimation *currentAnim = arrayFrames[i]->getAnimations()->operator[](animi);
							canim = cframe->addChildBack(currentAnim->getName().c_str(), 0, -1, -1, &currentAnim, 0);
							for (u32 bii = 0; bii < currentAnim->getBonesInformations()->size(); bii++) {
								SBoneInformations bi = currentAnim->getBonesInformations()->operator[](bii);
								for (u32 manaci = 0; manaci < bi.getManualActions()->size(); manaci++) {
									CManualAction *ma = bi.getManualActions()->operator[](manaci);
									IGUITreeViewNode *cmanim;
									stringw name = stringw(ma->getName()) + stringw(" - ") + stringw(bi.getNode()->getBoneName());
									cmanim = canim->addChildBack(name.c_str(), 0, -1, -1, &ma, 0);
								}
							}
						}
							
					}
				}
			}

			if (event.GUIEvent.Caller == acceptPredecessors) {
				IGUITreeViewNode *selectedPredecessor = predecessorstv->getSelected();
				if (selectedPredecessor->getLevel() == 4) { // 3 ?
					selectedManualAction->getPredecessors()->push_back((CManualAction *)predecessorstv->getSelected()->getData());
					predecessors->addItem(selectedPredecessor->getText());
					predecessorsWindow->remove();
				} else {
					devices->addInformationDialog(L"Informations", L"You must select a manual action...", EMBF_OK, true, predecessorsWindow);
				}
			}
			if (event.GUIEvent.Caller == cancelPredecessors) {
				predecessorsWindow->remove();
			}

			//PREVIEWS
			if (event.GUIEvent.Caller == preview) {
				previewManualAction();
			}
			if (event.GUIEvent.Caller == previewFrame) {
				previewEntireFrame();
			}
		}

		if (event.GUIEvent.EventType == EGET_EDITBOX_CHANGED) {
			if (event.GUIEvent.Caller == nameeb) {
				if (selectedManualAction) {
					selectedManualAction->setName(nameeb->getText());
					manualActionslb->setItem(manualActionslb->getSelected(), selectedManualAction->getName().c_str(), -1);
				}
			}
			if (event.GUIEvent.Caller == timeToStartcb) {
				if (selectedManualAction) {
					selectedManualAction->setTimeToStart(devices->getCore()->getF32(stringc(timeToStartcb->getText()).c_str()));
				}
			}
			if (event.GUIEvent.Caller == timeToCompletecb) {
				if (selectedManualAction) {
					selectedManualAction->setTimeToComplete(devices->getCore()->getF32(stringc(timeToCompletecb->getText()).c_str()));
				}
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
				if (selectedManualAction) {
					selectedManualAction->setType((E_MANUAL_ACTION_TYPE)typecb->getSelected());
				}
			}

			if (event.GUIEvent.Caller == senscb) {
				if (selectedManualAction) {
					selectedManualAction->setSens((E_MANUAL_ACTION_SENS)senscb->getSelected());
				}
			}
		}
	}

	return false;
}
