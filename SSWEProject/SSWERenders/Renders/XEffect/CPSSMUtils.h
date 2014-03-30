#ifndef _PSSM_UTILS_H_
#define _PSSM_UTILS_H_

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;

class CPSSMUtils
{
public:
    CPSSMUtils(ICameraSceneNode* vLightCamera, int vSplitsCount, float vSplitLambda = 0.5);
    ~CPSSMUtils() {};

    void CalculateSplitDistances(ICameraSceneNode* vCamera);
    void CalculateFrustumCorners(ICameraSceneNode* vCamera, int vCurrent, float vScale);
    void CalculateLightForFrustum(int vCurrent);

    ICameraSceneNode* getLightCamera();
    void setLightCamera(ICameraSceneNode* vLightCamera);

    matrix4& getProjectionMatrix(int vCurrent);
    matrix4& getViewMatrix(int vCurrent);

    int getSplitsCount();
    void setSplitsCount(int vSplitsCount);

    float getSplitLambda();
    void setSplitLambda(float vSplitLambda);

    float getSplitDistance(int vCurrent);
    int getCurrentPass();

private:
    float Clamp(float A, float Min, float Max);

    ICameraSceneNode* LightCamera;

    int SplitsCount;
    float SplitLambda;
    float *SplitDistance;

    vector3df FrustumCorner[8];
    matrix4 mP[4];
    matrix4 mV[4];

    float Near;
    float Far;

    int Current;
};


#endif
