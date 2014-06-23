#ifndef __I_SSWE_HDR_MANAGER_H_INCLUDED__
#define __I_SSWE_HDR_MANAGER_H_INCLUDED__

#include <irrlicht.h>

class IHDRManager {

public:

	/// Brightness Threshold
	virtual void setBrightnessThreshold(irr::f32 value) = 0;
	virtual irr::f32 getBrightnessThreshold() = 0;

	/// Gaussian Coefficient
	virtual void setGaussianCoefficient(irr::f32 value) = 0;
	virtual irr::f32 getGaussianCoefficient() = 0;

	/// Gaussian Mean
	virtual void setGaussianMean(irr::f32 value) = 0;
	virtual irr::f32 getGaussianMean() = 0;

	/// Gaussian Standard Deviation
	virtual void setGaussianStandardDeviation(irr::f32 value) = 0;
	virtual irr::f32 getGaussianStandardDeviation() = 0;

	/// Exposure
	virtual void setExposure(irr::f32 value) = 0;
	virtual irr::f32 getExprosure() = 0;

	/// Luminance Min
	virtual void setMinimumLuminance(irr::f32 value) = 0;
	virtual irr::f32 getMinimumLuminance() = 0;
	
	/// Luminance Max
	virtual void setMaximumLuminance(irr::f32 value) = 0;
	virtual irr::f32 getMaximumLuminance() = 0;

	/// Luminance Increase Rate
	virtual void setLuminanceIncreaseRate(irr::f32 value) = 0;
	virtual irr::f32 getLuminanceIncreaseRate() = 0;

	/// Luminance Increase Rate
	virtual void setLuminanceDecreaseRate(irr::f32 value) = 0;
	virtual irr::f32 getLumnanceDecreaseRate() = 0;

};


#endif