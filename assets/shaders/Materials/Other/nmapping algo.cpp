#include <irrlicht.h>

// --------------------------------------------------------------------------

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



IrrlichtDevice *device;


const stringc vertBumpShader =
"uniform vec3 fvLightPosition1;"
"uniform vec3 fvLightPosition2;"
"uniform vec3 fvLightPosition3;"
"uniform vec3 fvLightPosition4;"
""
"uniform float fLightStrength1;"
"uniform float fLightStrength2;"
"uniform float fLightStrength3;"
"uniform float fLightStrength4;"
""
"uniform mat4 matWorldInverse;"
""
"varying vec2 Texcoord;"
"varying vec3 ViewDirection;"
"varying vec3 LightDirection1;"
"varying vec3 LightDirection2;"
"varying vec3 LightDirection3;"
"varying vec3 LightDirection4;"
"varying vec4 LightDistMultiplier;"
""   
"float getLengthSQR (vec3 vec)"
"{"
"return(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);"
"}"  
""   
"void main( void )"
"{"
""   
"   mat4 LightTransform= gl_ModelViewMatrix;"
"   LightTransform=LightTransform*matWorldInverse;"
""      
"   gl_Position = ftransform();"
"   Texcoord    = gl_MultiTexCoord0.xy;"
""    
"   vec4 fvObjectPosition = gl_ModelViewMatrix * gl_Vertex;"
"   vec4 fvLightPos1 = LightTransform * vec4(fvLightPosition1,1.0);"
"   vec4 fvLightPos2 = LightTransform * vec4(fvLightPosition2,1.0);"
"   vec4 fvLightPos3 = LightTransform * vec4(fvLightPosition3,1.0);"
"   vec4 fvLightPos4 = LightTransform * vec4(fvLightPosition4,1.0);"
""   
"   vec3 fvViewDirection  =  - fvObjectPosition.xyz;"
""   
"   vec3 fvLightDirection1 = (fvLightPos1.xyz - fvObjectPosition.xyz);"
"   vec3 fvLightDirection2 = (fvLightPos2.xyz - fvObjectPosition.xyz);"
"   vec3 fvLightDirection3 = (fvLightPos3.xyz - fvObjectPosition.xyz);"
"   vec3 fvLightDirection4 = (fvLightPos4.xyz - fvObjectPosition.xyz);"
""   
"   LightDistMultiplier[0]=1.0/(getLengthSQR (fvLightDirection1)/(fLightStrength1*10000.0));"
"   LightDistMultiplier[1]=1.0/(getLengthSQR (fvLightDirection2)/(fLightStrength2*10000.0));"
"   LightDistMultiplier[2]=1.0/(getLengthSQR (fvLightDirection3)/(fLightStrength3*10000.0));"
"   LightDistMultiplier[3]=1.0/(getLengthSQR (fvLightDirection4)/(fLightStrength4*10000.0));"
""     
"   vec3 fvNormal         = gl_NormalMatrix * gl_Normal;"
""   
"   vec3 fvTangent   = -vec3(abs(gl_Normal.y) + abs(gl_Normal.z), abs(gl_Normal.x), 0);"
"   vec3 fvBinormal =cross(fvTangent,gl_Normal);  "
"   fvTangent=gl_NormalMatrix*cross(fvBinormal,gl_Normal);"
"   fvBinormal=gl_NormalMatrix*fvBinormal;"
"" 
""      
"   ViewDirection.x  = dot( fvTangent, fvViewDirection );"
"   ViewDirection.y  = dot( fvBinormal, fvViewDirection );"
"   ViewDirection.z  = dot( fvNormal, fvViewDirection );"
""   
"   LightDirection1.x  = dot( fvTangent, fvLightDirection1.xyz );"
"   LightDirection1.y  = dot( fvBinormal, fvLightDirection1.xyz );"
"   LightDirection1.z  = dot( fvNormal, fvLightDirection1.xyz );"
""   
"   LightDirection2.x  = dot( fvTangent, fvLightDirection2.xyz );"
"   LightDirection2.y  = dot( fvBinormal, fvLightDirection2.xyz );"
"   LightDirection2.z  = dot( fvNormal, fvLightDirection2.xyz );"
""   
"   LightDirection3.x  = dot( fvTangent, fvLightDirection3.xyz );"
"   LightDirection3.y  = dot( fvBinormal, fvLightDirection3.xyz );"
"   LightDirection3.z  = dot( fvNormal, fvLightDirection3.xyz );"
""   
"   LightDirection4.x  = dot( fvTangent, fvLightDirection4.xyz );"
"   LightDirection4.y  = dot( fvBinormal, fvLightDirection4.xyz );"
"   LightDirection4.z  = dot( fvNormal, fvLightDirection4.xyz );"
""   
"}";
// --------------------------------------------------------------------------

// OpenGL Fragment Program 1.1
const stringc fragBumpShader =
"uniform vec4 fvAmbient;"
"uniform vec4 fvLight1Color;"
"uniform vec4 fvLight2Color;"
"uniform vec4 fvLight3Color;"
"uniform vec4 fvLight4Color;"

"uniform float fSpecularPower;"
"uniform float fSpecularStrength;"
"uniform float fBumpStrength;"

"uniform sampler2D baseMap;"
"uniform sampler2D bumpMap;"

"varying vec2 Texcoord;"
"varying vec3 ViewDirection;"
"varying vec3 LightDirection1;"
"varying vec3 LightDirection2;"
"varying vec3 LightDirection3;"
"varying vec3 LightDirection4;"
"varying vec4 LightDistMultiplier;"

"void main( void )"
"{"
   
"   vec3  fvLightDirection1 = normalize( LightDirection1 );"
"   vec3  fvLightDirection2 = normalize( LightDirection2 );"
"   vec3  fvLightDirection3 = normalize( LightDirection3 );"
"   vec3  fvLightDirection4 = normalize( LightDirection4 );"
   
"   vec3  fvNormal         = texture2D( bumpMap, Texcoord ).yxz;"
"   fvNormal.xy*=2.0;"
"   fvNormal.xy-=1.0;"
   
"   fvNormal=(vec3(0.0,0.0,1.0)-fvNormal)*fBumpStrength+fvNormal;"
   
"   fvNormal=normalize(fvNormal);"
   
"   float fNDotL1           = max(dot(fvNormal, fvLightDirection1),0.0)-0.1; "
"   float fNDotL2           = max(dot(fvNormal, fvLightDirection2),0.0)-0.1; "
"   float fNDotL3           = max(dot(fvNormal, fvLightDirection3),0.0)-0.1; "
"   float fNDotL4           = max(dot(fvNormal, fvLightDirection4),0.0)-0.1; "
   
"   vec3  fvReflection1     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection1 ); "
"   vec3  fvReflection2     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection2 ); "
"   vec3  fvReflection3     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection3 ); "
"   vec3  fvReflection4     = normalize( ( ( 2.0 * fvNormal )  ) - fvLightDirection4 ); "
   
"   vec3  fvViewDirection  = normalize( ViewDirection );"
   
"   float fRDotV1          = max( 0.0, dot( fvReflection1, fvViewDirection ) );"
"   float fRDotV2          = max( 0.0, dot( fvReflection2, fvViewDirection ) );"
"   float fRDotV3          = max( 0.0, dot( fvReflection3, fvViewDirection ) );"
"   float fRDotV4          = max( 0.0, dot( fvReflection4, fvViewDirection ) );"
   
"   vec4  fvBaseColor      = texture2D( baseMap, Texcoord );"
   
"   vec4  fvTotalAmbient   = fvAmbient * fvBaseColor; "
   
"   vec4  fvTotalDiffuse   = fvLight1Color * fNDotL1* fvBaseColor*LightDistMultiplier[0]; "
"   vec4  fvTotalSpecular  = fNDotL1*fvLight1Color * ( pow( fRDotV1, fSpecularPower ) )*LightDistMultiplier[0];"
   
"         fvTotalDiffuse   += fvLight2Color * fNDotL2* fvBaseColor*LightDistMultiplier[1]; "
"         fvTotalSpecular  += fNDotL2*fvLight2Color * ( pow( fRDotV2, fSpecularPower ) )*LightDistMultiplier[1];  "
   
"         fvTotalDiffuse   += fvLight3Color * fNDotL3* fvBaseColor*LightDistMultiplier[2]; "
"         fvTotalSpecular  += fNDotL3*fvLight3Color * ( pow( fRDotV3, fSpecularPower ) )*LightDistMultiplier[2];  "
         
"         fvTotalDiffuse   += fvLight4Color * fNDotL4* fvBaseColor*LightDistMultiplier[3]; "
"         fvTotalSpecular  += fNDotL4*fvLight4Color * ( pow( fRDotV4, fSpecularPower ) )*LightDistMultiplier[3]; "
   
"   vec4 color=( fvTotalAmbient + fvTotalDiffuse+ (fvTotalSpecular*fSpecularStrength));"
"   if(color.r>1.0){color.gb+=color.r-1.0;}"
"   if(color.g>1.0){color.rb+=color.g-1.0;}"
"   if(color.b>1.0){color.rg+=color.b-1.0;}"
"   gl_FragColor = color;"

"}"
;

class Shader_Bump_callback: public video::IShaderConstantSetCallBack
{
public:
    float fLightStrength[4]; //you know I=1/r²?? i changed it to I=1/(r²/fLightStrength) for better results
    vector3df fvLightPosition[4]; //the light positions
    SColorf fvLightColor[4]; 
    SColorf fvAmbient;      
    float fSpecularPower;   // S=pow(fDiffuseIntensity,fSpecularPower)*fSpecularStrength; 
    float fSpecularStrength;// will result in less glossing surfaces
    float fBumpStrength;    //strength of the bumpmapping.. higher values will result in more "bumpy" surfaces 
    
  virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
  {

    int var0=0;
    services->setPixelShaderConstant("baseMap", (float*)(&var0), 1); //the colormap
    int var1=1;
    services->setPixelShaderConstant("bumpMap", (float*)(&var1), 1); //the normalmap
    
    core::matrix4 invWorld = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    invWorld.makeInverse();
    services->setPixelShaderConstant("matWorldInverse", (float*)(&invWorld), 16);
    
    services->setPixelShaderConstant("fLightStrength1", (float*)(&fLightStrength[0]), 1);
    services->setPixelShaderConstant("fLightStrength2", (float*)(&fLightStrength[1]), 1);
    services->setPixelShaderConstant("fLightStrength3", (float*)(&fLightStrength[2]), 1);
    services->setPixelShaderConstant("fLightStrength4", (float*)(&fLightStrength[3]), 1);
    
    services->setPixelShaderConstant("fvLightPosition1", (float*)(&fvLightPosition[0]), 3);
    services->setPixelShaderConstant("fvLightPosition2", (float*)(&fvLightPosition[1]), 3);
    services->setPixelShaderConstant("fvLightPosition3", (float*)(&fvLightPosition[2]), 3);
    services->setPixelShaderConstant("fvLightPosition4", (float*)(&fvLightPosition[3]), 3);
    
    services->setPixelShaderConstant("fvAmbient", (float*)(&fvAmbient), 4);
    services->setPixelShaderConstant("fvLight1Color", (float*)(&fvLightColor[0]), 4);
    services->setPixelShaderConstant("fvLight2Color", (float*)(&fvLightColor[1]), 4);
    services->setPixelShaderConstant("fvLight3Color", (float*)(&fvLightColor[2]), 4);
    services->setPixelShaderConstant("fvLight4Color", (float*)(&fvLightColor[3]), 4);

    services->setPixelShaderConstant("fSpecularPower", (float*)(&fSpecularPower), 1);
    services->setPixelShaderConstant("fSpecularStrength", (float*)(&fSpecularStrength), 1);
    services->setPixelShaderConstant("fBumpStrength", (float*)(&fBumpStrength), 1);


   }
};




// --------------------------------------------------------------------------

#pragma comment(lib, "Irrlicht.lib")


int main()
{
   // Create device
   video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

   device = createDevice(driverType, dimension2di(800, 600), 32, false, false, false, 0);
   if (!device) {
      printf("Error creating Irrlicht device\n");
      return 0;
   }

   // Obtain device internals
   IVideoDriver* driver = device->getVideoDriver();
   ISceneManager* smgr = device->getSceneManager();
   // Set a window caption
   device->setWindowCaption(L"NormalMapShader - Irrlicht Engine Demo");

   // Create GLSL shaders
   video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
   s32 mtlToonShader = video::EMT_SOLID; // Fallback material type
   bool bCanDoGLSL_1_1 = false;
   if (gpu && (driverType == video::EDT_OPENGL)) {
      bCanDoGLSL_1_1 = true; // provisionally accept
      if (!driver->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1)) {
         printf("queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) failed\n");
         bCanDoGLSL_1_1 = false;
      }
      if (!driver->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1)) {
         printf("queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) failed\n");
         bCanDoGLSL_1_1 = false;
      }
   }
    
   if (bCanDoGLSL_1_1) {
   Shader_Bump_callback *callback= new Shader_Bump_callback;
   
   callback->fBumpStrength=4;
   callback->fSpecularStrength=0.9;
   callback->fSpecularPower=20;
   
   callback->fvAmbient=SColorf(0.02,0.02,0.02);
   
   callback->fLightStrength[0]=1;
   callback->fvLightColor[0]=SColorf(1.00,0.52,0.31); 
   callback->fvLightPosition[0]=vector3df(100,-10,0);
    
   callback->fLightStrength[1]=1;
   callback->fvLightColor[1]=SColorf(0.31,0.55,1.0);
   callback->fvLightPosition[1]=vector3df(-100,10,0); 
   
    
 mtlToonShader = gpu->addHighLevelShaderMaterial(
               vertBumpShader.c_str(), "main", video::EVST_VS_1_1,
               fragBumpShader.c_str(), "main", video::EPST_PS_1_1,
               callback, video::EMT_SOLID);
   } else {
      // This demo is for OpenGL!
      printf("This demo requires OpenGL with GLSL High-Level shaders\n");
      mtlToonShader = video::EMT_SOLID;
   }

   // Add an animated mesh
   IAnimatedMesh* mesh = smgr->getMesh("dwarf.x");


   ISceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
   if (node)
   {
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setMaterialTexture( 0, driver->getTexture("Fieldstone.tga") );
      node->setMaterialTexture( 1, driver->getTexture("Fieldstonebumpdot3.tga") );
      node->setMaterialType((video::E_MATERIAL_TYPE)mtlToonShader); // Override material type
   }

   // Add a viewing camera
   smgr->addCameraSceneNodeFPS();

   // Main rendering loop
   while(device->run())
   {
        vector3df rot=node->getRotation();
        rot.Y+=0.1;
        node->setRotation(rot);
      driver->beginScene(true, true, SColor(0,0,0,0));
      smgr->drawAll();
      driver->endScene();
   }

   device->drop();

   // Done
   return 0;
}
