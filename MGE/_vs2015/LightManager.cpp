#include "LightManager.hpp"
#include "Light_.hpp"
#include <iostream>
#include "GeneralHelpers.hpp"
#include <algorithm>

namespace Engine
{
	namespace Rendering
	{
		LightManager::LightManager() : _ambientLightStrength(0)
		{
		}

		LightManager::~LightManager()
		{
			_directionalLights.clear();
			_pointLights.clear();
			_spotLights.clear();
		}

		void LightManager::addLight(Light_* light)
		{
			if (findLight(light)) return;

			switch (light->getLightType())
			{
			case LightType::Directional: _directionalLights.push_back(light); break;
			case LightType::Point: _pointLights.push_back(light); break;
			case LightType::Spot: _spotLights.push_back(light); break;
			default: std::cout << "Unrecognized Light Type to add!" << std::endl; break;
			}
		}

		void LightManager::removeLight(Light_* light)
		{
			if (!findLight(light)) return;

			switch (light->getLightType())
			{
			case LightType::Directional: List::removeFrom(_directionalLights, light); break;
			case LightType::Point: List::removeFrom(_pointLights, light); break;
			case LightType::Spot: List::removeFrom(_spotLights, light); break;
			default: std::cout << "Unrecognized Light Type to remove!" << std::endl; break;
			}
		}

		bool LightManager::containsLight(Light_* light) const
		{
			return findLight(light);
		}

		int LightManager::getDirectionalLightsAmount() const
		{
			return static_cast<int>(_directionalLights.size());
		}

		int LightManager::getPointLightsAmount() const
		{
			return static_cast<int>(_pointLights.size());
		}

		int LightManager::getSpotLightsAmount() const
		{
			return static_cast<int>(_spotLights.size());
		}

		Light_* LightManager::getDirectionalLight(const int index) const
		{
			return _directionalLights[index];
		}

		Light_* LightManager::getPointLight(const int index) const
		{
			return _pointLights[index];
		}

		Light_* LightManager::getSpotLight(const int index) const
		{
			return _spotLights[index];
		}

		std::vector<Light_*> LightManager::getDirectionalLights() const
		{
			return _directionalLights;
		}

		std::vector<Light_*> LightManager::getPointLights() const
		{
			return _pointLights;
		}

		std::vector<Light_*> LightManager::getSpotLights() const
		{
			return _spotLights;
		}

		Light_** LightManager::getDirectionalLightsArray() const
		{
			return getArray(_directionalLights);
		}

		Light_** LightManager::getPointLightsArray() const
		{
			return getArray(_pointLights);
		}

		Light_** LightManager::getSpotLightsArray() const
		{
			return getArray(_spotLights);
		}

		void LightManager::setAttenuation(const float c1, const float c2, const float c3)
		{
			_attenuationConstants = glm::vec3(c1, c2, c3);
		}

		glm::vec3 LightManager::getAttenuation() const
		{
			return _attenuationConstants;
		}

		void LightManager::setAmbientLightColor(const glm::vec3 color)
		{
			_ambientLightColor = glm::clamp(color, glm::vec3(0), glm::vec3(1));
		}

		glm::vec3 LightManager::getAmbientLightColor() const
		{
			return _ambientLightColor;
		}

		void LightManager::setAmbientStrength(const float strength)
		{
			_ambientLightStrength = strength;
		}

		float LightManager::getAmbientStrength() const
		{
			return _ambientLightStrength;
		}

		bool LightManager::findLight(Light_* light) const
		{
			return
				std::find(_directionalLights.begin(), _directionalLights.end(), light) != _directionalLights.end() ||
				std::find(_pointLights.begin(), _pointLights.end(), light) != _pointLights.end() ||
				std::find(_spotLights.begin(), _spotLights.end(), light) != _spotLights.end();
		}
	}
}
