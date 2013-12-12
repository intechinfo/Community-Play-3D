/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUITerrainPainting.h"

#include "../../MainWindow/UIMainWindow.h"

CUITerrainPainter::CUITerrainPainter(CDevices *_devices, STerrainsData _tdata, CUIMainWindow *mw) {
	//DEVICES AND DATAS
	devices = _devices;

	tdata = _tdata;
	node = (ITerrainSceneNode*)tdata.getNode();
	s32 terrainSizeWidth = devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width;
	s32 terrainSizeHeight = devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Height;

	leftClickDown = false;
	rightClickDown = false;

	terrainPainter = new CTerrainPainter(devices, node, dimension2di(terrainSizeWidth, terrainSizeHeight));
	grassPainter = new CGrassPainter(devices, node);

	timer = devices->getDevice()->getTimer();

	//CONFIGURE ARROW
	arrow = devices->getSceneManager()->addAnimatedMeshSceneNode(devices->getSceneManager()->addArrowMesh("arrow", 
			SColor(255, 255, 0, 0), SColor(255, 0, 255, 0)), NULL);
	arrow->setMaterialFlag(video::EMF_LIGHTING, false);
	arrow->setScale(vector3df(5,5,5));
	arrow->setRotation(vector3df(0,0,180));

	//GET TERRAIN INFORMATIONS
	terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

	oldTerrainPosition = node->getPosition();
	oldTerrainRotation = node->getRotation();
	oldTerrainScale = node->getScale();

	//---------------------------------------------------------------------------------------------
	//----------------------------------------------GUI--------------------------------------------
	//---------------------------------------------------------------------------------------------
	//GUI ELEMENTS
	stringc workingPath = devices->getWorkingDirectory().c_str();
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	window = gui->addWindow(rect<s32>(980, 110, 1340, 340), false, L"Terrain Painting", 0, -1);

	tabctrl = gui->addTabControl(rect<s32>(0, 30, 10, 10), window, true, true, -1);
	painting = tabctrl->addTab(L"Terrain", -1);
	grassPainting = tabctrl->addTab(L"Grass", -1);
	treePainting = tabctrl->addTab(L"Tree", -1);

	//---------------------------------------------------------------------------------------------
	//------------------------------------------TERRAIN--------------------------------------------
	//---------------------------------------------------------------------------------------------
	gui->addStaticText(L"Strength :", rect<s32>(10, 70, 70, 90), true, true, painting, -1, false);
	stepValueeb = gui->addEditBox(L"", rect<s32>(70, 70, 170, 90), true, painting, -1);
	stepsb = gui->addScrollBar(true, rect<s32>(10, 90, 350, 110), painting, -1);
	stepsb->setMin(1);
	stepsb->setMax(terrainPainter->getMaxHeight()*2);
	stepsb->setSmallStep(1);
	stepsb->setLargeStep(1);

	gui->addStaticText(L"Radius :", rect<s32>(10, 110, 70, 130), true, true, painting, -1, false);
	radiusValueeb = gui->addEditBox(L"", rect<s32>(70, 110, 170, 130), true, painting, -1);
	radiussb = gui->addScrollBar(true, rect<s32>(10, 130, 350, 150), painting, -1);
	radiussb->setMin(1);
	radiussb->setMax((terrainSizeHeight) / 8);
	radiussb->setSmallStep(1);
	radiussb->setLargeStep(1);

	gui->addStaticText(L"Smooth :", rect<s32>(10, 180, 80, 200), true, true, painting, -1, false);
	smoothFactor = gui->addEditBox(L"20", rect<s32>(80, 180, 180, 200), true, painting, -1);
	runSmoothing = gui->addButton(rect<s32>(180, 180, 280, 200), painting, -1, L"Run", L"Run smoothing");

	stepsb->setPos(terrainPainter->getCurrentStep());
	radiussb->setPos(terrainPainter->getCurrentRadius());

	stepValueeb->setText(stringw(terrainPainter->getCurrentStep()).c_str());
	radiusValueeb->setText(stringw(terrainPainter->getCurrentRadius()).c_str());

	//---------------------------------------------------------------------------------------------
	//--------------------------------------------GRASS--------------------------------------------
	//---------------------------------------------------------------------------------------------
	gui->addStaticText(L"Scale :", rect<s32>(10, 70, 70, 90), true, true, grassPainting, -1, false);
	grassScaleValueeb = gui->addEditBox(L"10", rect<s32>(70, 70, 170, 90), true, grassPainting, -1);
	grassScalesb = gui->addScrollBar(true, rect<s32>(10, 90, 350, 110), grassPainting, -1);
	grassScalesb->setPos(10);
	grassScalesb->setMin(1);
	grassScalesb->setMax(10000);
	grassScalesb->setSmallStep(1);
	grassScalesb->setLargeStep(1);

	gui->addStaticText(L"Quality :", rect<s32>(10, 110, 70, 130), true, true, grassPainting, -1, false);
	grassQualityeb = gui->addEditBox(L"3", rect<s32>(70, 110, 170, 130), true, grassPainting, -1);
	grassQualitysb = gui->addScrollBar(true, rect<s32>(10, 130, 350, 150), grassPainting, -1);
	grassQualitysb->setPos(3);
	grassQualitysb->setMin(1);
	grassQualitysb->setMax(10);
	grassQualitysb->setSmallStep(1);
	grassQualitysb->setLargeStep(1);

	gui->addStaticText(L"Space :", rect<s32>(10, 150, 70, 170), true, true, grassPainting, -1, false);
	grassSpaceeb = gui->addEditBox(L"10", rect<s32>(70, 150, 170, 170), true, grassPainting, -1);
	grassSpacesb = gui->addScrollBar(true, rect<s32>(10, 170, 350, 190), grassPainting, -1);
	grassSpacesb->setPos(10);
	grassSpacesb->setMin(1);
	grassSpacesb->setMax(1000);
	grassSpacesb->setSmallStep(1);
	grassSpacesb->setLargeStep(1);

	gui->addStaticText(L"Textures :", rect<s32>(10, 200, 310, 220), true, true, grassPainting, -1, false);
	grassRemoveTexture = gui->addButton(rect<s32>(310, 200, 330, 220), grassPainting, -1, L"-", L"Remove selected texture");
	grassAddTexture = gui->addButton(rect<s32>(330, 200, 350, 220), grassPainting, -1, L"+", L"Add a grass texture");
	grassTextures = gui->addListBox(rect<s32>(10, 220, 350, 360), grassPainting, -1, true);

	//---------------------------------------------------------------------------------------------
	//----------------------------------------------GUI--------------------------------------------
	//---------------------------------------------------------------------------------------------

	cancel = gui->addButton(rect<s32>(250, 190, 350, 220), window, -1, L"Cancel", L"Close and discard changes");
	accept = gui->addButton(rect<s32>(140, 190, 240, 220), window, -1, L"Accept", L"Close and save changes");

	//---------------------------------------------------------------------------------------------
	//------------------------------------------/GUI-----------------------------------------------
	//---------------------------------------------------------------------------------------------

	//EVENTS RECEIVERS AND TYPES
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);
	paintingType = EPT_TERRAIN;

	for (u32 i=0; i < tabctrl->getTabCount(); i++)
		tabctrl->getTab(i)->setRelativePosition(position2di(tabctrl->getTab(i)->getRelativePosition().UpperLeftCorner.X,
															tabctrl->getTab(i)->getRelativePosition().UpperLeftCorner.Y-50));
}

CUITerrainPainter::~CUITerrainPainter() {

}

void CUITerrainPainter::update() {
	window->setRelativePosition(rect<s32>(devices->getVideoDriver()->getScreenSize().Width-window->getRelativePosition().getWidth(), 75,
										  devices->getVideoDriver()->getScreenSize().Width, devices->getVideoDriver()->getScreenSize().Height-20));
	tabctrl->setRelativePosition(rect<s32>(5, 20, window->getRelativePosition().getWidth()-5, window->getRelativePosition().getHeight()-50));
	cancel->setRelativePosition(position2di(250, window->getRelativePosition().getHeight()-40));
	accept->setRelativePosition(position2di(140, window->getRelativePosition().getHeight()-40));

	//RECALCULATE TRIANGLES IF TERRAIN'S TRANSFORMATION CHANGES
	if (oldTerrainScale != node->getScale() || oldTerrainPosition != node->getPosition() || oldTerrainRotation != node->getRotation()) {
		oldTerrainPosition = node->getPosition();
		oldTerrainRotation = node->getRotation();
		oldTerrainScale = node->getScale();
		terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);
	}

	u32 now = timer->getTime();
	//GET SELECTED VERTEX
	const position2di clickPosition = devices->getDevice()->getCursorControl()->getPosition(); 
	const line3df ray = devices->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, devices->getSceneManager()->getActiveCamera());
	vector3df pos;
	triangle3df Tri;
	ISceneNode *outNode;
	if (devices->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, pos, Tri, outNode)) {
		static const s32 scale = node->getScale().X;
		static const s32 size = terrainPainter->getTerrainSize().Width;
		s32 x = (s32)(pos.X / scale);
		s32 z = (s32)(pos.Z / scale);
		s32 index = x * size + z * size;

		terrainPainter->setVertexIndex(x * size + z);

		if ((leftClickDown || rightClickDown) && !devices->isCtrlPushed() && devices->getGUIEnvironment()->getFocus() == 0) {
			if (paintingType == EPT_TERRAIN)
				terrainPainter->RaiseTerrainVertex(pos, terrainPainter->getCurrentStep(), leftClickDown);
			else if (paintingType == EPT_GRASS)
				grassPainter->paint(pos, grassScalesb->getPos(), grassQualitysb->getPos(), grassSpacesb->getPos(), rightClickDown);
			
		}

		x *= scale;
		z *= scale;

		arrow->setPosition(vector3df(x, node->getHeight(x, z) + 5, z));
		if (paintingType == EPT_TERRAIN)
			terrainPainter->drawBrush(vector3df(arrow->getPosition().X, arrow->getPosition().Y-5, arrow->getPosition().Z));
		else if (paintingType == EPT_GRASS)
			grassPainter->drawPreview(vector3df(arrow->getPosition().X, arrow->getPosition().Y-5, arrow->getPosition().Z));
	}
}

bool CUITerrainPainter::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			if (paintingType == EPT_TERRAIN) {
				if (terrainPainter->getCurrentRadius() < radiussb->getMax() && terrainPainter->getCurrentRadius() > radiussb->getMin()) {
					terrainPainter->setCurrentRadius(terrainPainter->getCurrentRadius() + event.MouseInput.Wheel*0.1f);
					radiusValueeb->setText(stringw(terrainPainter->getCurrentRadius()).c_str());
					radiussb->setPos(terrainPainter->getCurrentRadius());
				}
			} else if (paintingType == EPT_GRASS) {
				if (grassPainter->getCurrentScale() < grassScalesb->getMax() && grassPainter->getCurrentScale() > grassScalesb->getMin()) {
					grassPainter->setCurrentScale(grassPainter->getCurrentScale() + event.MouseInput.Wheel*0.1f);
					grassScaleValueeb->setText(stringw(grassPainter->getCurrentScale()).c_str());
					grassScalesb->setPos(grassPainter->getCurrentScale());
				}
			}
		}
		if ((event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN || event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) && !devices->isCtrlPushed()) {
            
		}
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
			leftClickDown = true;
		}
		if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) {
			rightClickDown = true;
		}
		if ((event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP || event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP) && !devices->isCtrlPushed()
			&& devices->getGUIEnvironment()->getFocus() == 0)
		{
			if (paintingType == EPT_TERRAIN) {
				//RECALCULATE TRIANGLES FOR COLLISIONS
				if (!devices->isCtrlPushed()) {
					devices->getSceneManager()->getMeshManipulator()->recalculateNormals(node->getMesh(), true);
					node->setPosition(node->getPosition());
					terrainSelector = devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);
				}

				//SMOOTH TERRAIN
				terrainPainter->applyTransform(0);

				node->setPosition(node->getPosition());
                node->setMaterialType(node->getMaterial(0).MaterialType);
			}

			devices->getXEffect()->setAllShadowLightsRecalculate();
		}
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP && devices->getGUIEnvironment()->getFocus() == 0) {
			leftClickDown = false;
		}
		if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP && devices->getGUIEnvironment()->getFocus() == 0) {
			rightClickDown = false;
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				arrow->remove();
				delete terrainPainter;
				delete grassPainter;
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == cancel) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				arrow->remove();
				delete this;
			}
			if (event.GUIEvent.Caller == accept) {

			}

			//---------------------------------------------------------------------------------------------
			//------------------------------------------TERRAIN--------------------------------------------
			//---------------------------------------------------------------------------------------------
			if (event.GUIEvent.Caller == runSmoothing) {
				//SMOOTH BACK THE TERRAIN
				s32 smoothFactors32 = devices->getCore()->getS32(smoothFactor->getText());
				if (smoothFactors32 <= 0)
					smoothFactors32 = 1;

				//SMOOTH TERRAIN
				terrainPainter->applyTransform(smoothFactors32);
				//RECALCULATE TRIANGLES FOR COLLISIONS
				if (!devices->isCtrlPushed()) {
					devices->getSceneManager()->getMeshManipulator()->recalculateNormals(node->getMesh(), true);
					node->setPosition(node->getPosition());
					terrainSelector = devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);
				}
				node->setPosition(node->getPosition());
			}
			//---------------------------------------------------------------------------------------------
			//------------------------------------------GRASS----------------------------------------------
			//---------------------------------------------------------------------------------------------
			if (event.GUIEvent.Caller == grassAddTexture) {
				grassChooseTexture = devices->createFileOpenDialog(L"Choose Grass Texture", CGUIFileSelector::EFST_OPEN_DIALOG, 
																   devices->getGUIEnvironment()->getRootGUIElement(), true);
			}
			if (event.GUIEvent.Caller == grassRemoveTexture) {
				if (grassTextures->getSelected() != -1) {
					grassPainter->getTextures()->erase(grassTextures->getSelected());
					grassTextures->removeItem(grassTextures->getSelected());
				}
			}
		}

		//SCROLL BARS
		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			//TERRAIn
			if (event.GUIEvent.Caller == stepsb) {
				terrainPainter->setCurrentStep(stepsb->getPos());
				stepValueeb->setText(stringw(stepsb->getPos()).c_str());
			}
			if (event.GUIEvent.Caller == radiussb) {
				terrainPainter->setCurrentRadius(radiussb->getPos());
				radiusValueeb->setText(stringw(radiussb->getPos()).c_str());
			}

			//GRASS
			if (event.GUIEvent.Caller == grassScalesb) {
				grassPainter->setCurrentScale(grassScalesb->getPos());
				grassScaleValueeb->setText(stringw(grassScalesb->getPos()).c_str());
			}
			if (event.GUIEvent.Caller == grassQualitysb) {
				grassPainter->setCurrentQuality(grassQualitysb->getPos());
				grassQualityeb->setText(stringw(grassQualitysb->getPos()).c_str());
			}
			if (event.GUIEvent.Caller == grassSpacesb) {
				grassPainter->setCurrentDistance(grassSpacesb->getPos());
				grassSpaceeb->setText(stringw(grassSpacesb->getPos()).c_str());
			}
		}

		//TABS
		if (event.GUIEvent.EventType == EGET_TAB_CHANGED) {
			if (event.GUIEvent.Caller == tabctrl) {
				if (tabctrl->getActiveTab() == painting->getNumber())
					paintingType = EPT_TERRAIN;
				else if (tabctrl->getActiveTab() == grassPainting->getNumber())
					paintingType = EPT_GRASS;
			}
		}

		//LIST BOXES
		if (event.GUIEvent.EventType == EGET_LISTBOX_CHANGED) {
			if (event.GUIEvent.Caller == grassTextures) {
				if (grassTextures->getSelected() != -1)
					grassPainter->setTextureIndex(grassTextures->getSelected());
			}
		}

		//FILES SELECTED
		if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
			if (event.GUIEvent.Caller == grassChooseTexture) {
				ITexture *texture = devices->getVideoDriver()->getTexture(grassChooseTexture->getFileName());
				if (texture) {
					grassPainter->getTextures()->push_back(texture);
					grassTextures->addItem(stringw(grassChooseTexture->getFileName()).c_str());
					grassTextures->setSelected(grassTextures->getItemCount()-1);
					grassPainter->setTextureIndex(grassTextures->getSelected());
				} else {
					devices->addErrorDialog(L"Error", L"Cannot load the texture, wrong format or not a texture.", EMBF_OK);
				}
			}
		}
	}

	return false;
}
