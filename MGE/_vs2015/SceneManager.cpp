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
		if (_activeScene)
			unloadScene();

		_activeScene = std::make_unique<Scene>(Engine::File::clipPath(path), path);
		_activeScene->initialize(true, true);
		return getActiveScene();
	}

	Scene* SceneManager::getActiveScene() const
	{
		return _activeScene.get();
	}

	void SceneManager::initialize()
	{
	}

	void SceneManager::unloadScene()
	{
		ServiceLocator::instance()->resetServices(this);
		_activeScene = nullptr;
	}

	Game* SceneManager::getGame()
	{
		if (!_game) _game = ServiceLocator::instance()->getService<Game>();
		return _game;
	}

	void SceneManager::reset()
	{

	}
}
