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
#include "../../../User Interfaces/CUIWindowEditNode.h"

#include "../../../GUI Extension/CodeEditor/CGUICodeEditor.h"
#include "../../../GUI Extension/ViewPort/CGUIViewPort.h"

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
    
    IGUIContextMenu *menu, *submenu;
    
    IGUIButton *removeOGLMaterialShader, *addOGLMaterialShader, *editOGLMaterialShader;
    IGUIListBox *openGLMaterialsList;
    
    IGUIButton *removeD3DMaterialShader, *addD3DMaterialShader, *editD3DMaterialShader;
    IGUIListBox *direct3DMaterialsList;
    
    IGUIEditBox *materialName;
    
    IGUIButton *close;
    //-----------------------------------
    
    //-----------------------------------
    //EDIT WINDOW
    IGUIWindow *editMaterialWindow;
    
    IGUIButton *vLoadFromFile, *vEdit;
    IGUIButton *pLoadFromFile, *pEdit;
    
    IGUIComboBox *vShaderType;
    IGUIComboBox *pShaderType;
    IGUIComboBox *bShaderType;
    
    CGUIEditBoxIRB *constantsCodeBox;
    IGUIComboBox *editorChoice;
    
    IGUIViewport *viewPort;
    ISceneManager *smgr;
    IGUIButton *buildMaterial, *editPreviewNode;
    IGUIComboBox *previewNodeChoice;
    
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
    bool openingVertex, openingPixel;
    bool editingConstants, editingVertexShader, editingPixelShader;
    //-----------------------------------
    
};

#endif
