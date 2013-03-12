//
//  CImporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 01/10/12.
//
//

#ifndef __C_UI_IMPORTER_H_INCLUDED__
#define __C_UI_IMPORTER_H_INCLUDED__

#include "../Device/CDevices.h"

#include "../UserInterfaces/CUIWindowEditNode.h"

class CImporter {

public:

	CImporter(CDevices *_devices);
	~CImporter();

	void importScene(stringc file_path);

private:

	CDevices *devices;

	IrrXMLReader *xmlReader;
	std::string element;

	void read(std::string node);
	void readWithNextElement(std::string node, std::string nextNode);
};

#endif
