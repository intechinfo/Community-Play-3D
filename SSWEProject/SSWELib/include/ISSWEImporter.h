#ifndef __I_SSWE_IMPORTER_H_INCLUDED__
#define __I_SSWE_IMPORTER_H_INCLUDED__

#include <irrlicht.h>

class ISSWEImporter {
public: 

	virtual void importScene(irr::core::stringc file_path) = 0;

};

#endif