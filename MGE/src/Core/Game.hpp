#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "../../_vs2015/Service.hpp"
#include "../_vs2015/ColliderManager.h"
#include <memory>
#include "../_vs2015/Music.hpp"

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
using namespace Engine::Audio;
class LuaScript;

namespace Engine
{
	class SceneManager;
	class ResourceManager;

	namespace Rendering
	{
		class RenderManager;
		class LightManager;
	}
	namespace Core
	{
		class GameLoop;
	}
	class Game : public Engine::Service
	{
	public:
		Game();
		virtual ~Game();

		//creates a window, initializes glew, a renderer and a world instance
		void initialize() override;
		sf::RenderWindow* getWindow();
		void processEvents();
		void run();
		bool isRunning() const;
		void exit();

		static LuaScript* luaScript();
		static LuaScript* levelLuaScript();
		static void levelLuaLoad(std::string path);
		static void deleteLevelLuaScript();

		Music* backGround;
	private:
		static LuaScript* _luaScript;
		static LuaScript* _levelLuaScript;
		//print info about the current driver version etc
		static void printVersionInfo();
		//initialize the extension wrangler
		static void initializeGlew();
		void initializeWindow();
		void initializeServices();
		void initializeMusic();	
		//initialize the actual scene, HAS to be done by a subclass
		void initializeScene() const;
		//void initializeScene(const std::string& filepath) const;
		//std::unique_ptr<sf::RenderWindow> _window;  //sfml window to render into
		sf::RenderWindow *  _window;  //sfml window to render into
		//Renderer* _renderer;        //the renderer class to render the world
		//World* _world;              //the root game object that represents our scene
		//float _fps;                 //stores the real fps
		Rendering::RenderManager* _renderManager;
		Rendering::LightManager* _lightManager;
		ColliderManager* _colliderManager;
		Core::GameLoop* _gameLoop;
		SceneManager* _sceneManager;
		ResourceManager* _resourceManager;

		std::string _defaultScenePath = "Level_1.json";

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;
	public:
		void reset() override;
	};
}

#endif // GAME_HPP
