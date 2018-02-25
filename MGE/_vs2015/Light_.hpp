#pragma once
#ifndef LIGHT__HPP
#define LIGHT__HPP
#include "Component.hpp"
#include <glm/glm.hpp>

namespace Engine
{
	namespace Rendering
	{
		enum class LightType { Directional, Point, Spot };

		class LightManager;

		class Light_ :public Core::Component
		{
		public:

			Light_();
			virtual ~Light_();

			virtual void setLightType(LightType type);
			virtual LightType getLightType();

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
			LightManager * _lightManager;

			LightType _lightType;

			float _intensity;
			float _range;
			float _fallOffAngle;
			float _fallOffOuterAngle;

			glm::vec3 _lightColor;
		};
	}
}

#endif