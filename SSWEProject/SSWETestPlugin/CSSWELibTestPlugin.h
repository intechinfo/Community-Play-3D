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
#include <DLLExport.h>

class CSSWELibTestPlugin : ISSWELibPlugin, IEventReceiver {
public:

	CSSWELibTestPlugin();
	~CSSWELibTestPlugin();

	bool OnEvent(const SEvent &event);

	//-----------------------------------
	//ISSWELibPlugin

	void setDevices(IDevices *_devices);
	IDevices *getDevices() { return devices; }

	void close();
	void open();

	//-----------------------------------

private:

	//-----------------------------------
	//DATAS
	IDevices *devices;
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