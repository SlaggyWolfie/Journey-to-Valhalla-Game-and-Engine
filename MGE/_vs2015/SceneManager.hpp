#pragma once
#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP
#include "Manager.hpp"
#include "Core/Game.hpp"

namespace Engine
{
	class Scene;

	class SceneManager : public Manager
	{
	public:
		SceneManager() = default;
		virtual ~SceneManager();

		Scene* loadScene(const std::string& path);
		Scene* getActiveScene() const;

		void initialize() override;
	private:
		std::unique_ptr<Scene> _activeScene = nullptr;
		void unloadScene();
		Game* _game = nullptr;
		Game* getGame();
	public:
		void reset() override;
	};
}

#endif // SCENE_MANAGER_HPP