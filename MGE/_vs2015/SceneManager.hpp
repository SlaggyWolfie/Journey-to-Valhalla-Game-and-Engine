#pragma once
#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP
#include "Manager.hpp"

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
	};
}

#endif // SCENE_MANAGER_HPP