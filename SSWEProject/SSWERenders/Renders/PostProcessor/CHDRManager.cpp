#include "stdafx.h"
#include "CHDRManager.h"

#include "../HDR/CHDRPostProcess.h"
#include "../HDR/CHDRScreenQuad.h"
#include "../HDR/CPhongShaderManager.h"
#include "../HDR/CGlobalContext.h"
#include "../HDR/CPostProcessingManager.h"
#include "../HDR/CDSBloomGenerator.h"
#include "../HDR/CLuminanceGenerator.h"
#include "../HDR/CAmplifier.h"

CHDRManager::CHDRManager(Graphics::HDRPostProcess *instance, irr::video::SMaterial *phongInstance) {
	pp = instance;

	phong = *phongInstance;
}

CHDRManager::~CHDRManager() {

}

//-----------------------------------
/// Brightness Threshold
void CHDRManager::setBrightnessThreshold(irr::f32 value) {
	pp->GetBloomGenerator()->SetBrightnessThreshold(value);
}

irr::f32 CHDRManager::getBrightnessThreshold() {
	return pp->GetBloomGenerator()->GetBrightessThreshold();
}
//-----------------------------------

//-----------------------------------
/// Gaussian Coefficient
void CHDRManager::setGaussianCoefficient(irr::f32 value) {
	pp->GetBloomGenerator()->SetGaussianCoefficient(value);
}

irr::f32 CHDRManager::getGaussianCoefficient() {
	return pp->GetBloomGenerator()->GetGaussianCoefficient();
}
//-----------------------------------

//-----------------------------------
/// Gaussian Mean
void CHDRManager::setGaussianMean(irr::f32 value) {
	pp->GetBloomGenerator()->SetGaussianMean(value);
}

irr::f32 CHDRManager::getGaussianMean() {
	return pp->GetBloomGenerator()->GetGaussianMean();
}
//-----------------------------------

//-----------------------------------
/// Gaussian Standard Deviation
void CHDRManager::setGaussianStandardDeviation(irr::f32 value) {
	pp->GetBloomGenerator()->SetGaussianStandardDeviation(value);
}

irr::f32 CHDRManager::getGaussianStandardDeviation() {
	return pp->GetBloomGenerator()->GetGaussianStandardDeviation();
}
//-----------------------------------

//-----------------------------------
/// Exposure
void CHDRManager::setExposure(irr::f32 value) {
	pp->SetExposure(value);
}

irr::f32 CHDRManager::getExprosure() {
	return pp->GetExposure();
}
//-----------------------------------

//-----------------------------------
/// Luminance Min
void CHDRManager::setMinimumLuminance(f32 value) {
	pp->SetMinimumLuminance(value);
}

irr::f32 CHDRManager::getMinimumLuminance() {
	return pp->GetMinimumLuminance();
}
//-----------------------------------

//-----------------------------------
/// Luminance Max
void CHDRManager::setMaximumLuminance(f32 value) {
	pp->SetMaximumLuminance(value);
}

irr::f32 CHDRManager::getMaximumLuminance() {
	return pp->GetMaximumLuminance();
}
//-----------------------------------

//-----------------------------------
/// Luminance Increase Rate
void CHDRManager::setLuminanceIncreaseRate(irr::f32 value) {
	pp->SetLuminanceIncreaseRate(value);
}

irr::f32 CHDRManager::getLuminanceIncreaseRate() {
	return pp->GetLuminanceIncreaseRate();
}
//-----------------------------------

//-----------------------------------
/// Luminance Increase Rate
void CHDRManager::setLuminanceDecreaseRate(f32 value) {
	pp->SetLuminanceDecreaseRate(value);
}

irr::f32 CHDRManager::getLumnanceDecreaseRate() {
	return pp->GetLumnanceDecreaseRate();
}
//-----------------------------------
