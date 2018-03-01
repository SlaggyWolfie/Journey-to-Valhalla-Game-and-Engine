#pragma once
#ifndef LIGHT_MANAGER_HPP
#define LIGHT_MANAGER_HPP
#include "Manager.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace Engine
{
	namespace Rendering
	{
		class Light_;

		class LightManager : public Manager
		{
		public:
			LightManager();
			virtual ~LightManager();
			void initialize() override;

			void addLight(Light_* light);
			void removeLight(Light_* light);
			bool containsLight(Light_* light) const;

			int getDirectionalLightsAmount() const;
			int getPointLightsAmount() const;
			int getSpotLightsAmount() const;

			Light_* getDirectionalLight(int index) const;
			Light_* getPointLight(int index) const;
			Light_* getSpotLight(int index) const;

			std::vector<Light_*> getDirectionalLights() const;
			std::vector<Light_*> getPointLights() const;
			std::vector<Light_*> getSpotLights() const;

			Light_** getDirectionalLightsArray() const;
			Light_** getPointLightsArray() const;
			Light_** getSpotLightsArray() const;

			void setAttenuation(float c1 = 0, float c2 = 0, float c3 = 0);
			glm::vec3 getAttenuation() const;

			void setAmbientLightColor(glm::vec3 color);
			glm::vec3 getAmbientLightColor() const;

			void setAmbientStrength(float strength);
			float getAmbientStrength() const;

			void renderShadowMaps();
			//void renderShadows();
		private:
			RenderManager * _renderManager = nullptr;
			RenderManager* getRenderManager();

			bool findLight(Light_* light) const;

			std::vector<Light_*> _directionalLights;
			std::vector<Light_*> _pointLights;
			std::vector<Light_*> _spotLights;

			glm::vec3 _attenuationConstants = glm::vec3(0);

			glm::vec3 _ambientLightColor = glm::vec3(0);
			float _ambientLightStrength = 0;

			template <typename T>
			T* getArray(std::vector<T> list) const;

		};

		template <typename T>
		T* LightManager::getArray(std::vector<T> list) const
		{
			T* array = new T[static_cast<int>(list.size())];
			std::copy(list.begin(), list.end(), array);
			return array;
		}
	}
}

#endif