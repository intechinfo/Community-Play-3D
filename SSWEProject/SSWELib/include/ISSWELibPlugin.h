#ifndef __I_SSWE_LIB_PLUGIN_H_INCLUDED__
#define __I_SSWE_LIB_PLUGIN_H_INCLUDED__

/*
Interface for CP3D GUI Plugins.

These plugins will be used to allow users add features to the editor.
To get an example you can refer the CP3DUltimateTool plugin made by CP3D (open-source) at :

With these plugins you can develop your own tools to create a scenario maker as example, and then create your scenario exporter.
The goal is to have the possibility of making tests in real-time with real-time results.
*/

#include <irrlicht.h>

#include <IDevices.h>

class ISSWELibPlugin : irr::IEventReceiver, IUpdate {
public:
	/// Sets the CP3D device that can acceed to the irrlicht device
	/// Cf. IDevices.h
	void setDevices(IDevices *_devices) {
		devices = _devices;
	}
	/// Returns the CP3D device. So far only used by CP3D
	IDevices *getDevices() {
		return devices;
	}

	/// Sets the working directory of the plugin
	/// directory = Volume:/.../Plugins/SSWE/
	void setWorkingDirectory(irr::core::stringw _workingDirectory) {
		workingDirectory = _workingDirectory;
	}

	/// update method, called every loop
	virtual void update() = 0;

	/// Event receiver
	virtual bool OnEvent(const SEvent &event) = 0;

	/// Called after the CP3D has built the plugin's instance
	/// open() is used to initialize elements like GUI elements, scene nodes, etc.
	virtual void open() {
		devices->getEventReceiver()->AddEventReceiver(this, 0, this);
	}

	/// Called when the the specialized class of your plugins calls close
	/// close() is used to remove elements like GUI elements, scene nodes, etc.
	/// Then, you can free the memory yourself
	virtual void close() {
		devices->getEventReceiver()->RemoveEventReceiver(this);
		delete this;
		//devices->getCoreData()->destroySSWEPlugin(this);
	}

protected:
	/// Main class that is a more advanced Irrlicht Device
	IDevices *devices;

	/// plugin's working directory (example : C:/my_folder/SSWE/Plugins/SSWE/)
	irr::core::stringw workingDirectory;
};

#endif
