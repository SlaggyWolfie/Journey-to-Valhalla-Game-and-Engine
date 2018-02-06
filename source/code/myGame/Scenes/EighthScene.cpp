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
#include "myGame/Scenes/EighthScene.hpp"
#include "myGame/Materials/LitMaterial.hpp"
#include "myGame/Materials/LitTextureMaterial.hpp"
#include "myGame/Materials/LitTerrainMaterial.hpp"
#include "myGame/Materials/WaterMaterial.hpp"

namespace MyGame
{
	//construct the game class into _window, _renderer and hud (other parts are initialized by build)
	EighthScene::EighthScene() :AbstractGame(), _hud(0)
	{
	}

	void EighthScene::initialize() {
		//setup the core part
		AbstractGame::initialize();

		//setup the custom part so we can display some text
		std::cout << "Initializing HUD" << std::endl;
		_hud = new DebugHud(_window);
		std::cout << "HUD initialized." << std::endl << std::endl;
	}

	//build the game _world
	void EighthScene::_initializeScene()
	{
		//MESHES
		Mesh* planeMeshHighPoly = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");
		Mesh* cubeMesh = Mesh::load(config::MGE_MODEL_PATH + "cube_flat.obj");

		//MATERIALS
		LitTerrainMaterial* terrainMaterial = new LitTerrainMaterial(
			Texture::load(MyGame::TEXTURE_PATH + "terrain/myterrain_raw.png"),
			Texture::load(MyGame::TEXTURE_PATH + "terrain/SplatmapInvisible.png"),
			//Texture::load(config::MGE_TEXTURE_PATH + "terrain/heightmap.png"),
			//Texture::load(config::MGE_TEXTURE_PATH + "terrain/splatmap.png"),
			Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse2.jpg"),
			Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse1.jpg"),
			Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse3.jpg"),
			Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse4.jpg"), 
			513/513, 513/513, 2 * 600/513.0f);

		AbstractMaterial* waterMaterial =
			new WaterMaterial(Texture::load(config::MGE_TEXTURE_PATH + "terrain/water.jpg"));
			//new TextureMaterial(Texture::load(config::MGE_TEXTURE_PATH + "terrain/water.jpg"));
			//new ColorMaterial(glm::vec3(0, 0, 1));

		//SCENE

		GameObject* floor = new GameObject("Floor", glm::vec3(0, 0, 0));
		floor->scale(glm::vec3(3));
		floor->setMesh(planeMeshHighPoly);
		floor->setMaterial(terrainMaterial);
		_world->add(floor);

		GameObject* water = new GameObject("Water", glm::vec3(0, 0.2f, 0));
		water->scale(glm::vec3(6));
		water->setMesh(planeMeshHighPoly);
		water->setMaterial(waterMaterial);
		floor->add(water);

		CameraOrbitBehaviour* orbitCamera;
		Camera* camera = new Camera("Camera", glm::vec3(0, 14, 14));
		camera->rotate(glm::radians(-45.0f), glm::vec3(1, 0, 0));
		camera->setBehaviour(new KeysBehaviour(5));
		//camera->setBehaviour(
		//		(orbitCamera =
		//			new CameraOrbitBehaviour(floor,
		//				90, 45, 135,
		//				90,
		//				25, 0.5f, 10)));
		_world->add(camera);
		_world->setMainCamera(camera);

		Light::setAmbientLightColor(glm::vec3(1, 1, 1));
		//light->setAmbientLightColor(glm::vec3(0.1f, 0, 0));
		//Light::setAmbientContribution(1);
		Light::setAmbientContribution(0.01f);

		//Light::setAttenuation();
		Light::setAttenuation(0.11f, 0.5f, 0.2f);
		//Light::setAttenuation(1, 0.7f, 1.8f);

		//Lights
		CameraOrbitBehaviour* orbit;
		ColorMaterial* lightMat = new ColorMaterial(glm::vec3(1, 1, 1));
		Light* light = new Light("White Light", glm::vec3(0, 5, 4));
		//ColorMaterial* lightMat = new ColorMaterial(glm::vec3(1, 1, 0));
		//Light* light = new Light("Yellow Light", glm::vec3(0, 5, 4));
		light->setBehaviour(new KeysBehaviour(15));
		light->setBehaviour(
			(orbit =
				new CameraOrbitBehaviour(floor,
					90, 45, 135,
					90,
					7, 0.5f, 10)));

		light->setColor(glm::vec3(1, 1, 1));
		light->rotate(-45, glm::vec3(1, 0, 0));
		//light->setColor(glm::vec3(1, 1, 0));
		//light->setLightIntensity(0.1f);

		light->scale(glm::vec3(0.1f));
		light->setMesh(cubeMesh);
		light->setMaterial(lightMat);

		light->setLightType(Light::Type::Directional);
		//light->setLightType(Light::Type::Point);
		//light->setLightType(Light::Type::Spotlight);

		_world->add(light);

		AbstractMaterial* lightMatRed = new ColorMaterial(glm::vec3(1, 0, 0));
		Light* lightRed = new Light("Red Light", glm::vec3(0, 6, 0));
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

	void EighthScene::_render() {
		AbstractGame::_render();
		_updateHud();
	}

	void EighthScene::_updateHud() {
		std::string debugInfo = "";
		debugInfo += std::string("FPS:") + std::to_string((int)_fps) + "\n";

		_hud->setDebugInfo(debugInfo);
		_hud->draw();
	}

	EighthScene::~EighthScene()
	{
		//dtor
		delete _hud;
	}
}