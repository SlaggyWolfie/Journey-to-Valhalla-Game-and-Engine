#include <iostream>

#include "Game.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "GameLoop.hpp"

#include "../../externals/glew_sfml_glm/include/lua/lua.hpp"
//#include "RenderManager.hpp"
#include "../_vs2015/RenderManager.hpp"
#include "../_vs2015/LightManager.hpp"
#include "../_vs2015/SceneManager.hpp"
#include "../_vs2015/ServiceLocator.hpp"
#include "../../_vs2015/GameObject_.hpp"
#include "../_vs2015/Scene.hpp"
#include "../_vs2015/Model.hpp"
#include "../_vs2015/Texture_.hpp"
#include <map>
#include "../_vs2015/Camera_.hpp"
#include "../../_vs2015/Light_.hpp"
#include "../_vs2015/Transform.hpp"
#include "Deserealizer.hpp"

//#include "GameLoop.hpp"

namespace Engine
{
	Game::Game()
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

		initializeScene();
	}

	sf::RenderWindow* Game::getWindow() const
	{
		return _window.get();
	}

	bool Game::currentlyRunning() const
	{
		if (_window == nullptr && _window.get() == nullptr) return false;
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
		_window = std::make_unique<sf::RenderWindow>(
			sf::VideoMode(1280, 960), "Thot", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3));
		//_window->setVerticalSyncEnabled(true);
		//std::cout << std::to_string(_window.get() != nullptr) << std::endl;
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

		//Register
		Engine::ServiceLocator::instance()->addService(this);
		Engine::ServiceLocator::instance()->addService(_sceneManager);
		Engine::ServiceLocator::instance()->addService(_gameLoop);
		Engine::ServiceLocator::instance()->addService(_lightManager);
		Engine::ServiceLocator::instance()->addService(_renderManager);

		_sceneManager->initialize();
		_renderManager->initialize();
		_gameLoop->initialize();
		_lightManager->initialize();
	}

	void Game::initializeScene() const
	{
		initializeScene(_defaultScenePath);
	}


	void Game::initializeScene(const std::string& filepath) const
	{
		//Deserealizer _d;
		//Core::GameObject_* sth = Model::loadModel(_d.gms[0]->getName()+".fbx");
		//sth->getTransform()->setPosition(_d.gms[0]->getTransform()->getPosition());
		//std::cout << sth->getTransform()->getPosition();
		std::cout << "Loading Scene..." << std::endl;
		//load scene
		//Core::GameObject_* camera = new Core::GameObject_("Cam", "", glm::vec3(0, 0, 2000));
		//Core::GameObject_* lightgo = new Core::GameObject_("Light", "", glm::vec3(0,0,2000));
		//Rendering::Light_* light = new Rendering::Light_();
		//lightgo->addComponent(light);
		//light->setLightType(Rendering::LightType::Point);
		//lightgo->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		//Core::Camera_* cameraComp = new Core::Camera_();
		//camera->addComponent(cameraComp);
		//Core::Camera_::setMainCamera(cameraComp);

		//Core::GameObject_* go = Model::loadModel("Tower.fbx");
		//go->getTransform()->setPosition(go->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//lightgo->getTransform()->setPosition(go->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//Core::GameObject_* go = Model::loadModel("cube_smooth.obj");

		//std::cout << go->getComponent<Rendering::Renderer_>()->getGameObject()->getName() << std::endl;
		//go->destroy();
		_sceneManager->loadScene(filepath)->initialize(true);
		std::cout << "Loaded Scene." << std::endl;

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
				if (event.key.code == sf::Keyboard::Escape) {
					exit = true;
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
