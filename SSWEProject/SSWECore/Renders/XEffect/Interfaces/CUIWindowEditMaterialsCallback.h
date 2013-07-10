//
//  CRenderCallback.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_MATERIALS_CALLBACK_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_MATERIALS_CALLBACK_H_INCLUDED__

#include "Device/CDevices.h"
#include "UserInterfaces/CUIWindowEditNode.h"
#include "UserInterfaces/CUIWindowEditWater.h"

#include "GUIExtension/CodeEditor/CGUICodeEditor.h"
#include "GUIExtension/ViewPort/CGUIViewPort.h"

#include "CShaderCallback.h"

class CUIWindowEditMaterialsCallback : public IEventReceiver  {
public:
    
    CUIWindowEditMaterialsCallback(CDevices *_devices);
	~CUIWindowEditMaterialsCallback();

	bool OnEvent(const SEvent &event);

	//-----------------------------------
	//METHODS
	void open(CShaderCallback *_callback);
	//-----------------------------------

	//-----------------------------------
	//GETTERS
	CDevices *getDevices();
	rect<s32> getSize();
	CShaderCallback *getCallback();
	SWaterSurfacesData *getWaterSurfaceData();
	void setWaterSurfaceData(SWaterSurfacesData *_watersurfaceData);
	//-----------------------------------

private:
    
	//-----------------------------------
	//DATAS
	CDevices *devices;

	IVideoDriver *driver;
	IGUIEnvironment *gui;

	CShaderCallback *callback;
	//-----------------------------------
    
	//-----------------------------------
	//PREVIEW DATAS
	ISceneManager *smgr;
	ISceneNode *previewNode;
	IAnimatedMesh *planeMesh;
	SWaterSurfacesData *waterSurfaceData;
	//-----------------------------------

	//-----------------------------------
	//devices->getGUIEnvironment() ELEMENTS
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
	IGUIButton *buildMaterial, *editPreviewNode;
	IGUIComboBox *previewNodeChoice;

	IGUIListBox *console;
	ELOG_LEVEL logLevel;

	IGUIButton *closeEditMaterialWindow;

	CUIWindowEditWater *editWaterAddon;
	//-----------------------------------

	//-----------------------------------
	//EDITING INFORMATIONS
	bool editingConstants, editingVertexShader, editingPixelShader;
	IGUIFileOpenDialog *openConstantsfod, *openVertexShaderfod, *openPixelShaderfod;
	rect<s32> oldSize;
	//-----------------------------------

};

#endif
