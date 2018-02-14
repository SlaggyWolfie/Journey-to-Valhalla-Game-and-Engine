#include <iostream>

#include "Game.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"
#include "GameLoop.hpp"
Game::Game() 
{
	//ctor
}

Game::~Game()
{
	//dtor
}

void Game::initialize() {
	std::cout << "Initializing engine..." << std::endl << std::endl;
	_initializeWindow();
	_printVersionInfo();
	_initializeGlew();
	_initializeRenderer();
	_initializeWorld();
	_initializeScene();


	//scenes 
	GameLoop* _gameLoop = new GameLoop(_renderer);
	 //GameLoop gameLoop();
	//renderUpdate()
	//update()

	std::cout << std::endl << "Engine initialized." << std::endl << std::endl;
}

///SETUP

void Game::_initializeWindow() {
	std::cout << "Initializing window..." << std::endl;
	_window = new sf::RenderWindow(sf::VideoMode(800, 600), "My Game!", sf::Style::Default, sf::ContextSettings(24, 8, 0, 3, 3));
	//_window->setVerticalSyncEnabled(true);
	std::cout << "Window initialized." << std::endl << std::endl;
}

void Game::_printVersionInfo() {
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

void Game::_initializeGlew() {
	std::cout << "Initializing GLEW..." << std::endl;
	//initialize the opengl extension wrangler
	GLint glewStatus = glewInit();
	std::cout << "Initialized GLEW, status (1 == OK, 0 == FAILED):" << (glewStatus == GLEW_OK) << std::endl << std::endl;
}

void Game::_initializeRenderer() {
	//setup our own renderer
	std::cout << "Initializing renderer..." << std::endl;
	_renderer = new Renderer();
	_renderer->setClearColor(0, 0, 0);
	std::cout << "Renderer done." << std::endl << std::endl;
}

void Game::_initializeWorld() {
	//setup the world
	std::cout << "Initializing world..." << std::endl;
	_world = new World();
	std::cout << "World initialized." << std::endl << std::endl;
}



// create scene here 
//renderer renderer
//scene scene;

//GameLoop _gameLoop;
//_gameLoop->Update();
//_gameLoop->RenderUpdate();
//updates all the gameObjcts
//_gameLoop has the list of stuff to update which it updates gameobj.update()