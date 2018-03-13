#pragma once
#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include <memory>
#include "Manager.hpp"
#include <SFML/System/Clock.hpp>
#include "FunctionGroup.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <SFML/Graphics/Text.hpp>
#include "TextHUD.hpp"
#include "mge/util/DebugHud.hpp"

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
			void calculateFPS(bool print = false);

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
			float _fps = 0;
			std::unique_ptr<sf::Clock> _fpsClock = nullptr;
			int _frameCount =  0;
			float _timeSinceLastFPSCalculation = 0;
			std::unique_ptr<TextHUD> _fps_hud;
			std::unique_ptr<DebugHud> _debugHud;
			void setupFPSHUD();

			//Other
			LightManager* getLightManager();
			LightManager* _lightManager;

			sf::RenderWindow* getWindow();
			sf::RenderWindow* _window;
		};
	}
}

#endif //RENDER_MANAGER_HPP