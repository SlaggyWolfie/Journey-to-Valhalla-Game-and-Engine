#include "GameLoop.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"
#include "../../_vs2015/RenderManager.hpp"
#include "../../_vs2015/Time.hpp"
#include "../../_vs2015/ServiceLocator.hpp"
#include "../_vs2015/GameObject_.hpp"
#include "../_vs2015/Component.hpp"
#include <GL/glew.h>
#include "../_vs2015/SceneManager.hpp"
#include "../../_vs2015/Sound.hpp"
#include "../_vs2015/Text.hpp"
#include "LuaScript.h"

namespace Engine
{
	namespace Core
	{
		GameLoop::GameLoop()
		{
			//createOwnedLoops();
		}

		GameLoop::~GameLoop()
		{
			//delete _renderer;
			//delete _game;
			destroyOwnedLoops();
		}

		void GameLoop::initialize()
		{
			//_game = ServiceLocator::instance()->getService<Game>();
			//_window = ServiceLocator::instance()->getService<Game>()->getWindow();
			//_collisionManager = ServiceLocator::instance()->getService<Collisions::CollisionManager>();
			//_physicsManager = ServiceLocator::instance()->getService<Physics::PhysicsManager>();
			//_renderManager = ServiceLocator::instance()->getService<Rendering::RenderManager>();
		}

		void GameLoop::subscribe(Component* component)
		{
			if (!isSubscribed(component))
				_components.push_back(component);
			//_components.push_back(std::shared_ptr<Component>(component));
		}

		void GameLoop::unsubscribe(Component* component)
		{
			//const auto check = find(component);

			//if (check != nullptr)
			//	List::removeFrom(_components, check);

			if (isSubscribed(component))
				List::removeFrom(_components, component);
		}

		bool GameLoop::isSubscribed(Component* component)
		{
			//return find(component) != nullptr;
			return !_components.empty() && std::find(_components.begin(), _components.end(), component) != _components.end();
		}

		ColliderManager * GameLoop::getColliderManager()
		{
			if (!_colliderManager) _colliderManager = ServiceLocator::instance()->getService<ColliderManager>();
			return _colliderManager;
		}

		//std::shared_ptr<Component> GameLoop::find(Component* component)
		//{
		//	for (std::shared_ptr<Component> i : _components)
		//	{
		//		if (i.get() == component)
		//			return i;
		//	}

		//	return nullptr;
		//}

		void GameLoop::run()
		{
			//if (getRenderManager() == nullptr) std::cout << "What" << std::endl;
			getRenderManager()->startFPSClock();

			//settings to make sure the update loop runs at 60 fps
			const sf::Time timePerFixedFrame = sf::seconds(1.0f / 60.0f);
			Utility::Time::start(timePerFixedFrame.asSeconds());
			float lag = 0;

			start();
			_started = true;
			while (getGame()->isRunning())
			{
				//while(_renderManager == nullptr) _renderManager->startFPSClock();
				Utility::Time::update();
				lag += Utility::Time::deltaTime();
				const float fixedTimeStep = Utility::Time::fixedDeltaTime();

				while (lag > fixedTimeStep)
				{
					lag -= fixedTimeStep;
					if (!Engine::Utility::Time::isPaused()) fixedUpdate();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
					ServiceLocator::instance()->getService<SceneManager>()->loadScene("finalLvl1.json");
				//ServiceLocator::instance()->getService<SceneManager>()->loadScene("Assets/Scenes/test.json");
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
					Engine::Audio::Sound::playOneShot("Assets/Audio/Sounds/donnerre2.wav");
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
					Engine::UI::Text::drawHint = !Engine::UI::Text::drawHint;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
					std::cout << Engine::UI::Text::hint()->getTextInformation() << std::endl;

				if (!Engine::Utility::Time::isPaused())
				{
					update();
					lateUpdate();
				}

				getRenderManager()->render();

				//getWindow()->display();

				//getRenderManager()->calculateFPS();

				//empty the event queue
				getGame()->processEvents();
			}
		}

		bool GameLoop::hasStarted() const
		{
			return _started;
		}

		void GameLoop::start()
		{
			Game::luaScript()->start();

			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled()
						//&& comp->getGameObject() && comp->getGameObject()->isActive()
						)
					{
						comp->_started = true;
						comp->start();
					}
		}

		void GameLoop::update()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled()
						//&& comp->getGameObject() && comp->getGameObject()->isActive()
						)
						comp->update();

			Game::luaScript()->update();
		}

		void GameLoop::fixedUpdate()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled()
						//&& comp->getGameObject() && comp->getGameObject()->isActive()
						)
						comp->fixedUpdate();
		}

		void GameLoop::lateUpdate()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled()
						//&& comp->getGameObject() && comp->getGameObject()->isActive()
						)
						comp->lateUpdate();
		}

		void GameLoop::renderDebugging()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled()
						//&& comp->getGameObject() && comp->getGameObject()->isActive()
						)
						comp->renderDebug();
		}

		Game* GameLoop::getGame()
		{
			if (!_game)
				_game = ServiceLocator::instance()->getService<Game>();

			return _game;
		}

		sf::RenderWindow* GameLoop::getWindow()
		{
			if (!_window) _window = getGame()->getWindow();
			return _window;
		}

		Rendering::RenderManager* GameLoop::getRenderManager()
		{
			if (!_renderManager)
				_renderManager = ServiceLocator::instance()->getService<Rendering::RenderManager>();

			return _renderManager;
		}

		void GameLoop::createOwnedLoops()
		{
			_components = std::vector<Component*>();
		}

		void GameLoop::destroyOwnedLoops()
		{
			_components.clear();
		}

		void GameLoop::reset()
		{
			destroyOwnedLoops();
			createOwnedLoops();
		}
	}
}


