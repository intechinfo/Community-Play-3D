#ifndef __C_UI_NODE_FACTORY_PLANAR_MAPPING_H_INCLUDED__
#define __C_UI_NODE_FACTORY_PLANAR_MAPPING_H_INCLUDED__

#include "Device/CDevices.h"

class CUINodeFactoryPlanarMapping : public IEventReceiver {

public:

	CUINodeFactoryPlanarMapping(CDevices *_devices);
	~CUINodeFactoryPlanarMapping();

	void open(ISceneNode *node, IMesh *mesh);

	bool OnEvent(const SEvent &event);

private:

	//-----------------------------------
	//DEVICES ELEMENTS AND DATAS
	CDevices *devices;

	ISceneNode *nodeToEdit;
	IMesh *meshToEdit;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;
	IGUIEditBox *horizontalResolution, *verticalResolution;
	IGUIComboBox *axis;
	IGUIEditBox *offsetX, *offsetY, *offsetZ;
	IGUICheckBox *generalPTM;

	IGUIButton *accept, *cancel;

	IGUIWindow *messageBoxAccept;
	//-----------------------------------

};

#endif
