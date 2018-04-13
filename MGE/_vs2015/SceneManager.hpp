#pragma once
#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP
#include "Manager.hpp"
#include "Core/Game.hpp"
#include <queue>

namespace Engine
{
	class Scene;

	class SceneManager : public Manager
	{
	public:
		SceneManager() = default;
		virtual ~SceneManager();

		Scene* loadScene(const std::string& path);
		void queueScene(const std::string& path);
		Scene* getActiveScene() const;
		
		void update();

		void initialize() override;
		void unloadScene();
		std::string getLevel(int number) const;
	private:
		std::string _queue;
		std::unique_ptr<Scene> _activeScene = nullptr;
		Game* _game = nullptr;
		Game* getGame();
	public:
		void reset() override;
		bool loading = false;
	};
}

#endif // SCENE_MANAGER_HPP