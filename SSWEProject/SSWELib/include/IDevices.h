#ifndef __I_DEVICES_H_INCLUDED__
#define __I_DEVICES_H_INCLUDED__

#include "../SSWELib/GUIExtension/FileSelector/CGUIFileSelector.h"

#include <ICollisionManager.h>
#include <ISSWECore.h>
#include <ISSWECoreData.h>
#include <ISSWERender.h>
#include <IGUICodeEditor.h>
#include <IAnimationController.h>
#include <IMonitorRegisterer.h>
#include <IFactory.h>
#include <IMeshSimplificator.h>

namespace cp3d {
namespace core {

class IDevices {

public: 
	//-----------------------------------
	//IRRLICHT
	/// Returns the irrlicht device
	virtual irr::IrrlichtDevice *getDevice() = 0;
	/// Returns the irrlicht video driver
	virtual irr::video::IVideoDriver *getVideoDriver() = 0;

	/// Returns the main scene manager
	virtual irr::scene::ISceneManager *getSceneManager() = 0;

	/// Removes a scene manager from the list of scene managers
	virtual void removeSceneManager(irr::scene::ISceneManager *smgrToDelete) = 0;
	/// Sets the scene manager to draw as main
	virtual void setSceneManagerToDraw(irr::scene::ISceneManager *smgrToDraw) = 0;

	/// Creates a new scene manager (added into the scene managers)
	virtual irr::scene::ISceneManager *createNewSceneManager() = 0;
	/// Returns the current scene manager that is rendering the scene
	virtual irr::scene::ISceneManager *getRenderingSceneManager() = 0;

	/// Returns the GUI environment
	virtual irr::gui::IGUIEnvironment *getGUIEnvironment() = 0;

	/// Returns the IEventReceiver manager
	virtual ISSWEEventsManager *getEventReceiver() = 0;

	/// Returns the working directory of the application
	virtual irr::core::stringc getWorkingDirectory() = 0;
	//-----------------------------------

	//-----------------------------------
	//RENDERS
	/// Returns the ISSWERender pointer, for rendering the scene
	virtual video::ISSWERender *getXEffect() = 0;
	/// Returns the shadows filter type
	virtual video::E_FILTER_TYPE getXEffectFilterType() = 0;
	/// Sets if draw the scene or not with the ISSWERender
	virtual void setXEffectDrawable(bool draw) = 0;
	/// Returns if the ISSWERender is rendering the scene
	virtual bool isXEffectDrawable() = 0;
	/// Returns the current shader extension (.glsl for OpenGL & .hlsl for D3D devices)
	virtual irr::core::stringc getShaderExt() = 0;

	/// Returns if the scene is rendered (using ISSWERender and default rendering method)
	virtual bool isRenderScene() = 0;
	/// Sets if render the scene
	virtual void setRenderScene(bool _renderScene) = 0;

	/// Sets if render the GUI
	virtual void setRenderGUI(bool _renderGUI) = 0;
	/// Returns if the GUI is rendered
	virtual bool isRenderingGUI() = 0;

	/// Sets if we render the full traitments (includes occlusion queries)
	virtual void setRenderFullPostTraitements(bool _render) = 0;
	/// Returns if the full traitements are renderered
	virtual bool isRenderingFullPostTraitements() = 0;

	/// Updates the CP3D device
	virtual void updateDevice() = 0;
	//-----------------------------------

	//-----------------------------------
	//CORE
	/// Returns the Core pointer, containing useful functions
	virtual ISSWECore *getCore() = 0;
	/// Returns the CoreData of the current project
	virtual ISSWECoreData *getCoreData() = 0;

	/// Returns the Collision manager of CP3D
	virtual ICollisionManager *getCollisionManager() = 0;

	/// Returns the AnimationController of CP3D
	virtual cp3d::controllers::IAnimationController *getAnimationController() = 0;

	/// Returns the nodes factory to create lights, cubes, etc.
	virtual cp3d::scene::IFactory *getNodeFactory() = 0;

	/// Returns the pointer to the mesh simplificator. Then, you can create LOD on the flag
	virtual cp3d::scene::IMeshSimplificator *getMeshSimplificator() = 0;
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	/// Returns the FPS Camera of the scene
	virtual irr::scene::ICameraSceneNode *getFPSCamera() = 0;
	/// Returns the maya camera of the scene
	virtual irr::scene::ICameraSceneNode *getMayaCamera() = 0;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	/// Returns the current project name
	virtual irr::core::stringw getProjectName() = 0;
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	/// For UI Plugins, creates an open file dialog
	virtual irr::gui::IGUIFileOpenDialog *createFileOpenDialog(irr::core::stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, bool modal=false) = 0;
	/// For UI Plugins, creates an open file dialog
	virtual irr::gui::IGUIFileOpenDialog *createFileOpenDialog(irr::core::stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent, bool modal=false) = 0;

	/// For UI Plugins, creates an information dialog
	virtual irr::gui::IGUIWindow *addInformationDialog(irr::core::stringw title, irr::core::stringw text, irr::s32 flag, bool modal=true, irr::gui::IGUIElement *parent=0) = 0;
	/// For UI Plugins, creates an error dialog
	virtual irr::gui::IGUIWindow *addErrorDialog(irr::core::stringw title, irr::core::stringw text, irr::s32 flag) = 0;
	/// For UI Plugins, creates an warning dialog
	virtual irr::gui::IGUIWindow *addWarningDialog(irr::core::stringw title, irr::core::stringw text, irr::s32 flag) = 0;
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	/// For GUI plugins, creates a code editor
	virtual tool::IGUICodeEditor *createGUICodeEditor() = 0;
	/// For GUI plugins, returns the current data selected in the main window
	virtual ISData *getSelectedData() = 0;
	//-----------------------------------

	//-----------------------------------
	//INPUT METHODS
	/// Returns if the CTRL key is pushed
	virtual bool isCtrlPushed() = 0;
	/// Returns if the SHIFT key is pushed
	virtual bool isShiftPushed() = 0;

	/// Returns the IMonitorRegisterer to register or delete custom monitors
	virtual video::IMonitorRegisterer *getMonitorRegister() = 0;
};

} /// End namespace core
} /// End namespace cp3d

#endif