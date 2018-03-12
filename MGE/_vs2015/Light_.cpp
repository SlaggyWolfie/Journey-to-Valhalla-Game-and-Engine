#include "Light_.hpp"
#include "LightManager.hpp"
#include "ServiceLocator.hpp"
#include "ShadowMap.hpp"
#include "Transform.hpp"
#include "GameObject_.hpp"

namespace Engine
{
	namespace Rendering
	{
		Light_::Light_() :
			_shadowMap(nullptr),
			_lightType(LightType::Point),
			_intensity(1), _fallOffAngle(15),
			_fallOffOuterAngle(20), _lightColor(glm::vec3(1, 1, 0))
		{
			_lightManager = ServiceLocator::instance()->getService<LightManager>();

			_shadowMap = std::make_unique<ShadowMap>();
			_lightManager->addLight(this);
		}

		Light_::~Light_()
		{
			_lightManager->removeLight(this);
			_shadowMap = nullptr;
		}

		ShadowMap* Light_::getShadowMap() const
		{
			return _shadowMap.get();
		}

		glm::mat4 Light_::getLightSpaceMatrix() const
		{
			//space for static and caching later on
			return calculateLightSpaceMatrix();
		}

		void Light_::setLightType(const LightType type)
		{
			if (_lightType != type)
			{
				_lightManager->removeLight(this);
				_lightType = type;
				_lightManager->addLight(this);
			}
		}

		LightType Light_::getLightType() const
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

		glm::mat4 Light_::calculateLightSpaceMatrix() const
		{
			const glm::mat4 lightProjectionMatrix = _lightType == LightType::Directional ?
				glm::ortho(-_range, _range, -_range, _range, -_range, _range) :
				glm::perspective(90.0f, 1.0f, -_range, _range);
			const glm::mat4 lightViewMatrix = getGameObject()->getTransform()->getMatrix4X4();

			return lightProjectionMatrix * glm::inverse(lightViewMatrix);
			return lightProjectionMatrix * lightViewMatrix;
		}

		glm::vec3 Light_::getColor() const
		{
			return _lightColor;
		}
	}
}
