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
	fprintf(export_file, "<rootPS>\n\n");

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

	fprintf(export_file, "\n</rootPS>\n");
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

	if (group->getRenderer())
		exportRenderer((SPK::IRR::IRRQuadRenderer*)group->getRenderer(), export_file);

	if (group->getModel())
		exportModel(group->getModel(), export_file);

	for (u32 i=0; i < group->getEmitters().size(); i++)
		exportEmitter(group->getEmitters()[i], export_file);

	for (u32 i=0; i < group->getModifiers().size(); i++) {
		fprintf(export_file, "\n");
		fprintf(export_file, "\t\t\t <modifier>\n");
		exportModifier(group->getModifiers()[i], export_file);
		fprintf(export_file, "\t\t\t </modifier>\n");
		fprintf(export_file, "\n");
	}

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

	fprintf(export_file, "\t\t\t\t <enabledParams> \n");
	{
		for (int i=0; i < (int)SPK::PARAM_CUSTOM_0; i++) {
			if (model->isEnabled(static_cast<SPK::ModelParam>(i))) {
				u32 nbValues = model->getNbValues(static_cast<SPK::ModelParam>(i));
				int flagInt;
				{
					switch (i) {
					case SPK::PARAM_RED:			flagInt = SPK::FLAG_RED;			break;
					case SPK::PARAM_GREEN:			flagInt = SPK::FLAG_GREEN;			break;
					case SPK::PARAM_BLUE:			flagInt = SPK::FLAG_BLUE;			break;
					case SPK::PARAM_ALPHA:			flagInt = SPK::FLAG_ALPHA;			break;
					case SPK::PARAM_SIZE:			flagInt = SPK::FLAG_SIZE;			break;
					case SPK::PARAM_MASS:			flagInt = SPK::FLAG_MASS;			break;
					case SPK::PARAM_ANGLE:			flagInt = SPK::FLAG_ANGLE;			break;
					case SPK::PARAM_TEXTURE_INDEX:  flagInt = SPK::FLAG_TEXTURE_INDEX;	break;
					case SPK::PARAM_ROTATION_SPEED: flagInt = SPK::FLAG_ROTATION_SPEED; break;

					default: flagInt = SPK::FLAG_NONE; break;
					}
				}

				if (nbValues == 0) {
					fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" />\n", flagInt);
					continue;
				}

				if (nbValues == 1)
					fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" death=\"%f\" />\n", flagInt, model->getParamValue(static_cast<SPK::ModelParam>(i), 0));

				if (nbValues == 2) {
					if (model->isRandom(static_cast<SPK::ModelParam>(i)))
						fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" rand0=\"%f\" rand1=\"%f\" />\n",
						flagInt,
						model->getParamValue(static_cast<SPK::ModelParam>(i), 0),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 1));
					else /// Mutable
						fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" birth=\"%f\" death=\"%f\" />\n",
						flagInt,
						model->getParamValue(static_cast<SPK::ModelParam>(i), 0),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 1));
				}
				if (nbValues == 4) {
					fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"%d\" minBirth=\"%f\" maxBirth=\"%f\" minDeath=\"%f\" maxDeath=\"%f\" />\n",
						flagInt,
						model->getParamValue(static_cast<SPK::ModelParam>(i), 0),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 1),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 2),
						model->getParamValue(static_cast<SPK::ModelParam>(i), 3));
				}

			} else {
				fprintf(export_file, "\t\t\t\t\t <enabledparam value=\"-1\" />\n");
			}
		}
	}
	fprintf(export_file, "\t\t\t\t </enabledParams> \n");

	fprintf(export_file, "\t\t\t\t <name value=\"%s\" />\n", model->getName().c_str());
	fprintf(export_file, "\t\t\t\t <lifeTime min=\"%f\" max=\"%f\" />\n", model->getLifeTimeMin(), model->getLifeTimeMax());

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

void CUIParticleExporter::exportEmitter(SPK::Emitter *emitter, FILE *export_file) {
	fprintf(export_file, "\n");

	//-------------------------------------------------
	/// Write the value of the E_PS_EMITTER_TYPE for easier way to load from this XML
	SPK::StraightEmitter *straightEmitter = dynamic_cast<SPK::StraightEmitter*>(emitter);
	SPK::StaticEmitter *staticEmitter = dynamic_cast<SPK::StaticEmitter*>(emitter);
	SPK::SphericEmitter *sphericEmitter = dynamic_cast<SPK::SphericEmitter*>(emitter);
	SPK::RandomEmitter *randomEmitter = dynamic_cast<SPK::RandomEmitter*>(emitter);
	SPK::NormalEmitter *normalEmitter = dynamic_cast<SPK::NormalEmitter*>(emitter);

	cp3d::ps::E_PS_EMITTER_TYPE emType;
	if (straightEmitter) emType = cp3d::ps::ECET_STRAIGHT;
	else if (staticEmitter) emType = cp3d::ps::ECET_STATIC;
	else if (sphericEmitter) emType = cp3d::ps::ECET_SPHERIC;
	else if (randomEmitter) emType = cp3d::ps::ECET_RANDOM;
	else if (normalEmitter) emType = cp3d::ps::ECET_NORMAL;
	else emType = cp3d::ps::ECET_UNKNOWN;

	fprintf(export_file, "\t\t\t <emitter value=\"%d\" >\n", emType);
	//-------------------------------------------------

	fprintf(export_file, "\t\t\t\t <name value=\"%s\" />\n", emitter->getName().c_str());
	fprintf(export_file, "\t\t\t\t <flow value=\"%f\" />\n", emitter->getFlow());
	fprintf(export_file, "\t\t\t\t <force min=\"%f\" max=\"%f\" />\n", emitter->getForceMin(), emitter->getForceMax());
	fprintf(export_file, "\t\t\t\t <active value=\"%d\" />\n", emitter->isActive());
	fprintf(export_file, "\t\t\t\t <tank value=\"%d\" />\n", emitter->getTank());

	fprintf(export_file, "\n\t\t\t\t <zone>\n");
	{
		exportZone("\t\t\t\t\t ", emitter->getZone(), export_file);
	}
	fprintf(export_file, "\t\t\t\t </zone>\n\n");

	fprintf(export_file, "\t\t\t </emitter>\n");
	fprintf(export_file, "\n");
}

void CUIParticleExporter::exportModifier(SPK::Modifier *modifier, FILE *export_file) {

	/// Collision
	SPK::Collision *collision = dynamic_cast<SPK::Collision*>(modifier);
	if (collision) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_COLLISION);
		fprintf(export_file, "\t\t\t\t <scale value=\"%f\" />\n", collision->getScale());
		fprintf(export_file, "\t\t\t\t <elasticity value=\"%f\" />\n", collision->getElasticity());
		return;
	}

	/// Destroyer
	SPK::Destroyer *destroyer = dynamic_cast<SPK::Destroyer*>(modifier);
	if (destroyer) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_DESTROYER);
		return;
	}

	/// Linear Force
	SPK::LinearForce *linearForce = dynamic_cast<SPK::LinearForce*>(modifier);
	if (linearForce) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_LINEAR_FORCE);
		fprintf(export_file, "\t\t\t\t <force X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",
			linearForce->getForce().x, linearForce->getForce().y, linearForce->getForce().z);
		return;
	}

	/// Obstacle
	SPK::Obstacle *obstacle = dynamic_cast<SPK::Obstacle*>(modifier);
	if (obstacle) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_OBSTACLE);
		fprintf(export_file, "\t\t\t\t <boundingRatio value=\"%f\" />\n", obstacle->getBouncingRatio());
		fprintf(export_file, "\t\t\t\t <friction value=\"%f\" />\n", obstacle->getFriction());
		return;
	}

	/// Point Mass
	SPK::PointMass *pointMass = dynamic_cast<SPK::PointMass*>(modifier);
	if (pointMass) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_POINT_MASS);
		fprintf(export_file, "\t\t\t\t <mass value=\"%f\" />\n", pointMass->getMass());
		fprintf(export_file, "\t\t\t\t <minDistance value=\"%f\" />\n", pointMass->getMinDistance());
		return;
	}

	/// Rotator
	SPK::Rotator *rotator = dynamic_cast<SPK::Rotator*>(modifier);
	if (rotator) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_ROTATOR);
		return;
	}

	/// Vortex
	SPK::Vortex *vortex = dynamic_cast<SPK::Vortex*>(modifier);
	if (vortex) {
		fprintf(export_file, "\t\t\t\t <type value=\"%d\" />\n", cp3d::ps::EGMT_VORTEX);
		fprintf(export_file, "\t\t\t\t <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",
			vortex->getPosition().x, vortex->getPosition().y, vortex->getPosition().z);
		fprintf(export_file, "\t\t\t\t <direction X=\"%f\" Y=\"%f\" Z=\"%f\" />\n",
			vortex->getDirection().x, vortex->getDirection().y, vortex->getDirection().z);

		fprintf(export_file, "\t\t\t\t <rotationSpeed value=\"%f\" />\n", vortex->getRotationSpeed());
		fprintf(export_file, "\t\t\t\t <attractionSpeed value=\"%f\" />\n", vortex->getAttractionSpeed());
		return;
	}
}

void CUIParticleExporter::exportZone(stringc tabs, SPK::Zone *zone, FILE *export_file) {
	/// AABox
	SPK::AABox *aabox = dynamic_cast<SPK::AABox*>(zone);
	if (aabox) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_AABOX);
		fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			aabox->getPosition().x, aabox->getPosition().y, aabox->getPosition().z);
		fprintf(export_file, stringc(tabs + " <dimension X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			aabox->getDimension().x, aabox->getDimension().y, aabox->getDimension().z);
		return;

	}

	/// Cylinder
	SPK::Cylinder *cylinder = dynamic_cast<SPK::Cylinder*>(zone);
	if (cylinder) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_CYLINDER);
		fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			cylinder->getPosition().x, cylinder->getPosition().y, cylinder->getPosition().z);
		fprintf(export_file, stringc(tabs + " <direction X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			cylinder->getDirection().x, cylinder->getDirection().y, cylinder->getDirection().z);

		fprintf(export_file, stringc(tabs + " <radius value=\"%f\" />\n").c_str(), cylinder->getRadius());
		fprintf(export_file, stringc(tabs + " <length value=\"%f\" />\n").c_str(), cylinder->getLength());
		return;
	}

	/// Line
	SPK::Line *line = dynamic_cast<SPK::Line*>(zone);
	if (line) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_POINT);
		fprintf(export_file, stringc(tabs + " <p1 X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			line->getBound(0).x, line->getBound(0).y, line->getBound(0).z);
		fprintf(export_file, stringc(tabs + " <p2 X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			line->getBound(1).x, line->getBound(1).y, line->getBound(1).z);
		return;
	}

	/// Plane
	SPK::Plane *plane = dynamic_cast<SPK::Plane*>(zone);
	if (plane) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_PLANE);
		fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			plane->getPosition().x, plane->getPosition().y, plane->getPosition().z);
		fprintf(export_file, stringc(tabs + " <normal X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			plane->getNormal().x, plane->getNormal().y, plane->getNormal().z);
		return;
	}

	/// Point
	SPK::Point *point = dynamic_cast<SPK::Point*>(zone);
	if (point) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_POINT);
		fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			point->getPosition().x, point->getPosition().y, point->getPosition().z);
		return;
	}

	/// Ring
	SPK::Ring *ring = dynamic_cast<SPK::Ring*>(zone);
	if (ring) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_RING);
		fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			ring->getPosition().x, ring->getPosition().y, ring->getPosition().z);
		fprintf(export_file, stringc(tabs + " <normal X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			ring->getNormal().x, ring->getNormal().y, ring->getNormal().z);

		fprintf(export_file, stringc(tabs + " <minRadius value=\"%f\" />\n").c_str(), ring->getMinRadius());
		fprintf(export_file, stringc(tabs + " <maxRadius value=\"%f\" />\n").c_str(), ring->getMaxRadius());
		return;
	}

	/// Sphere
	SPK::Sphere *sphere = dynamic_cast<SPK::Sphere*>(zone);
	if (sphere) {
		fprintf(export_file, stringc(tabs + " <type value=\"%d\" />\n").c_str(), cp3d::ps::EZT_SPHERE);
		fprintf(export_file, stringc(tabs + " <position X=\"%f\" Y=\"%f\" Z=\"%f\" />\n").c_str(),
			sphere->getPosition().x, sphere->getPosition().y, sphere->getPosition().z);

		fprintf(export_file, stringc(tabs + " <radius value=\"%f\" />\n").c_str(), sphere->getRadius());
		return;
	}
}
