//
//  CUIWindowEditNode.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 29/09/12.
//
//

#ifndef __C_UI_WINDOW_EDIT_NODE_H_INCLUDED__
#define __C_UI_WINDOW_EDIT_NODE_H_INCLUDED__

#include "../Device/CDevices.h"

#include "../CharacterEdition/CAction.h"

enum CXT_EDIT_WINDOW_EVENTS {
    CXT_EDIT_WINDOW_EVENTS_GENERAL_POSITION = 0x50000,
    
    CXT_EDIT_WINDOW_EVENTS_TEXLAYER_1,
    CXT_EDIT_WINDOW_EVENTS_TEXLAYER_2,
    CXT_EDIT_WINDOW_EVENTS_TEXLAYER_3,
    CXT_EDIT_WINDOW_EVENTS_TEXLAYER_4,
    CXT_EDIT_WINDOW_EVENTS_GENERAL_LIGHTING,
    CXT_EDIT_WINDOW_EVENTS_GENERAL_VISIBLE,
    CXT_EDIT_WINDOW_EVENTS_GENERAL_MATERIAL_TYPE,
    CXT_EDIT_WINDOW_EVENTS_GENERAL_SHADOWS,
    CXT_EDIT_WINDOW_EVENTS_GENERAL_DEPTH_PASS,
    CXT_EDIT_WINDOW_EVENTS_GENERAL_EXCL_LIGHT_CALC,
    
    CXT_EDIT_WINDOW_EVENTS_PATCH_SIZE,
    
    CXT_EDIT_WINDOW_EVENTS_LIGHTING,
    CXT_EDIT_WINDOW_EVENTS_MATERIALS,
    
    CXT_EDIT_WINDOW_EVENTS_TEXTURE_1,
    
    CXT_EDIT_WINDOW_EVENTS_SELECT_1,
    CXT_EDIT_WINDOW_EVENTS_SELECT_2,
    CXT_EDIT_WINDOW_EVENTS_SELECT_3,
    CXT_EDIT_WINDOW_EVENTS_SELECT_4,
    
    CXT_EDIT_WINDOW_EVENTS_DIFFUSE_A,
    CXT_EDIT_WINDOW_EVENTS_DIFFUSE_R,
    CXT_EDIT_WINDOW_EVENTS_DIFFUSE_G,
    CXT_EDIT_WINDOW_EVENTS_DIFFUSE_B,
    
    CXT_EDIT_WINDOW_EVENTS_AMBIANT_A,
    CXT_EDIT_WINDOW_EVENTS_AMBIANT_R,
    CXT_EDIT_WINDOW_EVENTS_AMBIANT_G,
    CXT_EDIT_WINDOW_EVENTS_AMBIANT_B,
    
    CXT_EDIT_WINDOW_EVENTS_SPECULAR_A,
    CXT_EDIT_WINDOW_EVENTS_SPECULAR_R,
    CXT_EDIT_WINDOW_EVENTS_SPECULAR_G,
    CXT_EDIT_WINDOW_EVENTS_SPECULAR_B,
    
    CXT_EDIT_WINDOW_EVENTS_EMISSIVE_A,
    CXT_EDIT_WINDOW_EVENTS_EMISSIVE_R,
    CXT_EDIT_WINDOW_EVENTS_EMISSIVE_G,
    CXT_EDIT_WINDOW_EVENTS_EMISSIVE_B,
    
    CXT_EDIT_LIGHT_WINDOW_EVENTS_ADVANCED_COLOR,
    
    CXT_EDIT_WINDOW_EVENTS_MATERIAL_TYPE,
    
    CXT_EDIT_WINDOW_EVENTS_ANISOTROPIC_FILTER,
    CXT_EDIT_WINDOW_EVENTS_BILINEAR_FILTER,
    CXT_EDIT_WINDOW_EVENTS_TRILINEAR_FILTER,
    
    CXT_EDIT_WINDOW_EVENTS_ANTI_ALIASING,
    
    CXT_EDIT_WINDOW_EVENTS_BACK_FACE_CULLING,
    CXT_EDIT_WINDOW_EVENTS_FRONT_FACE_CULLING,
    
    CXT_EDIT_WINDOW_EVENTS_COLOR_MASK,
    CXT_EDIT_WINDOW_EVENTS_COLOR_MATERIAL,
    
    CXT_EDIT_WINDOW_EVENTS_TEXTURE_WRAP,
    CXT_EDIT_WINDOW_EVENTS_ZBUFFER,
    CXT_EDIT_WINDOW_EVENTS_ZWRITE_ENABLE,

	CXT_EDIT_WINDOW_EVENTS_DRAW_ANIMATIONS,
    
    CXT_EDIT_WINDOW_EVENTS_APPLY_BUTTON,
    CXT_EDIT_WINDOW_EVENTS_CLOSE_BUTTON
};

class CUIWindowEditNode : public IEventReceiver, cp3d::core::IUpdate {
    
public:
    
    CUIWindowEditNode(CDevices *_devices);
    ~CUIWindowEditNode();
    
    void open(ISceneNode *node, stringw prefix);
    void open(ISceneNode *node, stringw prefix, bool modal);
    
    bool isVisible() { return editWindow->isVisible(); }
    void setVisible() { editWindow->setVisible(!editWindow->isVisible()); }

	void activateCloseButtons(bool activateCloseButton, bool activateWindowCloseButton);
    
    void createMaterialTypesComboBox(IGUIElement *element);
    
    E_MATERIAL_TYPE getMaterialType(s32 pos);

	void update();
    bool OnEvent(const SEvent &event);
    
private:

	//-----------------------------------
    //FLIP COLLABORATIVE
	
	//-----------------------------------
    
    CDevices *devices;
    
    void setMaterialTextures();
    s32 getMaterialTypeCB(E_MATERIAL_TYPE type);

	void applyToMeshBuffers(u32 id, ITexture *tex);
    
    //-----------------------------------
    //EDIT WINDOW USER INTERFACE ELEMENTS
    IGUIWindow *editWindow;
    IGUITabControl *tabCtrl;
    
    IGUITab *generalTab, *advancedTab, *flagsTab, *generalFlagsTab, *physicsTab, *animatedTab;
    
    IGUIButton *applyButton, *closeButton;
    
    //GENERAL TAB
    IGUIEditBox *ebNodeName;
	IGUIButton *cursorPositionbtn;
    IGUIEditBox *ebNodePositionX, *ebNodePositionY, *ebNodePositionZ;
    IGUIEditBox *ebNodeRotationX, *ebNodeRotationY, *ebNodeRotationZ;
    IGUIEditBox *ebNodeScaleX, *ebNodeScaleY, *ebNodeScaleZ;
    IGUIEditBox *ebTextureLayerPath1, *ebTextureLayerPath2, *ebTextureLayerPath3, *ebTextureLayerPath4;
    IGUIButton *browseButton1, *browseButton2, *browseButton3, *browseButton4;
    IGUICheckBox *generalLighting, *generalVisible;
    IGUIComboBox *generalMaterialCB;
    IGUIComboBox *shadowed;
    IGUICheckBox *nodeToDepthPass, *excludeLightingCalc; //EXCLUDE LIGHTING CALCULATION IS NOW LIGHT SCATTERING
    
    //HEIGHT MAPS TAB
    IGUIComboBox *patchSize;
    IGUIEditBox *maxLOD;
    
    //FLAGS & MATERIALS TAB
    IGUISpinBox *ravelSpin;
    IGUICheckBox *lighting;
    IGUIScrollBar *materialsBar;
    IGUIStaticText *materialNumber;
    IGUIEditBox *mTexture1, *mTexture2, *mTexture3, *mTexture4;
    IGUIEditBox *mNodeDiffuseColorR, *mNodeDiffuseColorG, *mNodeDiffuseColorB, *mNodeDiffuseColorA;
    IGUIEditBox *mNodeAmbiantColorR, *mNodeAmbiantColorG, *mNodeAmbiantColorB, *mNodeAmbiantColorA;
    IGUIEditBox *mNodeSpecularColorR, *mNodeSpecularColorG, *mNodeSpecularColorB, *mNodeSpecularColorA;
    IGUIEditBox *mNodeEmissiveColorR, *mNodeEmissiveColorG, *mNodeEmissiveColorB, *mNodeEmissiveColorA;
    IGUIComboBox *materialType;
    IGUICheckBox *anisotropicFilter, *bilinearFilter, *trilinearFilter;
    IGUICheckBox *antiAliasing;
    IGUICheckBox *backFaceCulling, *frontFaceCulling;
    IGUICheckBox *colorMask, *colorMaterial;
    IGUICheckBox *textureWrap, *zbuffer, *zwriteEnable;
    
    //GLOBAL FLAGS
    IGUICheckBox *gAnisotropicFilter, *gBilinearFilter, *gTrilinearFilter;
    IGUICheckBox *gAntiAliasing;
    IGUICheckBox *gBFCulling, *gFFCulling;
    IGUICheckBox *gColorMask, *gColorMaterial;
    IGUICheckBox *gTextureWrap, *gZBuffer, *gZWriteEnable;

	//PHYSICS
	IGUICheckBox *penablePhysics;
	IGUIComboBox *pBodyType;
	IGUIEditBox *pMasseb;
	IGUIButton *pEditBody;
	cp3d::core::ISData::E_ISDATA_PHYSIC_TYPE lastPhysicBodyType;

	//ANIMATED
	IGUICheckBox *drawAnimations;
	IGUIEditBox *savedAnimationsPath;
	IGUIButton *browseSavedAnimations;
	IGUIComboBox *chooseSavedAnimation;
	IGUIFileOpenDialog *browseSavedAnimationDialog;

    //-----------------------------------
    
    //-----------------------------------
    //EDITING NODE INFORMATIONS
    ISceneNode *nodeToEdit;
    stringw nodeToEditPrefix;
    u32 currentBrowse;
    bool isWindowed;
    
    //FLAGS & Materials
    u32 rsCurrentPos, totalSpacing;

	s32 lastMaterialType;
	u32 lastMaterial;
    //-----------------------------------
    
};

#endif
