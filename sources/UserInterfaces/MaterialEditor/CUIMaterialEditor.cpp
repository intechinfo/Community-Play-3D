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
}

CUIMaterialEditor::~CUIMaterialEditor() {
    lastTabTextColor = NULL;
    scbars = NULL;
    lastScrollBarPos = NULL;
    
    devices->getEventReceiver()->RemoveEventReceiver(this);
}

void CUIMaterialEditor::open(ISceneNode *node) {
    nodeToEdit = node;
    
    //WINDOW
    meWindow = gui->addWindow(rect<s32>(driver->getScreenSize().Width/2 - 1150/2, driver->getScreenSize().Height/2 - 730/2, 
                                        driver->getScreenSize().Width/2 + 1150/2, driver->getScreenSize().Height/2 + 730/2), 
                              false, L"Material Editor", 0, -1);
    
    //CONTEXT MENU
    menu = gui->addMenu(meWindow);
    menu->addItem(L"File", -1, true, true);
	menu->addItem(L"Edit", -1, true, true);
    
	menu->getSubMenu(0)->addItem(L"Open Saved Materials", -1);
    menu->getSubMenu(0)->addSeparator();
	menu->getSubMenu(0)->addItem(L"Save", -1);
	menu->getSubMenu(0)->addSeparator();
	menu->getSubMenu(0)->addItem(L"Close", -1);
    
	menu->getSubMenu(1)->addItem(L"Restore", -1);
    
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
    
    IGUIImageList* imageList = devices->getGUIEnvironment()->createImageList(driver->getTexture(stringc(devices->getWorkingDirectory().c_str()) +
                                                                                                stringc("GUI/iconlist.png")), 
                                                                             dimension2d<int>(16, 16), true);
	if (imageList) {
		materialsView->setImageList(imageList);
		imageList->drop();
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
    
    gui->addStaticText(L"", rect<s32>(290, 70, 300, 710), true, true, meWindow, -1, false);
    
    //TABS
    tabctrl = gui->addTabControl(rect<int>(310, 70, 1140, 680), meWindow, true, true, 0);
    texturesTab = tabctrl->addTab(L"Textures");
    tabctrl->addTab(L"Colors");
    
    lastTabTextColor = texturesTab->getTextColor();
    texturesTab->setTextColor(SColor(255, 255, 255, 255));
    
    //---------------------------------------------------------------------------------------------------------
	//GENERAL TAB
    matEditorTextures = new CUIMaterialEditorTextures(devices, texturesTab, nodeToEdit);
    matEditorTextures->update(&node->getMaterial(0));
    //---------------------------------------------------------------------------------------------------------
    
    //---------------------------------------------------------------------------------------------------------
	//MANAGE SCROOL BARS
    for (u32 i=0; i < tabctrl->getTabCount(); i++) {
        IGUITab *tab = tabctrl->getTab(i);
        core::list<IGUIElement *>::ConstIterator element = tab->getChildren().begin();
        u32 maxPosition=0;
        for (; element != tab->getChildren().end(); ++element) {
            IGUIElement *it = (IGUIElement *)*element;
            if ((it->getRelativePosition().UpperLeftCorner.Y + it->getRelativePosition().getHeight()) > tab->getRelativePosition().getHeight()) {
                maxPosition = it->getRelativePosition().UpperLeftCorner.Y + it->getRelativePosition().getHeight() - tab->getRelativePosition().getHeight();
            }
        }
        IGUIScrollBar *scrollbar = gui->addScrollBar(false, rect<s32>(812, 0, 829, 573), tab, -1);
        scbars.push_back(scrollbar);
        tab->addChild(scrollbar);
        scrollbar->setMin(0);
        scrollbar->setMax(maxPosition);
    }
    lastScrollBarPos = 0;
    //---------------------------------------------------------------------------------------------------------
    
    close = gui->addButton(rect<s32>(1040, 690, 1140, 720), meWindow, -1, L"Close", L"Close The Button");
}

bool CUIMaterialEditor::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        //IF BUTTON CLICKED
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            if (event.GUIEvent.Caller == close) {
                nodeToEdit->setMaterialFlag(EMF_WIREFRAME, false);
                
                meWindow->remove();
                
                delete matEditorTextures;
                delete this;
            }
        }
        
        //IF TREE VIEW
        if (event.GUIEvent.EventType == EGET_TREEVIEW_NODE_SELECT) {
            if (event.GUIEvent.Caller == materialsView) {
                if (materialsView->getSelected()->getParent() == rootTreeViewNode) {
                    selectedMaterial = (SMaterial *)materialsView->getSelected()->getData();
                    restoreMaterial = (SMaterial *)materialsView->getSelected()->getData();
                    
                    matEditorTextures->update(selectedMaterial);
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
                    IGUIElement *tab = tabctrl->getTab(tabctrl->getActiveTab());
                    core::list<IGUIElement *>::ConstIterator element = tab->getChildren().begin();
                    for (; element != tab->getChildren().end(); ++element) {
                        IGUIElement *it = (IGUIElement *)*element;
                        if (it != scbars[i]) {
                            if (scbars[i]->getPos() > lastScrollBarPos) {
                                it->setRelativePosition(position2di(it->getRelativePosition().UpperLeftCorner.X, 
                                                                    it->getRelativePosition().UpperLeftCorner.Y-scbars[i]->getPos()));
                            } else {
                                it->setRelativePosition(position2di(it->getRelativePosition().UpperLeftCorner.X, 
                                                                    it->getRelativePosition().UpperLeftCorner.Y+scbars[i]->getPos()));
                            }
                        }
                    }
                    lastScrollBarPos = scbars[i]->getPos();
                }
            }
        }
    }
    
    return false;
}
