#pragma once
#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <memory>
#include "../../_vs2015/Service.hpp"
#include "../../_vs2015/FunctionGroup.hpp"
#include "../_vs2015/RenderManager.hpp"
#include "../_vs2015/Component.hpp"
#include "../_vs2015/ColliderManager.h"

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
			void initialize() override;
			void subscribe(Component* component);
			void unsubscribe(Component* component);
			bool isSubscribed(Component* component);

			void run();
			bool hasStarted() const;
		private:
			bool _started = false;

			std::vector<Component*> _components;
			//std::vector<std::shared_ptr<Component>> _components;
			//std::shared_ptr<Component> find(Component* component);
			void start();
			void update();
			void fixedUpdate();
			void lateUpdate();

			Game* getGame();
			Rendering::RenderManager* getRenderManager();
			ColliderManager* getColliderManager();
			ColliderManager* _colliderManager;

			Game* _game = nullptr;
			Rendering::RenderManager* _renderManager = nullptr;
			//AnimationManager _animationManager;
			//CollisionManager _collisionManager;
			//PhysicsManager _physicsManager;

			void createOwnedLoops();
			void destroyOwnedLoops();
		};
	}
}

#endif // GAMELOOP_HPP
