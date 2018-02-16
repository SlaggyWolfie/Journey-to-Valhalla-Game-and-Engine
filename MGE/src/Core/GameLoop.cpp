#include "GameLoop.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include "Game.hpp"
#include "../../_vs2015/RenderManager.hpp"
#include "../../_vs2015/Time.hpp"
#include "../../_vs2015/ServiceLocator.hpp"

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

		void GameLoop::run()
		{
			//setting to calculate fps
			//sf::Clock renderClock;
			//int frameCount = 0;
			//float timeSinceLastFPSCalculation = 0;
			_renderManager->startFPSClock();

			//settings to make sure the update loop runs at 60 fps
			const sf::Time timePerFixedFrame = sf::seconds(1.0f / 60.0f);
			Utility::Time::start(timePerFixedFrame.asSeconds());
			float lag = 0;

			while (_game->running())
			{
				Utility::Time::update();
				lag += Utility::Time::deltaTime();

				while (lag > Utility::Time::fixedDeltaTime())
				{
					lag -= Utility::Time::fixedDeltaTime();
					_fixedUpdate->execute(/*timePerFrame.asSeconds()*/);
				}

				_update->execute(/*timePerFrame.asSeconds()*/);
				_lateUpdate->execute();

				//0 doesn't matter for now
				_renderManager->render(0);

				//empty the event queue
				_game->processEvents();
			}
		}

		void GameLoop::createOwnedLoops()
		{
			_update = std::make_unique<Utility::FunctionGroup<Component*>>();
			_fixedUpdate = std::make_unique<Utility::FunctionGroup<Component*>>();
			_lateUpdate = std::make_unique<Utility::FunctionGroup<Component*>>();
		}

		void GameLoop::destroyOwnedLoops()
		{
			_update.release();
			_fixedUpdate.release();
			_lateUpdate.release();
		}
	}
}


