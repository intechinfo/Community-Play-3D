#ifndef __I_DEVICES_H_INCLUDED__
#define __I_DEVICES_H_INCLUDED__

#include "../SSWERenders/Renders/XEffect/XEffects.h"

#include "../SSWECore/SSWECore/Core/CCoreObjectPlacement.h"

#include "../SSWELib/GUIExtension/FileSelector/CGUIFileSelector.h"
#include "../SSWELib/UserInterfaces/ProcessesLogger/CUIProcessesLogger.h"
#include "../SSWELib/SceneNodes/Camera/CRiggedCamera.h"

#include "ICollisionManager.h"
#include "ISSWECore.h"
#include "ISSWECoreData.h"

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

	virtual stringc getWorkingDirectory() = 0;

	virtual stringw getContextName() = 0;
	virtual void setContextName(stringw _contextName) = 0;
	//-----------------------------------

	//-----------------------------------
	//RENDERS
	virtual EffectHandler *getXEffect() = 0;
	virtual E_FILTER_TYPE getXEffectFilterType() = 0;
	virtual void setXEffectDrawable(bool draw) = 0;
	virtual bool isXEffectDrawable() = 0;
	virtual stringc getShaderExt() = 0;
	virtual void rebuildXEffect() = 0;

	//virtual CRenderCallbacks *getRenderCallbacks() = 0;

	//virtual ShaderGroup* getDOF() = 0;

	virtual bool isRenderScene() = 0;
	virtual void setRenderScene(bool _renderScene) = 0;

	virtual void setRenderGUI(bool _renderGUI) = 0;
	virtual bool isRenderingGUI() = 0;

	virtual void setRenderFullPostTraitements(bool _render) = 0;
	virtual bool isRenderingFullPostTraitements() = 0;
	//-----------------------------------

	//-----------------------------------
	//CORE
	virtual ISSWECore *getCore() = 0;
	virtual ISSWECoreData *getCoreData() = 0;

	virtual ICollisionManager *getCollisionManager() = 0;
	virtual CCoreObjectPlacement *getObjectPlacement() = 0;

	virtual CUIProcessesLogger *getProcessesLogger() = 0;
	//-----------------------------------

	//-----------------------------------
	//CAMERAS
	virtual ICameraSceneNode *getFPSCamera() = 0;
	virtual ICameraSceneNode *getMayaCamera() = 0;
	virtual CCameraRig *getCameraRig() = 0;
	//-----------------------------------

	//-----------------------------------
	//METHODS
	virtual void createDevice(SIrrlichtCreationParameters parameters) = 0;

	virtual vector3df getCursorPosition() = 0;
	virtual ISceneNode *getCursor() = 0;

	//virtual void updateDevice() = 0;
	//virtual void updateEntities() = 0;

	//virtual void reupdate(EffectHandler *_effect = 0) = 0;

	virtual stringw getProjectName() = 0;
	//virtual void setProjectName(stringw _projectName) = 0;
	//-----------------------------------

	//-----------------------------------
	//UI ADVANCED DIALOG METHODS
	virtual CGUIFileSelector *createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, bool modal=false) = 0;
	virtual CGUIFileSelector *createFileOpenDialog(stringw title, CGUIFileSelector::E_FILESELECTOR_TYPE type, IGUIElement *parent, bool modal=false) = 0;

	virtual IGUIWindow *addInformationDialog(stringw title, stringw text, s32 flag, bool modal=true, IGUIElement *parent=0) = 0;
	virtual IGUIWindow *addErrorDialog(stringw title, stringw text, s32 flag) = 0;
	virtual IGUIWindow *addWarningDialog(stringw title, stringw text, s32 flag) = 0;
	//-----------------------------------

	//-----------------------------------
	//INPUT METHODS
	virtual bool isCtrlPushed() = 0;
	virtual bool isShiftPushed() = 0;

	virtual SKeyMap getKeyMap(int i) = 0;
	virtual void setKeyMap(SKeyMap _keyMap, int i) = 0;
	virtual void applyKeyMapOnFPSCamera() = 0;
};

#endif