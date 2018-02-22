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
			virtual ~RenderManager();
			void addRenderer(Renderer_* renderer) const;
			void removeRenderer(Renderer_* renderer) const;
			bool containsRenderer(Renderer_* renderer) const;
			void calculateFPS();

			void render(float deltaTime);

			void startFPSClock();
			float getFPS() const;
		private:

			void renderOpaque() const;
			void renderTransparent() const;

			std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _renderOpaque;
			std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _renderTransparent;
			void createOwnedLoops();
			void destroyOwnedLoops();

			//FPS
			float _fps;
			std::unique_ptr<sf::Clock> _fpsClock;
			int _frameCount;
			float _timeSinceLastFPSCalculation;

			//Other
			sf::RenderWindow* _window;
		};
	}
}

#endif //RENDER_MANAGER_HPP