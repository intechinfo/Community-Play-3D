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
	terrainSize = devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width;
	u32 terrainSizeHeight = devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Height;

	leftClickDown = false;
	rightClickDown = false;

	timer = devices->getDevice()->getTimer();
	then = 0, then30 = 0;

	//INIT DECALE MANAGER
	decalMgr = new DecalManager(devices->getSceneManager());
	decalMgr->setTerrain(node);

	circlePath = devices->getWorkingDirectory() + "GUI/terrain_painting_circle.png";
	circleTex = devices->getVideoDriver()->getTexture(circlePath.c_str());

	arrow = devices->getSceneManager()->addAnimatedMeshSceneNode(devices->getSceneManager()->addArrowMesh("arrow", 
			SColor(255, 255, 0, 0), SColor(255, 0, 255, 0)), NULL);
	arrow->setMaterialFlag(video::EMF_LIGHTING, false);
	arrow->setScale(vector3df(5,5,5));
	arrow->setRotation(vector3df(0,0,180));

	//GET TERRAIN VERTICES LINE PER LINE (ARRAY -> ARRAY(S3DVertex2TCoords))
	IMesh* pMesh = node->getMesh(); 
	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices();
		u32 currentLine = 0;
		array<S3DVertex2TCoords *> newVertices;
		terrainVerticesLines.push_back(newVertices);
		for (u32 i=0; i < pMesh->getMeshBuffer(b)->getVertexCount(); i++) {
			if (i == terrainSize * currentLine+1) {
				currentLine++;
				terrainVerticesLines.push_back(newVertices);
			}
			terrainVerticesLines[currentLine].push_back(&pVertices[i]);
		}
	}

	terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

	//GUI ELEMENTS
	devices->getEventReceiver()->AddEventReceiver(this, 0, this);


	mw->getMainWindow()->setVisible(false);
}

CUITerrainPainter::~CUITerrainPainter() {

}

void CUITerrainPainter::RaiseTerrainVertex(s32 index, f32 step, bool up) {
	IMesh* pMesh = node->getMesh(); 

	for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 

		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 

		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices(); 

		u32 indexInterval = 50;
		if (index >= 20 && index < terrainSize * terrainSize) {
			u32 selectedLine = (index / devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width);
			for (u32 i=selectedLine-20; i < selectedLine+20; i++) {
				for (u32 j=index-(selectedLine*terrainSize)-10; j < index-(selectedLine*terrainSize)+10; j++) {
					terrainVerticesLines[i][j]->Pos.Y += (up) ? step : -step;
				}
			}

			devices->getDevice()->getLogger()->log(stringc(stringc("vertex position Y = ") + stringc(pVertices[index].Pos.Y)).c_str());
		} else {
			u32 selectedLine = (index / devices->getVideoDriver()->getTexture(tdata.getPath().c_str())->getOriginalSize().Width);
			for (u32 i=0; i < 20; i++) {
				for (u32 j=index-(selectedLine*terrainSize)-10; j < index-(selectedLine*terrainSize)+10; j++) {
					terrainVerticesLines[i][j]->Pos.Y += (up) ? step : -step;
				}
			}
		}
	}

	node->setPosition(node->getPosition());
}

void CUITerrainPainter::update() {
	u32 now = timer->getTime();
	if (then30 < now) {
		const position2di clickPosition = devices->getDevice()->getCursorControl()->getPosition(); 
		const line3df ray = devices->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, devices->getSceneManager()->getActiveCamera());
		vector3df pos;
		triangle3df Tri;
		ISceneNode *outNode;
		if (devices->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, pos, Tri, outNode)) {
			static const s32 scale = node->getScale().X;
			static const s32 size = terrainSize;
			s32 x = (s32)(pos.X / scale);
			s32 z = (s32)(pos.Z / scale);
			s32 index = x * size + z * size;
			if (decalMgr->getDecals(circlePath.c_str()).size() == 0) {
				decalMgr->addDecal(circlePath.c_str(), pos, vector3df(100, 100, 100), ray.start - ray.end, 
									0, 0, 0, devices->getSceneManager()->getActiveCamera()->getFarValue(), true);
			}

			vertexIndex = x * size + z;

			if ((leftClickDown || rightClickDown) && !devices->isCtrlPushed() && then < now) {
				RaiseTerrainVertex(vertexIndex, 2.f, leftClickDown);
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
			if (decalMgr->getDecals(circlePath.c_str()).size() > 0) {
				decalMgr->getDecals(circlePath.c_str())[0]->setScale(vector3df(decalMgr->getDecals(circlePath.c_str())[0]->getScale().X) + event.MouseInput.Wheel * 0.1);
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
					vector3df decalSize = decalMgr->getDecals(circlePath.c_str())[0]->getScale();
					decalMgr->getDecals(circlePath.c_str())[0]->remove();
					decalMgr->addDecal(circlePath.c_str(), pos, decalSize*10, ray.start - ray.end, 
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

	return false;
}
