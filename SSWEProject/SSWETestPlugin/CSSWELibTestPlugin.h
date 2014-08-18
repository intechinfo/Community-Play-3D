//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_GENERIC_MONITOR_H_INCLUDED__
#define __C_GENERIC_MONITOR_H_INCLUDED__

#include <irrlicht.h>

#include <ISSWELibPlugin.h>

class CSSWELibTestPlugin : public cp3d::core::ISSWELibPlugin {
public:

	CSSWELibTestPlugin();
	~CSSWELibTestPlugin();

	virtual bool OnEvent(const SEvent &event);
	virtual void update();

	//-----------------------------------
	//ISSWELibPlugin

	void close();
	void open();

	//-----------------------------------

private:

	//-----------------------------------
	//DATAS

	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;

	//TOP
	IGUIComboBox *engines;
	IGUIEditBox *toFolder;
	IGUIButton *browseExportFolder;
	IGUIFileOpenDialog *toFolderFileOpenDialog;

	//MODELS
	IGUICheckBox *exportStaticModels;
	IGUICheckBox *exportAnimatedModels;
	IGUICheckBox *exportCustomMaterials;
	IGUICheckBox *exportEffectFilters;

	IGUICheckBox *exportAnimators;
	IGUICheckBox *exportTextures;
	IGUICheckBox *exportAnimations;
	IGUIButton *editExportAnimations;

	IGUICheckBox *exportLuaScripts;

	//DESIGN
	IGUICheckBox *createHTMLFile;
	IGUICheckBox *makeJSMinimized;

	//BUTTONS
	IGUIButton *exportbtn;
	IGUIButton *exportInformations;
	//-----------------------------------

};

#endif
