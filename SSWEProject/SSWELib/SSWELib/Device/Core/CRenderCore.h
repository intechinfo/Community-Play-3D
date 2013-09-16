//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_RENDER_CORE_H_INCLUDED__
#define __C_RENDER_CORE_H_INCLUDED__

#include "../../../../SSWECore/SSWECore/Core/CCore.h"

class CDevices;

class CRenderCore {
    
public:
    
    CRenderCore(CDevices *_devices);
    ~CRenderCore();

	void update();
    
private:
    
	CDevices *devices;

	ITexture *rt[6];
	ISceneNode *skybox;
    ICameraSceneNode *camera;
	vector3df lastUpdatePosition;
};

#endif
