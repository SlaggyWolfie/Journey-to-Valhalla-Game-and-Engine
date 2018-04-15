#include <iostream>

#include "Game.hpp"
#include "GameLoop.hpp"

#include "../../externals/glew_sfml_glm/include/lua/lua.hpp"
//#include "RenderManager.hpp"
#include "../_vs2015/RenderManager.hpp"
#include "../_vs2015/LightManager.hpp"
#include "../_vs2015/SceneManager.hpp"
#include "../_vs2015/ResourceManager.hpp"
#include "../_vs2015/ServiceLocator.hpp"
#include "../_vs2015/Scene.hpp"
#include "../_vs2015/Texture_.hpp"
#include "Deserealizer.hpp"
#include <glm.hpp>
#include "LuaScript.h"
#include "../../_vs2015/InputHandler.hpp"
#include "../_vs2015/Button.hpp"
#include "../_vs2015/Sound.hpp"


//#include "GameLoop.hpp"

namespace Engine
{
	LuaScript* Game::_luaScript = nullptr;
	LuaScript* Game::_levelLuaScript = nullptr;

	Game::Game() : _renderManager(nullptr), _lightManager(nullptr), _colliderManager(nullptr), _gameLoop(nullptr),
		_sceneManager(nullptr)
	{
		//ctor
	}

	Game::~Game()
	{
		//dtor
		//_window.release();
		ServiceLocator::destroyInstance();
		_window = nullptr;
	}

	void Game::initialize()
	{
		std::cout << "Initializing engine..." << std::endl << std::endl;
		initializeWindow();
		printVersionInfo();
		initializeGlew();
		initializeServices();
		std::cout << std::endl << "Engine initialized." << std::endl << std::endl;
		initializeMusic();

		//LuaScript l = LuaScript();

		//l.Initialize();
		//l.start();
	}

	sf::RenderWindow* Game::getWindow()
	{
		//return _window.get();
		return _window;
	}

	bool Game::isRunning() const
	{
		if (_window == nullptr) return false;
		return _window->isOpen();
	}

	void Game::exit()
	{

		if (_window->isOpen()) _window->close();
		//delete this;
	}

	LuaScript* Game::luaScript()
	{
		if (!_luaScript)
		{
			_luaScript = new LuaScript;
			_luaScript->Initialize();
		}
		return _luaScript;
	}

	LuaScript* Game::levelLuaScript()
	{
		if (!_levelLuaScript)
		{
			_levelLuaScript = new LuaScript;
			_levelLuaScript->Initialize();
		}
		return _levelLuaScript;
	}

	void Game::levelLuaLoad(const std::string path)
	{
		if (_levelLuaScript) deleteLevelLuaScript();
		std::cout << "Boi" << std::endl;
		//_levelLuaScript = new LuaScript(path);
		_levelLuaScript->Initialize();
	}

	void Game::deleteLevelLuaScript()
	{
		delete _levelLuaScript;
		_levelLuaScript = nullptr;
	}

	///SETUP

	void Game::initializeWindow() {

		std::cout << "Initializing window..." << std::endl;
		_window = new sf::RenderWindow(
			sf::VideoMode(1280, 1024), "Game", sf::Style::Default
			//)
			, sf::ContextSettings(24, 8, 0, 3, 3, sf::ContextSettings::Attribute::Default, false));
		//_window->setVerticalSyncEnabled(true);
		std::cout << "Window initialized." << std::endl << std::endl;
		
	}

	void Game::printVersionInfo()
	{
		std::cout << "Context info:" << std::endl;
		std::cout << "----------------------------------" << std::endl;
		//print some debug stats for whoever cares
		const GLubyte *vendor = glGetString(GL_VENDOR);
		const GLubyte *renderer = glGetString(GL_RENDERER);
		const GLubyte *version = glGetString(GL_VERSION);
		const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		//nice consistency here in the way OpenGl retrieves values
		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		printf("GL Vendor : %s\n", vendor);
		printf("GL Renderer : %s\n", renderer);
		printf("GL Version (string) : %s\n", version);
		printf("GL Version (integer) : %d.%d\n", major, minor);
		printf("GLSL Version : %s\n", glslVersion);

		std::cout << "----------------------------------" << std::endl << std::endl;
	}

	void Game::initializeGlew()
	{
		std::cout << "Initializing GLEW..." << std::endl;
		//initialize the opengl extension wrangler
		const GLint glewStatus = glewInit();
		std::cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << std::endl << std::endl;
	}

	void Game::initializeServices()
	{
		//Create
		_gameLoop = new Engine::Core::GameLoop();
		_renderManager = new Engine::Rendering::RenderManager();
		_lightManager = new Engine::Rendering::LightManager();
		_sceneManager = new Engine::SceneManager();
		_colliderManager = new ColliderManager();
		_resourceManager = new ResourceManager();

		//Register
		Engine::ServiceLocator::instance()->addService(this);
		Engine::ServiceLocator::instance()->addService(_resourceManager);
		Engine::ServiceLocator::instance()->addService(_sceneManager);
		Engine::ServiceLocator::instance()->addService(_gameLoop);
		Engine::ServiceLocator::instance()->addService(_lightManager);
		Engine::ServiceLocator::instance()->addService(_renderManager);
		Engine::ServiceLocator::instance()->addService(_colliderManager);


		//_sceneManager->initialize();
		_renderManager->initialize();
	}
	void Game::initializeMusic()
	{
		_resourceManager->cache("Assets/Audio/Sounds/PressurePlateRelease.wav");
		_resourceManager->cache("Assets/Audio/Sounds/PressurePlateRelease1.wav");
		_resourceManager->cache("Assets/Audio/Sounds/StoneDoorClose.wav");
		_resourceManager->cache("Assets/Audio/Sounds/StoneDoorOpen.wav");

		music = new Music;
		music->setVolume(0.15f);
		background = new Engine::Audio::Music("Assets/Audio/Howling wind.wav");
		background->setVolume(0.4f);
		background->setLooping(true);
		background->play();
	}
	void Game::initializeScene() const
	{
		//_sceneManager->loadScene("finalLvl1.json");//->initialize(true, true);
		//_sceneManager->loadScene("finalLvl1.json");//->initialize(true, true);
		//_sceneManager->loadScene("debug.json");//->initialize(true, true);
		//_sceneManager->loadScene(_defaultScenePath);//->initialize(true, true);

	}

	void Game::reset()
	{

	}

	void Game::processEvents()
	{
		sf::Event event{};
		bool exit = false;
		InputHandler::update();

		//we must empty the event queue
		while (_window->pollEvent(event))
		{
			//give all system event listeners a chance to handle events
			//optionally to be implemented by you for example you could implement a
			//SystemEventDispatcher / SystemEventListener pair which allows Listeners to
			//register with the dispatcher and then do something like:
			//SystemEventDispatcher::dispatchEvent(event);

			InputHandler::updateEvent(event);

			switch (event.type)
			{
			case sf::Event::MouseWheelMoved:
			case sf::Event::MouseWheelScrolled:
			{
				//mouseScrollWheel += (event.mouseWheel.delta * 0.01f);
				//if (mouseScrollWheel < 0) mouseScrollWheel = 0;
				break;
			}
			case sf::Event::Closed:
				exit = true;
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Escape)
				{
					UI::Button::TogglePauseMenu();
				}
				break;
			case sf::Event::Resized:
				//would be better to move this to the renderer
				//this version implements nonconstrained match viewport scaling
				glViewport(0, 0, event.size.width, event.size.height);
				break;

			default:
				break;
			}
		}
		if (exit) Game::exit();
		//return event;
	}

	void Game::run()
	{
		_gameLoop->run();
	}
}
