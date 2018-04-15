#pragma once
#ifndef RENDER_MANAGER_HPP
#define RENDER_MANAGER_HPP

#include <memory>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include "Manager.hpp"
#include <vector>
#include "Core/GameLoop.hpp"
//#include "Text.hpp"
//#include "mge/util/DebugHud.hpp"

class ComponentUI;

namespace Engine
{
	namespace UI { class Text; }
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
			void addUI(ComponentUI * ui);
			void removeUI(ComponentUI * ui);
			void removeRenderer(Renderer_* renderer);
			bool containsRenderer(Renderer_* renderer) const;
			void calculateFPS(bool print = false);

			void render();

			void startFPSClock();
			float getFPS() const;
		private:
			void renderOpaque() const;
			void renderTransparent() const;
			void renderDebugging();
			void renderUI();
			bool _debugMode = false;

			std::vector<Renderer_*> _opaqueRenderers;
			std::vector<Renderer_*> _transparentRenderers;
			std::vector<ComponentUI*> _uiRenderers;

			void renderShadowBuffer(Light_* light);
			//void renderShadows();

			//std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _renderOpaque;
			//std::unique_ptr<Utility::FunctionGroup<Renderer_*>> _renderTransparent;
			void createOwnedLoops();
			void destroyOwnedLoops();

			//FPS
			float _fps = 0;
			std::unique_ptr<sf::Clock> _fpsClock = nullptr;
			int _frameCount = 0;
			float _timeSinceLastFPSCalculation = 0;
			std::unique_ptr<UI::Text> _fps_hud = nullptr;
			//std::unique_ptr<DebugHud> _debugHud;
			void setupFPSHUD();

			//Other
			Core::GameLoop* getGameLoop();
			Core::GameLoop* _gameLoop = nullptr;

			LightManager* getLightManager();
			LightManager* _lightManager = nullptr;

			sf::RenderWindow* getWindow();
		public:
			void reset() override;
		private:
			sf::RenderWindow* _window;
		};
	}
}

#endif //RENDER_MANAGER_HPP