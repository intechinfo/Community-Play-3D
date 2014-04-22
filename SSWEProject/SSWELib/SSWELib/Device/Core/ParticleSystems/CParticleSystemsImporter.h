//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#ifndef __C_PARTICLE_SYSTEMS_IMPORTER_H_INCLUDED__
#define __C_PARTICLE_SYSTEMS_IMPORTER_H_INCLUDED__

#include "../../CDevices.h"
#include "CParticleSystemsTypes.h"

class CParticleSystemsImporter {

public:

	CParticleSystemsImporter(CDevices *devices);
	~CParticleSystemsImporter();

	SParticleSystem createParticleSystemFromFile(stringc filename, bool addToParticleSystemsArray=false);


private:

	//--------------------------
    //DATAS
	CDevices *devices;

	IrrXMLReader *xmlReader;
	std::string element;

	struct SEnableParamsValues {
		bool enabled;

		int nbParams;

		bool isRandom;
		bool isMutable;

		float parameters[4];
	};
	//--------------------------

	//--------------------------
    //METHODS
	SPK::Group *buildGroup();
	SPK::IRR::IRRQuadRenderer *buildRenderer();
	SPK::Model *buildModel();
	SPK::Emitter *buildEmitter();
	SPK::Modifier *buildModifier();
	SPK::Zone *buildZone();

	//XML METHODS
	void readNextElemement();
	void read(std::string node);
	void readWithNextElement(std::string node, std::string nextNode);

	//BUILD METHODS
	template<class T>
	vector3d<T> buildVector3d();
	template<class T>
	dimension2d<T> buildDimension2d();

	SPK::Vector3D getSPKVector3dFromIrr(irr::core::vector3df vector) {
		return SPK::Vector3D(vector.X, vector.Y, vector.Z);
	}
	//--------------------------

};

#endif
