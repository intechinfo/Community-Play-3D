#ifndef __I_HDR_MANAGER_H_INCLUDED__
#define __I_HDR_MANAGER_H_INCLUDED__

#include <SSWERenders.h>

#include "../HDR/CHDRScreenQuad.h"
#include "../HDR/CPhongShaderManager.h"
#include "../HDR/CGlobalContext.h"
#include "../HDR/CPostProcessingManager.h"
#include "../HDR/CHDRPostProcess.h"
#include "../HDR/CDSBloomGenerator.h"
#include "../HDR/CLuminanceGenerator.h"
#include "../HDR/CAmplifier.h"

#include "../../../SSWELib/include/ISSWEHDRManager.h"

class CHDRManager : public IHDRManager {

public:

	CHDRManager(Graphics::HDRPostProcess *instance, irr::video::SMaterial *phongInstance);
	~CHDRManager();

	irr::video::SMaterial getPhongMaterial() { return phong; }

	//-----------------------------------
	/// Brightness Threshold
	void setBrightnessThreshold(irr::f32 value);
	irr::f32 getBrightnessThreshold();
	//-----------------------------------

	//-----------------------------------
	/// Gaussian Coefficient
	void setGaussianCoefficient(irr::f32 value);
	irr::f32 getGaussianCoefficient();
	//-----------------------------------

	//-----------------------------------
	/// Gaussian Mean
	void setGaussianMean(irr::f32 value);
	irr::f32 getGaussianMean();
	//-----------------------------------

	//-----------------------------------
	/// Gaussian Standard Deviation
	void setGaussianStandardDeviation(irr::f32 value);
	irr::f32 getGaussianStandardDeviation();
	//-----------------------------------

	//-----------------------------------
	/// Exposure
	void setExposure(irr::f32 value);
	irr::f32 getExprosure();
	//-----------------------------------

	//-----------------------------------
	/// Luminance Min
	void setMinimumLuminance(f32 value);
	irr::f32 getMinimumLuminance();
	//-----------------------------------
	
	//-----------------------------------
	/// Luminance Max
	void setMaximumLuminance(f32 value);
	irr::f32 getMaximumLuminance();
	//-----------------------------------

	//-----------------------------------
	/// Luminance Increase Rate
	void setLuminanceIncreaseRate(irr::f32 value);
	irr::f32 getLuminanceIncreaseRate();
	//-----------------------------------

	//-----------------------------------
	/// Luminance Increase Rate
	void setLuminanceDecreaseRate(f32 value);
	irr::f32 getLumnanceDecreaseRate();
	//-----------------------------------

private:

	//-----------------------------------
	//DATA
	Graphics::HDRPostProcess *pp;

	irr::video::SMaterial phong;
	//-----------------------------------

};


#endif
