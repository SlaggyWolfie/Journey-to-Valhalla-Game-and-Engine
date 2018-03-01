#pragma once
#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include <memory>
#include "Manager.hpp"
#include <SFML/System/Clock.hpp>
#include "FunctionGroup.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

namespace Engine
{
	namespace Rendering
	{
		class Renderer_;
		class LightManager;
		class Light_;

		class RenderManager : public Manager
		{
			friend class LightManager;
		public:
			RenderManager();
			virtual ~RenderManager();
			void initialize() override;
			void addRenderer(Renderer_* renderer);
			void removeRenderer(Renderer_* renderer);
			bool containsRenderer(Renderer_* renderer) const;
			void calculateFPS();

			void render(float deltaTime);

			void startFPSClock();
			float getFPS() const;
		private:
			void renderOpaque() const;
			void renderTransparent() const;

			std::vector<Renderer_*> _opaqueRenderers;
			std::vector<Renderer_*> _transparentRenderers;

			void renderShadowBuffer(Light_* light);
			//void renderShadows();

			//std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _renderOpaque;
			//std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _renderTransparent;
			void createOwnedLoops();
			void destroyOwnedLoops();

			//FPS
			float _fps;
			std::unique_ptr<sf::Clock> _fpsClock;
			int _frameCount;
			float _timeSinceLastFPSCalculation;

			//Other
			LightManager* getLightManager();
			LightManager* _lightManager;

			sf::RenderWindow* getWindow();
			sf::RenderWindow* _window;
		};
	}
}

#endif //RENDER_MANAGER_HPP