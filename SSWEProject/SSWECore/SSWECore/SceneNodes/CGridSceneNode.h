#ifndef __C_GRID_SCENE_NODE_H__
#define __C_GRID_SCENE_NODE_H__

#include <SSWECore.h>

#include "ISceneNode.h"
#include "SMeshBuffer.h"

namespace irr {
    namespace scene {
        #ifndef _SSWE_LINUX_
        class SSWE_CORE_API CGridSceneNode : public ISceneNode {
        #else
        class CGridSceneNode : public ISceneNode {
        #endif

        public:

            CGridSceneNode(ISceneNode* parent, ISceneManager* smgr, s32 id = -1,
                           u32 spacing = 8, u32 size = 1024, video::SColor gridcolor = video::SColor(255,128,128,128),
                           u32 accentlineoffset = 8, video::SColor accentgridcolor = video::SColor(255,192,192,192),
                           bool axislinestate = false);

            virtual CGridSceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newSceneManager = 0);

            virtual void OnRegisterSceneNode();

            virtual void render();

            virtual const core::aabbox3d<f32>& getBoundingBox() const;

            virtual u32 getMaterialCount();

            virtual video::SMaterial& getMaterial(u32 i);

            void RegenerateGrid();

            u32 GetSpacing();

            u32 GetSize();

            video::SColor GetGridColor();

            u32 GetAccentlineOffset();

            video::SColor GetAccentlineColor();

            bool AreAxisLineActive();

            video::SColor GetAxisLineXColor();

            video::SColor GetAxisLineZColor();

            void SetSpacing(u32 newspacing);

            void SetSize(u32 newsize);

            void SetGridColor(video::SColor newcolor);

            void SetAccentlineOffset(u32 newoffset);

            void SetAccentlineColor(video::SColor newcolor);

            void SetAxisLineActive(bool active);

            void SetAxisLineXColor(video::SColor XLine);

            void SetAxisLineZColor(video::SColor ZLine);

            void SetMaterial(video::SMaterial newMaterial);

        private:

            u32 m_spacing;
            u32 m_size;
            video::SColor m_gridcolor;
            video::SColor m_accentgridcolor;
            u32 m_accentlineoffset;
            bool m_AxisLineState;
            video::SColor m_XLineColor;
            video::SColor m_ZLineColor;

            SMeshBuffer Buffer;
        };

    };
};

#endif

