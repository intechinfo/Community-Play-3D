#include "stdafx.h"

#include "CTerrain.h"
#include <irrlicht.h>

using namespace irr;

CTerrain::CTerrain(c8* HeightmapFile, E_TERRAIN_QUALITY Quality, f32 ScaleTexture, scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id)
				   : scene::ISceneNode(parent, smgr, id)
{	
    video::IImage *Heightmap = SceneManager->getVideoDriver()->createImageFromFile(HeightmapFile);

    u16 Size = Heightmap->getDimension().Width;	
    s32 SizeOfTiles = 0;

    switch(Size) {
        case 65:
            SizeOfTiles=(Size/4)+1;
            NbsTiles = 4*4;
            break;
        case 129:
            SizeOfTiles=(Size/8)+1;
            NbsTiles = 8*8;
            break;
        case 257:
			SizeOfTiles=(Size/16)+1;
            NbsTiles = 16*16;
            break;
        case 513:
            SizeOfTiles=(Size/16)+1;
            NbsTiles = 16*16;
            break;
        case 769:
            SizeOfTiles=(Size/24)+1;
            NbsTiles = 24*24;
            break;
        case 1025:
            SizeOfTiles=(Size/32)+1;
            NbsTiles = 32*32;
            break;
        case 2049:
            SizeOfTiles=(Size/32)+1;
            NbsTiles = 32*32;
            break;
        default:
            SizeOfTiles=(Size/16)+1;
            NbsTiles = 16*16;
            break;
    }

    TerrainMesh = new scene::SMesh();

    u32 SOTiles = irr::core::ceil32((f32)SizeOfTiles/(f32)Quality);

    CTTileBuffer=new scene::SMeshBufferLightMap* [NbsTiles];

    u32 TileX=0, TileZ=0;
    for (u32 i =0; i < NbsTiles; ++i) {
		CTTileBuffer[i]=new scene::SMeshBufferLightMap();
        CTTileBuffer[i]->Vertices.set_used(SizeOfTiles*SizeOfTiles);
        CTTileBuffer[i]->Indices.set_used(SizeOfTiles*SizeOfTiles*6);

        const f32 tdSize = 1.0f/(f32)((Size-1.0f));
        u32 Index=0;
        u16 NbsIndices=0,NbsVertices=0;

         for(u32 x=TileX; x<(TileX+SizeOfTiles); x+=Quality) {
            for (u32 z=TileZ; z<(TileZ+SizeOfTiles); z+=Quality) {
                if (NbsVertices < (SOTiles*SOTiles)-SOTiles-1) {
                    Index = NbsVertices;
                    u32 TestValue = ( (((x-TileX)/Quality)+1) * ((z-TileZ)/Quality) + ((x-TileX)/Quality) );
                    if (Index != TestValue || (x-TileX==0 && z < TileZ+SizeOfTiles-Quality)) {
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+1;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+SOTiles+1;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+SOTiles+1;
                        CTTileBuffer[i]->Indices[NbsIndices++]=Index+SOTiles;
                    }
                }
				video::S3DVertex2TCoords Vertex;
                Vertex.Normal = core::vector3df(0,1,0);
                Vertex.Pos.X = (f32)x;
                video::SColor pixelColor(Heightmap->getPixel(x,Size-1-z));
                Vertex.Pos.Y = (f32) pixelColor.getLuminance()/10.0f;
                Vertex.Pos.Z = (f32)z;
                Vertex.TCoords = core::vector2d<f32>( (f32)(x*tdSize), (f32)((Size-1-z)*tdSize));
                Vertex.TCoords2 = core::vector2d<f32>( (f32)(x*tdSize), (f32)((Size-1-z)*tdSize))*ScaleTexture;	
				Vertex.Color = video::SColor(255,255,255,255);

				CTTileBuffer[i]->Vertices[NbsVertices]=Vertex;

                NbsVertices++;				
            }
        }
		
        CTTileBuffer[i]->Material.Lighting = false;
        CTTileBuffer[i]->Material.Wireframe = false;
        CTTileBuffer[i]->Material.BackfaceCulling = true;
        CTTileBuffer[i]->Material.GouraudShading = true;
        CTTileBuffer[i]->Material.FogEnable = false;		
		
		CTTileBuffer[i]->Vertices.set_used(NbsVertices);
        CTTileBuffer[i]->Indices.set_used(NbsIndices);
		
		for(s32 j = 0; j < ((Quality+1)/2); j++)  {
            for(u32 index = 2; index < (SOTiles * SOTiles - 2); index++)  {                
                CTTileBuffer[i]->Vertices[index].Pos.Y += (1/8)*
                (CTTileBuffer[i]->Vertices[index-2].Pos.Y +
                2*CTTileBuffer[i]->Vertices[index-1].Pos.Y +
                2*CTTileBuffer[i]->Vertices[index].Pos.Y +
                2*CTTileBuffer[i]->Vertices[index+1].Pos.Y +
                CTTileBuffer[i]->Vertices[index+2].Pos.Y);
            }
        }
        
        for(s32 k = 0; k < ((Quality+1)/2); k++)  { 
            for(u32 index = SOTiles; index < (SOTiles * (SOTiles - 1)); index++)  {
                CTTileBuffer[i]->Vertices[index].Pos.Y = (CTTileBuffer[i]->Vertices[index - SOTiles].Pos.Y + CTTileBuffer[i]->Vertices[index + SOTiles].Pos.Y ) / 2.0f; 
			} 
        }		

        CTTileBuffer[i]->recalculateBoundingBox();

		CTTileBuffer[i]->setHardwareMappingHint(scene::EHM_STATIC);	

        TerrainMesh->addMeshBuffer(CTTileBuffer[i]);
       
		TileX+=SizeOfTiles-1;
		if(TileX >= NbsTiles) {
            TileX=0;
            
            TileZ+=SizeOfTiles-1;
            if(TileZ >= NbsTiles) {
                TileZ=0;
            }
        }		
    }

	AutomaticCullingState = scene::EAC_OFF;

    Heightmap->drop();
}

CTerrain::~CTerrain() {
	SceneManager->getVideoDriver()->removeTexture(CTTileBuffer[0]->Material.getTexture(0));
	
	for(u32 i=0;i<NbsTiles;++i) {
        if (CTerrain::CTTileBuffer[i] != NULL) {	
			SceneManager->getVideoDriver()->removeHardwareBuffer(CTTileBuffer[i]);
			CTerrain::CTTileBuffer[i]->drop();
        }
    }
	delete [] CTTileBuffer;	

	SceneManager->getMeshCache()->removeMesh(TerrainMesh);

    if(CTerrain::TerrainMesh != NULL) {
        CTerrain::TerrainMesh->drop();
    }

	this->remove();	
}

void CTerrain::OnRegisterSceneNode() {
    if (IsVisible)
        SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
}

void CTerrain::render() {
    scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
    const scene::SViewFrustum* frustum = cam->getViewFrustum();
    video::IVideoDriver* Driver = SceneManager->getVideoDriver();
    core::vector3df Pos = cam->getPosition();
	Pos.Y = getPosition().Y;	
	Driver->setTransform(video::ETS_WORLD,AbsoluteTransformation);	

	if (DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY) {
		video::SMaterial m = CTTileBuffer[0]->getMaterial();
		m.Lighting = false;
		m.Wireframe = true;
        m.BackfaceCulling = false;
		Driver->setMaterial(m);
	}
	else {
		Driver->setMaterial(video::SMaterial(CTTileBuffer[0]->Material));
	}
	
    for (u32 i=0; i<NbsTiles; i++) {
        if (CTTileBuffer[i] != NULL) {
            if( frustum->getBoundingBox().intersectsWithBox(CTTileBuffer[i]->getBoundingBox())==true) {
				if(CTTileBuffer[i]->BoundingBox.getCenter().getDistanceFromSQ(Pos) < RenderDistance) {					
					Driver->drawMeshBuffer(CTTileBuffer[i]);
				}
            }
        }
    }	

	if(DebugDataVisible) {  
        for (u32 i=0;i<NbsTiles;i++) {			
			if (CTTileBuffer[i] != NULL) {
				if (DebugDataVisible & scene::EDS_BBOX_BUFFERS) {							
					video::SMaterial m;
					m.Lighting = false;
					Driver->setMaterial(m);
					video::SColor c (255,0,255,0);				
					
					Driver->draw3DBox(CTTileBuffer[i]->getBoundingBox(), c);
				}

				if (DebugDataVisible & scene::EDS_BBOX) {							
					video::SMaterial m;
					m.Lighting = false;
					Driver->setMaterial(m);
					video::SColor c (255,255,0,0);				
					
					Driver->draw3DBox(this->getBoundingBox(), c);
				}				
				
				if( frustum->getBoundingBox().intersectsWithBox(CTTileBuffer[i]->getBoundingBox())==true) {
                    f64 ActualDistance = CTTileBuffer[i]->BoundingBox.getCenter().getDistanceFromSQ(Pos);
                    if(ActualDistance < RenderDistance) {
						// normals
						if (DebugDataVisible & scene::EDS_NORMALS) {	
							video::SMaterial m;
							m.Lighting = false;
							Driver->setMaterial(m);
							video::SColor c (255,0,0,255 );

							for(u16 j = 0 ; j < CTTileBuffer[i]->getVertexCount(); j++) {
								video::S3DVertex2TCoords v = CTTileBuffer[i]->Vertices[j];
								core::vector3df h = v.Normal * getScale().Y * 4;
								Driver->draw3DLine ( v.Pos, v.Pos + h, c );
							}
						}
                    }
                }
            }
        }
    }
}

const core::aabbox3d<f32>& CTerrain::getBoundingBox() const {
    return BoundingBox;
}

void CTerrain::recalculateBoundingBox() {
	BoundingBox.MinEdge.X = CTTileBuffer[0]->getBoundingBox().MinEdge.X;
    BoundingBox.MinEdge.Y = CTTileBuffer[0]->getBoundingBox().MinEdge.Y;
    BoundingBox.MinEdge.Z = CTTileBuffer[0]->getBoundingBox().MinEdge.Z;

	BoundingBox.MaxEdge.X = CTTileBuffer[NbsTiles-1]->getBoundingBox().MaxEdge.X;
	BoundingBox.MaxEdge.Y = CTTileBuffer[NbsTiles-1]->getBoundingBox().MaxEdge.Y;
	BoundingBox.MaxEdge.Z = CTTileBuffer[NbsTiles-1]->getBoundingBox().MaxEdge.Z;

    for(s32 i=0; i<NbsTiles; i++) {
		if(BoundingBox.MinEdge.Y > CTTileBuffer[i]->getBoundingBox().MinEdge.Y) 
			BoundingBox.MinEdge.Y = CTTileBuffer[i]->getBoundingBox().MinEdge.Y;

        if(BoundingBox.MaxEdge.Y < CTTileBuffer[i]->getBoundingBox().MaxEdge.Y) 
			BoundingBox.MaxEdge.Y = CTTileBuffer[i]->getBoundingBox().MaxEdge.Y;
    }
}

void CTerrain::setPosition(const core::vector3df &Pos)
{
	for (u32 i=0;i<NbsTiles;i++) {
        if (CTTileBuffer[i] != NULL) {
            for (u32 j=0;j<CTTileBuffer[i]->getVertexCount();++j) {
                CTTileBuffer[i]->Vertices[j].Pos+=Pos;
            }
            CTTileBuffer[i]->recalculateBoundingBox();
        }
    }
}

void CTerrain::setScale(const core::vector3df &Scale) {
	for (u32 i=0;i<NbsTiles;i++) {
        if (CTTileBuffer[i] != NULL) {
            for (u32 j=0;j<CTTileBuffer[i]->getVertexCount();++j) {
                CTTileBuffer[i]->Vertices[j].Pos*=Scale;
            }
            CTTileBuffer[i]->recalculateBoundingBox();
        }
    }
}


u32 CTerrain::getMaterialCount() {
    return 1;
}

video::SMaterial& CTerrain::getMaterial(u32 i) {
    return Material;
}

void CTerrain::setColorTexture(c8* FileName) {
    for (u32 i=0;i<NbsTiles;++i) {
        if (CTTileBuffer[i] != NULL) {
			CTTileBuffer[i]->Material.MaterialType = video::EMT_SOLID;
			CTTileBuffer[i]->Material.TextureLayer[0].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
			CTTileBuffer[i]->Material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
			CTTileBuffer[i]->Material.TextureLayer[0].TextureWrapV = video::ETC_CLAMP;
		}
    }
}

void CTerrain::setDetailTexture(c8* FileName) {
	for (u32 i=0;i<NbsTiles;++i) {
        if (CTTileBuffer[i] != NULL) {
            CTTileBuffer[i]->Material.MaterialType = video::EMT_DETAIL_MAP;
            CTTileBuffer[i]->Material.TextureLayer[1].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
		}
    }
}

void CTerrain::setRenderDistance(f32 Distance) {
    RenderDistance = Distance * Distance;
}

void CTerrain::setMaterialType(video::E_MATERIAL_TYPE Mat) {
    for (u32 i=0;i < NbsTiles;++i) {
        if (CTTileBuffer[i] != NULL) {
            CTTileBuffer[i]->Material.MaterialType = Mat;
        }
    }
}

void CTerrain::setMaterialFlag(video::E_MATERIAL_FLAG Mat, bool Flag) {
	for (u32 i=0;i < NbsTiles;++i) {
        if (CTTileBuffer[i] != NULL) {
			CTTileBuffer[i]->Material.setFlag(Mat, Flag);
        }
    }
}

scene::IMesh* CTerrain::getMesh() {
    return (scene::IMesh*)TerrainMesh;
}

f32 CTerrain::getHeight(f32 x, f32 z) {
    scene::SMeshBufferLightMap* TempBuffer=NULL;

    f32 ValueReturn = 0.0f;

    for (u32 i=0;i < NbsTiles;++i) {
        if (CTTileBuffer[i] != NULL) {
            core::vector3df Vec(x,CTTileBuffer[i]->getBoundingBox().getCenter().Y,z);
            if(CTTileBuffer[i]->getBoundingBox().isPointInside(Vec)) {
                TempBuffer=CTTileBuffer[i];
                break;
            }
        }
    }

    if(TempBuffer != NULL) {
        scene::SMesh* Mesh=new scene::SMesh();
        Mesh->addMeshBuffer(TempBuffer);
        scene::ITriangleSelector* selector= SceneManager->createTriangleSelector((scene::IMesh*)Mesh,this);
        this->setTriangleSelector(selector);

        core::line3d<f32> line;
        line.start = core::vector3d<f32>(x,-200000,z);
        line.end = core::vector3d<f32>(x,200000,z);

        core::vector3df intersection;
        core::triangle3df tri;

		ISceneNode *node;
        if (SceneManager->getSceneCollisionManager()->getCollisionPoint(line, selector, intersection,tri, node)) {
            ValueReturn = intersection.Y;
        }
        selector->drop();
        Mesh->drop();        
    }
    else {
        ValueReturn=-999999.999f;
    }

    return ValueReturn;
}

void CTerrain::setTexture(u32 NumTex, c8* FileName) {
    for (u32 i=0;i<NbsTiles;++i) {
        if (CTTileBuffer[i] != NULL) {
			irr::video::IVideoDriver * driver = this->SceneManager->getVideoDriver();

			switch(NumTex) {
			case 0:
				CTTileBuffer[i]->Material.TextureLayer[0].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
				CTTileBuffer[i]->Material.TextureLayer[0].TextureWrapU = video::ETC_CLAMP;
				CTTileBuffer[i]->Material.TextureLayer[0].TextureWrapV = video::ETC_CLAMP;
				break;

			case 1:				
				CTTileBuffer[i]->Material.TextureLayer[1].BilinearFilter = true;				
				CTTileBuffer[i]->Material.TextureLayer[1].Texture = SceneManager->getVideoDriver()->getTexture(FileName);		
				break;

			default:
				CTTileBuffer[i]->Material.TextureLayer[NumTex].Texture = SceneManager->getVideoDriver()->getTexture(FileName);
				break;
			}
		}
    }
}

void CTerrain::setShader(c8 * shaderFileName, scene::ISceneManager* smgr) {
    video::IVideoDriver* driver = smgr->getVideoDriver();
    s32 newMaterialType1 = 0;
    video::IGPUProgrammingServices* gpu = smgr->getVideoDriver()->getGPUProgrammingServices();
	
    if (gpu) {
        MyShaderCallBack* mc = new MyShaderCallBack();

		newMaterialType1 = gpu->addHighLevelShaderMaterialFromFiles(
			shaderFileName, "VS", video::EVST_VS_2_0,
			shaderFileName, "PS", video::EPST_PS_2_0,
			mc, video::EMT_SOLID);
		
		mc->drop();
    }  	

    setMaterialType((video::E_MATERIAL_TYPE)newMaterialType1);	
}
