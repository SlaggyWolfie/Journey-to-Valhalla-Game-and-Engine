#include "RenderManager.hpp"
#include <GL/glew.h>
#include "Renderer_.hpp"
#include "FunctionGroup.hpp"
#include <functional>
#include "../_vs2015/GameObject_.hpp"
#include "../_vs2015/Transform.hpp"
#include <map>
#include "../_vs2015/Camera_.hpp"
#include "Texture_.hpp"
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include <algorithm>

//#include "LightManager.hpp"

namespace Engine
{
	namespace Rendering
	{
		RenderManager::RenderManager() :
			_fps(0), _frameCount(0), _timeSinceLastFPSCalculation(0),
			_window(nullptr)
		{
		}

		RenderManager::~RenderManager()
		{
			//delete _lightManager;
			_fpsClock = nullptr;
			destroyOwnedLoops();
			Texture_::textureMap.clear();
		}

		void RenderManager::initialize()
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

			//glClearColor(0, 0, 0, 1.0f);
			Game* game = ServiceLocator::instance()->getService<Game>();
			//std::cout << std::to_string(game != nullptr) << std::endl;
			_window = game->getWindow();

			createOwnedLoops();
		}

		void RenderManager::addRenderer(Renderer_* renderer)
		{
			if (renderer->_renderQueue == RenderQueue::Opaque)
			{
				//std::function<void()> func = std::bind(&Renderer_::render, &*renderer);
				//_renderOpaque->subscribe(renderer, [&] {renderer->render();});
				//_renderOpaque->subscribe(renderer, *&func);
				//_ro.push_back(&*renderer);
				if (!containsRenderer(renderer)) //List::removeFrom(_ro, renderer);
					_ro.push_back(renderer);
				//_ro.
			}
			else
			{
				//_renderTransparent->subscribe(renderer, [&] {renderer->render();});
				//std::function<void()> func = std::bind(&Renderer_::render, &*renderer);
				//_renderTransparent->subscribe(renderer, *&func);
				if (!containsRenderer(renderer)) //List::removeFrom(_ro, renderer);
					_rt.push_back(renderer);
			}
		}

		void RenderManager::removeRenderer(Renderer_* renderer)
		{
			if (renderer->_renderQueue == RenderQueue::Opaque)
			{
				//_renderOpaque->unsubscribe(renderer);
				if (containsRenderer(renderer)) List::removeFrom(_ro, renderer);
			}
			else
				if (containsRenderer(renderer)) List::removeFrom(_rt, renderer);
			//_renderTransparent->unsubscribe(renderer);
		}

		bool RenderManager::containsRenderer(Renderer_* renderer) const
		{
			if (renderer->_renderQueue == RenderQueue::Opaque)
				std::find(_ro.begin(), _ro.end(), renderer) != _ro.end();

			return
				//_renderTransparent->isSubscribed(renderer);
				std::find(_rt.begin(), _rt.end(), renderer) != _rt.end();
		}

		void RenderManager::render(const float deltaTime)
		{
			//std::cout << "Pl0x" << std::endl;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDisable(GL_BLEND);
			//glEnable(GL_BLEND);

			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			glCullFace(GL_BACK);

			renderOpaque();
			renderTransparent();
			_window->display();

			//calculateFPS();
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
			for (auto r : _ro) r->render();
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
			for (Renderer_* itr : _rt)
				//for (Renderer_* itr : vector)
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

			_ro = std::vector<Renderer_*>();
			_rt = std::vector<Renderer_*>();
		}

		void RenderManager::destroyOwnedLoops()
		{
			_ro.clear();
			_rt.clear();
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
