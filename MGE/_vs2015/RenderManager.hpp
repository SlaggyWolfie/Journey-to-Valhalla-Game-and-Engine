#pragma once
#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include <memory>
#include "Manager.hpp"
#include <SFML/System/Clock.hpp>
#include "FunctionGroup.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace Engine
{
	namespace Rendering
	{
		class Renderer_;
		class LightManager;

		class RenderManager : public Manager
		{
		public:
			RenderManager();
			~RenderManager();
			void addRenderer(Renderer_* renderer);
			void removeRenderer(Renderer_* renderer);
			void calculateFPS();

			void render(float deltaTime);

			void startFPSClock();
		protected:
		private:

			std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _render;

			//FPS
			float _fps;
			std::unique_ptr<sf::Clock> _renderClock;
			int _frameCount;
			float _timeSinceLastFPSCalculation;

			//Other
			sf::RenderWindow* _window;

			LightManager* _lightManager;
		};
	}
}

#endif //RENDER_MANAGER_HPP