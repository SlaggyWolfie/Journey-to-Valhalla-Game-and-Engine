#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"

float Light::_ambientContribution = 0.1f;
glm::vec3 Light::_ambientLightColor = glm::vec3(1, 1, 1);
glm::vec3 Light::_attenuationConstants = glm::vec3(0, 0, 0);

Light::Light(const std::string& pName, const glm::vec3& pPosition) :GameObject(pName, pPosition)
,
_lightType(Light::Type::Point),
_intensity(1), _lightColor(glm::vec3(1, 1, 0)),
_fallOffAngle(15), _fallOffOuterAngle(20)
{}

Light::~Light() {
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void Light::_setWorldRecursively(World* pWorld) {

	//store the previous and new world before and after storing the given world
	World* previousWorld = _world;
	GameObject::_setWorldRecursively(pWorld);
	World* newWorld = _world;

	//check whether we need to register or unregister
	if (previousWorld != nullptr) previousWorld->unregisterLight(this);
	if (newWorld != nullptr) newWorld->registerLight(this);
}

void Light::setLightType(Type type)
{
	_lightType = type;
}

void Light::setLightType(int type)
{
	_lightType = (Light::Type)type;
}

Light::Type Light::getLightType()
{
	return _lightType;
}

//void Light::setLightDirection(glm::vec3 direction)
//{
//	if (_lightType == Light::Type::Directional) _lightDirection = direction;
//	else _lightDirection = glm::vec3(0, 0, 0);
//}

glm::vec3 Light::getLightDirection()
{
	//gets the forward vector or the -z vector
	//return -getWorldTransform()[2];
	return -glm::vec3(getTransform()[2]);
	//return glm::vec3(getTransform()[2]);
}

glm::vec3 Light::getLightDirectionNormalized()
{
	return glm::normalize(getLightDirection());
	//return glm::fastNormalize(getLightDirection());
}

glm::vec3 Light::getLightPosition()
{
	return getWorldPosition();
}


void Light::setAmbientContribution(float ambientContribution)
{
	_ambientContribution = ambientContribution;
}

float Light::getAmbientContribution()
{
	return _ambientContribution;
}

void Light::setLightIntensity(float intensity)
{
	_intensity = intensity;
}

float Light::getLightIntensity()
{
	return _intensity;
}

void Light::setFallOffAngle(float angle)
{
	_fallOffAngle = angle;
}

float Light::getFallOffAngle()
{
	return _fallOffAngle;
}

float Light::getFallOff()
{
	//std::cout << glm::cos(glm::radians(_fallOffAngle)) << std::endl;
	return glm::cos(glm::radians(_fallOffAngle));
}

void Light::setFallOffOuterAngle(float angle)
{
	_fallOffOuterAngle = angle;
}

float Light::getFallOffOuterAngle()
{
	return _fallOffOuterAngle;
}

float Light::getFallOffOuter()
{
	return glm::cos(glm::radians(_fallOffOuterAngle));
}

void Light::setColor(glm::vec3 color)
{
	_lightColor = color;
}

glm::vec3 Light::getColor()
{
	return _lightColor;
}

void Light::setAmbientLightColor(glm::vec3 color)
{
	_ambientLightColor = color;
}

glm::vec3 Light::getAmbientLightColor()
{
	return _ambientLightColor;
}

void Light::setAttenuation(float c0, float c1, float c2)
{
	_attenuationConstants[0] = c0;
	_attenuationConstants[1] = c1;
	_attenuationConstants[2] = c2;
}

glm::vec3 Light::getAttenuationConstants()
{
	return _attenuationConstants;
}

//float * Light::getAttenuation()
//{
//	return _attenuationConstants;
//}


