#ifndef __POST_PROCESS_EFFECT_MOTION_BLUR__
#define __POST_PROCESS_EFFECT_MOTION_BLUR__

#include <SSWERenders.h>
#include <irrlicht.h>
using namespace irr;

class SSWE_RENDERS_API CMotionBlurCallback : public video::IShaderConstantSetCallBack
{
public:
	float m_ScreenWidth, m_ScreenHeight, m_Strength;
	int texture1, texture2;
	u32 m_time;
public:
	CMotionBlurCallback(float screenWidth, float screenHeight, float strength)
	{
		m_ScreenWidth  = screenWidth;
		m_ScreenHeight = screenHeight;
		m_Strength     = strength;
       
		texture1 = 0;
		texture2 = 1;
	}
	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		if (services->getVideoDriver()->getDriverType() == video::EDT_OPENGL) {
			services->setPixelShaderConstant("texture1", &texture1, 1);
			services->setPixelShaderConstant("texture2", &texture2, 1);
		}
       
		if (services->getVideoDriver()->getDriverType() != video::EDT_OPENGL) {
			services->setVertexShaderConstant("screenWidth", &m_ScreenWidth, 1);
			services->setVertexShaderConstant("screenHeight", &m_ScreenHeight, 1);
		}
		services->setPixelShaderConstant("strength", &m_Strength, 1);
	}
};

class IPostProcessMotionBlur : scene::ISceneNode
{
private:
	core::aabbox3d<f32> m_BBox;
	video::S3DVertex    m_Vertices[6];
	video::SMaterial    m_Material1;
	video::SMaterial    m_Material2;
	video::ITexture*    m_rtNext;
	video::ITexture*    m_rtPrev;
	video::ITexture*    m_rtAccum;
   
	irr::core::stringc vertex_shader_hlsl;
	irr::core::stringc pixel_shader_1_hlsl;
	irr::core::stringc pixel_shader_2_hlsl;

	irr::core::stringc vertex_shader_glsl;
	irr::core::stringc pixel_shader_1_glsl;
	irr::core::stringc pixel_shader_2_glsl;

	CMotionBlurCallback* callback;
   
public:

   IPostProcessMotionBlur(scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id)
      : scene::ISceneNode(parent, smgr, id)
   {
      m_Vertices[0] = video::S3DVertex(-1.0f, -1.0f, 0.0f, 0, 0, 0, video::SColor(0), 0.0f, 1.0f);
      m_Vertices[1] = video::S3DVertex(-1.0f, 1.0f, 0.0f, 0, 0, 0, video::SColor(0), 0.0f, 0.0f);
      m_Vertices[2] = video::S3DVertex(1.0f, 1.0f, 0.0f, 0, 0, 0, video::SColor(0), 1.0f, 0.0f);
      m_Vertices[3] = video::S3DVertex(1.0f, -1.0f, 0.0f, 0, 0, 0, video::SColor(0), 1.0f, 1.0f);
      m_Vertices[4] = video::S3DVertex(-1.0f, -1.0f, 0.0f, 0, 0, 0, video::SColor(0), 0.0f, 1.0f);
      m_Vertices[5] = video::S3DVertex(1.0f, 1.0f, 0.0f, 0, 0, 0, video::SColor(0), 1.0f, 0.0f);

	  vertex_shader_hlsl = ""
		"float screenWidth;\n"
		"float screenHeight;\n"

		"struct VS_OUTPUT\n"
		"{\n"
		"   float4 position   : POSITION;\n"
		"   float2 texCoord   : TEXCOORD0;\n"
		"};\n"

		"VS_OUTPUT vertexMain(in float4 position : POSITION,\n"
		"					 in float2 texCoord : TEXCOORD0)\n"
		"{\n"
		"   VS_OUTPUT Out;\n"
		"   position.xy    = sign(position.xy);\n"
		"   Out.position   = float4(position.xy, 0.0f, 1.0);\n"
		"   Out.texCoord.x = 0.5 * (1.0 + position.x + (1.0 / screenWidth));\n"
		"   Out.texCoord.y = 0.5 * (1.0 - position.y + (1.0 / screenHeight));\n"
		"   return Out; \n"
		"}\n";
		
	pixel_shader_1_hlsl = ""
		"sampler texture1 : register(s0);\n"
		"sampler texture2 : register(s1);\n"

		"float strength;\n"

		"float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR\n"
		"{\n"
		"   float4 tex1 = tex2D(texture1, texCoord);\n"
		"   float4 tex2 = tex2D(texture2, texCoord);\n"
		   
		"   return lerp(tex1, tex2, strength);\n"
		"}\n";
		
	pixel_shader_2_hlsl = ""
		"sampler texture1 : register(s0);\n"

		"float4 pixelMain(float2 texCoord : TEXCOORD0) : COLOR\n"
		"{\n"
		"   return tex2D(texture1, texCoord);\n"
		"}\n";

    vertex_shader_glsl =
    "varying vec2 vTexCoord;"
    "void main(void)"
    "{"
    " vec2 Position;"
    " Position.xy = sign(gl_Vertex.xy);"
    " gl_Position = vec4(Position.xy, 0.0, 1.0);"
    "vTexCoord =Position.xy *.5 + .5;"
    "}";
    
    pixel_shader_1_glsl =
    "uniform sampler2D texture1;"
    "uniform sampler2D texture2;"
    "varying vec2 vTexCoord;"
    "uniform float strength;"
    "void main()"
    "{"
    "  gl_FragColor = mix( texture2D( texture1, vTexCoord ), texture2D( texture2, vTexCoord ), vec4( strength,strength,strength,strength) );"
    "}";
    
    
    pixel_shader_2_glsl =
    "uniform sampler2D texture1;"
    "varying vec2 vTexCoord;"
    "void main()"
    "{"
    "  gl_FragColor =texture2D( texture1, vTexCoord );"
    "}";

   }

   ~IPostProcessMotionBlur()
   {
      m_rtPrev->drop();
      m_rtNext->drop();
      m_rtAccum->drop();
   }

   CMotionBlurCallback *getCallback() { return callback; }

   void initiate(u32 sizeW, u32 sizeH, float strength)
   {
      video::IVideoDriver* driver = SceneManager->getVideoDriver();

      video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

      callback = new CMotionBlurCallback((float)sizeW, (float)sizeH, strength);

      m_Material1.MaterialType = (video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(
		 (driver->getDriverType() == video::EDT_OPENGL ? vertex_shader_glsl.c_str() : vertex_shader_hlsl.c_str()), "vertexMain", video::EVST_VS_1_1,
		 (driver->getDriverType() == video::EDT_OPENGL ? pixel_shader_1_glsl.c_str() : pixel_shader_1_hlsl.c_str()), "pixelMain", video::EPST_PS_1_4,
         callback, video::EMT_SOLID);

	  m_Material2.MaterialType = (video::E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial(
		  (driver->getDriverType() == video::EDT_OPENGL ? vertex_shader_glsl.c_str() : vertex_shader_hlsl.c_str()), "vertexMain", video::EVST_VS_1_1,
		  (driver->getDriverType() == video::EDT_OPENGL ? pixel_shader_2_glsl.c_str() : pixel_shader_2_hlsl.c_str()), "pixelMain", video::EPST_PS_1_1,
         0, video::EMT_SOLID);

      m_rtNext  = driver->addRenderTargetTexture(irr::core::dimension2d<u32>(sizeW, sizeH));
      m_rtPrev  = driver->addRenderTargetTexture(irr::core::dimension2d<u32>(sizeW, sizeH));
      m_rtAccum = driver->addRenderTargetTexture(irr::core::dimension2d<u32>(sizeW, sizeH));

	  m_Material1.TextureLayer[0].Texture = m_rtNext;
      m_Material1.TextureLayer[1].Texture = m_rtPrev;

	  m_Material2.TextureLayer[0].Texture = m_rtAccum;

      callback->drop();
   }

   void render(CScreenQuad *_quad) {
	   this->render();
   }

   virtual void render()
	{
		u16 indices[] = {0, 1, 2, 3, 4, 5};
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		driver->setRenderTarget(m_rtNext, true, true, 0);
		SceneManager->drawAll();

		driver->setRenderTarget(m_rtAccum, true, true, 0);
		driver->setMaterial(m_Material1);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawIndexedTriangleList(&m_Vertices[0], 6, &indices[0], 2);

		driver->setRenderTarget(m_rtPrev, true, true, 0);
		driver->setMaterial(m_Material2);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawIndexedTriangleList(&m_Vertices[0], 6, &indices[0], 2);
	}
	virtual void renderFinal()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();
		u16 indices[] = {0, 1, 2, 3, 4, 5};
		driver->setMaterial(m_Material2);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawIndexedTriangleList(&m_Vertices[0], 6, &indices[0], 2);
	}
	virtual video::SMaterial& getMaterial(s32 i)
	{
		return m_Material1;
	}
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_BBox;
	}
};

#endif
