#ifndef H_XEFFECTS_SQ
#define H_XEFFECTS_SQ

#include <SSWERenders.h>
#include <irrlicht.h>

class SSWE_RENDERS_API CScreenQuad
{
public:
	CScreenQuad()
	{
		Material.Wireframe = false;
		Material.Lighting = false;
		Material.ZWriteEnable = false;

		Vertices[0] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
		Vertices[1] = irr::video::S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,0.0f);
		Vertices[2] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
		Vertices[3] = irr::video::S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,1.0f);
		Vertices[4] = irr::video::S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,irr::video::SColor(0x0),0.0f,1.0f);
		Vertices[5] = irr::video::S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,irr::video::SColor(0x0),1.0f,0.0f);
	}

	virtual void render(irr::video::IVideoDriver* driver)
	{
		irr::u16 indices[6] = {0, 1, 2, 3, 4, 5};

		driver->setMaterial(Material);
		driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
		driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
	}

	virtual irr::video::SMaterial& getMaterial()
	{
		return Material;
	}

	irr::video::ITexture* rt[2];

private:
	irr::video::S3DVertex Vertices[6];
	irr::video::SMaterial Material;
};

//SCREEN QUAD HDR

class CScreenQuadHDRPipeline : public irr::scene::ISceneNode{
		irr::core::aabbox3df aabb;					//An axis aligned bounding box. Actually not needed.
		irr::video::SMaterial material;				//The material used to render the Scene Node
	 	irr::video::S3DVertex2TCoords vertices[4]; 	//The vertices of the Scene Node.
												//Normally we wouldn't need more
												//than one set of UV coordinates.
												//But if we are to use the builtin materials, this is necesary

public:
	 	CScreenQuadHDRPipeline(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
                               irr::core::dimension2d<irr::u32> screenSize)
	 	: irr::scene::ISceneNode(parent,mgr,id)
	 	{
	 		irr::f32 shiftX,shiftY;
	 		irr::core::dimension2d<irr::u32> currentResolution;

/**Here we initialize the vertices of the screen Aligned quad*/

	 		currentResolution = screenSize;

	 		aabb.reset(0,0,0);

	 		shiftX = 0.5/currentResolution.Width;	//This small shift is necesary to compensate the texture sampling bias
	 		shiftY = 0.5/currentResolution.Height;	//It avoids that our effect becomes too blurry.

			vertices[0] = irr::video::S3DVertex2TCoords(
						-1.0f,-1.0f,0.0f,
						0.0f,0.0f,-1.0f,
						irr::video::SColor(255,255,255,255),
						shiftX,1+shiftY,
						shiftX,1+shiftY);

			vertices[1] = irr::video::S3DVertex2TCoords(
						1.0f,-1.0,0.0f,
						0.0f,0.0f,-1.0f,
						irr::video::SColor(255,255,255,255),
						1.0f+shiftX,1+shiftY,
						1.0f+shiftX,1+shiftY);

			vertices[2] = irr::video::S3DVertex2TCoords(
						-1.0f,1.0,0.0f,
						0.0f,0.0f,-1.0f,
						irr::video::SColor(255,255,255,255),
						shiftX,shiftY,
						shiftX,shiftY);

			vertices[3] = irr::video::S3DVertex2TCoords(
						1.0f,1.0f,0.0f,
						0.0f,0.0f,-1.0f,
						irr::video::SColor(255,255,255,255),
						1.0f+shiftX,shiftY,
						1.0f+shiftX,shiftY);

			/**Now we proceed to initialize the appropriate settings for the material we are going to use
			We can alter these later, but for the time being, initializing then here will do no harm*/

			material.Lighting = false;							//No need for lighting.
			material.MaterialType = irr::video::EMT_LIGHTMAP_ADD;	//This will add both first and second textures :)
			material.BackfaceCulling=false;						//not needed, but simplifies things
			setAutomaticCulling(irr::scene::EAC_OFF);				//We don't need this scene
																//node to be culled because we render it in screen space.
		}

	 	~CScreenQuadHDRPipeline() { }

		const irr::core::aabbox3df& getBoundingBox() const {
			return aabb;
		}

	 	void OnRegisterSceneNode() {
			//This method is empty because it is best for us to render this scene node manually.
			//So, it is never really rendered on its own, if we don't tell it to do so.
		}

	 	void render() {
			irr::video::IVideoDriver* drv = getSceneManager()->getVideoDriver();
			irr::core::matrix4 proj;
			irr::u16 indices[] = {0,1,2,3,1,2};
			//A triangle list

			drv->setMaterial(material);

			drv->setTransform(irr::video::ETS_PROJECTION, irr::core::IdentityMatrix);
			drv->setTransform(irr::video::ETS_VIEW, irr::core::IdentityMatrix);

			drv->drawIndexedTriangleList(&vertices[0],4,&indices[0],2);

		}

	 	irr::u32 getMaterialCount() {
			return 1;	//There is only one material
		}

		irr::video::SMaterial& getMaterial(irr::u32 i) {
			return material;//We always return the same material, so there is no need for more.
		}
};



#endif