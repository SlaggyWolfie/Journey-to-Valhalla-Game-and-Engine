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
	class SceneManager;
	class Scene;

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
			void renderDebugging();
			bool hasStarted() const;
		private:
			bool _started = false;

			std::vector<Component*> _components;

			void start();
			void update();
			void fixedUpdate();
			void lateUpdate();

			Engine::SceneManager* getSceneManager();
			Game* getGame();
			sf::RenderWindow* getWindow();
			Rendering::RenderManager* getRenderManager();
			ColliderManager* getColliderManager();

			Engine::SceneManager* _sceneManager = nullptr;
			Game* _game = nullptr;
			sf::RenderWindow* _window = nullptr;
			Rendering::RenderManager* _renderManager = nullptr;
			ColliderManager* _colliderManager = nullptr;

			void createOwnedLoops();
			void destroyOwnedLoops();
		public:
			void reset() override;
		};
	}
}

#endif // GAMELOOP_HPP
