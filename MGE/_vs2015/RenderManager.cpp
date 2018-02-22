#include "RenderManager.hpp"
#include <GL/glew.h>
#include "Renderer_.hpp"
#include "FunctionGroup.hpp"
#include <functional>
#include "../_vs2015/GameObject_.hpp"
#include "../_vs2015/Transform.hpp"
#include <map>
#include "../_vs2015/Camera_.hpp"

//#include "LightManager.hpp"

namespace Engine
{
	namespace Rendering
	{
		RenderManager::RenderManager() :
			_fps(0), _frameCount(0), _timeSinceLastFPSCalculation(0),
			_window(nullptr)
		{
			//make sure we test the depthbuffer
			glEnable(GL_DEPTH_TEST);

			//tell opengl which vertex winding is considered to be front facing
			glFrontFace(GL_CCW);

			//tell opengl to enable face culling for the back faces
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glClearColor(
				static_cast<float>(0x2d) / 0xff,
				static_cast<float>(0x6b) / 0xff,
				static_cast<float>(0xce) / 0xff,
				1.0f);
			createOwnedLoops();
		}

		RenderManager::~RenderManager()
		{
			//delete _lightManager;
			destroyOwnedLoops();
		}

		void RenderManager::addRenderer(Renderer_* renderer) const
		{
			if (renderer->_renderQueue == Opaque)
			{
				std::function<void()> func = std::bind(&Renderer_::render, &*renderer);
				//_renderOpaque->subscribe(renderer, [&] {renderer->render();});
				_renderOpaque->subscribe(renderer, *&func);
			}
			else
			{
				//_renderTransparent->subscribe(renderer, [&] {renderer->render();});
				std::function<void()> func = std::bind(&Renderer_::render, &*renderer);
				_renderTransparent->subscribe(renderer, *&func);
			}
		}

		void RenderManager::removeRenderer(Renderer_* renderer) const
		{
			if (renderer->_renderQueue == Opaque)
				_renderOpaque->unsubscribe(renderer);
			else
				_renderTransparent->unsubscribe(renderer);
		}

		bool RenderManager::containsRenderer(Renderer_* renderer) const
		{
			if (renderer->_renderQueue == Opaque)
				return _renderOpaque->isSubscribed(renderer);

			return _renderTransparent->isSubscribed(renderer);
		}

		void RenderManager::render(const float deltaTime)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDisable(GL_BLEND);

			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			glCullFace(GL_BACK);

			renderOpaque();
			renderTransparent();

			_window->display();

			calculateFPS();
		}

		void RenderManager::startFPSClock()
		{
			_fpsClock = std::make_unique<sf::Clock>();
			_frameCount = 0;
			_timeSinceLastFPSCalculation = 0;
		}

		float RenderManager::getFPS() const
		{
			return _fps;
		}

		void RenderManager::renderOpaque() const
		{
			//std::cout << "Not Done. RenderManager" << std::endl;
			//std::cout << "Added predicate" << std::endl;
			_renderOpaque->execute();
		}

		void RenderManager::renderTransparent() const
		{
			/*std::cout << "Not Done. RenderManager" << std::endl;
			_renderTransparent->execute();*/

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//Sort transparent renderers
			std::multimap<float, Renderer_*> sorted;
			const glm::vec3 cameraPosition = Core::Camera_::getMainCamera()->getPosition();

			//Iterate over, add based on distance squared to camera
			const std::vector<Renderer_*> vector = _renderTransparent->getObjects();
			for (Renderer_* itr : vector)
				if (itr != nullptr && itr->getGameObject() != nullptr)
					sorted.insert(std::make_pair(glm::length2(cameraPosition - 
						itr->getGameObject()->getTransform()->getPosition()), itr));

			for (const std::multimap<float, Renderer_*>::value_type itr : sorted)
				if (itr.second->isEnabled() && itr.second->getGameObject()->isActive())
					itr.second->render();

			glDisable(GL_BLEND);
		}

		void RenderManager::createOwnedLoops()
		{
			std::function<bool(Renderer_*)> predicate =
				[](Renderer_* comp) -> bool { return comp->isEnabled() && comp->getGameObject()->isActive(); };
			_renderOpaque = std::make_unique<Utility::FunctionGroup<Renderer_*>>(predicate);
			_renderTransparent = std::make_unique<Utility::FunctionGroup<Renderer_*>>(predicate);
		}

		void RenderManager::destroyOwnedLoops()
		{
			_renderTransparent.release();
			_renderOpaque.release();
		}

		void RenderManager::calculateFPS()
		{
			//fps count is updated once every 1 second
			_frameCount++;
			_timeSinceLastFPSCalculation += _fpsClock->restart().asSeconds();
			if (_timeSinceLastFPSCalculation > 1)
			{
				_fps = _frameCount / _timeSinceLastFPSCalculation;
				_timeSinceLastFPSCalculation -= 1;
				_frameCount = 0;
			}
		}
	}
}
