//
//  CUIRenderingInfos.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 19/01/13.
//
//

#ifndef __C_UI_ANIMATED_MESH_VIEWER_H_INCLUDED__
#define __C_UI_ANIMATED_MESH_VIEWER_H_INCLUDED__

#include "../../Device/CDevices.h"
#include "../../GUIExtension/ViewPort/CGUIViewPort.h"

class CMeshFactory;

class CUIAnimatedMeshViewer : public IEventReceiver {

public:

	CUIAnimatedMeshViewer(CDevices *_devices);
	~CUIAnimatedMeshViewer();

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
    //DATAS
	CDevices *devices;

	ISceneManager *smgr;
	IAnimatedMesh *mesh;
	IAnimatedMeshSceneNode *node;

	IGUIFileOpenDialog *openDialog;

	bool meshAlreadyExists;
	stringw meshPath;

	CMeshFactory *factory;
	//-----------------------------------

	//-----------------------------------
    //GUI ELEMENTS
	/// Main
	IGUIWindow *window;
	CGUIViewport *viewPort;

	/// Righ view
	IGUIStaticText *zone;

	IGUIEditBox *scaleeb;
	IGUIButton *scalePlus, *scaleLess;

	IGUIStaticText *frameTxt;
	IGUIEditBox *frameBegin, *frameEnd;
	IGUIButton *previewFrame;

	IGUIButton *open, *reload;

	IGUIButton *close;
	//-----------------------------------

};

#endif