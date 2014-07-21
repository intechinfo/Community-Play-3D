//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_SSWE_ULTIMATE_TOOL_H_INCLUDED__
#define __C_SSWE_ULTIMATE_TOOL_H_INCLUDED__

/*
include irrlicht.h to get irrlicht methods
include ISSWELibPlugin.h to enable plugin system
include DLLExport.h to precise how the createSSWELibPlugin will be compiled
*/

#include <irrlicht.h>

#include <ISSWELibPlugin.h>

class CSSWEUltimateTool : public ISSWELibPlugin {
public:

	/// Constructor of the class
	CSSWEUltimateTool();
	/// Destructor
	~CSSWEUltimateTool();

	/// Method to handle events into the plugin
	/// Herited from irrlicht irr::IEventReceiver
	bool OnEvent(const SEvent &event);

	/// update method to update the current plugin.
	/// Here, we update the window relative positions
	void update();

	//-----------------------------------
	//ISSWELibPlugin

	/// Close the plugin (ISSWELibPlugin)
	void close();
	/// Open the plugin (ISSWELibPlugin)
	void open();

	//-----------------------------------

private:

	//-----------------------------------
	//METHODS
	/// Adds an area text to specify what is the section
	void addArea(irr::core::stringw name);
	/// Adds a button in the last area
	irr::gui::IGUIButton *addButton(irr::core::stringw tooltipText, irr::video::ITexture *texture);
	/// Gets the Y element's position for the areas and buttons
	irr::u32 getNewElementPosition();

	/// Method to run the openscad application
	void runOpenSCAD();
	void buildOpenSCADFromFile();
	//-----------------------------------

	//-----------------------------------
	//DATAS
	/// Check is button is at left or right  of the line. Default value : true
	bool buttonIsLeftElement:1;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	irr::gui::IGUIWindow *window;

	/// Meshes
	irr::gui::IGUIButton *addSuzanneButton,
						 *addCylinderButton,
						 *addCornerButton;

	/// OpenSCAD
	irr::gui::IGUIButton *createNewOpenSCADDocButton,
						 *openscadButton,
						 *openscadOpenButton;

	/// Primitives
	irr::gui::IGUIButton *splitSelectedNodeButton;

	/// Sound
	irr::gui::IGUIButton *soundViewerButton;

	/// Quit button
	irr::gui::IGUIButton *quitButton;

	/// FileOpen dialogs
	IGUIFileOpenDialog *openScadFileDialog;
	//-----------------------------------

};

#endif
