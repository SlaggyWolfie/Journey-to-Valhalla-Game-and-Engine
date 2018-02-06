#ifndef BETTERLIGHT_HPP
#define BETTERLIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
class OldLight : public GameObject
{
public:
	enum Type { Directional, Point, Spotlight };

	OldLight(const std::string& aName = nullptr, const glm::vec3& aPosition = glm::vec3(2.0f, 10.0f, 5.0f));
	virtual ~OldLight();

	//override set parent to register/deregister light...
	virtual void _setWorldRecursively(World* pWorld) override;

	virtual void setLightType(Type type);
	virtual void setLightType(int type);
	virtual Type getLightType();

	//only works if the light type is DIRECTIONAL
	//virtual void setLightDirection(glm::vec3 direction);
	virtual glm::vec3 getLightDirection();
	virtual glm::vec3 getLightDirectionNormalized();
	virtual glm::vec3 getLightPosition();

	virtual void setAmbientContribution(float ambientContribution);
	virtual float getAmbientContribution();

	virtual void setLightIntensity(float intensity);
	virtual float getLightIntensity();

	virtual void setFallOffAngle(float angle);
	virtual float getFallOffAngle();
	virtual float getFallOff();

	virtual void setFallOffOuterAngle(float angle);
	virtual float getFallOffOuterAngle();
	virtual float getFallOffOuter();

	virtual void setColor(glm::vec3 color);
	virtual glm::vec3 getColor();

	virtual void setAmbientLightColor(glm::vec3 color);
	virtual glm::vec3 getAmbientLightColor();

	//virtual void setAttenuation(float c1 = 0, float c2 = 0, float c3 = 0);
	//virtual float* getAttenuation()
	virtual void setAttenuation(float c1 = 0, float c2 = 0, float c3 = 0);
	virtual glm::vec3 getAttenuationConstants();
private:
	Type _lightType;
	glm::vec3 _lightDirection;
	float _ambientContribution;
	float _intensity;
	float _fallOffAngle;
	float _fallOffOuterAngle;

	glm::vec3 _color;
	glm::vec3 _ambientLightColor;

	glm::vec3 _attenuationConstants;
	//float* _attenuationConstants;
};

#endif // BETTERLIGHT_HPP
