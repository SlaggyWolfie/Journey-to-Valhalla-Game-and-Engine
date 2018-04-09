#include <iostream>

#include "Game.hpp"
#include "GameLoop.hpp"

#include "../../externals/glew_sfml_glm/include/lua/lua.hpp"
//#include "RenderManager.hpp"
#include "../_vs2015/RenderManager.hpp"
#include "../_vs2015/LightManager.hpp"
#include "../_vs2015/SceneManager.hpp"
#include "../_vs2015/ServiceLocator.hpp"
#include "../_vs2015/Scene.hpp"
#include "../_vs2015/Texture_.hpp"
#include "Deserealizer.hpp"
#include <glm.hpp>
#include "LuaScript.h"
#include "../../_vs2015/InputHandler.hpp"

//#include "GameLoop.hpp"

namespace Engine
{
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


		////make sure we test the depthbuffer
		//glEnable(GL_DEPTH_TEST);

		////tell opengl which vertex winding is considered to be front facing
		//glFrontFace(GL_CCW);

		////tell opengl to enable face culling for the back faces
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_FRAMEBUFFER_SRGB);

		//glClearColor(
		//	static_cast<float>(0x2d) / 0xff,
		//	static_cast<float>(0x6b) / 0xff,
		//	static_cast<float>(0xce) / 0xff,
		//	1.0f);

		LuaScript l = LuaScript();

		l.Initialize();
		l.start();
		
		//initializeScene(); 
		
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glDisable(GL_BLEND);
		////glEnable(GL_BLEND);

		//glDepthFunc(GL_LESS);
		//glDepthMask(GL_TRUE);

		//glCullFace(GL_BACK);

		//glViewport(0, 0, getWindow()->getSize().x, getWindow()->getSize().y);

		//glm::vec3 translation = glm::vec3(0, 0, 3);
		//glm::vec3 point1 = glm::vec3(3, 0, -3);
		//glm::vec3 point2 = glm::vec3(-3, 0, -3);

		//glm::mat4 model = glm::translate(translation);
		//glm::mat4 view = glm::translate(-translation);
		//glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1280.0f / 1024.0f, 0.1f, 100.0f);

		//while (true)
		//{
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	glUseProgram(0);
		//	glMatrixMode(GL_PROJECTION);
		//	glLoadMatrixf(glm::value_ptr(proj));
		//	glMatrixMode(GL_MODELVIEW);
		//	glLoadMatrixf(glm::value_ptr(view * model));

		//	glBegin(GL_LINES);
		//	glColor3fv(glm::value_ptr(glm::vec3(1, 0, 0)));
		//	glVertex3fv(glm::value_ptr(point1));
		//	glVertex3fv(glm::value_ptr(point2));
		//	glEnd();

		//	sf::Event event;
		//	while (_window->pollEvent(event)) {}
		//	_window->display();
		//}
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

	///SETUP

	void Game::initializeWindow() {

		std::cout << "Initializing window..." << std::endl;
		_window = new sf::RenderWindow(
			sf::VideoMode(1280, 1024), "Game", sf::Style::Default
			//)
			, sf::ContextSettings(24, 8, 0, 3, 3, sf::ContextSettings::Attribute::Default, true));
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

		//Register
		Engine::ServiceLocator::instance()->addService(this);
		Engine::ServiceLocator::instance()->addService(_sceneManager);
		Engine::ServiceLocator::instance()->addService(_gameLoop);
		Engine::ServiceLocator::instance()->addService(_lightManager);
		Engine::ServiceLocator::instance()->addService(_renderManager);
		Engine::ServiceLocator::instance()->addService(_colliderManager);


		//_sceneManager->initialize();
		_renderManager->initialize();
	}

	void Game::initializeScene() const
	{
		//_sceneManager->loadScene("finalLvl1.json");//->initialize(true, true);
		//_sceneManager->loadScene("finalLvl1.json");//->initialize(true, true);
		_sceneManager->loadScene("debug.json");//->initialize(true, true);

	}

	void Game::reset()
	{
		
	}

	void Game::processEvents()
	{
		sf::Event event{};
		bool exit = false;

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
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) 
					exit = true;
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
