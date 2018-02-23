#include "Light_.hpp"
#include "LightManager.hpp"
#include "ServiceLocator.hpp"

namespace Engine
{
	namespace Rendering
	{
		Light_::Light_() :
			_lightType(LightType::Point),
			_intensity(1), _fallOffAngle(15),
			_fallOffOuterAngle(20), _lightColor(glm::vec3(1, 1, 0))
		{
			_lightManager = ServiceLocator::instance()->getService<LightManager>();

			_lightManager->addLight(this);
		}

		Light_::~Light_()
		{
			_lightManager->removeLight(this);
		}

		void Light_::setLightType(const LightType type)
		{
			_lightType = type;
		}

		LightType Light_::getLightType()
		{
			return _lightType;
		}

		void Light_::setLightIntensity(const float intensity)
		{
			_intensity = intensity;
		}

		float Light_::getLightIntensity() const
		{
			return _intensity;
		}

		void Light_::setRange(const float range)
		{
			_range = range;
		}

		float Light_::getRange() const
		{
			return _range;
		}

		void Light_::setFallOffAngle(const float angle)
		{
			_fallOffAngle = angle;
		}

		float Light_::getFallOffAngle() const
		{
			return _fallOffAngle;
		}

		float Light_::getFallOffRadians() const
		{
			//std::cout << glm::cos(glm::radians(_fallOffAngle)) << std::endl;
			return glm::cos(glm::radians(_fallOffAngle));
		}

		void Light_::setFallOffOuterAngle(const float angle)
		{
			_fallOffOuterAngle = angle;
		}

		float Light_::getFallOffOuterAngle() const
		{
			return _fallOffOuterAngle;
		}

		float Light_::getFallOffOuterRadians() const
		{
			return glm::cos(glm::radians(_fallOffOuterAngle));
		}

		void Light_::setColor(const glm::vec3 color)
		{
			_lightColor = color;
		}

		bool Light_::isUniquePerGameObject()
		{
			return true;
		}

		glm::vec3 Light_::getColor() const
		{
			return _lightColor;
		}
	}
}
