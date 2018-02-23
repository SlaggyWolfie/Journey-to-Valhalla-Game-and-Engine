#include "SceneManager.hpp"
#include "Scene.hpp"

namespace Engine
{
	SceneManager::~SceneManager()
	{
		_activeScene = nullptr;
	}

	Scene* SceneManager::loadScene(const std::string& path)
	{
		_activeScene = std::make_unique<Scene>(path);
		_activeScene->initialize();
		return getActiveScene();
	}

	Scene* SceneManager::getActiveScene() const
	{
		return _activeScene.get();
	}

	void SceneManager::initialize()
	{
	}
}
