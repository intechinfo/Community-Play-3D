//
//  CUIOpenFileDialog.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 12/11/12.
//
//

#include "stdafx.h"
#include "CParticleSystemsImporter.h"

//---------------------------------------------------------------------------------------------
//------------------------------------PUBLIC METHODS-------------------------------------------
//---------------------------------------------------------------------------------------------

CParticleSystemsImporter::CParticleSystemsImporter(CDevices *_devices) {
	devices = _devices;


}


CParticleSystemsImporter::~CParticleSystemsImporter() {

}

SParticleSystem *CParticleSystemsImporter::createParticleSystemFromFile(stringc filename) {
	SPK::IRR::IRRSystem::setClampStep(true,0.1f);
	SPK::IRR::IRRSystem::useAdaptiveStep(0.001f,0.01f);

	xmlReader = createIrrXMLReader(filename.c_str());
	if (!xmlReader) /// The file does not exists
		return 0;

	SParticleSystem ps("New Particle System Imported");
	ps.createBaseNode(devices->getSceneManager());

	read("rootPS");
	readWithNextElement("system", "rootPS");

	if (element == "system") {
		/// System exists, so create it
		SPK::IRR::IRRSystem *system = SPK::IRR::IRRSystem::create(ps.getBaseNode(), devices->getSceneManager());
		ps.getSystems()->push_back(system);

		read("aabbComputing");
		system->enableAABBComputing(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
		read("enableAutoUpdate");
		system->setAutoUpdateEnabled(devices->getCore()->getU32(xmlReader->getAttributeValue("value")), false);
		read("updateOnlyIfVisible");
		system->setAutoUpdateEnabled(system->isAutoUpdateEnabled(), devices->getCore()->getU32(xmlReader->getAttributeValue("value")));

		readWithNextElement("group", "system");
		while (element == "group") {

			system->addGroup(buildGroup());

			readWithNextElement("group", "system");
		}

	}


	/// Convention, read the root even if cursor is after
	read("rootPS");

	devices->getCoreData()->getParticleSystems()->push_back(ps);

	/// Return last particle system added
	return &devices->getCoreData()->getParticleSystems()->operator[](devices->getCoreData()->getParticleSystems()->size()-1);
}

//---------------------------------------------------------------------------------------------
//-----------------------------------PRIVATE METHODS-------------------------------------------
//---------------------------------------------------------------------------------------------

SPK::Group *CParticleSystemsImporter::buildGroup() {
	SPK::Group *group = SPK::Group::create();

	read("name");
	group->setName(stringc(xmlReader->getAttributeValue("value")).c_str());
	read("gravity");
	group->setGravity(getSPKVector3dFromIrr(buildVector3d<f32>()));
	read("aabbComputing");
	group->enableAABBComputing(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("friction");
	group->setFriction(devices->getCore()->getF32(xmlReader->getAttributeValue("value")));
	read("enableSorting");
	group->enableSorting(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("enableDistanceComputing");
	group->enableDistanceComputation(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));

	read("renderer");
	group->setRenderer(buildRenderer());
	read("renderer");

	read("model");
	group->setModel(buildModel());

	/// Read emitters & modifiers
	while (xmlReader->read()) {
		element = xmlReader->getNodeName();
		if (element == "emitter")
			group->addEmitter(buildEmitter());

		if (element == "modifier")
			group->addModifier(buildModifier());

		/// End of group, so break and return group
		if (element == "group")
			break;
	}


	return group;
}

SPK::Modifier *CParticleSystemsImporter::buildModifier() {
	SPK::Modifier *modifier = SPK::Collision::create();

	return modifier;
}

SPK::Emitter *CParticleSystemsImporter::buildEmitter() {
	SPK::Emitter *emitter;

	cp3d::ps::E_PS_EMITTER_TYPE emType = (cp3d::ps::E_PS_EMITTER_TYPE)devices->getCore()->getS32(xmlReader->getAttributeValue("value"));
	if (emType == cp3d::ps::ECET_STRAIGHT) emitter = SPK::StraightEmitter::create();
	else if (emType == cp3d::ps::ECET_STATIC) emitter = SPK::StaticEmitter::create();
	else if (emType == cp3d::ps::ECET_SPHERIC) emitter = SPK::SphericEmitter::create();
	else if (emType == cp3d::ps::ECET_RANDOM) emitter = SPK::RandomEmitter::create();
	else if (emType == cp3d::ps::ECET_NORMAL) emitter = SPK::NormalEmitter::create();

	read("name");
	emitter->setName(stringc(xmlReader->getAttributeValue("value")).c_str());
	read("flow");
	f32 flow = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));
	read("force");
	emitter->setForce(devices->getCore()->getF32(xmlReader->getAttributeValue("min")),
					  devices->getCore()->getF32(xmlReader->getAttributeValue("max")));
	read("active");
	emitter->setActive(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("tank");
	emitter->setTank(devices->getCore()->getF32(xmlReader->getAttributeValue("value")));

	read("zone");
	emitter->setZone(buildZone());
	read("zone");

	read("emitter");

	emitter->setFlow(flow);

	return emitter;
}

SPK::Zone *CParticleSystemsImporter::buildZone() {
	SPK::Zone *zone;

	read("type");
	cp3d::ps::E_PS_ZONE_TYPE ztype = (cp3d::ps::E_PS_ZONE_TYPE)devices->getCore()->getU32(xmlReader->getAttributeValue("value"));

	if (ztype == cp3d::ps::EZT_AABOX) {
		zone = SPK::AABox::create();
		read("position");
		zone->setPosition(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("dimension");
		((SPK::AABox*)zone)->setDimension(getSPKVector3dFromIrr(buildVector3d<f32>()));
	} else
	if (ztype == cp3d::ps::EZT_CYLINDER) {
		zone = SPK::Cylinder::create();
		read("position");
		zone->setPosition(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("direction");
		((SPK::Cylinder*)zone)->setDirection(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("radius");
		((SPK::Cylinder*)zone)->setRadius(devices->getCore()->getF32(xmlReader->getAttributeValue("value")));
		read("length");
		((SPK::Cylinder*)zone)->setLength(devices->getCore()->getF32(xmlReader->getAttributeValue("value")));
	} else
	if (ztype == cp3d::ps::EZT_LINE) {
		zone = SPK::Line::create();
		read("p1");
		SPK::Vector3D p1 = getSPKVector3dFromIrr(buildVector3d<f32>());
		read("p2");
		SPK::Vector3D p2 = getSPKVector3dFromIrr(buildVector3d<f32>());
		((SPK::Line*)zone)->setBounds(p1, p2);
	} else
	if (ztype == cp3d::ps::EZT_PLANE) {
		zone = SPK::Plane::create();
		read("position");
		zone->setPosition(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("normal");
		((SPK::Plane*)zone)->setNormal(getSPKVector3dFromIrr(buildVector3d<f32>()));
	} else
	if (ztype == cp3d::ps::EZT_POINT) {
		zone = SPK::Point::create();
		read("position");
		zone->setPosition(getSPKVector3dFromIrr(buildVector3d<f32>()));
	} else
	if (ztype == cp3d::ps::EZT_RING) {
		zone = SPK::Ring::create();
		read("position");
		zone->setPosition(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("normal");
		((SPK::Ring*)zone)->setNormal(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("minRadius");
		f32 minRadius = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));
		read("maxRadius");
		f32 maxRadius = devices->getCore()->getF32(xmlReader->getAttributeValue("value"));
		((SPK::Ring*)zone)->setRadius(minRadius, maxRadius);
	} else
	if (ztype == cp3d::ps::EZT_SPHERE) {
		zone = SPK::Sphere::create();
		read("position");
		zone->setPosition(getSPKVector3dFromIrr(buildVector3d<f32>()));
		read("radius");
		((SPK::Sphere*)zone)->setRadius(devices->getCore()->getF32(xmlReader->getAttributeValue("value")));
	} else
	{
		assert(0);
	}

	return zone;
}

SPK::Model *CParticleSystemsImporter::buildModel() {
	//-------------------------------------------------
	/// Read enabled params and store them
	array<SEnableParamsValues> params;
	int enableFlags = 0;
    int mutableFlags = 0;
    int randomFlags = 0;
    int interpolatedFlags = 0;

	read("enabledParams");
	while (xmlReader->read()) {
		element = xmlReader->getNodeName();

		if (element == "enabledParams") /// If no more params then we break
			break;

		SEnableParamsValues v;
		v.enabled = false;

		if (element == "enabledparam" && devices->getCore()->getS32(xmlReader->getAttributeValue("value")) != -1) {
			int tempFlag = devices->getCore()->getU32(xmlReader->getAttributeValue("value"));

			enableFlags |= tempFlag;
			v.enabled = true;

			if (xmlReader->getAttributeValue("death") && !xmlReader->getAttributeValue("birth")) { /// Only enabled
				v.isRandom = false;
				v.isMutable = false;
				v.nbParams = 1;
				v.parameters[0] = devices->getCore()->getF32(xmlReader->getAttributeValue("death"));
			} else
			if (xmlReader->getAttributeValue("birth") && xmlReader->getAttributeValue("death")) { /// Mutable flag
				v.isRandom = false;
				v.isMutable = true;
				v.nbParams = 2;
				v.parameters[0] = devices->getCore()->getF32(xmlReader->getAttributeValue("birth"));
				v.parameters[1] = devices->getCore()->getF32(xmlReader->getAttributeValue("death"));
				mutableFlags |= tempFlag;
			} else
			if (xmlReader->getAttributeValue("rand0")) { /// Random flag
				v.isRandom = true;
				v.isMutable = false;
				v.nbParams = 2;
				v.parameters[0] = devices->getCore()->getF32(xmlReader->getAttributeValue("rand0"));
				v.parameters[1] = devices->getCore()->getF32(xmlReader->getAttributeValue("rand1"));
				randomFlags |= tempFlag;
			} else
			if (xmlReader->getAttributeValue("minBirth")) { /// Random & Mutable flags
				v.isRandom = true;
				v.isMutable = true;
				v.nbParams = 4;
				v.parameters[0] = devices->getCore()->getF32(xmlReader->getAttributeValue("minBirth"));
				v.parameters[1] = devices->getCore()->getF32(xmlReader->getAttributeValue("maxBirth"));
				v.parameters[2] = devices->getCore()->getF32(xmlReader->getAttributeValue("minDeath"));
				v.parameters[3] = devices->getCore()->getF32(xmlReader->getAttributeValue("maxDeath"));
				randomFlags |= tempFlag;
				mutableFlags |= tempFlag;
			} else /// Just interpolated flag :)
			{
				interpolatedFlags |= tempFlag;
			}
		}

		params.push_back(v);
	}

	SPK::Model* model = SPK::Model::create(enableFlags, mutableFlags, randomFlags, interpolatedFlags);
	//-------------------------------------------------

	//-------------------------------------------------
	/// Fill model with saved params
	for (int i=0; i < (int)SPK::PARAM_CUSTOM_0; i++) {
		SPK::ModelParam param = static_cast<SPK::ModelParam>(i);
		if (params[i].enabled) {
			if (params[i].nbParams == 1) {
				model->setParam(param, params[i].parameters[0]);
				continue;
			}

			if (params[i].nbParams == 2) {
				model->setParam(param, params[i].parameters[0], params[i].parameters[1]);
				continue;
			}

			if (params[i].nbParams == 4) {
				model->setParam(param, params[i].parameters[0], params[i].parameters[1], 
									   params[i].parameters[2], params[i].parameters[3]);
				continue;
			}
		}
	}
	params.clear();
	//-------------------------------------------------

	read("name");
	model->setName(stringc(xmlReader->getAttributeValue("value")).c_str());
	read("lifeTime");
	model->setLifeTime(devices->getCore()->getF32(xmlReader->getAttributeValue("min")),
					   devices->getCore()->getF32(xmlReader->getAttributeValue("max")));

	readWithNextElement("interpolators", "model");
	while (element == "interpolators") {

		while (xmlReader->read()) {
			element = xmlReader->getNodeName();

			if (element == "interpolators") /// If no more params then we break
				break;

			if (element == "interpolator") {
				SPK::ModelParam param = static_cast<SPK::ModelParam>(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
				SPK::Interpolator *interpolator = model->getInterpolator(param);
				interpolator->addEntry(devices->getCore()->getF32(xmlReader->getAttributeValue("x")),
									   devices->getCore()->getF32(xmlReader->getAttributeValue("y0")),
									   devices->getCore()->getF32(xmlReader->getAttributeValue("y1")));
			}
		}

		readWithNextElement("interpolators", "model");
	}

	return model;
}

SPK::IRR::IRRQuadRenderer *CParticleSystemsImporter::buildRenderer() {
	SPK::IRR::IRRQuadRenderer *renderer = SPK::IRR::IRRQuadRenderer::create(devices->getDevice());

	read("name");
	renderer->setName(stringc(xmlReader->getAttributeValue("value")).c_str());
	{
		read("scale");
		core::dimension2df scale = buildDimension2d<f32>();
		renderer->setScale(scale.Width, scale.Height);
	}

	read("texturingMode");
	renderer->setTexturingMode((SPK::TexturingMode)devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("blendingMode");
	renderer->setBlending((SPK::BlendingMode)devices->getCore()->getU32(xmlReader->getAttributeValue("value")));

	{
		read("atlasDimension");
		core::dimension2df atlasDimension = buildDimension2d<f32>();
		renderer->setAtlasDimensions(atlasDimension.Width, atlasDimension.Height);
	}
	read("texture");
	renderer->setTexture(devices->getVideoDriver()->getTexture(xmlReader->getAttributeValue("value")));
	read("enableAlphaTest");
	renderer->enableRenderingHint(SPK::ALPHA_TEST, devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("enableDepthTest");
	renderer->enableRenderingHint(SPK::DEPTH_TEST, devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("enableDepthWrite");
	renderer->enableRenderingHint(SPK::DEPTH_WRITE, devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("setActive");
	renderer->setActive(devices->getCore()->getU32(xmlReader->getAttributeValue("value")));
	read("alphaTest");
	renderer->setAlphaTestThreshold(devices->getCore()->getF32(xmlReader->getAttributeValue("value")));

	return renderer;
}

//---------------------------------------------------------------------------------------------
//-----------------------------------PRIVATE XML METHODS---------------------------------------
//---------------------------------------------------------------------------------------------

void CParticleSystemsImporter::readWithNextElement(std::string node, std::string nextNode) {
    xmlReader->read();
    element = xmlReader->getNodeName();
    if (element != node && element != nextNode) {
        while (xmlReader && element != node && element != nextNode && xmlReader->read()) {
            element = xmlReader->getNodeName();
            printf("current element : %s\n", element.c_str());
        }
    }
}

void CParticleSystemsImporter::read(std::string node) {
    xmlReader->read();
    element = xmlReader->getNodeName();
    if (element != node) {
        while (xmlReader && element != node && xmlReader->read()) {
            element = xmlReader->getNodeName();
            printf("current element : %s\n", element.c_str());
        }
    }
}

//---------------------------------------------------------------------------------------------
//---------------------------------PRIVATE BUILD METHODS---------------------------------------
//---------------------------------------------------------------------------------------------

template<class T>
vector3d<T> CParticleSystemsImporter::buildVector3d() {
	vector3d<T> vec = vector3d<T>(xmlReader->getAttributeValueAsFloat("X"),
                                  xmlReader->getAttributeValueAsFloat("Y"),
                                  xmlReader->getAttributeValueAsFloat("Z"));
    return vec;
}

template<class T>
dimension2d<T> CParticleSystemsImporter::buildDimension2d() {
	dimension2d<T> dim = dimension2d<T>(devices->getCore()->getF32(xmlReader->getAttributeValue("W")),
										devices->getCore()->getF32(xmlReader->getAttributeValue("H")));
	return dim;
}
