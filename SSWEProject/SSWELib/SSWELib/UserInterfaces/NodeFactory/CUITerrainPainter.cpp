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

		array<S3DVertex2TCoords *> newVertices;
		array<bool> newAgainUpVertices;
		terrainVerticesLines.push_back(newVertices);
		againUpVertices.push_back(newAgainUpVertices);

		IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
		if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		S3DVertex2TCoords* pVertices = (S3DVertex2TCoords*)pMeshBuffer->getVertices();

		for (u32 i=0; i < pMesh->getMeshBuffer(b)->getVertexCount(); i++) {
			if (pVertices[i].Pos.Y < minHeight)
				minHeight = pVertices[i].Pos.Y;

			if (pVertices[i].Pos.Y > maxHeight)
				maxHeight = pVertices[i].Pos.Y;

			terrainVerticesLines[b].push_back(&pVertices[i]);
			againUpVertices[b].push_back(true);
		}
	}
	heightInterval = (minHeight >= 0) ? maxHeight - minHeight : maxHeight + minHeight;

	terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

	oldTerrainPosition = node->getPosition();
	oldTerrainRotation = node->getRotation();
	oldTerrainScale = node->getScale();

	canPaint = true;

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
	radiussb->setMax((terrainSizeHeight) / 8);
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

void CUITerrainPainter::smoothTerrain(IMeshBuffer* mb, u32 indice, s32 smoothFactor)
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

void CUITerrainPainter::RaiseTerrainVertex(vector3df clickPos, f32 step, bool up) {
	f32 nodescale = node->getBoundingBox().getExtent().X;

	for (u32 i=0; i < node->getMesh()->getMeshBufferCount(); i++) {

		IMeshBuffer* meshBuffer = node->getMesh()->getMeshBuffer(i);
		if (meshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
		S3DVertex2TCoords* mb_vertices = (S3DVertex2TCoords*) meshBuffer->getVertices();

		for (u32 j = 0; j < meshBuffer->getVertexCount(); j += 1) {
			if (!againUpVertices[i][j]) continue;

			vector3df realPos = mb_vertices[j].Pos;//*(node->getScale()/nodescale) + (node->getPosition()-node->getTerrainCenter());
			clickPos.Y = realPos.Y;

			if(realPos.getDistanceFrom(clickPos) < currentRadius) {
				//f32 ratio = sin(radius - realPos.getDistanceFrom(clickPos));
				f32 ratio = currentRadius - realPos.getDistanceFrom(clickPos);
				mb_vertices[j].Pos.Y += (currentStep * (ratio));///(node->getScale().X/nodescale));
				againUpVertices[i][j] = false;
			}

			//if(mb_vertices[j].Pos.Y > nodescale/4) mb_vertices[j].Pos.Y = nodescale/4;
			//if(mb_vertices[j].Pos.Y < -(nodescale*0.1f)) mb_vertices[j].Pos.Y = -(nodescale*0.1f);
		}

		//devices->getSceneManager()->getMeshManipulator()->recalculateNormals(node->getMesh(),true);
		//terrainSelector = devices->getSceneManager()->createTriangleSelector(node->getMesh(), 0);

		//node->getMesh()->setDirty();
	}

	canPaint = false;
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

		vertexIndex = x * size + z;

		if ((leftClickDown || rightClickDown) && !devices->isCtrlPushed() && devices->getGUIEnvironment()->getFocus() == 0) {
			RaiseTerrainVertex(pos, currentStep, leftClickDown);
		}

		x *= scale;
		z *= scale;

		arrow->setPosition(vector3df(x, node->getHeight(x, z) + 5, z));
		drawBrush(vector3df(x, node->getHeight(x, z) + 5, z));
	}
}

bool CUITerrainPainter::OnEvent(const SEvent &event) {

	if (event.EventType == EET_MOUSE_INPUT_EVENT) {
		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
			currentRadius += event.MouseInput.Wheel*0.1f;
			radiusValueeb->setText(stringw(currentRadius).c_str());
			radiussb->setPos(currentRadius);
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
			//RECALCULATE TRIANGLES FOR COLLISIONS
			if (!devices->isCtrlPushed())
				terrainSelector = devices->getSceneManager()->createTerrainTriangleSelector(node, 0);

			//RESET AGAIN UP
			for (u32 i=0; i < againUpVertices.size(); i++)
				for (u32 j=0; j < againUpVertices[i].size(); j++)
					againUpVertices[i][j] = true;

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
				arrow->remove();
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

			if (event.GUIEvent.Caller == runSmoothing) {
				//SMOOTH BACK THE TERRAIN
				s32 smoothFactors32 = devices->getCore()->getS32(smoothFactor->getText());
				if (smoothFactors32 <= 0)
					smoothFactors32 = 1;

				IMesh* pMesh = node->getMesh(); 
				for (u32 b=0; b<pMesh->getMeshBufferCount(); ++b)  {
					IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b); 
					if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue; 
					smoothTerrain(pMeshBuffer, b, smoothFactors32);
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
			}
		}
	}

	return false;
}

void CUITerrainPainter::drawBrush(vector3df pos) {
	f32 radius = currentRadius;

	vector3df position = pos;//this->getMousePosition3D(100).pickedPos;

	if (position==vector3df(0,0,0))
		return;

	SMaterial m;
	m.Lighting=false;
	devices->getVideoDriver()->setMaterial(m);
	devices->getVideoDriver()->setTransform(video::ETS_WORLD, core::matrix4());

	// Render the size of the brush.
	f32 framesize = 5;
	int step=10;
	for (int i=0; i<(360); i=i+step) {
		float degInRad = i*DEGTORAD;
		vector3df pos=position;
		pos.X+=cos(degInRad)*radius;
		pos.Z+=sin(degInRad)*radius;
		pos.Y=getHeightAt(pos)+5;

		float degInRad2 = (i+step)*DEGTORAD;
		vector3df pos2=position;
		pos2.X+=cos(degInRad2)*radius;
		pos2.Z+=sin(degInRad2)*radius;
		pos2.Y=getHeightAt(pos2)+5;
		//driver->draw3DLine(pos,pos2,video::SColor(255,255,255,0));

		vector3df pos3=position;
		pos3.X+=cos(degInRad)*(radius+framesize);
		pos3.Z+=sin(degInRad)*(radius+framesize);
		pos3.Y=pos.Y;

		vector3df pos4=position;
		pos4.X+=cos(degInRad2)*(radius+framesize);
		pos4.Z+=sin(degInRad2)*(radius+framesize);
		pos4.Y=pos2.Y;

		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos4,pos3,pos),video::SColor(255,255,255,255));
		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos,pos2,pos4),video::SColor(255,255,255,255));
	}

	// Center circle for the brush give the center
	radius=5;
	framesize = 2;
	step=15;
	for (int i=0; i<(360); i=i+step) {
		float degInRad = i*DEGTORAD;
		vector3df pos=position;
		pos.X+=cos(degInRad)*radius;
		pos.Z+=sin(degInRad)*radius;
		pos.Y=getHeightAt(pos)+5;

		float degInRad2 = (i+step)*DEGTORAD;
		vector3df pos2=position;
		pos2.X+=cos(degInRad2)*radius;
		pos2.Z+=sin(degInRad2)*radius;
		pos2.Y=getHeightAt(pos2)+5;
		//driver->draw3DLine(pos,pos2,video::SColor(255,255,255,0));

		vector3df pos3=position;
		pos3.X+=cos(degInRad)*(radius+framesize);
		pos3.Z+=sin(degInRad)*(radius+framesize);
		pos3.Y=pos.Y;

		vector3df pos4=position;
		pos4.X+=cos(degInRad2)*(radius+framesize);
		pos4.Z+=sin(degInRad2)*(radius+framesize);
		pos4.Y=pos2.Y;

		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos4,pos3,pos),video::SColor(255,255,255,255));
		devices->getVideoDriver()->draw3DTriangle(triangle3df(pos,pos2,pos4),video::SColor(255,255,255,255));
	}
}

f32 CUITerrainPainter::getHeightAt(vector3df pos) {
	scene::ISceneCollisionManager* collMan = devices->getSceneManager()->getSceneCollisionManager();
	core::line3d<f32> ray;
    ray.start = pos+vector3df(0,+250,0);
    ray.end = ray.start + (pos+vector3df(0,-250.0f,0) - ray.start).normalize() * 1000.0f;
	core::vector3df intersection;
    core::triangle3df hitTriangle;
	scene::ISceneNode * selectedSceneNode =
    collMan->getSceneNodeAndCollisionPointFromRay(
		ray,
		intersection,
		hitTriangle,
		100,
		0);

	if (selectedSceneNode)
		return intersection.Y;
	else
		return 0;

    IMeshBuffer* meshBuffer = ((IMeshSceneNode*)node)->getMesh()->getMeshBuffer(0);
	S3DVertex* mb_vertices = (S3DVertex*) meshBuffer->getVertices();
	f32 height = 0;
	vector3df nearestVertex = vector3df(1000,1000,1000);

	for (unsigned int j = 0; j < meshBuffer->getVertexCount(); j += 1) {
		vector3df realPos = mb_vertices[j].Pos*(node->getScale()/node->getBoundingBox().getExtent().X) + node->getPosition();

	    if(pos.getDistanceFrom(vector3df(realPos.X,0,realPos.Z)) < pos.getDistanceFrom(vector3df(nearestVertex.X,0,nearestVertex.Z))) {
	        nearestVertex = realPos;
	        height = realPos.Y;
	    }
	}

	return height;
}
