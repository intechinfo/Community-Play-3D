#include "stdafx.h"

#include "CPSSMUtils.h"

CPSSMUtils::CPSSMUtils(ICameraSceneNode* vLightCamera, int vSplitsCount, float vSplitLambda) : SplitLambda(vSplitLambda), LightCamera(0),
            SplitsCount(0), SplitDistance(0), Near(0), Far(0), Current(0)
{
    for(int i = 0; i < 4; i++)
    {
        FrustumCorner[i] = vector3df(0,0,0);
        mP[i] = 0;
        mV[i] = 0;
    }

    setLightCamera(vLightCamera);

    if(vSplitsCount > 0)
    {
        if(vSplitsCount > 4)
        SplitsCount = 4;
        else
        SplitsCount = vSplitsCount;
    }
    else
    SplitsCount = 1;
}

void CPSSMUtils::CalculateSplitDistances(ICameraSceneNode* vCamera)
{
    delete[] SplitDistance;
    SplitDistance = new float[SplitsCount + 1];

    float vNear = vCamera->getNearValue();
    float vFar = vCamera->getFarValue();

    SplitLambda = Clamp(SplitLambda, 0.0f, 1.0f);

    for(int i = 0; i < SplitsCount; i++)
    {
        float Param = i / (float)SplitsCount;
        float Log = vNear * powf((vFar / vNear), Param);
        float Uniform = vNear + (vFar - vNear) * Param;
        SplitDistance[i] = Log * SplitLambda + Uniform * (1.0 - SplitLambda);
    }

    SplitDistance[0] = vNear;
    SplitDistance[SplitsCount] = vFar;
}

void CPSSMUtils::CalculateFrustumCorners(ICameraSceneNode* vCamera, int vCurrent, float vScale)
{
    if(vCurrent < 0 || vCurrent > 3)
    vCurrent = 0;

    Current = vCurrent;

    vector3df vSource = vCamera->getPosition();
    vector3df vTarget = vCamera->getTarget();
    vector3df vUp = vCamera->getUpVector();
    float vFOV = vCamera->getFOV();
    float vAspect = vCamera->getAspectRatio();
    float vNear = SplitDistance[Current];
    float vFar = SplitDistance[Current + 1];

    vector3df vZ = vTarget - vSource;
    vZ.normalize();

    vector3df vX = vUp.crossProduct(vZ);
    vX.normalize();

    vector3df vY = vZ.crossProduct(vX);

    float NearPlaneHeight = tanf(vFOV * 0.5f) * vNear;
    float NearPlaneWidth = NearPlaneHeight * vAspect;

    float FarPlaneHeight = tanf(vFOV * 0.5f) * vFar;
    float FarPlaneWidth = FarPlaneHeight * vAspect;

    vector3df NearPlaneCenter = vSource + vZ * vNear;
    vector3df FarPlaneCenter = vSource + vZ * vFar;

    FrustumCorner[0] = vector3df(NearPlaneCenter - vX*NearPlaneWidth - vY*NearPlaneHeight);
    FrustumCorner[1] = vector3df(NearPlaneCenter - vX*NearPlaneWidth + vY*NearPlaneHeight);
    FrustumCorner[2] = vector3df(NearPlaneCenter + vX*NearPlaneWidth + vY*NearPlaneHeight);
    FrustumCorner[3] = vector3df(NearPlaneCenter + vX*NearPlaneWidth - vY*NearPlaneHeight);

    FrustumCorner[4] = vector3df(FarPlaneCenter - vX*FarPlaneWidth - vY*FarPlaneHeight);
    FrustumCorner[5] = vector3df(FarPlaneCenter - vX*FarPlaneWidth + vY*FarPlaneHeight);
    FrustumCorner[6] = vector3df(FarPlaneCenter + vX*FarPlaneWidth + vY*FarPlaneHeight);
    FrustumCorner[7] = vector3df(FarPlaneCenter + vX*FarPlaneWidth - vY*FarPlaneHeight);

    vector3df vCenter(0,0,0);

    for(int i = 0; i < 8; i++)
    vCenter += FrustumCorner[i];

    vCenter/=8;

    for(int i = 0; i < 8; i++)
    FrustumCorner[i] += (FrustumCorner[i] - vCenter) * (vScale - 1.0);
}

void CPSSMUtils::CalculateLightForFrustum(int vCurrent)
{
    if(vCurrent < 0 || vCurrent > 3)
    vCurrent = 0;

    Current = vCurrent;

    float MaxX = -FLT_MAX;
    float MaxY = -FLT_MAX;
    float MinX = FLT_MAX;
    float MinY = FLT_MAX;
    float MaxZ = 0;

    LightCamera->setNearValue(Near);
    LightCamera->setFarValue(Far);
    mP[Current].buildProjectionMatrixPerspectiveFovLH(LightCamera->getFOV(),1.0,LightCamera->getNearValue(),LightCamera->getFarValue());
    mV[Current] = LightCamera->getViewMatrix();

    matrix4 mLVP = mP[Current];
    mLVP *= mV[Current];

    for(int i = 0; i < 8; i++)
    {
        float X = FrustumCorner[i].X * mLVP[0] + FrustumCorner[i].Y * mLVP[4] + FrustumCorner[i].Z * mLVP[8] + mLVP[12];
        float Y = FrustumCorner[i].X * mLVP[1] + FrustumCorner[i].Y * mLVP[5] + FrustumCorner[i].Z * mLVP[9] + mLVP[13];
        float Z = FrustumCorner[i].X * mLVP[2] + FrustumCorner[i].Y * mLVP[6] + FrustumCorner[i].Z * mLVP[10] + mLVP[14];
        float W = FrustumCorner[i].X * mLVP[3] + FrustumCorner[i].Y * mLVP[7] + FrustumCorner[i].Z * mLVP[11] + mLVP[15];

        X /= W;
        Y /= W;

        if(X > MaxX)
        MaxX = X;

        if(Y > MaxY)
        MaxY = Y;

        if(X < MinX)
        MinX = X;

        if(Y < MinY)
        MinY = Y;

        if(Z > MaxZ)
        MaxZ = Z;
    }

    MaxX = Clamp(MaxX, -1.0f, 1.0f);
    MaxY = Clamp(MaxY, -1.0f, 1.0f);
    MinX = Clamp(MinX, -1.0f, 1.0f);
    MinY = Clamp(MinY, -1.0f, 1.0f);

    float nFar = MaxZ + Near + 1.5f;

    LightCamera->setNearValue(Near);
    LightCamera->setFarValue(Far);
    mP[Current].buildProjectionMatrixPerspectiveFovLH(LightCamera->getFOV(),1.0,LightCamera->getNearValue(),LightCamera->getFarValue());
    mV[Current] = LightCamera->getViewMatrix();

    float ScaleX = 2.0f / (MaxX - MinX);
    float ScaleY = 2.0f / (MaxY - MinY);

    float OffsetX = -0.5f * (MaxX + MinX) * ScaleX;
    float OffsetY = -0.5f * (MaxY + MinY) * ScaleY;

    matrix4 mCropView;
    mCropView[0] = ScaleX;
    mCropView[1] = 0.0f;
    mCropView[2] = 0.0f;
    mCropView[3] = 0.0f;

    mCropView[4] = 0.0f;
    mCropView[5] = ScaleY;
    mCropView[6] = 0.0f;
    mCropView[7] = 0.0f;

    mCropView[8] = 0.0f;
    mCropView[9] = 0.0f;
    mCropView[10] = 1.0f;
    mCropView[11] = 0.0f;

    mCropView[12] = OffsetX;
    mCropView[13] = OffsetY;
    mCropView[14] = 0.0f;
    mCropView[15] = 1.0f;

    mP[Current] = mCropView * mP[Current];

    mP[Current][10] /= nFar;
    mP[Current][14] /= nFar;
}

ICameraSceneNode* CPSSMUtils::getLightCamera()
{
    return LightCamera;
}

void CPSSMUtils::setLightCamera(ICameraSceneNode* vLightCamera)
{
    if(vLightCamera)
    {
        LightCamera = vLightCamera;
        Near = LightCamera->getNearValue();
        Far = LightCamera->getFarValue();
    }
}

matrix4& CPSSMUtils::getProjectionMatrix(int vCurrent)
{
    return mP[vCurrent];
}

matrix4& CPSSMUtils::getViewMatrix(int vCurrent)
{
    return mV[vCurrent];
}

int CPSSMUtils::getSplitsCount()
{
    return SplitsCount;
}

void CPSSMUtils::setSplitsCount(int vSplitsCount)
{
    if(vSplitsCount > 0)
    {
        if(vSplitsCount > 4)
        SplitsCount = 4;
        else
        SplitsCount = vSplitsCount;
    }
}

float CPSSMUtils::getSplitLambda()
{
    return SplitLambda;
}

void CPSSMUtils::setSplitLambda(float vSplitLambda)
{
    SplitLambda = vSplitLambda;
}

float CPSSMUtils::getSplitDistance(int vCurrent)
{
    return SplitDistance[vCurrent];
}

int CPSSMUtils::getCurrentPass()
{
    return Current;
}

float CPSSMUtils::Clamp(float A, float Min, float Max)
{
    if(A < Min)
    return Min;

    if(A > Max)
    return Max;

    return A;
}
