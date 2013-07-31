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
		array<SVerticesBeginInformations> newBeginVertices;
		terrainVerticesLines.push_back(newVertices);
		terrainVerticesBeginLines.push_back(newBeginVertices);
		for (u32 i=0; i < pMesh->getMeshBuffer(b)->getVertexCount(); i++) {
			if (i == terrainSizeWidth * currentLine+1) {
				currentLine++;
				terrainVerticesLines.push_back(newVertices);
				terrainVerticesBeginLines.push_back(newBeginVertices);
			}
			if (pVertices[i].Pos.Y < minHeight)
				minHeight = pVertices[i].Pos.Y;

			if (pVertices[i].Pos.Y > maxHeight)
				maxHeight = pVertices[i].Pos.Y;

			terrainVerticesLines[currentLine].push_back(&pVertices[i]);
			terrainVerticesBeginLines[currentLine].push_back(SVerticesBeginInformations());
		}
	}
	heightInterval = (minHeight >= 0) ? maxHeight - minHeight : maxHeight + minHeight;

	terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

	oldTerrainPosition = node->getPosition();
	oldTerrainRotation = node->getRotation();
	oldTerrainScale = node->getScale();

	canPaint = true;
	positionYTo = 0.f;

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
	stepsb->setMax(maxHeight+500);
	stepsb->setSmallStep(1);
	stepsb->setLargeStep(1);

	gui->addStaticText(L"Radius :", rect<s32>(10, 110, 70, 130), true, true, window, -1, false);
	radiusValueeb = gui->addEditBox(L"", rect<s32>(70, 110, 170, 130), true, window, -1);
	radiussb = gui->addScrollBar(true, rect<s32>(10, 130, 350, 150), window, -1);
	radiussb->setMin(1);
	radiussb->setMax((terrainSizeHeight) / 16);
	radiussb->setSmallStep(1);
	radiussb->setLargeStep(1);

	gui->addStaticText(L"Smooth :", rect<s32>(10, 180, 80, 200), true, true, window, -1, false);
	smoothFactor = gui->addEditBox(L"20", rect<s32>(80, 180, 180, 200), true, window, -1);
	runSmoothing = gui->addButton(rect<s32>(180, 180, 280, 200), window, -1, L"Run", L"Run smoothing");

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

void CUITerrainPainter::smoothTerrain(IMeshBuffer* mb, s32 smoothFactor)
{
	S3DVertex2TCoords *mbv = (S3DVertex2TCoords *)mb->getVertices();
	for (s32 run = 0; run < smoothFactor; ++run) {
		s32 yd = terrainSizeWidth;
		for (s32 y = 1; y < terrainSizeWidth - 1; ++y) {
			for (s32 x = 1; x < terrainSizeWidth - 1; ++x) {
				mbv[x + yd].Pos.Y =
					(mbv[x-1 + yd].Pos.Y + //LEFT
					mbv[x+1 + yd].Pos.Y + //RIGHT
					mbv[x + yd - terrainSizeWidth].Pos.Y + //ABOVE
					mbv[x + yd + terrainSizeWidth].Pos.Y) * 0.25f; //BELOW
			}
			yd += terrainSizeWidth;
		}
	}
}

void CUITerrainPainter::RaiseTerrainVertex(s32 index, f32 step, bool up) {
	IMesh* pMesh = node->getMesh(); 

	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 

		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 

		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices();

		array<array<SVerticesInformations>> selectedVertices;

		u32 indexInterval = currentRadius;

		//GET 2D ARRAY OF SELECTED VERTICES
		if (index >= indexInterval && index < terrainSizeWidth * terrainSizeHeight) {
			u32 selectedLine = (index / devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width);
			for (u32 i=selectedLine-indexInterval; i < selectedLine+indexInterval; i++) {
				array<SVerticesInformations> selectedVerticesLine;
				for (u32 j=index-(selectedLine*terrainSizeWidth)-indexInterval; j < index-(selectedLine*terrainSizeWidth)+indexInterval; j++) {
					selectedVerticesLine.push_back(SVerticesInformations(terrainVerticesLines[i][j], 
																		 vector2di(selectedLine-i, index-(selectedLine*terrainSizeWidth)-indexInterval),
																		 terrainVerticesBeginLines[i][j].againUp));
					terrainVerticesBeginLines[i][j].againUp = false;
				}
				selectedVertices.push_back(selectedVerticesLine);
			}
		} else {
			u32 selectedLine = (index / devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width);
			for (u32 i=0; i < indexInterval; i++) {
				array<SVerticesInformations> selectedVerticesLine;
				for (u32 j=index-(selectedLine*terrainSizeWidth)-indexInterval; j < index-(selectedLine*terrainSizeWidth)+indexInterval; j++) {
					selectedVerticesLine.push_back(SVerticesInformations(terrainVerticesLines[i][j], 
																		 vector2di(i, index-(selectedLine*terrainSizeWidth)-indexInterval-j),
																		 terrainVerticesBeginLines[i][j].againUp));
					terrainVerticesBeginLines[i][j].againUp = false;
				}
				selectedVertices.push_back(selectedVerticesLine);
			}
		}

		//GIVE SELECTED VERTICES A HIGHER POSITION IF againUP
		u32 centerLeftCornerLength = indexInterval;
		u32 multiplier = 1000;
		for (u32 i=0; i < selectedVertices.size(); i++) {
			for (u32 j=0; j < selectedVertices[i].size(); j++) {
				if (i != selectedVertices.size() && j != selectedVertices[i].size()) {
					if (canPaint)
						positionYTo = selectedVertices[i][j].vertice->Pos.Y + (up) ? (step*multiplier)/((centerLeftCornerLength+1)*selectedVertices[i][j].position.getLength()) 
																				   : -((step*multiplier)/((centerLeftCornerLength+1)*selectedVertices[i][j].position.getLength()));


					if (selectedVertices[i][j].vertice->Pos.Y < positionYTo - (up) ? (step*multiplier)/((centerLeftCornerLength+1)*selectedVertices[i][j].position.getLength()) 
																				   : -((step*multiplier)/((centerLeftCornerLength+1)*selectedVertices[i][j].position.getLength()))
																				   && selectedVertices[i][j].againUp)
					{
						selectedVertices[i][j].vertice->Pos.Y += (up) ? (step*multiplier)/((centerLeftCornerLength+1)*selectedVertices[i][j].position.getLength()) 
																	  : -((step*multiplier)/((centerLeftCornerLength+1)*selectedVertices[i][j].position.getLength()));
						
					}
				} else {
					if (canPaint)
						positionYTo = selectedVertices[i][j].vertice->Pos.Y + (up) ? step : -step;

					if (selectedVertices[i][j].vertice->Pos.Y < positionYTo - (up) ? step : -step && selectedVertices[i][j].againUp)
						selectedVertices[i][j].vertice->Pos.Y += (up) ? step : -step;
				}
			}
		}

		canPaint = false;

		for (u32 i=0; i < selectedVertices.size(); i++)
			selectedVertices[i].clear();

		selectedVertices.clear();
	}

	node->setPosition(node->getPosition());

}

void CUITerrainPainter::update() {
	window->setRelativePosition(rect<s32>(devices->getVideoDriver()->getScreenSize().Width-window->getRelativePosition().getWidth(), 75,
										  devices->getVideoDriver()->getScreenSize().Width, devices->getVideoDriver()->getScreenSize().Height-20));
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
	if (then30 < now) {
		//GET SELECTED VERTEX
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

		then30 = now + 30; //30 HERTZ
	}
}

bool CUITerrainPainter::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			//CHANGE RADIUS
			if (decalMgr->getDecals(circlePath.c_str()).size() > 0 && currentRadius < radiussb->getMax()) {
				currentRadius += event.MouseInput.Wheel * 0.1;
				decalMgr->getDecals(circlePath.c_str())[0]->setScale(vector3df(currentRadius));
				radiussb->setPos(currentRadius);
				radiusValueeb->setText(stringw(radiussb->getPos()).c_str());
			}
		}
		if ((event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN || event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) && !devices->isCtrlPushed()) {
			//PLACE THE CIRCLE AT FOCUS
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
		if ((event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP || event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP) && !devices->isCtrlPushed()
			&& devices->getGUIEnvironment()->getFocus() == 0)
		{
			//SMOOTH BACK THE TERRAIN
			IMesh* pMesh = node->getMesh(); 
			for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
				IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
				if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
				smoothTerrain(pMeshBuffer, 20);
			}

			//RECALCULATE TRIANGLES FOR COLLISIONS
			if (!devices->isCtrlPushed())
				terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

			//ALL THE VERTICES CAN againUp
			for (u32 i=0; i < terrainVerticesBeginLines.size(); i++)
				for (u32 j=0; j < terrainVerticesBeginLines[i].size(); j++)
					terrainVerticesBeginLines[i][j].againUp = true;

			node->setPosition(node->getPosition());

			canPaint = true;
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

			if (event.GUIEvent.Caller == runSmoothing) {
				//SMOOTH BACK THE TERRAIN
				s32 smoothFactors32 = devices->getCore()->getS32(smoothFactor->getText());
				if (smoothFactors32 <= 0)
					smoothFactors32 = 1;

				IMesh* pMesh = node->getMesh(); 
				for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
					IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
					if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
					smoothTerrain(pMeshBuffer, smoothFactors32);
				}

				//RECALCULATE TRIANGLES FOR COLLISIONS
				if (!devices->isCtrlPushed())
					terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

				node->setPosition(node->getPosition());
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
				if (decalMgr->getDecals(circlePath.c_str()).size() > 0)
					decalMgr->getDecals(circlePath.c_str())[0]->setScale(vector3df(currentRadius));
			}
		}
	}

	return false;
}
