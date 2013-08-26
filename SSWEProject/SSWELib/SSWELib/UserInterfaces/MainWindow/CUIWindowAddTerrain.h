//
//  CUIWindowAddOctTree.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 27/09/12.
//
//

#ifndef __C_UI_WINDOW_ADD_OCT_TREE_H_INCLUDED__
#define __C_UI_WINDOW_ADD_OCT_TREE_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../SceneNodes/TerrainSystem/CTerrainSystem.h"

enum GUI_WINDOW_EVENTS_ADD_OCT_TREE {
    CXT_WINDOW_ADD_OCT_TREE_EVENTS_CLOSE = 0x30000
};

class CUIWindowAddOctTree : public IEventReceiver {
  
public:
    
    CUIWindowAddOctTree(CDevices *_devices, IGUIListBox *_terrainsListBox);
    ~CUIWindowAddOctTree();

    void open();

    bool OnEvent(const SEvent &event);

private:
    
	//-----------------------------------
    //DEVICES AND DATAS
    CDevices *devices;
	//-----------------------------------
    
	//-----------------------------------
    //GUI ELEMENTS
	IGUIListBox *terrainsListBox;

	IGUIWindow *addOctTreeWindow;
	IGUIComboBox *terrainTypecb;
	IGUIButton *accept, *close;

	IGUIEditBox *addOctTreeEditBox;
	IGUIButton *selectMesh;

	//MESH
    IGUICheckBox *asMeshSceneNode;
	IGUIStaticText *minPolysPerNodeTxt;
    IGUIEditBox *minPolysPerNode;
	IGUIFileOpenDialog *openMesh;

	//TERRAIN
	IGUIComboBox *terrainPatchSizecb;
	IGUIEditBox *terrainMaxLODeb;
	IGUIEditBox *terrainSmoothFactoreb;

	//TERRAIN SPLATTING
	IGUIButton *terrainsOpenHeightMap;
	IGUIFileOpenDialog *openTerrainsHeightMap;
	stringw path_terrains_hgtmap;

	IGUIEditBox *terrainsMapSizeeb;
	IGUIEditBox *terrainsDimensioneb;
	IGUIEditBox *terrainsLODTextureScaleeb;
	IGUIEditBox *terrainsScale;
	IGUIComboBox *terrainsQualitycb;
	IGUIEditBox *terrainsDistanceMaxRendereb;
	IGUIEditBox *terrainsDistanceLoadeb;
	IGUIEditBox *terrainsDistanceUnloadeb;
	IGUICheckBox *terrainsEnableFog;

	IGUIEditBox *terrainsTexturePath;
	IGUIButton *terrainsSelectTexture;
	IGUIFileOpenDialog *openTerrainsTexture;
	stringw path_terrains_texture;
	//-----------------------------------

	//-----------------------------------
    //DATAS
    stringw path_file;
	//-----------------------------------

	//-----------------------------------
    //ENUMS
	enum ELOAD_TERRAIN_TYPE {
		ELTT_MESH = 0,
		ELTT_TERRAIN,
		ELTT_TERRAIN_SPLATTING
	} terrainType;
	//-----------------------------------
    
};

#endif
