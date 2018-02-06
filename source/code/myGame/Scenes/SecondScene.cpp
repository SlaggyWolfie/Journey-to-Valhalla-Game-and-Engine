#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "myGame/Behaviours/CameraOrbitBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "myGame/Scenes/SecondScene.hpp"

namespace MyGame
{
	//construct the game class into _window, _renderer and hud (other parts are initialized by build)
	SecondScene::SecondScene() :AbstractGame(), _hud(0)
	{
	}

	void SecondScene::initialize() {
		//setup the core part
		AbstractGame::initialize();

		//setup the custom part so we can display some text
		std::cout << "Initializing HUD" << std::endl;
		_hud = new DebugHud(_window);
		std::cout << "HUD initialized." << std::endl << std::endl;
	}

	//build the game _world
	void SecondScene::_initializeScene()
	{
		//MESHES

		//load a bunch of meshes we will be using throughout this demo
		//each mesh only has to be loaded once, but can be used multiple times:
		//F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
		Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
		Mesh* sphereMeshS = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");

		//MATERIALS

		//create some materials to display the cube, the plane and the light
		AbstractMaterial* runicStoneMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "runicfloor.png"));
		AbstractMaterial* bricksMaterial = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

		//SCENE SETUP

		//add the floor
		GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
		plane->scale(glm::vec3(5, 5, 5));
		plane->setMesh(planeMeshDefault);
		plane->setMaterial(runicStoneMaterial);
		_world->add(plane);

		GameObject* wall = new GameObject("wall", glm::vec3(0, 0, 0));
		wall->translate(glm::vec3(0, 2.5f, -5));
		wall->rotate(90 * 3.14159265f / 180, glm::vec3(1, 0, 0));
		wall->scale(glm::vec3(5, 5, 5));
		wall->setMesh(planeMeshDefault);
		wall->setMaterial(bricksMaterial);
		_world->add(wall);

		//add a moving sphere
		GameObject* sphere = new GameObject("sphere", glm::vec3(0, 0, 0));
		sphere->scale(glm::vec3(2.5, 2.5, 2.5));
		sphere->setMesh(sphereMeshS);
		sphere->setMaterial(runicStoneMaterial);
		sphere->setBehaviour(new KeysBehaviour());
		_world->add(sphere);

		//add camera 
		Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
		//camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
		camera->setBehaviour(
			new CameraOrbitBehaviour(sphere, 
				90, 45, 75,
				90,
				7, 0.5f)
		);
		
		_world->add(camera);
		_world->setMainCamera(camera);
	}

	void SecondScene::_render() {
		AbstractGame::_render();
		_updateHud();
	}

	void SecondScene::_updateHud() {
		std::string debugInfo = "";
		debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

		_hud->setDebugInfo(debugInfo);
		_hud->draw();
	}

	SecondScene::~SecondScene()
	{
		//dtor
	}
}