/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"
#include "CUIParticleExport.h"

CUIParticleExporter::CUIParticleExporter(CDevices *_devices, SParticleSystem *ps) {
    devices = _devices;
	this->ps = ps;
}

CUIParticleExporter::~CUIParticleExporter() {

}

void CUIParticleExporter::exportSystem(stringc filename) {
	FILE *export_file;
	export_file = fopen(filename.c_str(), "w");
	fprintf(export_file, "<?xml version=\"1.0\"?>\n\n");
	fprintf(export_file, "<rootAnim>\n\n");

	for (u32 i=0; i < ps->getSystems()->size(); i++) {
		SPK::IRR::IRRSystem *system = (SPK::IRR::IRRSystem*)ps->getSystems()->operator[](i);

		fprintf(export_file, "\t <system>\n");

		/// Export system's configuration
		//fprintf(export_file, "\t\t <name value=\"%s\" />\n", stringc(system->getName()).c_str());
		fprintf(export_file, "\t\t <aabbComputing value=\"%d\" />\n", system->isAABBComputingEnabled());
		fprintf(export_file, "\t\t <enableAutoUpdate value=\"%d\" />\n", system->isAutoUpdateEnabled());
		fprintf(export_file, "\t\t <updateOnlyIfVisible value=\"%d\" />\n", system->isUpdateOnlyWhenVisible());

		/// Export groups
		for (u32 j=0; j < system->getGroups().size(); j++)
			exportGroup(system->getGroups()[j], export_file);

		fprintf(export_file, "\t </system>\n");

	}

	fprintf(export_file, "\n</rootAnim>\n");
	fclose(export_file);
}

void CUIParticleExporter::exportGroup(SPK::Group *group, FILE *export_file) {
	fprintf(export_file, "\n");
	fprintf(export_file, "\t\t </group>\n");

	fprintf(export_file, "\t\t\t <name value=\"%s\" />\n", group->getName().c_str());
	fprintf(export_file, "\t\t\t <gravity X=\"%f\" Y=\"%f\" Z=\"%f\" />\n", group->getGravity().x, group->getGravity().y, group->getGravity().z);
	fprintf(export_file, "\t\t\t <aabbComputing value=\"%d\" />\n", group->isAABBComputingEnabled());
	fprintf(export_file, "\t\t\t <friction value=\"%f\" />\n", group->getFriction());
	fprintf(export_file, "\t\t\t <enableSorting value=\"%d\" />\n", group->isSortingEnabled());
	fprintf(export_file, "\t\t\t <enableDistanceComputing value=\"%d\" />\n", group->isDistanceComputationEnabled());

	exportRenderer((SPK::IRR::IRRQuadRenderer*)group->getRenderer(), export_file);
	exportModel(group->getModel(), export_file);

	fprintf(export_file, "\t\t </group>\n");
	fprintf(export_file, "\n");
}

void CUIParticleExporter::exportRenderer(SPK::IRR::IRRQuadRenderer *renderer, FILE *export_file) {
	fprintf(export_file, "\n");
	fprintf(export_file, "\t\t\t <renderer>\n");

	fprintf(export_file, "\t\t\t\t <name value=\"%s\" />\n", renderer->getName().c_str());
	fprintf(export_file, "\t\t\t\t <scale W=\"%f\" H=\"%f\" />\n", renderer->getScaleX(), renderer->getScaleY());
	fprintf(export_file, "\t\t\t\t <texturingMode value=\"%d\" />\n", renderer->getTexturingMode());
	fprintf(export_file, "\t\t\t\t <blendingMode value=\"%d\" />\n", renderer->getTexturingMode());
	fprintf(export_file, "\t\t\t\t <atlasDimension W=\"%f\" H=\"%f\" />\n", renderer->getAtlasDimensionX(), renderer->getAtlasDimensionY());
	fprintf(export_file, "\t\t\t\t <texture value=\"%s\" />\n", renderer->getTexture()->getName().getPath().c_str());
	fprintf(export_file, "\t\t\t\t <enableAlphaTest value=\"%d\" />\n", renderer->isRenderingHintEnabled(SPK::ALPHA_TEST));
	fprintf(export_file, "\t\t\t\t <enableDepthTest value=\"%d\" />\n", renderer->isRenderingHintEnabled(SPK::DEPTH_TEST));
	fprintf(export_file, "\t\t\t\t <enableDepthWrite value=\"%d\" />\n", renderer->isRenderingHintEnabled(SPK::DEPTH_WRITE));
	fprintf(export_file, "\t\t\t\t <setActive value=\"%d\" />\n", renderer->isActive());
	fprintf(export_file, "\t\t\t\t <alphaTest value=\"%f\" />\n", renderer->getAlphaTestThreshold());

	fprintf(export_file, "\t\t\t </renderer>\n");
	fprintf(export_file, "\n");
}

void CUIParticleExporter::exportModel(SPK::Model *model, FILE *export_file) {
	fprintf(export_file, "\n");
	fprintf(export_file, "\t\t\t <model>\n");

	fprintf(export_file, "\t\t\t\t <name value=\"%s\" />\n", model->getName().c_str());
	fprintf(export_file, "\t\t\t\t <lifeTime min=\"%f\" max=\"%f\" />\n", model->getLifeTimeMin(), model->getLifeTimeMax());
	fprintf(export_file, "\t\t\t\t <enabledParams> \n");
	{
		for (int i=0; i < (int)SPK::PARAM_CUSTOM_0; i++) {
			if (model->isEnabled(static_cast<SPK::ModelParam>(i))) {
				u32 nbValues = model->getNbValues(static_cast<SPK::ModelParam>(i));
				if (nbValues == 0) {
					fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" />\n", static_cast<SPK::ModelParam>(i));
					continue;
				}

				if (nbValues == 1)
					fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" death=\"%f\" />\n", static_cast<SPK::ModelParam>(i), model->getParamValue(static_cast<SPK::ModelParam>(i), 0));

				if (nbValues == 2) {
					if (model->isRandom(static_cast<SPK::ModelParam>(i)))
						fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" rand0=\"%f\" rand1=\"%f\" />\n",
						static_cast<SPK::ModelParam>(i),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 0),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 1));
					else /// Mutable
						fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" birth=\"%f\" death=\"%f\" />\n",
						static_cast<SPK::ModelParam>(i),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 0),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 1));
				}
				if (nbValues == 4) {
					fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" minBirth=\"%f\" maxBirth=\"%f\" minDeath=\"%f\" maxDeath=\"%f\" />\n",
						static_cast<SPK::ModelParam>(i),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 0),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 1),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 2),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 3));
				}
			}
		}
	}
	fprintf(export_file, "\t\t\t\t </enabledParams> \n");
	fprintf(export_file, "\t\t\t\t <interpolators> \n");
	{
		if (model->getNbInterpolated() > 0) {
			for (int i=0; i < (int)SPK::PARAM_CUSTOM_0; i++) {
				if (model->isInterpolated(static_cast<SPK::ModelParam>(i))) {
					SPK::Interpolator *inter = model->getInterpolator(static_cast<SPK::ModelParam>(i));

					std::set<SPK::InterpolatorEntry>::const_iterator it = inter->getGraph().begin();
					for (; it != inter->getGraph().end(); ++it) {
						fprintf(export_file, "\t\t\t\t\t <interpolator value=\"%d\" x=\"%f\" y0=\"%f\" y1=\"%f\" />\n",
							static_cast<SPK::ModelParam>(i), it->x, it->y0, it->y1);
					}

				}
			}
		}
	}
	fprintf(export_file, "\t\t\t\t </interpolators> \n");

	fprintf(export_file, "\t\t\t </model>\n");
	fprintf(export_file, "\n");
}
