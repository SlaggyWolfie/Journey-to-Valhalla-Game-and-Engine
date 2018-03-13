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
#include "ShadowMap.hpp"
#include "Light_.hpp"
#include "Shader.hpp"
#include "LightManager.hpp"
#include "mge/config.hpp"

//#include "LightManager.hpp"

namespace Engine
{
	namespace Rendering
	{
		RenderManager::RenderManager() :
			_fps(0), _frameCount(0), _timeSinceLastFPSCalculation(0), _fps_hud(nullptr), _lightManager(nullptr),
			_window(nullptr)
		{
		}

		RenderManager::~RenderManager()
		{
			//delete _lightManager;
			//delete _fpsClock;
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

			glEnable(GL_FRAMEBUFFER_SRGB);

			glClearColor(
				static_cast<float>(0x2d) / 0xff,
				static_cast<float>(0x6b) / 0xff,
				static_cast<float>(0xce) / 0xff,
				1.0f);
			//glClearColor(0, 0, 0, 1.0f);

			createOwnedLoops();
			setupFPSHUD();
		}

		void RenderManager::addRenderer(Renderer_* renderer)
		{
			if (renderer->_renderQueue == RenderQueue::Opaque)
			{
				if (!containsRenderer(renderer))
					_opaqueRenderers.push_back(renderer);
			}
			else
			{
				if (!containsRenderer(renderer))
					_transparentRenderers.push_back(renderer);
			}
		}

		void RenderManager::removeRenderer(Renderer_* renderer)
		{
			if (renderer->_renderQueue == RenderQueue::Opaque)
			{
				if (containsRenderer(renderer)) List::removeFrom(_opaqueRenderers, renderer);
			}
			else
				if (containsRenderer(renderer)) List::removeFrom(_transparentRenderers, renderer);
		}

		bool RenderManager::containsRenderer(Renderer_* renderer) const
		{
			if (renderer->_renderQueue == RenderQueue::Opaque)
				return std::find(_opaqueRenderers.begin(), _opaqueRenderers.end(), renderer) != _opaqueRenderers.end();

			return
				std::find(_transparentRenderers.begin(), _transparentRenderers.end(), renderer) != _transparentRenderers.end();
		}

		void RenderManager::render(const float deltaTime)
		{
			//getWindow()->clear();
			//getLightManager()->renderShadowMaps();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDisable(GL_BLEND);
			//glEnable(GL_BLEND);

			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);

			glCullFace(GL_BACK);

			glViewport(0, 0, getWindow()->getSize().x, getWindow()->getSize().y);
			renderOpaque();
			renderTransparent();

			//getWindow()->clear();
			_fps_hud->setTextInformation("FPS: " + std::to_string(getFPS()));
			_fps_hud->draw();
			//getWindow()->draw(_fps_hud->getTextObject());
			//_debugHud->draw();

			//sf::Text text;
			//sf::Font font;
			//font.loadFromFile("mge/fonts/arial.ttf");
			//text.setFont(font);
			//text.setString("healp");
			//text.setPosition(100, 100);
			//text.setFillColor(sf::Color::Red);
			//text.setCharacterSize(24);

			sf::Text text;
			text.setString("hh");
			sf::Font font;
			font.loadFromFile("mge/fonts/arial.ttf");
			text.setFont(font);
			getWindow()->draw(text);
			getWindow()->display();

			calculateFPS();
		}

		void RenderManager::startFPSClock()
		{
			//_fpsClock = new sf::Clock();
			//_fpsClock = std::unique_ptr<sf::Clock>(clock);
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
			for (auto r : _opaqueRenderers)
				if (r->isEnabled() && r->getGameObject()->isActive())
					r->render();
		}

		void RenderManager::renderTransparent() const
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//Sort transparent renderers
			std::multimap<float, Renderer_*> sorted;
			const glm::vec3 cameraPosition = Core::Camera_::getMainCamera()->getPosition();

			//Iterate over, add based on distance squared to camera
			for (Renderer_* itr : _transparentRenderers)
				if (itr != nullptr && itr->getGameObject() != nullptr)
					sorted.insert(std::make_pair(glm::length2(cameraPosition -
						itr->getGameObject()->getTransform()->getPosition()), itr));

			for (const std::multimap<float, Renderer_*>::value_type itr : sorted)
				if (itr.second->isEnabled() && itr.second->getGameObject()->isActive())
					itr.second->render();

			glDisable(GL_BLEND);
		}

		void RenderManager::renderShadowBuffer(Light_* light)
		{
			//std::cout << "\t Rendering shadow buffer." << std::endl;
			ShadowMap* shadowMap = light->getShadowMap();

			glViewport(0, 0, shadowMap->getWidth(), shadowMap->getHeight());

			shadowMap->bindFramebuffer();
			shadowMap->getShader()->bind();
			glClear(GL_DEPTH_BUFFER_BIT);
			shadowMap->pushLightSpaceMatrix(light->getLightSpaceMatrix());
			//shadowMap->bindTexture();
			for (Renderer_* renderer : _opaqueRenderers)
			{
				if (renderer->isEnabled() && renderer->castsShadows() && renderer->getGameObject()->isActive())
				{
					shadowMap->pushModelMatrix(renderer->getGameObject()->getTransform()->getMatrix4X4());
					//renderer->pushMatrices();
					renderer->pushMesh();
					//renderer->pushMatrices();
					//renderer->pushMesh();
					//renderer->render();
				}
			}

			//for (Renderer_* renderer : _transparentRenderers)
			//{
			//	if (renderer->isEnabled() && renderer->castsShadows() && renderer->getGameObject()->isActive())
			//	{
			//		shadowMap->pushModelMatrix(renderer->getGameObject()->getTransform()->getMatrix4X4());
			//		renderer->pushMatrices();
			//		renderer->pushMesh();
			//	}
			//}

			//ShadowMap::unbindTexture();
			Shader::unbind();
			ShadowMap::unbindFramebuffer();
		}

		void RenderManager::createOwnedLoops()
		{
			_opaqueRenderers = std::vector<Renderer_*>();
			_transparentRenderers = std::vector<Renderer_*>();
		}

		void RenderManager::destroyOwnedLoops()
		{
			_opaqueRenderers.clear();
			_transparentRenderers.clear();
		}

		void RenderManager::setupFPSHUD()
		{
			//_fps_hud = std::make_unique<TextHUD>(getWindow());
			_fps_hud = std::make_unique<TextHUD>();
			_fps_hud->setWindow(getWindow());
			_fps_hud->setFont("mge/fonts/arial.ttf");
			_fps_hud->setTextAlignment(Left_Justified);
			_fps_hud->setTextInformation("FPS");
			_fps_hud->getTextObject().setPosition(100, 100);
			_fps_hud->getTextObject().setCharacterSize(16);
			_fps_hud->getTextObject().setFillColor(sf::Color::White);
			//_fps_hud->getTextObject()->

			//_debugHud = std::make_unique<DebugHud>(getWindow());
			//_debugHud->setDebugInfo("Could you please draw?");
		}

		LightManager* RenderManager::getLightManager()
		{
			if (!_lightManager)
				_lightManager = ServiceLocator::instance()->getService<LightManager>();

			return _lightManager;
		}

		sf::RenderWindow* RenderManager::getWindow()
		{
			if (!_window)
				_window = ServiceLocator::instance()->getService<Game>()->getWindow();

			return _window;
		}

		void RenderManager::calculateFPS(const bool print)
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

			if (print) std::cout << "FPS: " + std::to_string(_fps) << std::endl;
		}
	}
}
