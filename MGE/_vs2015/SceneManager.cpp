#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Time.hpp"

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

		//Engine::Utility::Time::pause();
		Engine::Utility::Time::pause();
		_activeScene = std::make_unique<Scene>(Engine::File::clipPath(path), path);
		_activeScene->initialize(true, true);
		Engine::Utility::Time::unpause();
		return getActiveScene();
	}

	void SceneManager::queueScene(const std::string& path)
	{
		_queue = path;
		loading = true;
	}

	Scene* SceneManager::getActiveScene() const
	{
		return _activeScene.get();
	}

	void SceneManager::update()
	{
		if (_unloadQueue)
		{
			unloadScene();
			_unloadQueue = false;
		}

		if (!_queue.empty())
		{
			loadScene(_queue);
			_queue = std::string();
			loading = false;
		}
	}

	void SceneManager::initialize()
	{
	}

	std::string SceneManager::getLevel(const int number) const
	{
		return "Level_" + std::to_string(number) + ".json";
	}

	void SceneManager::unloadScene()
	{
		ServiceLocator::instance()->resetServices(this);
		_activeScene = nullptr;
	}

	void SceneManager::queueUnloadScene()
	{
		_unloadQueue = true;
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
