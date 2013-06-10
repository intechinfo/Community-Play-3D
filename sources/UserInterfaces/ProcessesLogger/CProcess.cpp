//
//  CUIRenderingInfos.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//
#include "stdafx.h"
#include "CProcess.h"

CProcess::CProcess(IGUIEnvironment *_gui, stringw _name) {
	gui = _gui;
	name = _name;
	finished = false;

	bg = gui->addStaticText(L"", rect<s32>(10, 30, 280, 90), true, false, 0, -1, true);

	nameTxt = gui->addStaticText(name.c_str(), rect<s32>(0, 3, 270, 30), false, true, bg, -1, false);
	nameTxt->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);

	bar = new CProgressBar(gui, -1, gui->getRootGUIElement());
	bar->setDimension(260, 25);
	bar->setTextColor(SColor(255, 255, 255, 255));
	bar->setBackgroundColor(SColor(255,0x44,0x44,0x44));
	bar->setFillColor(SColor(255,0x66,0x66,0x66));
}

CProcess::~CProcess() {
	nameTxt->remove();
	bg->remove();
	bar->remove();
}

