#include "stdafx.h"

#include "CGridSceneNode.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"

namespace irr
{
    namespace scene
    {
        CGridSceneNode::CGridSceneNode(ISceneNode* parent, ISceneManager* smgr, s32 id, 
                                       u32 spacing, u32 size, video::SColor gridcolor, u32 accentlineoffset, 
                                       video::SColor accentgridcolor, bool axislinestate)	: ISceneNode(parent, smgr, id), 
		m_spacing(spacing), m_size(size), 
		m_gridcolor(gridcolor), m_accentgridcolor(accentgridcolor),
        m_accentlineoffset(accentlineoffset), m_AxisLineState(axislinestate),
		m_XLineColor(video::SColor(255,255,0,0)), m_ZLineColor(video::SColor(255,0,0,255)) {
            Buffer.Material.Wireframe = false;
            Buffer.Material.Lighting = false;
            Buffer.Material.Thickness = 1;
            Buffer.Material.FogEnable = false;
            Buffer.Material.ZWriteEnable = false;
            
            AutomaticCullingState = EAC_FRUSTUM_BOX;
            
            RegenerateGrid();
        }
        
        CGridSceneNode* CGridSceneNode::clone(ISceneNode *newParent, ISceneManager *newSceneManager) {
            if (!newParent) 
                newParent = Parent;
            
            if (!newSceneManager) 
                newSceneManager = SceneManager;
            
            CGridSceneNode* clone = new CGridSceneNode(
                                                       Parent,
                                                       SceneManager,
                                                       ID,
                                                       m_spacing,
                                                       m_size*2,
                                                       m_gridcolor,
                                                       m_accentlineoffset,
                                                       m_accentgridcolor,
                                                       m_AxisLineState);
            
            if (!clone)
                return 0x0;
            
            clone->SetAxisLineXColor(m_XLineColor);
            clone->SetAxisLineZColor(m_ZLineColor);
            clone->SetMaterial(Buffer.Material);
            
            clone->drop();
            return clone;
        }
        
        void CGridSceneNode::OnRegisterSceneNode() {
            if (IsVisible)
                SceneManager->registerNodeForRendering(this);
            
            ISceneNode::OnRegisterSceneNode();
        }
        
        void CGridSceneNode::RegenerateGrid() {
            Buffer.Indices.clear();
            Buffer.Vertices.clear();
            
            u32 m_numVertices = ((m_size / m_spacing) + 1) * 2 * 2;
            if (m_accentlineoffset)
                m_numVertices += ((m_size / (m_spacing * m_accentlineoffset)) + 1) * 2 * 2;
            
            if (m_numVertices > 65535) {
                return;
            }
            
            core::vector3df leftMost = core::vector3df(0,0,0);
            leftMost.X -= m_size/2;
            leftMost.Z -= m_size/2;
            
            core::vector3df rightMost = core::vector3df(0,0,0);
            rightMost.X += m_size/2;
            rightMost.Z += m_size/2;
            
            u32 indexIndex = 0;
            
            for (u32 x = 0; x <= m_size; x+= m_spacing) {
                core::vector3df start = leftMost;
                start.X += x ;
                
                core::vector3df end = rightMost;
                end.X = start.X;
                
                Buffer.Vertices.push_back(video::S3DVertex(start, core::vector3df(0,1,0), m_gridcolor, core::vector2df(0.0f, 0.0f)));
                Buffer.Vertices.push_back(video::S3DVertex(end, core::vector3df(0,1,0), m_gridcolor, core::vector2df(0.0f, 0.0f)));
                
                Buffer.Indices.push_back(indexIndex++);
                Buffer.Indices.push_back(indexIndex++);
            }
            
            for (u32 z = 0; z <= m_size; z+= m_spacing) {
                core::vector3df start = leftMost;
                start.Z += z ;
                
                core::vector3df end = rightMost;
                end.Z = start.Z;
                
                Buffer.Vertices.push_back(video::S3DVertex(start, core::vector3df(0,1,0), m_gridcolor, core::vector2df(0.0f, 0.0f)));
                Buffer.Vertices.push_back(video::S3DVertex(end, core::vector3df(0,1,0), m_gridcolor, core::vector2df(0.0f, 0.0f)));
                
                Buffer.Indices.push_back(indexIndex++);
                Buffer.Indices.push_back(indexIndex++);
            }
            
            if (m_accentlineoffset > 0) {
                for (u32 x = 0; x <= m_size; x+= m_spacing*m_accentlineoffset) {
                    core::vector3df start = leftMost;
                    start.X += x ;
                    
                    core::vector3df end = rightMost;
                    end.X = start.X;
                    
                    Buffer.Vertices.push_back(video::S3DVertex(start, core::vector3df(0,1,0), m_accentgridcolor, core::vector2df(0.0f, 0.0f)));
                    Buffer.Vertices.push_back(video::S3DVertex(end, core::vector3df(0,1,0), m_accentgridcolor, core::vector2df(0.0f, 0.0f)));
                    
                    Buffer.Indices.push_back(indexIndex++);
                    Buffer.Indices.push_back(indexIndex++);
                }
                
                for (u32 z = 0; z <= m_size; z+= m_spacing*m_accentlineoffset) {
                    core::vector3df start = leftMost;
                    start.Z += z ;
                    
                    core::vector3df end = rightMost;
                    end.Z = start.Z;
                    
                    Buffer.Vertices.push_back(video::S3DVertex(start, core::vector3df(0,1,0), m_accentgridcolor, core::vector2df(0.0f, 0.0f)));
                    Buffer.Vertices.push_back(video::S3DVertex(end, core::vector3df(0,1,0), m_accentgridcolor, core::vector2df(0.0f, 0.0f)));
                    
                    Buffer.Indices.push_back(indexIndex++);
                    Buffer.Indices.push_back(indexIndex++);
                }
            }
            
            Buffer.BoundingBox = core::aabbox3df(-(f32)m_size/2,-0.5f,-(f32)m_size/2,(f32)m_size/2,0.5f,(f32)m_size/2);
        }
        
        void CGridSceneNode::render() {
            video::IVideoDriver* driver = SceneManager->getVideoDriver();
            
            if (driver) {
                driver->setMaterial(Buffer.Material);
                driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
                
                driver->drawVertexPrimitiveList(Buffer.getVertices(), Buffer.getVertexCount(), Buffer.getIndices(), Buffer.getVertexCount()/2, video::EVT_STANDARD, scene::EPT_LINES);
                
                if (m_AxisLineState) {
                    driver->draw3DLine(core::vector3df((f32)m_size,0,0),core::vector3df(-(f32)m_size,0,0),m_XLineColor);
                    driver->draw3DLine(core::vector3df(0,0,(f32)m_size),core::vector3df(0,0,-(f32)m_size),m_ZLineColor);
                }
            }
        }
        
        const core::aabbox3d<f32>& CGridSceneNode::getBoundingBox() const {
            return Buffer.BoundingBox;
        }
        
        u32 CGridSceneNode::getMaterialCount() {
            return 1;
        }
        
        video::SMaterial& CGridSceneNode::getMaterial(u32 i) {
            return Buffer.Material;
        }
        
        u32 CGridSceneNode::GetSpacing() {
            return m_spacing;
        }
        
        u32 CGridSceneNode::GetSize() {
            return m_size;
        }
        
        u32 CGridSceneNode::GetAccentlineOffset() {
            return m_accentlineoffset;
        }
        
        video::SColor CGridSceneNode::GetAccentlineColor() {
            return m_accentgridcolor;
        }
        
        video::SColor CGridSceneNode::GetGridColor() {
            return m_gridcolor;
        }
        
        bool CGridSceneNode::AreAxisLineActive() {
            return m_AxisLineState;
        }
        
        video::SColor CGridSceneNode::GetAxisLineXColor() {
            return m_XLineColor;
        }
        
        video::SColor CGridSceneNode::GetAxisLineZColor() {
            return m_ZLineColor;
        }
        
        void CGridSceneNode::SetSpacing(u32 newspacing) {
            m_spacing = newspacing;
            RegenerateGrid();
        }
        
        void CGridSceneNode::SetSize(u32 newsize) {
            m_size = newsize;
            RegenerateGrid();
        }
        
        void CGridSceneNode::SetAccentlineColor(video::SColor newcolor) {
            m_accentgridcolor = newcolor;
            RegenerateGrid();
        }
        
        void CGridSceneNode::SetAccentlineOffset(u32 newoffset) {
            m_accentlineoffset = newoffset;
            RegenerateGrid();
        }
        
        void CGridSceneNode::SetGridColor(video::SColor newcolor) {
            m_gridcolor = newcolor;
            RegenerateGrid();
        }
        
        void CGridSceneNode::SetAxisLineActive(bool active) {
            m_AxisLineState = active;
        }
        
        void CGridSceneNode::SetAxisLineXColor(video::SColor XLine) {
            m_XLineColor = XLine;
        }
        
        void CGridSceneNode::SetAxisLineZColor(video::SColor ZLine) {
            m_ZLineColor = ZLine;
        }
        
        void CGridSceneNode::SetMaterial(video::SMaterial newMaterial) {
            Buffer.Material = newMaterial;
        }
    };
};

