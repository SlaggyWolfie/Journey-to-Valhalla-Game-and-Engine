#pragma once
#ifndef LIGHT__HPP
#define LIGHT__HPP
#include "Component.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace Engine
{
	namespace Rendering
	{
		enum class LightType { Directional, Point, Spot };

		class LightManager;
		class ShadowMap;

		class Light_ : public Core::Component
		{
		public:

			Light_();
			virtual ~Light_();

			ShadowMap* getShadowMap() const;
			glm::mat4 getLightSpaceMatrix() const;

			void setLightType(LightType type);
			LightType getLightType() const;

			void setLightIntensity(float intensity);
			float getLightIntensity() const;

			void setRange(float range);
			float getRange() const;

			void setFallOffAngle(float angle);
			float getFallOffAngle() const;
			float getFallOffRadians() const;

			void setFallOffOuterAngle(float angle);
			float getFallOffOuterAngle() const;
			float getFallOffOuterRadians() const;

			void setColor(glm::vec3 color);
			glm::vec3 getColor() const;
		protected:
			bool isUniquePerGameObject() override;
		private:
			std::unique_ptr<ShadowMap> _shadowMap = nullptr;

			LightManager * _lightManager = nullptr;

			LightType _lightType = LightType::Point;

			float _intensity = 0;
			float _range = 0;
			float _fallOffAngle = 0;
			float _fallOffOuterAngle = 0;

			glm::vec3 _lightColor = glm::vec3(1);

			glm::mat4 calculateLightSpaceMatrix() const;
		};
	}
}

#endif