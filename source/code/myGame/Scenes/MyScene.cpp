#include <iostream>
#include <string>
#include <memory>

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

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "myGame/MyGameConfig.hpp"
#include "MyScene.hpp"

namespace MyGame
{
	FirstScene::FirstScene() : AbstractGame(), _hud(0)
	{
	}

	FirstScene::~FirstScene()
	{
	}

	void FirstScene::initialize()
	{
		AbstractGame::initialize();

		std::cout << "Initializing HUD" << std::endl;
		_hud = new DebugHud(_window);
		std::cout << "HUD Initialized." << std::endl << std::endl;
	}

	void FirstScene::_initializeScene()
	{
		//MESHES
		Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
		Mesh* smoothConeMesh = Mesh::load(config::MGE_MODEL_PATH + "cone_smooth.obj");
		Mesh* flatConeMesh = Mesh::load(config::MGE_MODEL_PATH + "cone_flat.obj");
		Mesh* teapotMesh = Mesh::load(config::MGE_MODEL_PATH + "teapot_flat.obj");
		Mesh* suzannaMesh = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");

		//MATERIALS

		AbstractMaterial* teapotMat = new ColorMaterial(glm::vec3(0.5f));
		AbstractMaterial* suzannaMat = new ColorMaterial(glm::vec3(1.0f, 0, 0));
		AbstractMaterial* landMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"));
		AbstractMaterial* brickMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

		//SCENE
		Camera* camera = new Camera("Moving Camera", glm::vec3(0, 5, 4));
		camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
		camera->setBehaviour(new KeysBehaviour(25));
		_world->add(camera);
		_world->setMainCamera(camera);

		GameObject* floor = new GameObject("Floor");
		floor->scale(glm::vec3(3, 3, 3));
		floor->setMesh(planeMeshDefault);
		floor->setMaterial(landMat);
		floor->setBehaviour(new RotatingBehaviour());
		_world->add(floor);

		GameObject* coneSmooth = new GameObject("Cone Smooth", glm::vec3(2, 2, 0));
		coneSmooth->setMesh(smoothConeMesh);
		coneSmooth->setMaterial(landMat);
		_world->add(coneSmooth);

		GameObject* coneFlat = new GameObject("Cone Flat", glm::vec3(-2, 2, 0));
		coneFlat->setMesh(flatConeMesh);
		coneFlat->setMaterial(landMat);
		_world->add(coneFlat);

		GameObject* teapot = new GameObject("Teapot", glm::vec3(0, 0.5f, 0));
		teapot->setMesh(teapotMesh);
		teapot->setMaterial(teapotMat);
		teapot->scale(glm::vec3(0.1f, 0.1f, 0.1f));
		floor->add(teapot);

		GameObject* suzanna = new GameObject("Suzanna", glm::vec3(0.3f, 0.5f, 0.0f));
		suzanna->setMesh(suzannaMesh);
		suzanna->setMaterial(suzannaMat);
		suzanna->scale(glm::vec3(0.1f));
		floor->add(suzanna);
	}

	void FirstScene::_render()
	{
		AbstractGame::_render();
		_updateHud();
	}

	void FirstScene::_updateHud() {
		std::string debugInfo = "";
		debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

		_hud->setDebugInfo(debugInfo);
		_hud->draw();
	}
}