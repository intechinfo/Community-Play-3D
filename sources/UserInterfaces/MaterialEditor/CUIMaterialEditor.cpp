//
//  CUIMaterialEditor.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/03/13.
//
//

#include "CUIMaterialEditor.h"

CUIMaterialEditor::CUIMaterialEditor(CDevices *_devices) {
    devices = _devices;
    
    driver = devices->getVideoDriver();
    smgr = devices->getSceneManager();
    gui = devices->getGUIEnvironment();
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    selectedMaterial = 0;

	mtFactory = new CMaterialEditorFactory(devices);
}

CUIMaterialEditor::~CUIMaterialEditor() {
    lastTabTextColor = NULL;

	scbars.clear();
    scbars = NULL;

	initialElementsPosition.clear();
	initialElementsPosition = NULL;
    
	delete matEditorTextures;
	delete matEditorColors;
	delete matEditorFlags;

    devices->getEventReceiver()->RemoveEventReceiver(this);
}

void CUIMaterialEditor::maximize() {
	if (meWindow->getRelativePosition().getWidth() == driver->getScreenSize().Width+2) {
		materialsView->getRoot()->clearChilds();
		materialsView->remove();
		materialsView = gui->addTreeView(rect<s32>(10, 70, 280, 540), meWindow, -1, true, true, false);

		meWindow->setRelativePosition(rect<s32>(driver->getScreenSize().Width/2 - 1150/2, driver->getScreenSize().Height/2 - 730/2, 
											    driver->getScreenSize().Width/2 + 1150/2, driver->getScreenSize().Height/2 + 730/2));
		tabctrl->setRelativePosition(rect<int>(310, 70, 1140, 680));
		close->setRelativePosition(rect<s32>(1040, 690, 1140, 720));
		viewPort->setRelativePosition(rect<s32>(10, 550, 280, 710));
		separatorText->setRelativePosition(rect<s32>(290, 70, 300, 710));
		meWindow->setDraggable(true);
	} else {
		position2d<s32> positionTabCtrl = tabctrl->getRelativePosition().UpperLeftCorner;
		s32 offsetWindowTabCtrl = meWindow->getRelativePosition().getWidth() - positionTabCtrl.X - tabctrl->getRelativePosition().getWidth();
		s32 offsetLeftViewTabCtrl = positionTabCtrl.X - viewPort->getRelativePosition().getWidth();
		s32 lastWindowHeight = meWindow->getRelativePosition().getHeight();
		s32 offsetUpperLower = materialsView->getRelativePosition().UpperLeftCorner.Y;

		meWindow->setRelativePosition(rect<s32>(0, 74, driver->getScreenSize().Width+2, driver->getScreenSize().Height+1));
		tabctrl->setRelativePosition(rect<s32>(meWindow->getRelativePosition().getWidth()-offsetWindowTabCtrl-tabctrl->getRelativePosition().getWidth(),
											   positionTabCtrl.Y,
											   meWindow->getRelativePosition().getWidth()-offsetWindowTabCtrl,
											   meWindow->getRelativePosition().getHeight()-50));
		separatorText->setRelativePosition(rect<s32>(tabctrl->getRelativePosition().UpperLeftCorner.X - offsetLeftViewTabCtrl, 
													 separatorText->getRelativePosition().UpperLeftCorner.Y,
													 tabctrl->getRelativePosition().UpperLeftCorner.X - offsetLeftViewTabCtrl + 10, 
													 meWindow->getRelativePosition().getHeight()-50));
		viewPort->setRelativePosition(rect<s32>(viewPort->getRelativePosition().UpperLeftCorner.X,
												//viewPort->getRelativePosition().UpperLeftCorner.Y, 
												meWindow->getRelativePosition().getHeight()/2 + offsetUpperLower - 50,
												separatorText->getRelativePosition().UpperLeftCorner.X - 20,
												meWindow->getRelativePosition().getHeight()-5));
		close->setRelativePosition(position2di(meWindow->getRelativePosition().getWidth()-110,
											   meWindow->getRelativePosition().getHeight()-40));

		rect<s32> materialsViewRect = materialsView->getRelativePosition();
		//materialsView->getRoot()->clearChilds();
		materialsView->remove();
		materialsView = gui->addTreeView(rect<s32>(materialsViewRect.UpperLeftCorner.X,
												   materialsViewRect.UpperLeftCorner.Y,
												   separatorText->getRelativePosition().UpperLeftCorner.X-20,
												   materialsViewRect.UpperLeftCorner.Y + 
												   //materialsViewRect.getHeight()), 
												   meWindow->getRelativePosition().getHeight()/2 - 55),
										 meWindow, -1, true, true, false);

		meWindow->setDraggable(false);
	}
	mtFactory->reupdateTreeView(nodeToEdit, materialsView, rootTreeViewNode, imageList);
}

void CUIMaterialEditor::minimize() {
	if (meWindow->getRelativePosition().getHeight() == 20) {
		maximize();
		materialsView->remove();
		materialsView = gui->addTreeView(rect<s32>(10, 70, 280, 540), meWindow, -1, true, true, false);
		mtFactory->reupdateTreeView(nodeToEdit, materialsView, rootTreeViewNode, imageList);
		meWindow->setRelativePosition(rect<s32>(driver->getScreenSize().Width/2 - 1150/2, driver->getScreenSize().Height/2 - 730/2, 
											    driver->getScreenSize().Width/2 + 1150/2, driver->getScreenSize().Height/2 + 730/2));
	} else {
		rect<s32> rpWindow = meWindow->getRelativePosition();
		meWindow->setRelativePosition(rect<s32>(0, driver->getScreenSize().Height - 36, 
												driver->getScreenSize().Width+2, driver->getScreenSize().Height-16));
	}
}

void CUIMaterialEditor::open(ISceneNode *node) {
	if (!node)
		return;

    nodeToEdit = node;
    
    //WINDOW
    meWindow = gui->addWindow(rect<s32>(driver->getScreenSize().Width/2 - 1150/2, driver->getScreenSize().Height/2 - 730/2, 
                                        driver->getScreenSize().Width/2 + 1150/2, driver->getScreenSize().Height/2 + 730/2), 
										false, stringw(stringw(L"Material Editor : ") + stringw(node->getName())).c_str(), 0, -1);
	//meWindow->getCloseButton()->remove();
	meWindow->getMinimizeButton()->setVisible(true);
	meWindow->getMaximizeButton()->setVisible(true);
	mtFactory->setParentGUIElement(meWindow);
    
    //CONTEXT MENU
    menu = gui->addMenu(meWindow);
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);
	menu->addItem(L"Factory", -1, true, true);
    
	menu->getSubMenu(0)->addItem(L"Open Saved Materials", 1);
    menu->getSubMenu(0)->addSeparator();
	menu->getSubMenu(0)->addItem(L"Save", 2);
	menu->getSubMenu(0)->addSeparator();
	menu->getSubMenu(0)->addItem(L"Close", 3);
    
	menu->getSubMenu(1)->addItem(L"Restore", 1);

	menu->getSubMenu(2)->addItem(L"Make And Create All Texture Layer 2 Normal Map", 1);
	menu->getSubMenu(2)->addItem(L"Make All Texture Layer 2 Normal Map", 2);
	menu->getSubMenu(2)->addSeparator();
	menu->getSubMenu(2)->addItem(L"Set All Emissive Color To Values...", -1);
	menu->getSubMenu(2)->addItem(L"Set All Specular Color To Values...", -1);
	menu->getSubMenu(2)->addItem(L"Set All Diffuse Color To Values...", -1);
	menu->getSubMenu(2)->addItem(L"Set All Ambiant Color To Values...", -1);
    
    //TOOL BAR
    bar = devices->getGUIEnvironment()->addToolBar(meWindow, -1);
    bar->setRelativePosition(position2di(0, 40));
    bar->setRelativePosition(rect<s32>(0, 43, 795, 70));
    
    ITexture* image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/folder.png"));
	bar->addButton(-1, 0, L"Open Saved Animation Configuration", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/edit.png"));
	bar->addButton(-1, 0, L"Edit The Node", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/save.png"));
	bar->addButton(0, 0, L"Export this scene", image, 0, false, true);
	image = devices->getVideoDriver()->getTexture(devices->getWorkingDirectory() + stringc("/GUI/help.png"));
	bar->addButton(0, 0, L"Open Help", image, 0, false, true);
    
    //TREE VIEW
    materialsView = gui->addTreeView(rect<s32>(10, 70, 280, 540), meWindow, -1, true, true, false);
    
    imageList = devices->getGUIEnvironment()->createImageList(driver->getTexture(stringc(devices->getWorkingDirectory().c_str()) +
                                                                                                stringc("GUI/iconlist.png")), 
                                                                             dimension2d<int>(16, 16), true);
	if (imageList) {
		materialsView->setImageList(imageList);
	}
    
    rootTreeViewNode = materialsView->getRoot()->addChildBack((stringw("Materials (") + stringw(node->getMaterialCount())
                                                              + stringw(")")).c_str(), 0);
    rootTreeViewNode->setExpanded(true);
    for (u32 i=0; i < node->getMaterialCount(); i++) {
        stringw name = L"Material ";
        name += i+1;
        IGUITreeViewNode *treeNode = rootTreeViewNode->addChildBack(name.c_str(), 0, 10);
        treeNode->setData(&node->getMaterial(i));
    }
    rootTreeViewNode->getFirstChild()->setSelected(true);
    
    //VIEW PORT
    viewPort = new CGUIViewport(gui, meWindow, 1, rect<s32>(10, 550, 280, 710), false); 
    if (viewPort) {
        viewPort->setSceneManager(smgr);
        viewPort->setOverrideColor(SColor(255, 0, 0, 0)); 
    }
    
    separatorText = gui->addStaticText(L"", rect<s32>(290, 70, 300, 710), true, true, meWindow, -1, false);
    
    //TABS
    tabctrl = gui->addTabControl(rect<int>(310, 70, 1140, 680), meWindow, true, true, 0);
    texturesTab = tabctrl->addTab(L"Textures");
    colorsTab = tabctrl->addTab(L"Colors");
	flagsTab = tabctrl->addTab(L"Flags & Params");
    
    lastTabTextColor = texturesTab->getTextColor();
    texturesTab->setTextColor(SColor(255, 255, 255, 255));
    
    //---------------------------------------------------------------------------------------------------------
	//GENERAL TAB
    matEditorTextures = new CUIMaterialEditorTextures(devices, texturesTab, nodeToEdit);
    matEditorTextures->update(&node->getMaterial(0));

	matEditorColors = new CUIMaterialEditorColors(devices, colorsTab, nodeToEdit);
	matEditorColors->update(&node->getMaterial(0));

	matEditorFlags = new CUIMaterialEditorFlags(devices, flagsTab, nodeToEdit);
	matEditorFlags->update(&node->getMaterial(0));
	if (nodeToEdit->getMaterialCount() == 1) {
		nodeToEdit->setMaterialFlag(EMF_WIREFRAME, false);
	}
    //---------------------------------------------------------------------------------------------------------
	tabctrl->setActiveTab(texturesTab);
    //---------------------------------------------------------------------------------------------------------
	//MANAGE SCROOL BARS
    for (u32 i=0; i < tabctrl->getTabCount(); i++) {
        IGUITab *tab = tabctrl->getTab(i);
        core::list<IGUIElement *>::ConstIterator element = tab->getChildren().begin();
        u32 maxPosition = 0;
		array<position2di> initialPositions;
        for (; element != tab->getChildren().end(); ++element) {
            IGUIElement *it = (IGUIElement *)*element;
            if ((it->getRelativePosition().UpperLeftCorner.Y + it->getRelativePosition().getHeight()) > tab->getRelativePosition().getHeight()) {
                maxPosition = it->getRelativePosition().UpperLeftCorner.Y + it->getRelativePosition().getHeight() - tab->getRelativePosition().getHeight();
            }
			initialPositions.push_back(it->getRelativePosition().UpperLeftCorner);
        }
		initialElementsPosition.push_back(initialPositions);
        IGUIScrollBar *scrollbar = gui->addScrollBar(false, rect<s32>(812, 0, 829, 573), tab, -1);
        scbars.push_back(scrollbar);
        tab->addChild(scrollbar);
        scrollbar->setMin(0);
        scrollbar->setMax(maxPosition+50);
		scrollbar->setText(L"editor:material_editor_scroll_bar_name");
    }
    //---------------------------------------------------------------------------------------------------------
    
    close = gui->addButton(rect<s32>(1040, 690, 1140, 720), meWindow, -1, L"Close", L"Close The Window");
}

void CUIMaterialEditor::updateElementsPositionsI(IGUIScrollBar *scroolbar) {
	IGUITab *tab = tabctrl->getTab(tabctrl->getActiveTab());
	core::list<IGUIElement *>::ConstIterator element = tab->getChildren().begin();
	u32 j = 0;
	for (; element != tab->getChildren().end(); ++element) {
		IGUIElement *it = (IGUIElement *)*element;
		if (it != scroolbar && it->getType() != EGUIET_FILE_OPEN_DIALOG) {
			it->setRelativePosition(position2di(initialElementsPosition[tab->getNumber()][j].X, 
												initialElementsPosition[tab->getNumber()][j].Y - scroolbar->getPos()));
		}
		j++;
	}
}

void CUIMaterialEditor::update() {
	selectedMaterial = (SMaterial *)materialsView->getSelected()->getData();
    restoreMaterial = (SMaterial *)materialsView->getSelected()->getData();
                    
    matEditorTextures->update(selectedMaterial);
	matEditorColors->update(selectedMaterial);
	matEditorFlags->update(selectedMaterial);
}

bool CUIMaterialEditor::OnEvent(const SEvent &event) {
    
	if (event.EventType == EET_USER_EVENT) {
		if (event.UserEvent.UserData1 == ECUE_REACTIVE_MINIMIZED_WINDOW) {
			if (event.UserEvent.UserData2 == meWindow->getReferenceCount()) {
				devices->getEventReceiver()->RemoveMinimizedWindow(this);
			}
		}
	}

    if (event.EventType == EET_GUI_EVENT) {
		//IF MENU SELECTED
		if (event.GUIEvent.EventType == EGET_MENU_ITEM_SELECTED) {
			IGUIContextMenu *tempMenu = (IGUIContextMenu *)event.GUIEvent.Caller;
			if (tempMenu == menu->getSubMenu(2)) {
				s32 id = tempMenu->getItemCommandId(tempMenu->getSelectedItem());
				if (id == 1) {
					mtFactory->setCreateAllTextureLayer2NormalMapped(nodeToEdit);
				}
				if (id == 2) {
					mtFactory->setAllTextureLayer2NormalMapped(nodeToEdit, 9.0f);
				}
				update();
			}
		}

		//IF WINDOW CLOSE
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			SEvent ev;
			ev.EventType = EET_GUI_EVENT;
			ev.GUIEvent.EventType = EGET_BUTTON_CLICKED;
			ev.GUIEvent.Caller = close;
			ev.GUIEvent.Element = close;
			OnEvent(ev);
		}

        //IF BUTTON CLICKED
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (meWindow) {
				if (event.GUIEvent.Caller == meWindow->getMaximizeButton()) {
					maximize();
				}
				if (event.GUIEvent.Caller == meWindow->getMinimizeButton()) {
					devices->getEventReceiver()->AddMinimizedWindow(this, meWindow);
				}
			}

            if (event.GUIEvent.Caller == close) {
                nodeToEdit->setMaterialFlag(EMF_WIREFRAME, false);
                
                meWindow->remove();
				meWindow = 0;

				devices->getEventReceiver()->RemoveEventReceiver(this);
                delete this;
            }
        }
        
        //IF TREE VIEW
        if (event.GUIEvent.EventType == EGET_TREEVIEW_NODE_SELECT) {
            if (event.GUIEvent.Caller == materialsView) {
                if (materialsView->getSelected()->getParent() == rootTreeViewNode) {
                    update();
                }
            }
        }
        
        //IF TAB CONTROLLER CHANGED
        if (event.GUIEvent.EventType == EGET_TAB_CHANGED) {
            if (event.GUIEvent.Caller == tabctrl) {
                for (u32 i=0; i < tabctrl->getTabCount(); i++) {
                    tabctrl->getTab(i)->setTextColor(gui->getSkin()->getColor(EGDC_BUTTON_TEXT));
                }
                tabctrl->getTab(tabctrl->getActiveTab())->setTextColor(SColor(255, 255, 255, 255));
            }
        }
        
        //IF A SCROLL BAR CHANGED
        if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
            for (u32 i=0; i < scbars.size(); i++) {
                if (event.GUIEvent.Caller == scbars[i]) {
					updateElementsPositionsI(scbars[i]);
					break;
                }
            }
        }
    }

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			IGUITab *currentTab = tabctrl->getTab(tabctrl->getActiveTab());
			if (devices->getGUIEnvironment()->getFocus()) {
				IGUIElement *focus = devices->getGUIEnvironment()->getFocus();
				if (focus->getParent() == currentTab || focus == currentTab) {
					core::list<IGUIElement *>::ConstIterator elements = currentTab->getChildren().begin();
					for (; elements != currentTab->getChildren().end(); ++elements) {
						for (u32 sci=0; sci < scbars.size(); sci++) {
							if (*elements == scbars[sci]) {
								scbars[sci]->setPos(scbars[sci]->getPos() - event.MouseInput.Wheel);
								updateElementsPositionsI(scbars[sci]);
								break;
							}
						}
					}
				}
			}
		}
	}
    
    return false;
}
