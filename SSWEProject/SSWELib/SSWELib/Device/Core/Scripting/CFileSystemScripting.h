#ifndef __C_FILE_SYSTEM_SCRIPTING_H_INCLUDED__
#define __C_FILE_SYSTEM_SCRIPTING_H_INCLUDED__

class CDevices;

#include <irrlicht.h>

class CFileSystemScripting {

public:

	CFileSystemScripting(CDevices *_devices);
	~CFileSystemScripting();

	IFileList *getFileList(irr::core::stringc path="", bool resetDirectory=false);

private:

	//-----------------------------------
	//DATAS
	CDevices *devices;
	irr::scene::ISceneManager *smgr;
	//-----------------------------------

};

#endif
