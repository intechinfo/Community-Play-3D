#ifndef __I_DEVICES_H_INCLUDED__
#define __I_DEVICES_H_INCLUDED__

#include "../SSWELib/GUIExtension/FileSelector/CGUIFileSelector.h"

#include <ICollisionManager.h>
#include <ISSWECore.h>
#include <ISSWECoreData.h>
#include <ISSWERender.h>
#include <IGUICodeEditor.h>

class IDevices {
public: 
	virtual irr::IrrlichtDevice *getDevice() = 0;
	virtual irr::video::IVideoDriver *getVideoDriver() = 0;

	virtual irr::scene::ISceneManager *getSceneManager() = 0;

	virtual irr::core::array<irr::scene::ISceneManager *> *getSceneManagers() = 0;
	virtual void removeSceneManager(irr::scene::ISceneManager *smgrToDelete) = 0;
	virtual void setSceneManagerToDraw(irr::scene::ISceneManager *smgrToDraw) = 0;
	virtual irr::scene::ISceneManager *createNewSceneManager() = 0;
	virtual irr::scene::ISceneManager *getRenderingSceneManager() = 0;

	virtual irr::gui::IGUIEnvironment *getGUIEnvironment() = 0;

	virtual ISSWEEventsManager *getEventReceiver() = 0;
	virtual bool isEditBoxEntered() = 0;
	virtual void setEditBoxEntered(bool _editBoxEntered) = 0;

	virtual irr::core::stringc getWorkingDirectory() = 0;

	virtual irr::core::stringw getContextName() = 0;
	virtual void setContextName(irr::core::stringw _contextName) = 0;
	//-----------------------------------

	//-----------------------------------
	//RENDERS
	virtual ISSWERender *getXEffect() = 0;
	virtual E_FILTER_TYPE getXEffectFilterType() = 0;
	virtual void setXEffectDrawable(bool draw) = 0;
	virtual bool isXEffectDrawable() = 0;
	virtual irr::core::stringc getShaderExt() = 0;
	virtual void rebuildXEffect() = 0;

	virtual bool isRenderScene() = 0;
	virtual void setRenderScene(bool _renderScene) = 0;

	virtual void setRenderGUI(bool _renderGUI) = 0;
	virtual bool isRenderingGUI() = 0;

	virtual void setRenderFullPostTraitements(bool _render) = 0;
	virtual bool isRenderingFullPostTraitements() = 0;

	virtual void updateDevice() = 0;
	//-----------------------------------

	//-----------------------------------
	//CORE
	virtual ISSWECore *getCore() = 0;
	virtual ISSWECoreData *getCoreData() = 0;

	virtual ICollisionManager *getCollisionManager() = 0;
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	virtual irr::scene::ICameraSceneNode *getFPSCamera() = 0;
	virtual irr::scene::ICameraSceneNode *getMayaCamera() = 0;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	virtual void createDevice(SIrrlichtCreationParameters parameters) = 0;

	virtual irr::core::vector3df getCursorPosition() = 0;
	virtual irr::scene::ISceneNode *getCursor() = 0;

	virtual void applyAnimationToModel(irr::scene::ISceneNode *node, irr::u32 animationNumber) = 0;
	virtual void applyAnimationFromNameToModel(irr::scene::ISceneNode *node, irr::core::stringc name) = 0;
	virtual irr::s32 getCurrentAnimationIndiceOf(irr::scene::ISceneNode *node) = 0;

	virtual irr::core::stringw getProjectName() = 0;
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	virtual irr::gui::IGUIFileOpenDialog *createFileOpenDialog(irr::core::stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, bool modal=false) = 0;
	virtual irr::gui::IGUIFileOpenDialog *createFileOpenDialog(irr::core::stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent, bool modal=false) = 0;

	virtual irr::gui::IGUIWindow *addInformationDialog(irr::core::stringw title, irr::core::stringw text, irr::s32 flag, bool modal=true, irr::gui::IGUIElement *parent=0) = 0;
	virtual irr::gui::IGUIWindow *addErrorDialog(irr::core::stringw title, irr::core::stringw text, irr::s32 flag) = 0;
	virtual irr::gui::IGUIWindow *addWarningDialog(irr::core::stringw title, irr::core::stringw text, irr::s32 flag) = 0;
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	virtual IGUICodeEditor *createGUICodeEditor() = 0;
	virtual ISData *getSelectedData() = 0;
	//-----------------------------------

	//-----------------------------------
	//INPUT METHODS
	virtual bool isCtrlPushed() = 0;
	virtual bool isShiftPushed() = 0;

	virtual irr::SKeyMap getKeyMap(int i) = 0;
	virtual void setKeyMap(irr::SKeyMap _keyMap, int i) = 0;
	virtual void applyKeyMapOnFPSCamera() = 0;
};

#endif