#ifndef _CLENSFLARESCENENODE_H
#define _CLENSFLARESCENENODE_H

#include <ISceneNode.h>
#include <S3DVertex.h>

namespace irr
{
namespace scene
{
	// enum with different flare types (used by SFlareData)
	enum E_FLARE_TYPE_E
	{
		EFTE_SUN = 0,
		EFTE_GLOW,
		EFTE_LENS,
		EFTE_STREAKS,
		EFTE_RING,
		EFTE_HALO,
		EFTE_CIRCLE,
		EFTE_POLY,
		EFTE_COUNT
	};

	// struct holding the flare specification
	struct SFlareData
	{
	public:
		// constructor
		SFlareData(E_FLARE_TYPE_E type, float position, float scale, video::SColor color)
		{
			Type = type;
			Position = position;
			Scale = scale;
			Color = color;
		}

		// flare type
		E_FLARE_TYPE_E Type;
		// position
		f32 Position;
		// flare scale
		f32 Scale;
		// flare color
		video::SColor Color;
	};

	class CLensFlareSceneNode : public ISceneNode
	{
	public:
		// constructor
		CLensFlareSceneNode(ISceneNode* parent, scene::ISceneManager* mgr, s32 id = -1);
		
		// destructor
		virtual ~CLensFlareSceneNode();

	protected:
		// material of the node
		video::SMaterial Material;
		
		// Bounding box
		core::aabbox3d<f32> BBox;

		// vertices and indices of a flare element
		video::S3DVertex Vertices[4];
		u16 Indices[6];

		// flare data array
		core::array<SFlareData> FlareData;

		// Strength of the flare effect (between 0 and 1)
		f32 Strength;
        
        // occlusion for distance between LF and active camera positions
        bool falseOcclusion;
		
	public:
		// typical OnRegisterSceneNode function
		virtual void OnRegisterSceneNode();
		
		// renders the node
		virtual void render();
		
		// returns the bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		
		// returns the node type
		virtual ESCENE_NODE_TYPE getType() const;
		
		// returns the material count
		virtual u32 getMaterialCount() const;
		
		// returns the material
		virtual video::SMaterial& getMaterial(u32 i);
		
		// writes attributes of the scene node.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options = 0) const;
		
		// reads attributes of the scene node.
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options = 0);
				
		// clones the node
		virtual ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0); 
		
		// returns the flare data array
		core::array<SFlareData>& getFlareData();

		// returns the strength (visibility) of the flares
		f32 getStrength() { return Strength; }
        
        // returns and set the false occlusion to the flare
        bool getFalseOcclusion() { return falseOcclusion; }
        void setFalseOcclusion(bool _falseOcclusion) { falseOcclusion = _falseOcclusion; }

		// sets the strength (visibility) of the flares
		void setStrength(f32 strength) { Strength = core::clamp(strength, 0.0f, 1.0f); }
	};

} // end namespace scene
} // end namespace irr

#endif

