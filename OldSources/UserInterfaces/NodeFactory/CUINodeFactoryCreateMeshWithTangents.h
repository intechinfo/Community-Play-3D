#ifndef __C_UI_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS_H_INCLUDED__
#define __C_UI_NODE_FACTORY_CREATE_MESH_WITH_TANGENTS_H_INCLUDED__

#include "../../Device/CDevices.h"

class CUINodeFactoryCreateMeshWithTangents : public IEventReceiver {

public:

	CUINodeFactoryCreateMeshWithTangents(CDevices *_devices);
	~CUINodeFactoryCreateMeshWithTangents();

	void open(ISceneNode *node, IMesh *mesh);

	void setType(ESCENE_NODE_TYPE type) { nodeType = type; }
	void setMinPolysPerNode(u32 value) { nodeMinPolysPerNode = value; }
	void setMeshPath(stringc path) { meshPath = path.c_str(); }

	bool OnEvent(const SEvent &event);

private:

	void create();

	//-----------------------------------
	//DEVICES ELEMENTS AND DATAS
	CDevices *devices;

	ISceneNode *nodeToEdit;
	IMesh *meshToEdit;
	ESCENE_NODE_TYPE nodeType;
	u32 nodeMinPolysPerNode;
	stringc meshPath;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS
	IGUIWindow *window;
	IGUICheckBox *recalculateNormals, *smooth, *angleWeighted, *recalculateTangents;

	IGUIButton *accept, *cancel;

	IGUIWindow *sure;
	//-----------------------------------

};

#endif
