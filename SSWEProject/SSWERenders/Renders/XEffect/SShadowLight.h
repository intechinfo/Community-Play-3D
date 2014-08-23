#ifndef H_S_SHADOW_LIGHT
#define H_S_SHADOW_LIGHT

#include <SSWERenders.h>
#include <irrlicht.h>

#include <ISSWERender.h>

using namespace cp3d::video;

/// Internal the CP3D, it allow point lights computation.
/// It is a "deffered rendering like", each pass we change the current target of the light
/// With this method the FPS will not fall but will need more than 24 FPS to be fuild.

enum E_SHADOW_LIGHT_CURRENT_PASS {
    ESLCP_FRONT = 0,
    ESLCP_LEFT,
    ESLCP_BACK,
    ESLCP_RIGHT,
    ESLCP_UP,
    ESLCP_DOWN,
    ESLCP_COUNT
};

/// Shadow light constructor. The first parameter is the square shadow map resolution.
/// This should be a power of 2 number, and within reasonable size to achieve optimal
/// performance and quality. Recommended sizes are 512 to 4096 subject to your target
/// hardware and quality requirements. The next two parameters are position and target,
/// the next one is the light color. The next two are very important parameters,
/// the far value and the near value. The higher the near value, and the lower the
/// far value, the better the depth precision of the shadows will be, however it will
/// cover a smaller volume. The next is the FOV, if the light was to be considered
/// a camera, this would be similar to setting the camera's field of view. The last
/// parameter is whether the light is directional or not, if it is, an orthogonal
/// projection matrix will be created instead of a perspective one.
struct SShadowLight : public cp3d::video::IShadowLight
{
public:

	SShadowLight(	irr::u32 shadowMapResolution,
					const irr::core::vector3df& position,
					const irr::core::vector3df& target,
					irr::video::SColorf lightColour = irr::video::SColor(0xffffffff),
					irr::f32 _nearValue = 10.0, irr::f32 farValue = 100.0,
					irr::f32 fov = 90.0 * irr::core::DEGTORAD64, bool directional = false, bool isParent=true)
					:	pos(position), tar(target), farPlane(directional ? 1.0f : farValue), diffuseColour(lightColour),
						mapRes(shadowMapResolution)
	{
		_nearValue = _nearValue <= 0.0f ? 0.1f : _nearValue;
		this->nearValue = _nearValue;
		frontOfView = fov;
		isDirectional = directional;
        isSpot = !directional;

		updateViewMatrix();

		if(directional)
			projMat.buildProjectionMatrixOrthoLH(fov, fov, nearValue, farValue);
		else
			projMat.buildProjectionMatrixPerspectiveFovLH(fov, 1.0f, nearValue, farValue);

		recalculate = true;
		autoRecalculate = false;
		isCamera = false;
		computeVolumetricLight = false;

		this->isParent = isParent;
		if (isParent)
			shadowLights.push_back(SShadowLight(mapRes, pos, tar, diffuseColour, this->nearValue, farPlane, frontOfView, directional, false));

		currentPass = ESLCP_DOWN;
	}

	E_SHADOW_LIGHT_CURRENT_PASS getNextPass() {
		if (currentPass+1 == ESLCP_COUNT)
			currentPass = ESLCP_FRONT;
		else
			currentPass = (E_SHADOW_LIGHT_CURRENT_PASS)(currentPass+1);

		return currentPass;
	}

	E_SHADOW_LIGHT_CURRENT_PASS getCurrentPass() {
		return currentPass;
	}

	/// Sets the light's position.
	void setPosition(const irr::core::vector3df& position) {
		pos = position;
		updateViewMatrix();
		if (!isParent) return;
		if (isDirectional || isSpot) {
			shadowLights[0].setPosition(position);
		} else {
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setPosition(position);
			shadowLights[0].setTarget(irr::core::vector3df(position.X-1.f, position.Y, position.Z));
			shadowLights[1].setTarget(irr::core::vector3df(position.X+1.f, position.Y, position.Z));
			shadowLights[2].setTarget(irr::core::vector3df(position.X, position.Y-1.f, position.Z));
			shadowLights[3].setTarget(irr::core::vector3df(position.X, position.Y+1.f, position.Z));
			shadowLights[4].setTarget(irr::core::vector3df(position.X, position.Y, position.Z-1.f));
			shadowLights[5].setTarget(irr::core::vector3df(position.X, position.Y, position.Z+1.f));
		}
	}
	/// Sets the light's target.
	void setTarget(const irr::core::vector3df& target) {
		tar = target;
		updateViewMatrix();
		if (!isParent) return;
		if (isDirectional || isSpot) {
			shadowLights[0].setTarget(target);
		}
	}

	/// Gets the light's position.
	const irr::core::vector3df& getPosition() const { return pos; }
	const irr::core::vector3df& getTarget()  const { return tar; }

	/// Sets the light's view matrix.
	void setViewMatrix(const irr::core::matrix4& matrix) {
		viewMat = matrix;
		irr::core::matrix4 vInverse;
		viewMat.getInverse(vInverse);
		pos = vInverse.getTranslation();
		if (!isParent) return;
		if (isDirectional || isSpot) {
			shadowLights[0].setViewMatrix(matrix);
		} else {
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setViewMatrix(matrix);
		}
	}

	/// Sets the light's projection matrix.
	void setProjectionMatrix(const irr::core::matrix4& matrix) {
		projMat = matrix;
		if (!isParent) return;
		if (isDirectional || isSpot)
			shadowLights[0].setProjectionMatrix(matrix);
		else
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setProjectionMatrix(matrix);
	}
	irr::core::matrix4& getViewMatrix() { return viewMat; }
	irr::core::matrix4& getProjectionMatrix() { return projMat; }

	/// Gets the light's far value.
	irr::f32 getFarValue() const { return farPlane; }
    void setFarValue(const irr::f32 farPlane) {
		this->farPlane = farPlane;
		recalculate = true;
		if (!isParent) return;
		if (isDirectional || isSpot) {
			shadowLights[0].setFarValue(farPlane);
		} else {
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setFarValue(farPlane);
		}
	}

	/// Gets the near value
	irr::f32 getNearValue() const { return nearValue; }
	void setNearValue(const irr::f32 nearValue) {
		this->nearValue = nearValue;
		if (!isParent) return;
		if(isDirectional)
			projMat.buildProjectionMatrixOrthoLH(frontOfView, frontOfView, nearValue, farPlane);
		else
			projMat.buildProjectionMatrixPerspectiveFovLH(frontOfView, 1.0f, nearValue, farPlane);
	}

	/// Gets the fov value
	irr::f32 getFOV() const { return frontOfView; }
	void setFOV(const irr::f32 fov) {
		frontOfView = fov;
		if(isDirectional)
			projMat.buildProjectionMatrixOrthoLH(frontOfView, frontOfView, nearValue, farPlane);
		else
			projMat.buildProjectionMatrixPerspectiveFovLH(frontOfView, 1.0f, nearValue, farPlane);
		if (!isParent) return;
		if (isDirectional || isSpot) {
			shadowLights[0].setFOV(fov);
		} else {
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setFOV(fov);
		}
	}

	/// Gets the light's color.
	const irr::video::SColorf& getLightColor() const { return diffuseColour; }
	void setLightColor(const irr::video::SColorf& lightColour)  {
		diffuseColour = lightColour;
		if (!isParent) return;
		if (isDirectional || isSpot)
			shadowLights[0].setLightColor(lightColour);
		else
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setLightColor(lightColour);
	}

	/// Sets the shadow map resolution for this light.
	void setShadowMapResolution(irr::u32 shadowMapResolution) {
		mapRes = shadowMapResolution;
		recalculate = true;
		if (!isParent) return;
		if (isDirectional || isSpot) {
			shadowLights[0].setShadowMapResolution(shadowMapResolution);
		} else {
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setShadowMapResolution(shadowMapResolution);
		}
	}
	irr::u32& getShadowMapResolution() { return mapRes; }

	/// Sets if we must recalculate shadow map
	bool mustRecalculate() { return recalculate; }
	void setRecalculate(bool recalculate) {
		this->recalculate = recalculate;
		if (!isParent) return;
		if (isDirectional || isSpot)
			shadowLights[0].setRecalculate(recalculate);
		else
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setRecalculate(recalculate);
	}

	/// Sets if we must auto recalculate shadow map
	bool isAutoRecalculate() { return autoRecalculate; }
	void setAutoRecalculate(bool autoRecalculate) {
		this->autoRecalculate = autoRecalculate;
		if (!isParent) return;
		if (isDirectional || isSpot)
			shadowLights[0].setAutoRecalculate(autoRecalculate);
		else
			for (irr::u32 i=0; i < 6; i++)
				shadowLights[i].setAutoRecalculate(autoRecalculate);
	}

	/// Sets if is torch mode
	bool isTorchMode() { return isCamera; }
	void setTorchMode(bool use) {
		if (isDirectional || isSpot)
			isCamera = use;
		if (!isParent) return;
		if (isDirectional ||isSpot)
			shadowLights[0].setTorchMode(use);
	}

	/// Get current light type
	E_SHADOW_LIGHT_TYPE getLightType() {
		return (!isDirectional && !isSpot) ? ESLT_POINT : (isDirectional) ? ESLT_DIRECTIONAL : ESLT_SPOT;
	}

	void setDirection(bool set) {
		if (set) {
			setLightType(ESLT_DIRECTIONAL);
		} else {
			/// Default Spot
			setLightType(ESLT_SPOT);
		}
	}

	/// Change the shadow light mode
	void setLightType(E_SHADOW_LIGHT_TYPE type) {
		shadowLights.clear();

		if (type == ESLT_DIRECTIONAL) {
			isDirectional = true;
            isSpot = false;
			projMat.buildProjectionMatrixOrthoLH(frontOfView, frontOfView, nearValue, farPlane);
            updateViewMatrix();
			shadowLights.push_back(SShadowLight(mapRes, pos, tar, diffuseColour, this->nearValue, farPlane, frontOfView, isDirectional, false));
		} else if (type == ESLT_SPOT) {
            isDirectional = false;
            isSpot = true;
            projMat.buildProjectionMatrixPerspectiveFovLH(frontOfView, 1.0f, nearValue, farPlane);
            updateViewMatrix();
			shadowLights.push_back(SShadowLight(mapRes, pos, tar, diffuseColour, this->nearValue, farPlane, frontOfView, isDirectional, false));
        } else {
			projMat.buildProjectionMatrixPerspectiveFovLH(frontOfView, 1.0f, nearValue, farPlane);
            updateViewMatrix();
            isSpot = false;
            isDirectional = false;
			for (irr::u32 i=0; i < 6; i++) {
				SShadowLight l = SShadowLight(mapRes, pos, tar, diffuseColour, this->nearValue, farPlane, frontOfView, false, false);
				l.setFOV(115.f * irr::core::DEGTORAD64);
				shadowLights.push_back(l);
			}
        }
	}

	irr::u32 getShadowLightCount() { return shadowLights.size(); }
	SShadowLight &getShadowLight(irr::u32 i) { return shadowLights[i]; }

	bool isComputeVolumetricLight() { return computeVolumetricLight; }

private:

	void updateViewMatrix()
	{
		viewMat.buildCameraLookAtMatrixLH(pos, tar,
			(pos - tar).dotProduct(irr::core::vector3df(1.0f, 0.0f, 1.0f)) == 0.0f ?
			irr::core::vector3df(0.0f, 0.0f, 1.0f) : irr::core::vector3df(0.0f, 1.0f, 0.0f));
	}

	irr::video::SColorf diffuseColour;
	irr::core::vector3df pos, tar;
	irr::f32 farPlane, frontOfView, nearValue;
	irr::core::matrix4 viewMat, projMat;
	irr::u32 mapRes;

	bool recalculate;
	bool autoRecalculate;
	bool isCamera;
	bool isDirectional;
    bool isSpot;
	bool isParent;
	bool computeVolumetricLight;

	irr::core::array<SShadowLight> shadowLights;
	E_SHADOW_LIGHT_CURRENT_PASS currentPass;
};

#endif
