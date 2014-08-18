//
//  CUIMeshSimplificator.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_MESH_SIMPLIFICATOR_H_INCLUDED__
#define __C_UI_MESH_SIMPLIFICATOR_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../GUIExtension/ViewPort/CGUIViewPort.h"
#include "../../GUIExtension/CodeEditor/CGUICodeEditor.h"

/// Tell the compiler that the CUIMeshSimplificatorUI class exists somewhere
class CUIMeshSimplificatorUI;

class CUIMeshSimplificator : public IEventReceiver {

public:

	CUIMeshSimplificator(CDevices *_devices);
	~CUIMeshSimplificator();

	bool OnEvent(const SEvent &event);

private:

	CDevices *devices;
	ISceneManager *smgr;

	IMesh *mesh;
	IMeshSceneNode *node;
	stringw consoleText;

	//-----------------------------------
    //GUI ELEMENTS
	CUIMeshSimplificatorUI *ui;
	IGUIFileOpenDialog *openMesh;
	//-----------------------------------

};

/// Declare the CUIMeshSimplificatorUI class
class CUIMeshSimplificatorUI {
	/// Tell that the main class (my class) will access to the private members
	/// Then, only class will access to the gui elements
	friend class CUIMeshSimplificator;
public:

	/// Constructor, create GUI elements here using the "gui" pointer
	CUIMeshSimplificatorUI(IGUIEnvironment *gui, IrrlichtDevice *device) {
		window = gui->addWindow(rect<s32>(580, 150, 1470, 690), /// draw rectangle
			false, /// modal window ?
			L"Mesh Simplificator Tool", /// Caption
			0, /// Parent ? 0 if parent is root
			-1); /// ID

		viewPort = new CGUIViewport(gui, window, -1, rect<s32>(10, 30, 560, 370));
        console = gui->addEditBox(L"", rect<s32>(10, 380, 560, 530), true, window, -1);
		addMesh = gui->addButton(rect<s32>(570, 30, 880, 50), window, -1, L"Open Mesh...", L"Open a new mesh");
		meshName = gui->addStaticText(L"No mesh selected.", rect<s32>(570, 60, 880, 80), true, true, window, -1, true);
		percentage = gui->addStaticText(L"Percentage :", rect<s32>(570, 100, 670, 120), true, true, window, -1, true);
		percentageEditBox = gui->addEditBox(L"95.0", rect<s32>(680, 100, 870, 120), true, window, -1);
		compute = gui->addButton(rect<s32>(570, 130, 880, 150), window, -1, L"Compute", L"Compute the mesh buffer");
		scaletxt = gui->addStaticText(L"Scale :", rect<s32>(570, 180, 880, 200), true, true, window, -1, true);

		scalebar = gui->addScrollBar(true, rect<s32>(570, 200, 880, 215), window, -1);
		scalebar->setMin(1);
		scalebar->setMax(1000);
		scalebar->setPos(10);

		accept = gui->addButton(rect<s32>(670, 500, 770, 530), window, -1, L"Accept", L"Accept");
		close = gui->addButton(rect<s32>(780, 500, 880, 530), window, -1, L"Close", L"Close and discard operations");
	}

	/// Destructor, remove the GUI parent elements here
	~CUIMeshSimplificatorUI() {
		/// Window is the higher level in the graph
		window->remove();
	}

private:
	IGUIWindow *window;
	CGUIViewport *viewPort;
	IGUIEditBox *console;
	IGUIButton *addMesh;
	IGUIStaticText *meshName;
	IGUIStaticText *percentage;
	IGUIEditBox *percentageEditBox;
	IGUIButton *compute;
	IGUIStaticText *scaletxt;
	IGUIScrollBar *scalebar;
	IGUIButton *close;
	IGUIButton *accept;
};

#endif