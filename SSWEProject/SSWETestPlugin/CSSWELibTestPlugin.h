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
	IGUIButton *closeWindow;
	//-----------------------------------

};

#endif