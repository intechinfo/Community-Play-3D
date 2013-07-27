/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUITerrainPainter.h"

#include "../MainWindow/UIMainWindow.h"

CUITerrainPainter::CUITerrainPainter(CDevices *_devices, STerrainsData _tdata, CUIMainWindow *mw) {
	//DEVICES AND DATAS
	devices = _devices;

	tdata = _tdata;
	node = (ITerrainSceneNode*)tdata.getNode();
	terrainSizeWidth = devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width;
	terrainSizeHeight = devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Height;

	leftClickDown = false;
	rightClickDown = false;

	timer = devices->getDevice()->getTimer();
	then = 0, then30 = 0;

	//INIT DECALE MANAGER
	decalMgr = new DecalManager(devices->getSceneManager());
	decalMgr->setTerrain(node);

	circlePath = devices->getWorkingDirectory() + "GUI/terrain_painting_circle.png";
	circleTex = devices->getVideoDriver()->getTexture(circlePath.c_str());

	//CONFIGURE ARROW
	arrow = devices->getSceneManager()->addAnimatedMeshSceneNode(devices->getSceneManager()->addArrowMesh("arrow", 
			SColor(255, 255, 0, 0), SColor(255, 0, 255, 0)), NULL);
	arrow->setMaterialFlag(video::EMF_LIGHTING, false);
	arrow->setScale(vector3df(5,5,5));
	arrow->setRotation(vector3df(0,0,180));

	//GET TERRAIN VERTICES LINE PER LINE (ARRAY -> ARRAY(S3DVertex2TCoords))
	minHeight = 0;
	maxHeight = 0;

	IMesh* pMesh = node->getMesh(); 
	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices();
		u32 currentLine = 0;
		array<S3DVertex2TCoords *> newVertices;
		terrainVerticesLines.push_back(newVertices);
		for (u32 i=0; i < pMesh->getMeshBuffer(b)->getVertexCount(); i++) {
			if (i == terrainSizeWidth * currentLine+1) {
				currentLine++;
				terrainVerticesLines.push_back(newVertices);
			}
			if (pVertices[i].Pos.Y < minHeight)
				minHeight = pVertices[i].Pos.Y;
			if (pVertices[i].Pos.Y > maxHeight)
				maxHeight = pVertices[i].Pos.Y;
			terrainVerticesLines[currentLine].push_back(&pVertices[i]);
		}
	}
	heightInterval = (minHeight >= 0) ? maxHeight - minHeight : maxHeight + minHeight;

	terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

	oldTerrainPosition = node->getPosition();
	oldTerrainRotation = node->getRotation();
	oldTerrainScale = node->getScale();

	//GUI ELEMENTS
	stringc workingPath = devices->getWorkingDirectory().c_str();
	IGUIEnvironment *gui = devices->getGUIEnvironment();
	window = gui->addWindow(rect<s32>(980, 110, 1340, 340), false, L"Terrain Painting", 0, -1);

	bar = gui->addToolBar(window, -1);
	bar->setRelativePosition(rect<s32>(0, 20, 360, 50));
	exportHeightMapbtn = bar->addButton(-1, L"", L"", devices->getVideoDriver()->getTexture(workingPath + "GUI/save.png"), 
										0, false, true);

	gui->addStaticText(L"Step :", rect<s32>(10, 70, 70, 90), true, true, window, -1, false);
	stepValueeb = gui->addEditBox(L"", rect<s32>(70, 70, 170, 90), true, window, -1);
	stepsb = gui->addScrollBar(true, rect<s32>(10, 90, 350, 110), window, -1);
	stepsb->setMin(1);
	stepsb->setMax(maxHeight);
	stepsb->setSmallStep(1);
	stepsb->setLargeStep(1);

	gui->addStaticText(L"Radius :", rect<s32>(10, 110, 70, 130), true, true, window, -1, false);
	radiusValueeb = gui->addEditBox(L"", rect<s32>(70, 110, 170, 130), true, window, -1);
	radiussb = gui->addScrollBar(true, rect<s32>(10, 130, 350, 150), window, -1);
	radiussb->setMin(1);
	radiussb->setMax((terrainSizeHeight) / 32);
	radiussb->setSmallStep(1);
	radiussb->setLargeStep(1);

	cancel = gui->addButton(rect<s32>(250, 190, 350, 220), window, -1, L"Cancel", L"Close and discard changes");
	accept = gui->addButton(rect<s32>(140, 190, 240, 220), window, -1, L"Accept", L"Close and save changes");

	//EVENTS RECEIVERS
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);

	//INITIALIZING TERRAIN PAINTING DATAS
	currentStep = (maxHeight > 0) ? 1 : 0;
	currentRadius = 1;

	stepsb->setPos(currentStep);
	radiussb->setPos(currentRadius);

	stepValueeb->setText(stringw(currentStep).c_str());
	radiusValueeb->setText(stringw(currentRadius).c_str());
}

CUITerrainPainter::~CUITerrainPainter() {

}

void CUITerrainPainter::RaiseTerrainVertex(s32 index, f32 step, bool up) {
	IMesh* pMesh = node->getMesh(); 

	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 

		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 

		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices();

		u32 indexInterval = currentRadius;

		if (index >= indexInterval && index < terrainSizeWidth * terrainSizeHeight) {
			u32 selectedLine = (index / devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width);
			for (u32 i=selectedLine-indexInterval; i < selectedLine+indexInterval; i++) {
				for (u32 j=index-(selectedLine*terrainSizeWidth)-indexInterval; j < index-(selectedLine*terrainSizeWidth)+indexInterval; j++) {
					terrainVerticesLines[i][j]->Pos.Y += (up) ? step : -step;
				}
			}
			//devices->getDevice()->getLogger()->log(stringc(stringc("vertex position Y = ") + stringc(pVertices[index].Pos.Y)).c_str());
		} else {
			u32 selectedLine = (index / devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width);
			for (u32 i=0; i < indexInterval; i++) {
				for (u32 j=index-(selectedLine*terrainSizeWidth)-indexInterval; j < index-(selectedLine*terrainSizeWidth)+indexInterval; j++) {
					terrainVerticesLines[i][j]->Pos.Y += (up) ? step : -step;
				}
			}
		}
	}

	node->setPosition(node->getPosition());
}

void CUITerrainPainter::update() {
	if (oldTerrainScale != node->getScale() || oldTerrainPosition != node->getPosition() || oldTerrainRotation != node->getRotation()) {
		oldTerrainPosition = node->getPosition();
		oldTerrainRotation = node->getRotation();
		oldTerrainScale = node->getScale();
		terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);
	}

	u32 now = timer->getTime();
	if (then30 < now) {
		const position2di clickPosition = devices->getDevice()->getCursorControl()->getPosition(); 
		const line3df ray = devices->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, devices->getSceneManager()->getActiveCamera());
		vector3df pos;
		triangle3df Tri;
		ISceneNode *outNode;
		if (devices->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, pos, Tri, outNode)) {
			static const s32 scale = node->getScale().X;
			static const s32 size = terrainSizeWidth;
			s32 x = (s32)(pos.X / scale);
			s32 z = (s32)(pos.Z / scale);
			s32 index = x * size + z * size;
			if (decalMgr->getDecals(circlePath.c_str()).size() == 0) {
				decalMgr->addDecal(circlePath.c_str(), pos, vector3df(currentRadius), ray.start - ray.end, 
									0, 0, 0, devices->getSceneManager()->getActiveCamera()->getFarValue(), true);
			}

			vertexIndex = x * size + z;

			if ((leftClickDown || rightClickDown) && !devices->isCtrlPushed() && then < now
				&& devices->getGUIEnvironment()->getFocus() == 0)
			{
				RaiseTerrainVertex(vertexIndex, currentStep, leftClickDown);
				then = now + 100;
			}

			x *= scale;
			z *= scale;

			decalMgr->getDecals(circlePath.c_str())[0]->setPosition(vector3df(x, node->getHeight(x, z) + decalMgr->getDecals(circlePath.c_str())[0]->getScale().X, z));
			arrow->setPosition(vector3df(x, node->getHeight(x, z) + 5, z));
		}

		then30 = now + 30;
	}
}

bool CUITerrainPainter::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			if (decalMgr->getDecals(circlePath.c_str()).size() > 0 && currentRadius < radiussb->getMax()) {
				currentRadius += event.MouseInput.Wheel * 0.1;
				decalMgr->getDecals(circlePath.c_str())[0]->setScale(vector3df(currentRadius));
				radiussb->setPos(currentRadius);
			}
		}
		if ((event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN || event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) && !devices->isCtrlPushed()) {
			if (decalMgr->getDecals(circlePath.c_str()).size() > 0) {
				const position2di clickPosition = devices->getDevice()->getCursorControl()->getPosition(); 
				const line3df ray = devices->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, devices->getSceneManager()->getActiveCamera());
				vector3df pos;
				triangle3df Tri;
				ISceneNode *outNode;
				if (devices->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, pos, Tri, outNode)) {
					decalMgr->getDecals(circlePath.c_str())[0]->remove();
					decalMgr->addDecal(circlePath.c_str(), pos, vector3df(currentRadius), ray.start - ray.end, 
										0, 0, 0, devices->getSceneManager()->getActiveCamera()->getFarValue(), true);
				}
			}
		}
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
			leftClickDown = true;
		}
		if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) {
			rightClickDown = true;
		}
		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP) {
			leftClickDown = false;
		}
		if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP) {
			rightClickDown = false;
		}
	}

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				decalMgr->getDecals(circlePath.c_str())[0]->remove();
				delete decalMgr;
				arrow->remove();
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == cancel) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				decalMgr->getDecals(circlePath.c_str())[0]->remove();
				delete decalMgr;
				arrow->remove();
				delete this;
			}
			if (event.GUIEvent.Caller == accept) {

			}
		}

		if (event.GUIEvent.EventType == EGET_SCROLL_BAR_CHANGED) {
			if (event.GUIEvent.Caller == stepsb) {
				currentStep = stepsb->getPos();
				stepValueeb->setText(stringw(stepsb->getPos()).c_str());
			}
			if (event.GUIEvent.Caller == radiussb) {
				currentRadius = radiussb->getPos();
				radiusValueeb->setText(stringw(radiussb->getPos()).c_str());
				decalMgr->getDecals(circlePath.c_str())[0]->setScale(vector3df(currentRadius));
			}
		}
	}

	return false;
}
