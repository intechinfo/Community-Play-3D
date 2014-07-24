//
//  IAudioManager.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __I_AUDIO_MANAGER_H_INCLUDED__
#define __I_AUDIO_MANAGER_H_INCLUDED__

#include <irrlicht.h>

#include "IAudioElement.h"
#include "IAudioElement3D.h"

namespace cp3d {
namespace audio {

typedef irr::core::list<IAudioElement *> IAudioElementList;

class IAudioManager {

public:

	///Initializes the audio manager.
	/// Returns true if success, false if failed
	virtual bool initialize() = 0;

	/// Closes the audio manager. Equivalent to Dispose in C#
	/// it frees all the loaded data
	virtual void close(bool deleteElements = true) {
		if (deleteElements) {
			IAudioElementList::ConstIterator it = Elements.begin();
			for (; it != Elements.end(); ++it) {
				(*it)->close();
				delete *it;
			}
		}

		Elements.clear();
	}

	/// Creates a IAudioElement 2D from the file located at "path"
	virtual IAudioElement *add2DAudioElementFromFile(irr::core::stringc path) = 0;

	/// Creates a IAudioElement 3D from the file located at "path"
	virtual IAudioElement3D *add3DAudioElementFromFile(irr::core::stringc path, irr::scene::ISceneManager *smgr) = 0;

	/// Add the audio element "element" to the element list
	virtual void addAudioElement(IAudioElement *element) {
		Elements.push_back(element);
	}

	/// Removes the element "element" from the list
	virtual bool removeAudioElement(IAudioElement *element, bool closeElement = true) {
		IAudioElementList::Iterator it = Elements.begin();
		for (; it != Elements.end(); ++it) {
			if (*it == element) {
				if (closeElement)
					(*it)->close();
				Elements.erase(it);
				return true;
			}
		}

		return false;
	}

	/// Returns the IAudioElement at index "index
	IAudioElement *getAudioElementAt(irr::s32 index) {
		IAudioElementList::Iterator it = Elements.begin();
		irr::u32 cindex = 0;

		for (; it != Elements.end(); ++it) {
			if (cindex == index) return *it;
			cindex++;
		}

		return 0;
	}

	/// Returns the Elements list
	IAudioElementList &getAudioElements() {
		return Elements;
	}

	/// Returns the amount of audio elements
	irr::u32 getAudioCount() {
		return Elements.size();
	}

protected:

	IAudioElementList Elements;

};

} /// End namespace audio
} /// End namespace cp3d

#endif