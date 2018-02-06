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
#include "myGame/Materials/WobbleMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "myGame/Behaviours/CameraOrbitBehaviour.hpp"
#include "myGame/Behaviours/ElevationBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "myGame/MyGameConfig.hpp"
#include "myGame/Scenes/SixthScene.hpp"
#include "myGame/Materials/LitMaterial.hpp"

namespace MyGame
{
	//construct the game class into _window, _renderer and hud (other parts are initialized by build)
	SixthScene::SixthScene() :AbstractGame(), _hud(0)
	{
	}

	void SixthScene::initialize() {
		//setup the core part
		AbstractGame::initialize();

		//setup the custom part so we can display some text
		std::cout << "Initializing HUD" << std::endl;
		_hud = new DebugHud(_window);
		std::cout << "HUD initialized." << std::endl << std::endl;
	}

	//build the game _world
	void SixthScene::_initializeScene()
	{
		//MESHES
		Mesh* planeMeshDefault = Mesh::load(config::MGE_MODEL_PATH + "plane.obj");
		Mesh* teapotMesh = Mesh::load(config::MGE_MODEL_PATH + "teapot_smooth.obj");
		Mesh* suzannaMesh = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");
		Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");
		Mesh* highPolyPlane = Mesh::load(config::MGE_MODEL_PATH + "plane20x20_2048tris_aligned_uvs.obj");
		Mesh* highPolySphere = Mesh::load(config::MGE_MODEL_PATH + "sphere_320_triangles_smooth.obj");

		//MATERIALS

		AbstractMaterial* teapotMat = new LitMaterial(LitMaterial::ShaderLit::Fragment, glm::vec3(0.5f));
		AbstractMaterial* suzannaMat = new LitMaterial(LitMaterial::ShaderLit::Fragment, glm::vec3(0, 0, 1));
		LitMaterial* landLitMat = new LitMaterial(LitMaterial::ShaderLit::Fragment, glm::vec3(1, 1, 1));
		AbstractMaterial* landMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "land.jpg"));
		AbstractMaterial* brickMat = new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg"));

		//SCENE
		Camera* camera = new Camera("Camera", glm::vec3(0, 5, 4));
		camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
		camera->setBehaviour(new KeysBehaviour(5));
		_world->add(camera);
		_world->setMainCamera(camera);

		GameObject* floor = new GameObject("Floor");
		floor->scale(glm::vec3(3, 3, 3));
		floor->setMesh(planeMeshDefault);
		//floor->setMesh(highPolyPlane);
		//floor->scale(glm::vec3(0.1f));
		//floor->scale(glm::vec3(10));
		floor->setMaterial(landLitMat);
		//floor->setBehaviour(new RotatingBehaviour());
		_world->add(floor);

		GameObject* teapot = new GameObject("Teapot", glm::vec3(0, 0.5f, 0));
		teapot->setMesh(teapotMesh);
		teapot->setMaterial(teapotMat);
		teapot->scale(glm::vec3(0.25f));
		_world->add(teapot);

		GameObject* suzanna = new GameObject("Suzanna", glm::vec3(1, 0.5f, 0.0f));
		suzanna->setMesh(suzannaMesh);
		suzanna->setMaterial(suzannaMat);
		suzanna->scale(glm::vec3(0.25f));
		_world->add(suzanna);

		Light::setAmbientLightColor(glm::vec3(1, 1, 1));
		//light->setAmbientLightColor(glm::vec3(0.1f, 0, 0));
		Light::setAmbientContribution(0.01f);

		//Light::setAttenuation();
		//Light::setAttenuation(0.11f, 0.5f, 0.2f);
		Light::setAttenuation(1, 0.7f, 1.8f);

		//Lights
		CameraOrbitBehaviour* orbit;
		ColorMaterial* lightMat = new ColorMaterial(glm::vec3(1, 1, 0));
		Light* light = new Light("Yellow Light", glm::vec3(0, 5, 4));
		light->setBehaviour(
			(orbit =
				new CameraOrbitBehaviour(teapot,
					90, 45, 135,
					90,
					3, 0.5f, 10)));
		//orbit->setScale(glm::vec3(0.3f));

		landLitMat->setShininess(10);
		//landLitMat->setSpecularStrength(0.1f);
		//landLitMat->setSpecularStrength(0.5f);
		light->setColor(glm::vec3(1, 1, 0));
		light->setLightIntensity(0.1f);

		light->scale(glm::vec3(0.1f));
		light->setMesh(cubeMesh);
		light->setMaterial(lightMat);

		//light->setLightType(Light::Type::Directional);
		//light->setLightType(Light::Type::Point);
		light->setLightType(Light::Type::Spotlight);

		_world->add(light);

		AbstractMaterial* lightMatRed = new ColorMaterial(glm::vec3(1, 0, 0));
		Light* lightRed = new Light("Red Light", glm::vec3(0, 2, 0));
		lightRed->setBehaviour(new ElevationBehaviour(15));
		lightRed->setColor(glm::vec3(1, 0, 0));
		lightRed->scale(glm::vec3(0.1f));
		lightRed->setMesh(cubeMesh);
		lightRed->setMaterial(lightMatRed);
		lightRed->setLightType(Light::Type::Point);
		lightRed->setLightIntensity(1);
		//lightRed->_setWorldRecursively();
		_world->add(lightRed);
	}

	void SixthScene::_render() {
		AbstractGame::_render();
		_updateHud();
	}

	void SixthScene::_updateHud() {
		std::string debugInfo = "";
		debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

		_hud->setDebugInfo(debugInfo);
		_hud->draw();
	}

	SixthScene::~SixthScene()
	{
		//dtor
		delete _hud;
	}
}