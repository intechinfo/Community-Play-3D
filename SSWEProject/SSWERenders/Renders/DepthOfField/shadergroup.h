#ifndef SHADERGROUP_H
#define SHADERGROUP_H

#include <SSWERenders.h>
#include "shadermaterial.h"
#include <irrlicht.h>
#include <list>
using namespace irr;

class SSWE_RENDERS_API ShaderGroup
{
    public:
        float sampleDist0;
        float sampleDist1;
        float range;
        float focus;
        float distanceScale;

        ShaderGroup(IrrlichtDevice* m_device, scene::ISceneManager* m_smgr, core::dimension2du resolution = core::dimension2du(512, 512));
        virtual ~ShaderGroup();
        int render(bool render);

        void add(scene::ISceneNode* node);
		void remove(scene::ISceneNode *node);

    protected:
    private:
        IrrlichtDevice* device;
        scene::ISceneManager* smgr;
        video::IGPUProgrammingServices*  gpu;
        scene::ISceneNode* postProcessorNode;
        video::ITexture* rt;
        ShaderMaterial* distance;
        ShaderMaterial* blur0;
        ShaderMaterial* blur1;
        ShaderMaterial* combine;
        std::list <scene::ISceneNode*> shaded;
};

#endif // SHADERGROUP_H
