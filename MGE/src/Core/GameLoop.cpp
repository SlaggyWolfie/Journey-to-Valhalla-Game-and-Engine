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
			_game = ServiceLocator::instance()->getService<Game>();
			//_window = ServiceLocator::instance()->getService<Game>()->getWindow();
			//_renderManager = ServiceLocator::instance()->getService<Rendering::RenderManager>();
			//_collisionManager = ServiceLocator::instance()->getService<Collisions::CollisionManager>();
			//_physicsManager = ServiceLocator::instance()->getService<Physics::PhysicsManager>();
			_renderManager = ServiceLocator::instance()->getService<Rendering::RenderManager>();
			createOwnedLoops();
		}

		GameLoop::~GameLoop()
		{
			//dtor
			//delete _renderer;
			//delete _game;
			destroyOwnedLoops();
		}

		Utility::FunctionGroup<Component*>* GameLoop::update() const
		{
			return _update.get();
		}

		Utility::FunctionGroup<Component*>* GameLoop::fixedUpdate() const
		{
			return _fixedUpdate.get();
		}

		Utility::FunctionGroup<Component*>* GameLoop::lateUpdate() const
		{
			return _lateUpdate.get();
		}

		void GameLoop::run() const
		{
			_renderManager->startFPSClock();

			//settings to make sure the update loop runs at 60 fps
			const sf::Time timePerFixedFrame = sf::seconds(1.0f / 60.0f);
			Utility::Time::start(timePerFixedFrame.asSeconds());
			float lag = 0;

			while (_game->running())
			{
				Utility::Time::update();
				lag += Utility::Time::deltaTime();
				const float fixedTimeStep = Utility::Time::fixedDeltaTime();

				while (lag > fixedTimeStep)
				{
					lag -= fixedTimeStep;
					//physics update
					//_collisionManager->execute();
					//_physicsManager->execute();
					_fixedUpdate->execute();
				}

				//_animationManager->execute();

				_update->execute();
				_lateUpdate->execute();

				//0 doesn't matter for now
				_renderManager->render(0);

				//empty the event queue
				_game->processEvents();
			}
		}

		void GameLoop::createOwnedLoops()
		{
			std::function<bool(Component*)> predicate =
				[](Component* comp) -> bool { return comp->isEnabled() && comp->getGameObject()->isActive(); };
			_update = std::make_unique<Utility::FunctionGroup<Component*>>(predicate);
			_fixedUpdate = std::make_unique<Utility::FunctionGroup<Component*>>(predicate);
			_lateUpdate = std::make_unique<Utility::FunctionGroup<Component*>>(predicate);
		}

		void GameLoop::destroyOwnedLoops()
		{
			_update.release();
			_fixedUpdate.release();
			_lateUpdate.release();
		}
	}
}


