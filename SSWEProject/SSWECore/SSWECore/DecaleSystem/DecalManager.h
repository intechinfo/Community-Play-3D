/**
 * DecalManager.h
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef DECALMANAGER_H_
#define DECALMANAGER_H_

#include <irrlicht.h>
#include "DecalSceneNode.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;
using namespace io;

class SSWE_CORE_API DecalManager {
private:
	irr::scene::ISceneManager* smgr;
	irr::video::IVideoDriver* driver;
	irr::scene::ITerrainSceneNode* terrain;
	irr::scene::IMetaTriangleSelector* metaSelector;
public:
	/**
	 * Constructor of DecalManager
	 * @param smgr
	 */
	DecalManager(irr::scene::ISceneManager* smgr);

	/**
	 * Destructor of DecalManager
	 */
	~DecalManager();

	/**
	 * Create new meta triangle selector
	 */
	void createMetaTriangleSelector();

	/**
	 * Get meta triangle selector
	 * @return metaSelector
	 */
	irr::scene::IMetaTriangleSelector* getMetaTriangleSelector();

	/**
	 * Get all decals or decal with the same texture
	 * @param texturePath (optional)
	 * @return decals
	 */
	irr::core::array<DecalSceneNode*> getDecals(const irr::io::path texturePath = "");

	/**
	 * Get permanent decals (decals with lifetime 0)
	 * @return decals
	 */
	irr::core::array<DecalSceneNode*> getPermanentDecals();

	/**
	 * Combine multiple decals into 1 decal (better draw performance) make sure they have the same material
	 * @param decals
	 * @return decals
	 */
	irr::core::array<DecalSceneNode*> combineDecals(irr::core::array<DecalSceneNode*> decals);

	/**
	 * Set terrain
	 * @param terrain
	 */
	void setTerrain(irr::scene::ITerrainSceneNode* terrain);

	/**
	 * Add mesh to where decals can be created on
	 * @param node
	 */
	void addMesh(irr::scene::IAnimatedMeshSceneNode* node);

	/**
	 * Add mesh to where decals can be created on
	 * @param node
	 */
	void addMesh(irr::scene::IMeshSceneNode* node);

	/**
	 * Add mesh to where decals can be created on
	 * @param mesh
	 * @param node
	 */
	void addMesh(irr::scene::IMesh* mesh, irr::scene::ISceneNode* node);

	/**
	 * Remove mesh
	 * @param node
	 */
	void removeMesh(irr::scene::ISceneNode* node);

	/**
	 * Remove all triangle selectors from the meta selector
	 */
	void clear();

	/**
	 * Get normal of triangle on position, which creates a line from top to bottom of the terrain node
	 * @param position
	 * @param useTerrainOnly
	 *
	 * @return normal
	 */
	irr::core::vector3df getNormal(irr::core::vector3df position, const bool useTerrainOnly = false);

	/**
	 * Get normal of triangle on position
	 * @param startPosition
	 * @param endPosition
	 * @param useTerrainOnly
	 *
	 * @return normal
	 */
	irr::core::vector3df getNormal(irr::core::vector3df startPosition, irr::core::vector3df endPosition, const bool useTerrainOnly = false);

	/**
	 * Get normal of triangle on position
	 * @param line
	 * @param useTerrainOnly
	 *
	 * @return normal
	 */
	irr::core::vector3df getNormal(irr::core::line3df line, const bool useTerrainOnly = false);

	/**
	 * Get collision point
	 * @param position
	 * @param useTerrainOnly
	 *
	 * @return collisionPoint
	 */
	irr::core::vector3df getCollisionPoint(irr::core::vector3df position, const bool useTerrainOnly = false);

	/**
	 * Add decal to world
	 * @param filename
	 * @param position center position of the decal
	 * @param dimension
	 * @param normal orientation of the decal
	 * @param textureRotation in degrees
	 * @param parent
	 * @param lifetime
	 * @param distance
	 * @param useTerrainOnly
	 *
	 * @return decal
	 */
	DecalSceneNode* addDecal(irr::io::path filename, irr::core::vector3df position, irr::core::vector3df dimension = irr::core::vector3df(1, 1, 1), irr::core::vector3df normal = irr::core::vector3df(0, 1, 0), const irr::f32 textureRotation = 0, irr::scene::ISceneNode* parent = 0,
			const irr::f32 lifeTime = 0, const irr::f32 distance = 0, const bool useTerrainOnly = false);

	/**
	 * Add decal to world
	 * @param texture
	 * @param position center position of the decal
	 * @param dimension
	 * @param normal orientation of the decal
	 * @param textureRotation in degrees
	 * @param parent
	 * @param lifetime
	 * @param distance
	 * @param useTerrainOnly
	 *
	 * @return decal
	 */
	DecalSceneNode* addDecal(irr::video::ITexture* texture, irr::core::vector3df position, irr::core::vector3df dimension = irr::core::vector3df(1, 1, 1), irr::core::vector3df normal = irr::core::vector3df(0, 1, 0), const irr::f32 textureRotation = 0, irr::scene::ISceneNode* parent = 0,
			const irr::f32 lifeTime = 0, const irr::f32 distance = 0, const bool useTerrainOnly = false);

private:
	/**
	 * Create mesh
	 * @param box
	 * @param rotationMatrix
	 * @param useTerrainOnly
	 *
	 * @return mesh
	 */
	irr::scene::IMesh* createMesh(irr::core::aabbox3df box, irr::core::matrix4 rotationMatrix, const bool useTerrainOnly = false);

	/**
	 * Clip triangle
	 * @param triangle to clip
	 *
	 * @return triangles
	 */
	irr::core::array<irr::core::triangle3df> clipTriangle(irr::core::triangle3df triangle);
};

#endif /* DECALMANAGER_H_ */
