//
//  CUISceneView.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/10/12.
//
//
#include "stdafx.h"
#include "CUISceneView.h"

CUISceneView::CUISceneView(CDevices *_devices) {
    devices = _devices;
    
    devices->getEventReceiver()->AddEventReceiver(this);
    
    windowSceneTreeNode = devices->getGUIEnvironment()->addWindow(rect<s32>(420, 80, 800, 530), false, L"Entire Tree View Nodes", 0, -1);
    windowSceneTreeNode->getMaximizeButton()->setVisible(true);
    
	nodeTreeView = devices->getGUIEnvironment()->addTreeView(rect<s32>(5, 30, 370, 400), windowSceneTreeNode, -1, true, true, false);
	closeButton = devices->getGUIEnvironment()->addButton(rect<s32>(5, 410, 85, 440), windowSceneTreeNode, CXT_VIEW_SCENE_VIEW_CLOSE, L"Close", L"Close This Window");
	windowSceneTreeNode->getCloseButton()->remove();
	//nodeTreeView->setToolTipText(L"Show the current SceneGraph");
	nodeTreeView->getRoot()->clearChilds();
	addSceneTreeItem(devices->getSceneManager()->getRootSceneNode(), nodeTreeView->getRoot());
	IGUIImageList* imageList = devices->getGUIEnvironment()->createImageList(devices->getVideoDriver()->getTexture("GUI/iconlist.png"), 
                                                                             dimension2d<int>(32, 32), true);
	if (imageList) {
		nodeTreeView->setImageList(imageList);
		imageList->drop();
	}
    windowSceneTreeNode->setVisible(false);
    
    windowed = false;
}

CUISceneView::~CUISceneView() {
    
}

void CUISceneView::open() {
    windowSceneTreeNode->setVisible(true);
    refresh();
}

void CUISceneView::refresh() {
    nodeTreeView->getRoot()->clearChilds();
    addSceneTreeItem(devices->getSceneManager()->getRootSceneNode(), nodeTreeView->getRoot());
}

void CUISceneView::addSceneTreeItem(ISceneNode *parent, IGUITreeViewNode *nodeParent) {
    
	IGUITreeViewNode* node;
	wchar_t msg[128];
	
	s32 imageIndex;
	core::list<ISceneNode *>::ConstIterator it = parent->getChildren().begin();
	for (; it != parent->getChildren().end(); ++it) {
		switch ( (*it)->getType () ) {
			case ESNT_Q3SHADER_SCENE_NODE: imageIndex = 0; break;
			case ESNT_CAMERA: imageIndex = 1; break;
			case ESNT_EMPTY: imageIndex = 2; break;
			case ESNT_MESH: imageIndex = 3; break;
			case ESNT_OCTREE: imageIndex = 3; break;
			case ESNT_ANIMATED_MESH: imageIndex = 4; break;
            case ESNT_SPHERE: imageIndex = 5; break;
			case ESNT_SKY_BOX: imageIndex = 5; break;
			case ESNT_BILLBOARD: imageIndex = 6; break;
			case ESNT_PARTICLE_SYSTEM: imageIndex = 7; break;
			case ESNT_TEXT: imageIndex = 8; break;
			case ESNT_LIGHT: imageIndex = 9; break;
			default:imageIndex = -1; break;
		}
		
		if (imageIndex < 0) {
			swprintf(msg, 128, L"%hs", (*it)->getName());
		}
		else {
			swprintf(msg, 128, L"%hs", (*it)->getName());
		}
		
		node = nodeParent->addChildBack(msg, 0, imageIndex);
		
		core::list<ISceneNodeAnimator*>::ConstIterator ait = (*it)->getAnimators().begin();
		for (; ait != (*it)->getAnimators().end(); ++ait) {
			imageIndex = -1;
			swprintf(msg, 128, L"%hs", devices->getDevice()->getSceneManager()->getAnimatorTypeName((*ait)->getType()));
			
			switch ((*ait)->getType()) {
				case ESNAT_FLY_CIRCLE:
				case ESNAT_FLY_STRAIGHT:
				case ESNAT_FOLLOW_SPLINE:
				case ESNAT_ROTATION:
				case ESNAT_TEXTURE:
				case ESNAT_DELETION:
				case ESNAT_COLLISION_RESPONSE:
				case ESNAT_CAMERA_FPS:
				case ESNAT_CAMERA_MAYA:
				default:break;
			}
			node->addChildBack(msg, 0, imageIndex);
		}
		stringc nameNodeTreeView = "";
		nameNodeTreeView += (*it)->getName();
		//if (!nameNodeTreeView) {
            addSceneTreeItem (*it, node);
		//}
	}
}

bool CUISceneView::OnEvent(const SEvent &event) {
    
    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
            
            if (event.GUIEvent.Caller == windowSceneTreeNode->getMaximizeButton()) {
                if (windowed) {
                    windowSceneTreeNode->setRelativePosition(rect<s32>(0, 70, 
                                                                       devices->getVideoDriver()->getScreenSize().Width,
                                                                       devices->getVideoDriver()->getScreenSize().Height-16));
                    windowSceneTreeNode->setDrawTitlebar(false);
                    windowSceneTreeNode->setDraggable(false);
                    nodeTreeView->setRelativePosition(rect<s32>(5, 30, 
                                                                windowSceneTreeNode->getRelativePosition().getWidth()-10, 
                                                                windowSceneTreeNode->getRelativePosition().getHeight()-40));
                    closeButton->setRelativePosition(position2d<int>(5, windowSceneTreeNode->getRelativePosition().getHeight()-35));
                } else {
                    windowSceneTreeNode->setRelativePosition(rect<s32>(420, 80, 800, 530));
                    windowSceneTreeNode->setDrawTitlebar(true);
                    windowSceneTreeNode->setDraggable(true);
                    nodeTreeView->setRelativePosition(rect<s32>(5, 30, 370, 400));
                    closeButton->setRelativePosition(rect<s32>(5, 410, 85, 440));
                }
                windowed = !windowed;
            }
            
            s32 id = event.GUIEvent.Caller->getID();
            switch (id) {
                case CXT_VIEW_SCENE_VIEW_CLOSE:
                    windowSceneTreeNode->setVisible(false);
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    return false;
}
