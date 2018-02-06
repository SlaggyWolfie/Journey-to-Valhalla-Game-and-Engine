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
#include "MySceneSP.hpp"

namespace MyGame
{
	FirstSceneSP::FirstSceneSP() : AbstractGame(), _hud(0)
	{
	}

	FirstSceneSP::~FirstSceneSP()
	{
	}

	void FirstSceneSP::initialize()
	{
		AbstractGame::initialize();

		std::cout << "Initializing HUD" << std::endl;
		_hud = new DebugHud(_window);
		std::cout << "HUD Initialized." << std::endl << std::endl;
	}

	void FirstSceneSP::_initializeScene()
	{
		////MESHES
		//std::shared_ptr<Mesh> planeMeshDefault = std::shared_ptr<Mesh>(Mesh::load(config::MGE_MODEL_PATH + "plane.obj"));
		//std::shared_ptr<Mesh> smoothConeMesh = std::shared_ptr<Mesh>(Mesh::load(config::MGE_MODEL_PATH + "cone_smooth.obj"));
		//std::shared_ptr<Mesh> flatConeMesh = std::shared_ptr<Mesh>(Mesh::load(config::MGE_MODEL_PATH + "cone_flat.obj"));
		//std::shared_ptr<Mesh> teapotMesh = std::shared_ptr<Mesh>(Mesh::load(config::MGE_MODEL_PATH + "teapot_flat.obj"));
		//std::shared_ptr<Mesh> suzannaMesh = std::shared_ptr<Mesh>(Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj"));

		////MATERIALS

		//std::shared_ptr<AbstractMaterial> teapotMat = std::shared_ptr<AbstractMaterial>(new ColorMaterial(glm::vec3(0.5f)));
		//std::shared_ptr<AbstractMaterial> suzannaMat = std::shared_ptr<AbstractMaterial>(new ColorMaterial(glm::vec3(1.0f, 0, 0)));
		//std::shared_ptr<AbstractMaterial> landMat = std::shared_ptr<AbstractMaterial>(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg")));
		//std::shared_ptr<AbstractMaterial> brickMat = std::shared_ptr<AbstractMaterial>(new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg")));

		////SCENE
		//Camera* camera = new Camera("Moving Camera", glm::vec3(0, 5, 4));
		//camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
		//camera->setBehaviour(new KeysBehaviour(25));
		//_world->add(camera);
		//_world->setMainCamera(camera);

		//GameObject* floor = new GameObject("Floor");
		//floor->scale(glm::vec3(3, 3, 3));
		//floor->setMesh(planeMeshDefault.get());
		//floor->setMaterial(landMat.get());
		//floor->setBehaviour(new RotatingBehaviour());
		//_world->add(floor);

		//GameObject* coneSmooth = new GameObject("Cone Smooth", glm::vec3(4, 4, 0));
		//coneSmooth->setMesh(smoothConeMesh.get());
		//coneSmooth->setMaterial(landMat.get());
		//_world->add(coneSmooth);

		//GameObject* coneFlat = new GameObject("Cone Flat", glm::vec3(-4, 4, 0));
		//coneFlat->setMesh(flatConeMesh.get());
		//coneFlat->setMaterial(landMat.get());
		//_world->add(coneFlat);

		//GameObject* teapot = new GameObject("Teapot", glm::vec3(0, 0.5f, 0));
		//teapot->setMesh(teapotMesh.get());
		//teapot->setMaterial(teapotMat.get());
		//teapot->scale(glm::vec3(0.1f, 0.1f, 0.1f));
		//floor->add(teapot);

		//GameObject* suzanna = new GameObject("Suzanna", glm::vec3(0.3f, 0.5f, 0.0f));
		//suzanna->setMesh(suzannaMesh.get());
		//suzanna->setMaterial(suzannaMat.get());
		//suzanna->scale(glm::vec3(0.1f));
		//floor->add(suzanna);
	}

	void FirstSceneSP::_render()
	{
		AbstractGame::_render();
		_updateHud();
	}

	void FirstSceneSP::_updateHud() {
		std::string debugInfo = "";
		debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

		_hud->setDebugInfo(debugInfo);
		_hud->draw();
	}
}