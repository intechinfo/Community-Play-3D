/*
 *  UIContextMenu.h
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __C_UI_PARTICLES_EXPORT_H_INCLUDED__
#define __C_UI_PARTICLES_EXPORT_H_INCLUDED__

#include "../../Device/CDevices.h"

#include "../../Device/Core/ParticleSystems/CParticleSystemsTypes.h"

class CUIParticleExporter {

public:

	CUIParticleExporter(CDevices *_devices, SParticleSystem *ps);
	~CUIParticleExporter();

	void exportSystem(stringc filename);

private:

	//-----------------------------------
	//DEVICE & DATAS
	CDevices *devices;
	SParticleSystem *ps;
	//-----------------------------------

	//-----------------------------------
	//GUI ELEMENTS

	//-----------------------------------

	//-----------------------------------
	//METHODS
	void exportGroup(SPK::Group *group, FILE *export_file);
	void exportRenderer(SPK::IRR::IRRQuadRenderer *renderer, FILE *export_file);
	void exportModel(SPK::Model *model, FILE *export_file);
	void exportEmitter(SPK::Emitter *emitter, FILE *export_file);
	void exportModifier(SPK::Modifier *modifier, FILE *export_file);

	void exportZone(stringc tabs, SPK::Zone *zone, FILE *export_file);
	//-----------------------------------

};

#endif
