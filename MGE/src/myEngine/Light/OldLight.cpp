#include "myEngine/Light/OldLight.hpp"
#include "mge/core/World.hpp"

OldLight::OldLight(const std::string& pName, const glm::vec3& pPosition) :GameObject(pName, pPosition)
,
_lightType(OldLight::Type::Point),
_ambientContribution(1), _ambientLightColor(glm::vec3(1, 1, 1)),
_intensity(0.5f), _color(glm::vec3(1, 1, 0)),
_fallOffAngle(15), _fallOffOuterAngle(20),
_attenuationConstants(glm::vec3(0,0,0))
//_attenuationConstants(new float[3])
{}

OldLight::~OldLight() {
	//delete[] _attenuationConstants;
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void OldLight::_setWorldRecursively(World* pWorld) {

	//store the previous and new world before and after storing the given world
	World* previousWorld = _world;
	GameObject::_setWorldRecursively(pWorld);
	World* newWorld = _world;

	//check whether we need to register or unregister
	//if (previousWorld != nullptr) previousWorld->unregisterLight(this);
	//if (newWorld != nullptr) newWorld->registerLight(this);
}

void OldLight::setLightType(Type type)
{
	_lightType = type;
}

void OldLight::setLightType(int type)
{
	_lightType = (OldLight::Type)type;
}

OldLight::Type OldLight::getLightType()
{
	return _lightType;
}

//void Light::setLightDirection(glm::vec3 direction)
//{
//	if (_lightType == Light::Type::Directional) _lightDirection = direction;
//	else _lightDirection = glm::vec3(0, 0, 0);
//}

glm::vec3 OldLight::getLightDirection()
{
	//gets the forward vector or the -z vector
	//return -getWorldTransform()[2];
	return glm::vec3(getTransform()[2]);
}

glm::vec3 OldLight::getLightDirectionNormalized()
{
	return glm::normalize(getLightDirection());
	//return glm::fastNormalize(getLightDirection());
}

glm::vec3 OldLight::getLightPosition()
{
	return getWorldPosition();
}


void OldLight::setAmbientContribution(float ambientContribution)
{
	_ambientContribution = ambientContribution;
}

float OldLight::getAmbientContribution()
{
	return _ambientContribution;
}

void OldLight::setLightIntensity(float intensity)
{
	_intensity = intensity;
}

float OldLight::getLightIntensity()
{
	return _intensity;
}

void OldLight::setFallOffAngle(float angle)
{
	_fallOffAngle = angle;
}

float OldLight::getFallOffAngle()
{
	return _fallOffAngle;
}

float OldLight::getFallOff()
{
	//std::cout << glm::cos(glm::radians(_fallOffAngle)) << std::endl;
	return glm::cos(glm::radians(_fallOffAngle));
}

void OldLight::setFallOffOuterAngle(float angle)
{
	_fallOffOuterAngle = angle;
}

float OldLight::getFallOffOuterAngle()
{
	return _fallOffOuterAngle;
}

float OldLight::getFallOffOuter()
{
	return glm::cos(glm::radians(_fallOffOuterAngle));
}

void OldLight::setColor(glm::vec3 color)
{
	_color = color;
}

glm::vec3 OldLight::getColor()
{
	return _color;
}

void OldLight::setAmbientLightColor(glm::vec3 color)
{
	_ambientLightColor = color;
}

glm::vec3 OldLight::getAmbientLightColor()
{
	return _ambientLightColor;
}

void OldLight::setAttenuation(float c0, float c1, float c2)
{
	_attenuationConstants[0] = c0;
	_attenuationConstants[1] = c1;
	_attenuationConstants[2] = c2;
}

glm::vec3 OldLight::getAttenuationConstants()
{
	return _attenuationConstants;
}

//float * BetterLight::getAttenuation()
//{
//	return _attenuationConstants;
//}


