//
//  CExporter.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//
#include "stdafx.h"
#include "CSSWELibTestPlugin.h"

void* createSSWELibPlugin() {
	return static_cast< void* > (new CSSWELibTestPlugin);
}

CSSWELibTestPlugin::CSSWELibTestPlugin() {

}

CSSWELibTestPlugin::~CSSWELibTestPlugin() {

}

void CSSWELibTestPlugin::close() {

}

void CSSWELibTestPlugin::open() {
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	gui->addWindow(rect<s32>(0, 0, 500, 500), false, L"Plugin test !", 0, -1);
}
