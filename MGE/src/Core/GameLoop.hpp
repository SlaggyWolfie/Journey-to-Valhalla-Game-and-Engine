#pragma once
#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <memory>
#include "../../_vs2015/Service.hpp"
#include "../../_vs2015/FunctionGroup.hpp"
#include "../_vs2015/RenderManager.hpp"

namespace Engine
{
	class Game;

	namespace Core
	{
		class Component;
		class GameLoop : public Service
		{
		public:

			GameLoop();
			virtual ~GameLoop();
			GameLoop(const GameLoop& other) = delete;
			GameLoop& operator=(const GameLoop& other) = delete;
			Utility::FunctionGroup<Component*>* update() const;
			Utility::FunctionGroup<Component*>* fixedUpdate() const;
			Utility::FunctionGroup<Component*>* lateUpdate() const;

			void run() const;

		//protected:
		private:
			std::unique_ptr<Utility::FunctionGroup<Component*>> _update;
			std::unique_ptr<Utility::FunctionGroup<Component*>> _fixedUpdate;
			std::unique_ptr<Utility::FunctionGroup<Component*>> _lateUpdate;
			Rendering::RenderManager* _renderManager;
			//AnimationManager _animationManager;
			//CollisionManager _collisionManager;
			//PhysicsManager _physicsManager;

			void createOwnedLoops();
			void destroyOwnedLoops();

			//sf::RenderWindow* _window;
			Game* _game;
		};
	}
}

#endif // GAMELOOP_HPP
