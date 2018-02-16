#include "RenderManager.hpp"
#include <GL/glew.h>

namespace Engine
{
	namespace Rendering
	{
		RenderManager::RenderManager()
		{
		}

		RenderManager::~RenderManager()
		{
		}

		void RenderManager::addRenderer(Renderer_* renderer)
		{

		}

		void RenderManager::removeRenderer(Renderer_* renderer)
		{
		}

		void RenderManager::render(const float deltaTime)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			_render->execute();
			_window->display();

			calculateFPS();
		}

		void RenderManager::startFPSClock()
		{
			_renderClock = std::make_unique<sf::Clock>();
			_frameCount = 0;
			_timeSinceLastFPSCalculation = 0;
		}

		void RenderManager::calculateFPS()
		{
			//fps count is updated once every 1 second
			_frameCount++;
			_timeSinceLastFPSCalculation += _renderClock->restart().asSeconds();
			if (_timeSinceLastFPSCalculation > 1)
			{
				_fps = _frameCount / _timeSinceLastFPSCalculation;
				_timeSinceLastFPSCalculation -= 1;
				_frameCount = 0;
			}
		}
	}
}
