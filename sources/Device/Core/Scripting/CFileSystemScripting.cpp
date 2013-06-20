//
//  CRenderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//
#include "stdafx.h"
#include "CFileSystemScripting.h"

#include "../../CDevices.h"

CFileSystemScripting::CFileSystemScripting(CDevices *_devices) {
	devices = _devices;
	smgr = devices->getSceneManager();
}

CFileSystemScripting::~CFileSystemScripting() {

}

IFileList *CFileSystemScripting::getFileList(irr::core::stringc path, bool resetDirectory) {
	IFileSystem *fs = devices->getDevice()->getFileSystem();

	stringc oldPath = fs->getWorkingDirectory().c_str();

	fs->changeWorkingDirectoryTo(path.c_str());
	IFileList *fl = fs->createFileList();

	if (resetDirectory)
		fs->changeWorkingDirectoryTo(oldPath.c_str());

	return fl;
}
