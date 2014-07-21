//
//  CUIWindowEditGrid.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 08/10/12.
//
//
#include "Stdafx.h"

#include "DefaultAudioManager.h"

#include <DLLExport.h>

cp3d::audio::IAudioManager *createAudioManager() {
	return (cp3d::audio::IAudioManager*)new cp3d::audio::CDefaultAudioManager();
}

namespace cp3d {
namespace audio {

CDefaultAudioManager::CDefaultAudioManager() {
	channel = 0;
}

CDefaultAudioManager::~CDefaultAudioManager() {

}

bool CDefaultAudioManager::initialize() {
	result = FMOD::System_Create(&system);
	assert(result == FMOD_OK);
    if (result != FMOD_OK) return false;

	system->init(32, FMOD_INIT_3D_RIGHTHANDED, 0);
	assert(result == FMOD_OK);
	if (result != FMOD_OK) return false;

	return true;
}

void CDefaultAudioManager::close(bool deleteElements) {
	IAudioManager::close();
	result = system->close();
	assert(result == FMOD_OK);
}

IAudioElement *CDefaultAudioManager::add2DAudioElementFromFile(irr::core::stringc path) {
	CAudioElement *element = new CAudioElement(system, path);
	Elements.push_back(element);

	return element;
}

IAudioElement3D *CDefaultAudioManager::add3DAudioElementFromFile(irr::core::stringc path, irr::scene::ISceneManager *smgr) {
	CAudioElement3D *element = new CAudioElement3D(system, smgr, path);
	Elements.push_back(element);

	return element;
}

} /// End namespace cp3d
} /// End namespace audio
