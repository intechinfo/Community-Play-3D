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

	//-----------------------------------
	//EDIT WINDOW
	IGUIWindow *editMaterialWindow;

	IGUIButton *vLoadFromFile, *vEdit;
	IGUIButton *pLoadFromFile, *pEdit;
	IGUIButton *cLoadFromFile, *cEdit;

	IGUIComboBox *vShaderType;
	IGUIComboBox *pShaderType;
	IGUIComboBox *bShaderType;

	IGUIButton *gLoadFromFile, *gEdit;

	CGUIEditBoxIRB *constantsCodeBox;
	IGUIComboBox *editorChoice;

	IGUIStaticText *separator, *previewText, *previewObjectText, *editText;

	IGUIViewport *viewPort;
	ISceneManager *smgr;
	IGUIButton *buildMaterial, *editPreviewNode;
	IGUIComboBox *previewNodeChoice;

	IGUIListBox *console;
	ELOG_LEVEL logLevel;

	IGUIButton *closeEditMaterialWindow;
	//-----------------------------------

	//-----------------------------------
	//EDITING NODES
	ISceneNode *previewNode;
	IAnimatedMesh *planeMesh;
	E_MATERIAL_TYPE editingMat;
	//-----------------------------------

	//-----------------------------------
	//EDITING VALUES
	bool openingVertex, openingPixel, openingGeometry;
	bool editingConstants, editingVertexShader, editingPixelShader;
	//-----------------------------------

};

#endif
