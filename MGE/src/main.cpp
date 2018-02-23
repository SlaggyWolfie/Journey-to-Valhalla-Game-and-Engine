#include <iostream>
#include <cstdio>

#include "mge/core/AbstractGame.hpp"
#include "mge/MGEDemo.hpp"
#include "myGame/Scenes/MyScene.hpp"
#include "myGame/Scenes/SecondScene.hpp"
#include "myGame/Scenes/ThirdScene.hpp"
#include "myGame/Scenes/FourthScene.hpp"
#include "myGame/Scenes/FifthScene.hpp"
#include "myGame/Scenes/SixthScene.hpp"
#include "myGame/Scenes/SeventhScene.hpp"
#include "myGame/Scenes/EighthScene.hpp"
#include "myGame/Scenes/NinthScene.hpp"
#include <lua\lua.hpp>
#include "Core\LuaScript.h"
#include "Core/Game.hpp"

/**
 * Main entry point for the Micro Engine.

 * Design goals:
 * - correct division of OpenGL into appropriate wrapper classes
 * - simple design
 * - each class should know as little as possible about other classes and non related parts of OpenGL
 * - the engine must allow different objects with different transforms, meshes, materials (shaders) etc
 * - consistent coding conventions
 * - reusable and extendable core set of classes which should require no modification
 *   (in other words it is possible to have a working "empty" example)
 *
 * All documentation is contained within the HEADER files, not the CPP files if possible.
 *
 */
 //using namespace demo;
 //using namespace MyGame;

int main()
{
	std::cout << "Starting Game" << std::endl;

	LuaScript* luaS = new LuaScript();
	luaS->start();
	luaS->update();
	//AbstractGame* game = new MyGame::FourthScene();
	//AbstractGame* game = new MyGame::FourthScene();
	//AbstractGame* game = new MyGame::FourthScene();
	//AbstractGame* game = new MyGame::FourthScene();
	//AbstractGame* game = new MyGame::SixthScene();
	//AbstractGame* game = new MyGame::NinthScene();
	Engine::Game* game = new Engine::Game();

	game->initialize();
	game->run();
	game->exit();

	delete game;

	return 0;
}