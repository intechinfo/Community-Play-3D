//
//  CUIEditEffects.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 20/11/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_MATERIALS_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_MATERIALS_H_INCLUDED__

#include "../../../Device/CDevices.h"
#include "../../../UserInterfaces/CUIWindowEditNode.h"

#include "../../../GUIExtension/CodeEditor/CGUICodeEditor.h"
#include "../../../GUIExtension/ViewPort/CGUIViewPort.h"

#include "CShaderCallback.h"
#include "CUIWindowEditMaterialsCallback.h"

#include <fstream>

class CUIWindowEditMaterials : public IEventReceiver {

public:

	CUIWindowEditMaterials(CDevices *_devices);
	~CUIWindowEditMaterials();

	void open();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;

	//-----------------------------------
	//MAIN WINDOW
	IGUIWindow *materialsWindow;

	IGUIContextMenu *menu;
	IGUIContextMenu *fileMenu, *helpMenu;

	IGUIFileOpenDialog *openShaderPackage, *saveShaderPackage;

	IGUIButton *removeOGLMaterialShader, *addOGLMaterialShader, *editOGLMaterialShader;
	IGUIListBox *openGLMaterialsList;

	IGUIEditBox *materialName;

	IGUIButton *close;
	//-----------------------------------

};

#endif
