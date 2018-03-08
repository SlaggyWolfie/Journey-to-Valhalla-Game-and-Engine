#include "GameLoop.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"
#include "../../_vs2015/RenderManager.hpp"
#include "../../_vs2015/Time.hpp"
#include "../../_vs2015/ServiceLocator.hpp"
#include "../_vs2015/GameObject_.hpp"
#include "../_vs2015/Component.hpp"

namespace Engine
{
	namespace Core
	{
		GameLoop::GameLoop()
		{
			//ctor
			createOwnedLoops();
			//_components;
		}

		GameLoop::~GameLoop()
		{
			//dtor
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

		ColliderManager * GameLoop::GetColliderM()
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

			while (getGame()->isRunning())
			{
				//while(_renderManager == nullptr) _renderManager->startFPSClock();
				Utility::Time::update();
				lag += Utility::Time::deltaTime();
				const float fixedTimeStep = Utility::Time::fixedDeltaTime();

				while (lag > fixedTimeStep)
				{
					lag -= fixedTimeStep;
					//physics update
					//_collisionManager->execute();
					//_physicsManager->execute();
					fixedUpdate();
				}

				//_animationManager->execute();

				update();
				lateUpdate();

				//0 doesn't matter for now
				getRenderManager()->render(0);
				getRenderManager()->calculateFPS();

				//empty the event queue
				getGame()->processEvents();
			}
		}

		void GameLoop::update()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled() && comp->getGameObject()->isActive())
						comp->update();
		}

		void GameLoop::fixedUpdate()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled() && comp->getGameObject()->isActive())
						comp->fixedUpdate();
		}

		void GameLoop::lateUpdate()
		{
			if (!_components.empty())
				for (const auto& comp : _components)
					if (comp->isEnabled() && comp->getGameObject()->isActive())
						comp->lateUpdate();
		}

		Game* GameLoop::getGame()
		{
			if (!_game)
				_game = ServiceLocator::instance()->getService<Game>();

			return _game;
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
	}
}


