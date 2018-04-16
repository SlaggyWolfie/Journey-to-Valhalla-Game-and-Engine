#include "Scene.hpp"
#include <utility>
#include "GameObject_.hpp"
#include "../_vs2015/Model.hpp"
#include "../_vs2015/Texture_.hpp"
#include "../_vs2015/Camera_.hpp"
#include "../_vs2015/Light_.hpp"
#include "../_vs2015/Transform.hpp"
#include "LightManager.hpp"
#include "ServiceLocator.hpp"
#include "../src/Core/Deserealizer.hpp"
#include "PlayerBaseComponent.h"
#include "collider.h"
#include "Core\LuaScript.h"
#include "PressurePlateBehaviour.h"
#include "GateBehaviour.h"
#include "Material_.hpp"
#include "Button.hpp"
#include "ServiceLocator.hpp"
#include "Core\Game.hpp"
#include "Time.hpp"
#include "SceneManager.hpp"
#include "RotatingComponent.hpp"
#include "Deserializer2.hpp"
#include "Text.hpp"
#include "AudioListener.hpp"

namespace Engine
{
	using namespace Rendering;
	Scene::Scene() : _name(""), _path(""), _gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
	{
	}

	Scene::Scene(std::string name, std::string path) : _name(std::move(name)), _path(std::move(path))
	{
	}

	Scene::~Scene()
	{
		for (auto& go : _gameObjects)
			go = nullptr;
		_gameObjects.clear();
	}

	void Scene::initialize(const bool hard, const bool fromFile)
	{
		std::cout << "Loading Scene " + _name << std::endl;

		//return;
		if (fromFile)
		{


			Deserializer2 deserializer2;
			deserializer2.deserializeIntoStructs(_path);
			//deserializer2.deserializeIntoStructs("Level_1.json");
			deserializeStructs2(deserializer2.gameStructs);

			//Model::debug(true);
			//Model::clipPaths = false;
			//Deserealizer deserealizer;
			//// deserealizer.deserializeIntoStructs("level1.json");
			//deserealizer.deserializeIntoStructs(_path);
			//deserializeStructs(deserealizer.structs, false);
		}
		if (hard)
			hardCode();
		if (hard && fromFile)
			neededHardCode();

		std::cout << "Loaded Scene " + _name << std::endl;
	}

	Core::GameObject_* Scene::findGameObject(const std::string& name, const bool exact) const
	{
		if (exact)
		{
			for (const auto& gameObject : _gameObjects)
				if (gameObject->getName() == name)
					return gameObject.get();
		}
		else
		{
			for (const auto& gameObject : _gameObjects)
				if (gameObject->getName().find(name) != std::string::npos)
					return gameObject.get();
		}

		return nullptr;
	}

	std::vector<Core::GameObject_*> Scene::findGameObjects(const std::string& name, const bool exact) const
	{
		std::vector<Core::GameObject_*> gameObjects;
		if (exact)
		{
			for (const auto& gameObject : _gameObjects)
				if (gameObject->getName() == name)
					gameObjects.push_back(gameObject.get());
		}
		else
		{
			for (const auto& gameObject : _gameObjects)
				if (gameObject->getName().find(name) != std::string::npos)
					gameObjects.push_back(gameObject.get());
		}

		return gameObjects;
	}

	Core::GameObject_** Scene::getGameObjectsArray() const
	{
		auto gameObjects = getGameObjects();
		const auto array = new Core::GameObject_*[static_cast<int>(gameObjects.size())];
		std::copy(gameObjects.begin(), gameObjects.end(), array);
		return array;
	}

	std::vector<Core::GameObject_*> Scene::getGameObjects() const
	{
		std::vector<Core::GameObject_*> vector = std::vector<Core::GameObject_*>();
		for (const auto & gameObject : _gameObjects)
			vector.push_back(gameObject.get());
		return vector;
	}

	std::string Scene::getName() const
	{
		return _name;
	}

	void Scene::addGameObject(Core::GameObject_* gameObject)
	{
		_gameObjects.push_back(std::unique_ptr<Core::GameObject_>(gameObject));
	}

	void Scene::deserializeStructs(std::vector<GameObject_s> structs, const bool clipPaths)
	{
		std::unordered_map<int, GameObject_*> id_to_go;
		//std::unordered_map<GameObject_*, int> go_to_id;

		std::cout << "Deserealizing " + std::to_string(structs.size()) + " objects." << std::endl;

		//first pass - initialize GOs
		for (GameObject_s& gameStruct : structs)
		{
			//std::cout << "\tObject " + std::to_string(++index) + "." << std::endl;
			GameObject_* gameObject = nullptr;

			if (gameStruct.name.find("Runestone") != std::string::npos)
			{
				std::cout << "runestone" << std::endl;
				gameStruct.meshName = "Crate.fbx";
				gameStruct.name = "Crate";
			}

			if (!gameStruct.meshName.empty() && gameStruct.meshName != std::string("")
				&& gameStruct.meshName.find('.') != std::string::npos
				)
			{
				if (clipPaths)
				{
					while (gameStruct.meshName.find('/') != std::string::npos)
					{
						gameStruct.meshName = gameStruct.meshName.substr(gameStruct.meshName.find('/') + 1, std::string::npos);
						std::cout << gameStruct.meshName << std::endl;
					}
				}


				/*if (gameStruct.meshName.find("Pine") != std::string::npos)
				{
					gameStruct.meshName = "";
					continue;
				}*/

				if (gameStruct.meshName.find("default") != std::string::npos)
				{
					//continue;
					if (gameStruct.name.find("Cylinder") != std::string::npos)
						gameStruct.meshName = "Cylinder.fbx";
				}
				//if (gameStruct.meshName.find("Main character") != std::string::npos)
				//{
				//	//continue;
				//	std::cout << "Getting here" << std::endl;
				//	gameStruct.meshName = "test-for-Slavi.obj";
				//}
				//if (gameStruct.name.find("default") != std::string::npos)
				//{
				//	gameStruct.meshName = "tiles.obj";
				//}

				//if (gameStruct.name.find("room1") != std::string::npos)
				//{
				//	std::cout << "rendering room" << std::endl;
				//	gameStruct.meshName = "roomFinal.obj";
				//}
				//if (gameStruct.meshName.find("default") != std::string::npos)
				//{
				//	//continue;
				//	if (gameStruct.name.find("Cylinder") != std::string::npos)
				//		gameStruct.meshName = "Cylinder.fbx";
				//	if ((gameStruct.name.find("Runestone") != std::string::npos))
				//	{
				//		gameStruct.meshName = "Crate.fbx";
				//	}
				//}


				//gameStruct.meshName.replace();
				//gameStruct.meshName.replace(
				//	gameStruct.meshName.end()-4,
				//	gameStruct.meshName.end(), ".obj");

				gameObject = Model::loadModel(gameStruct.meshName);
			}
			else
			{
				gameObject = new GameObject_("", "");
			}

			//if (gameStruct.name.find("Crate") != std::string::npos)
			//{
			//	gameObject->addComponent(new collider());
			//	gameObject->getComponent<collider>()->SetBoxSize(50, 50, 50);

			//	//gameObject->addComponent(new PlayerBaseComponent());
			//}

			//if (gameStruct.name.find("Gate") != std::string::npos)
			//{
			//	gameObject->addComponent(new collider());
			//	gameObject->addComponent(new GateBehaviour());

			//	for (const auto& plateGameObject : findGameObjects("Pressure plate ", false))
			//		gameObject->getComponent<GateBehaviour>()->AddPlate(plateGameObject->getComponent<PressurePlateBehaviour>());
			//}

			if (gameObject == nullptr) return;
			gameObject->setName(gameStruct.name);
			//if (gameObject->getName() == "default")
			//{
			//	gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load("Assets/Materials/Texture Maps/Grass_ep_basecolor_004.png"));
			//}

			//if (gameObject->getName() == "room1")
			//{
			//	gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load("Assets/Materials/Texture Maps/RockTiles_basecolor.png"));
			//}

			Transform* transform = gameObject->getTransform();
			gameStruct.position.x *= -1;
			transform->setLocalPosition(gameStruct.position);
			std::cout << gameStruct.name + " Position: " + glm::to_string(gameStruct.position) << std::endl << std::endl;
			gameStruct.rotation.y *= -1;
			gameStruct.rotation.z *= -1;
			transform->setLocalRotation(gameStruct.rotation);


			if (gameStruct.colliderSize.x != 0.0f || gameStruct.colliderSize.y != 0.0f || gameStruct.colliderSize.z != 0.0f)
			{
				collider* col = new collider();
				gameObject->addComponent(col);
				col->SetBoxSize(gameStruct.colliderSize.x, gameStruct.colliderSize.y, gameStruct.colliderSize.z);
				//col->SetTrans()
			}

			if (gameStruct.lightType != 0)
			{
				Light_* light = new Light_();
				light->setLightType(static_cast<LightType>(gameStruct.lightType - 1));
				light->setColor(gameStruct.lightColor);
				light->setLightIntensity(gameStruct.lightIntencity);
				gameObject->addComponent(light);
				//gameObject->addComponent()
			}

			if (gameObject->getName() == "plate1_1")
			{
				//gameStruct.position.x *= -1;
				//gameStruct.position.y = -0.001f;
				//gameStruct
				transform->setLocalPosition(-gameStruct.position);
			}

			//if (gameObject->getName() == "Pressure plate 1")
			//{
			//	gameObject->addComponent(new collider());
			//	gameObject->getComponent<collider>()->SetBoxSize(50, 50, 50);
			//	std::cout << "find PressurePlate1" << std::endl;
			//	gameObject->addComponent(new PressurePlateBehaviour());
			//	//transform->translate(glm::vec3(0.04f, 0, 0));
			//}

			//const auto check = glm::equal(transform->getScale(), glm::vec3(gameStruct.scale));
			//if (!check.x) transform->scale(glm::vec3(1/gameStruct.scale.x, 1, 1));
			//if (!check.y) transform->scale(glm::vec3(1, 1/gameStruct.scale.y, 1));
			//if (!check.z) transform->scale(glm::vec3(1, 1, 1/gameStruct.scale.z));
			//transform->scale(gameStruct.scale);
			//transform->setLocalScale(glm::vec3(1, 1, 1));
			if (gameStruct.meshName == "test-for-Slavi.obj")
			{
				gameObject->getTransform()->setScale(glm::vec3(0.01f, 0.01f, 0.01f));
				gameObject->addComponent(new collider());
				gameObject->getComponent<collider>()->SetBoxSize(50, 50, 50);
				gameObject->addComponent(new PlayerBaseComponent());
			}

			//transform->scaleWithPositions(glm::vec3(100));

			//transform->setLocalScale(gameStruct.scale);

			addGameObject(gameObject);
			id_to_go[gameStruct.selfID] = gameObject;
			//go_to_id[gameObject] = gameStruct.selfID;
		}

		//second pass - set parents
		//for (size_t i = 0; i < structs.size(); i++)
		//{
		//	const GameObject_s gameStruct = structs[i];
		//	GameObject_* gameObject = _gameObjects[i].get();

		//	const int parentID = gameStruct.parentID;

		//	if (parentID == 0) continue;

		//	Transform* transform = gameObject->getTransform();
		//	transform->setParent(id_to_go[parentID]->getTransform(), false);
		//}

		//for (auto& go : _gameObjects)
			//go->getTransform()->scaleWithPositions(glm::vec3(10));
		//	go->addComponent(new RotatingComponent());

		id_to_go.clear();
	}

	void Scene::deserializeStructs2(std::vector<GameStruct*> structs, const bool clipPaths)
	{
		std::unordered_map<int, GameObject_*> id_to_go;
		//std::unordered_map<GameObject_*, int> go_to_id;

		std::cout << "Deserealizing " + std::to_string(structs.size()) + " objects." << std::endl;

		//first pass - initialize GOs
		for (GameStruct* gameStruct : structs)
		{
			//Fixes
			if (gameStruct->name.find("Moveable") != std::string::npos) continue;
			if (gameStruct->name.find("DrawBridge") != std::string::npos)
			{
				MeshStruct* mesh = new MeshStruct;
				mesh->path = "Assets/Props/Large Props/Wooden_DrawBridge.fbx";
				gameStruct->components.push_back(mesh);
				gameStruct->transform->scale *= 0.01f;
			}

			if (gameStruct->name.find("Corpse") != std::string::npos)
			{
				gameStruct->transform->scale *= 0.001f;
			}

			if (gameStruct->name.find("Viking house 2") != std::string::npos)
			{
				std::cout << "\t SKIPPING VIKING HOUSE 2" << std::endl;
				continue;
			}

			//std::cout << "\tObject " + std::to_string(++index) + "." << std::endl;
			GameObject_* gameObject = nullptr;

			//Initialization (+ Mesh Loading if any)
			auto& components = gameStruct->components;
			MeshStruct* mesh = gameStruct->getComponent<MeshStruct>();
			if (mesh)
			{
				if (mesh->path.empty())
				{
					std::cout << "Skipping [" + mesh->path + "]" << std::endl;
					continue;
				}

				//Fixes
				if (mesh->path.find("default") != std::string::npos)
				{
					mesh->path = "Crate.fbx";
					gameStruct->transform->scale *= 0.01f;
				}
				if (mesh->path.find("haracter") != std::string::npos)
					mesh->path = File::findPath("shitbitch2.obj");

				if (mesh->path.find("ownhall") != std::string::npos)
				{
					mesh->path = File::findPath("Viking_Townhall.obj");
					std::cout << "\tTownhall!" << std::endl;
				}

				gameObject = Model::loadModel(mesh->path);
			}
			else
				gameObject = new GameObject_();

			gameObject->setName(gameStruct->name);
			gameObject->setTag(gameStruct->tag);

			//Transform Initialization
			Transform* transform = gameObject->getTransform();
			//Fix Orientation
			gameStruct->transform->position.x *= -1;
			gameStruct->transform->rotation.y *= -1;
			gameStruct->transform->rotation.z *= -1;

			if (gameStruct->name.find("Pillar") != std::string::npos)
			{
				//gameStruct->transform->position.z *= -1;
				std::cout << "Found Pillar" << std::endl;
				//gameObject->addComponent(new RotatingComponent);
			}

			//Set TRS
			transform->setLocalPosition(gameStruct->transform->position);
			transform->setLocalRotation(gameStruct->transform->rotation);
			transform->setLocalScale(gameStruct->transform->scale);

			std::cout << gameStruct->name + " position: " + glm::to_string(gameStruct->transform->position) << std::endl;

			for (ComponentStruct* component : components)
			{
				if (component == mesh) continue;
				gameObject->addComponent(component->makeObject());
			}

			//Fixes

			if (gameStruct->name.find("DrawBridge") != std::string::npos)
			{
				std::cout << "This is a Bridge" << std::endl;
				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(File::findPath("Wood_basecolor_001.png")));

				//for (int i = 0; i < transform->getChildCount(); i++)
				//{
				//	Transform* tf = transform->getChild(i);
				//	std::cout << tf->getGameObject()->getName() << std::endl;
				//}

				//auto ch = transform->getChildrenRecursive();
				//for (Transform* child : ch)
				//	std::cout << child->getGameObject()->getName() << std::endl;
			}

			if (gameStruct->name.find("Runestone") != std::string::npos)
			{
				//gameStruct->transform->position.z *= -1;


				if (!gameObject->getComponent<collider>()) gameObject->addComponent(new collider());
				gameObject->getComponent<collider>()->SetBoxSize(60, 60, 60);
				gameObject->getTransform()->scale(glm::vec3(1.75f));


			}

			if (mesh && mesh->path.find("Viking_Townhall.obj") != std::string::npos)
			{
				gameObject->getComponentInChildren<Material_>()->
					setDiffuseMap(Texture_::load(File::findPath
					("Viking_townhall_fixed_TownHall_Texture1_AlbedoTransparency.png")/*, TextureType::Diffuse, true, true*/));
				gameObject->getTransform()->translate(glm::vec3(-2, -6.8f, -2));


				gameObject->getTransform()->scale(glm::vec3(1.35f));
				//gameObject->addComponent<RotatingComponent>();
			}

			if (gameStruct->name.find("Crate") != std::string::npos)
				gameObject->getComponentInChildren<Material_>()->
				setDiffuseMap(Texture_::load(File::findPath
				("Crate_001_Crate_Texture_001_AlbedoTransparency.png")));

			if (gameStruct->name.find("Viking_Tower") != std::string::npos)
				gameObject->getComponentInChildren<Material_>()->
				setDiffuseMap(Texture_::load(File::findPath
					//("WoodTiles_basecolor.png")));
					("Wood_basecolor_001.png")));

			if (gameStruct->name.find("Polearm") != std::string::npos)
				gameObject->getComponentInChildren<Material_>()->
				setDiffuseMap(Texture_::load(File::findPath
				("Polearm_Texture.png")));

			if (gameStruct->name.find("Shield") != std::string::npos)
				gameObject->getComponentInChildren<Material_>()->
				setDiffuseMap(Texture_::load(File::findPath
				("Shield_Red_Texture.png")));

			if (gameStruct->name.find("Viking house 2") != std::string::npos)
			{
				gameObject->getComponentInChildren<Material_>()->
					setDiffuseMap(Texture_::load(File::findPath
					("Viking_house_2_Viking_House_Texture_3_AlbedoTransparency.png")/*, TextureType::Diffuse, true, true*/));
				gameObject->getTransform()->translate(glm::vec3(0, -4, 0));


				//gameObject->getTransform()->scale(glm::vec3(1.35f));
				//gameObject->addComponent<RotatingComponent>();
			}

			if (gameStruct->name.find("Candle") != std::string::npos)
			{
				std::cout << "this is a Candle" << std::endl;
				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(File::findPath("Candles_001_Candle_Material_AlbedoTransparency.png")));
			}

			if (gameStruct->name.find("Pot_001") != std::string::npos)
			{
				std::cout << "this is a Vase 1" << std::endl;
				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(File::findPath("Small vase_Small_vase_AlbedoTransparency.png")));
			}

			if (gameStruct->name.find("Pot_002") != std::string::npos || gameStruct->name.find("Pot_003") != std::string::npos)
			{
				std::cout << "this is a Vase 2" << std::endl;
				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(File::findPath("Small vase_Small_vase2_AlbedoTransparency.png")));
			}

			if (gameStruct->name.find("Pressure plate") != std::string::npos && gameStruct->name.find("order") == std::string::npos)
			{
				//gameStruct->transform->position.z *= -1;
				std::cout << "this is Plate" << std::endl;
				if (!gameObject->getComponent<collider>()) gameObject->addComponent(new collider());
				gameObject->getComponent<collider>()->SetBoxSize(100, 100, 100);
				gameObject->getComponent<collider>()->SetTrigger(true);
				gameObject->addComponent(new PressurePlateBehaviour());
				//gameObject->getComponent<Transform>()->rotate(glm::vec3(0, glm::radians(30.0f), 0));
			}

			if (gameStruct->name.find("Gate") != std::string::npos || gameStruct->name.find("Door") != std::string::npos)
			{
				//gameStruct->transform->position.z *= -1;
				std::cout << "this is Gate" << std::endl;
				if (!gameObject->getComponent<collider>()) gameObject->addComponent(new collider());
				//gameObject->getComponent<collider>()->SetBoxSize(100, 100, 100);
				gameObject->addComponent(new GateBehaviour());
				//gameObject->getComponent<Transform>()->rotate(glm::vec3(0, glm::radians(30.0f), 0));
			}

			if (gameStruct->name.find("Main character 1.2 updated") != std::string::npos)
			{
				//gameStruct->transform->position.z *= -1;
				std::cout << "this is Player" << std::endl;

				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(File::findPath("shitbitch2.png")));

				gameObject->addComponent(new PlayerBaseComponent());
				gameObject->getComponent<PlayerBaseComponent>()->normalY = gameObject->getTransform()->getPosition().y;

				if (!gameObject->getComponent<collider>()) gameObject->addComponent(new collider());
				gameObject->getComponent<collider>()->SetBoxSize(80, 150, 80);
			}

			if (mesh && mesh->path.find("Floor.fbx") != std::string::npos)
			{
				gameObject->getTransform()->scale(glm::vec3(0.01f));
				gameObject->getTransform()->scale(glm::vec3(0.01f));
				//gameObject->getTransform()->scale(glm::vec3(0.01f));
				//std::cout << "Found Floor" << std::endl;
				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(File::findPath("RockTiles_basecolor2.png")));
			}

			if (gameStruct->name.find("Stone_Coffin") != std::string::npos)
			{
				std::cout << "Find Coffin" << std::endl;
				gameObject->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load(
					File::findPath("Stone_Coffin_Coffin_Material_AlbedoTransparency.png")));

			}

			if (gameStruct->name.find("Tree") != std::string::npos)
			{
				std::cout << "This is a Tree" << std::endl;
				auto materials = gameObject->getComponentsInChildren<Material_>();
				std::cout << "It has " << materials.size() << " materials" << std::endl;

				for (unsigned i = 0; i < materials.size(); i++)
				{
					Material_* material = materials[i];
					std::cout << material->getGameObject()->getName() << std::endl;
					//if (material->getGameObject()->getName().find("Lea") != std::string::npos)
					if (i == 0)
					{
						std::cout << "This is a Leaf" << std::endl;
						material->setDiffuseMap(Texture_::load(File::findPath("Leaf_Material_001_Base_Color.png")));
						//continue;
					}
					else
					{
						std::cout << "This is Bark" << std::endl;
						material->setDiffuseMap(Texture_::load(File::findPath("Tree_Material_001_Base_Color.png")));
						//continue;
					}
				}
			}

			addGameObject(gameObject);
			id_to_go[gameStruct->transform->selfID] = gameObject;
		}

		////second pass - set parents
		//for (size_t i = 0; i < structs.size(); i++)
		//{
		//	const GameStruct* gameStruct = structs[i];
		//	GameObject_* gameObject = _gameObjects[i].get();

		//	const int parentID = gameStruct->transform->parentID;

		//	if (parentID == 0) continue;

		//	Transform* transform = gameObject->getTransform();
		//	transform->setParent(id_to_go[parentID]->getTransform(), false);
		//}

		//for (auto& go : _gameObjects)
		//go->getTransform()->scaleWithPositions(glm::vec3(10));
		//	go->addComponent(new RotatingComponent());

		id_to_go.clear();
	}

	void Scene::loadMenu()
	{
		using namespace Engine::UI;
		sf::RenderWindow* window = ServiceLocator::instance()->getService<Engine::Game>()->getWindow();
		const sf::Vector2u windowSize = window->getSize();
		const float width = static_cast<float>(windowSize.x);
		const float height = static_cast<float>(windowSize.y);

		Button* BackGround = new Button();
		BackGround->loadSprite("bg.jpg");

		Button* playButton = new Button();
		playButton->loadSprite("Play.png");
		playButton->getSprite().setPosition(10, height * 6 / 10);

		Button* OptionsButton = new Button();
		OptionsButton->loadSprite("Options.png");
		OptionsButton->getSprite().setPosition(10, height * 7 / 10);

		Button* CreditsButton = new Button();
		CreditsButton->loadSprite("Credits.png");
		CreditsButton->getSprite().setPosition(10, height * 8 / 10);

		Button* ExitButton = new Button();
		ExitButton->loadSprite("Exit.png");
		ExitButton->getSprite().setPosition(10, height * 9 / 10);

		if (true)
			//forloop trough all vectors of buttons and disable them except one vector
		//Text* testText = new Text(true);
		//testText->setFont("mge/fonts/arial.ttf");
		//testText->setTextInformation("Testxxxx");
		//testText->getTextObject().setPosition(10, height * 9 / 10);
		//testText->setWindow(window);
		//testText->setTextAlignment(UI::Left_Justified);
		//testText->getTextObject().setCharacterSize(100);
		//testText->getTextObject().setFillColor(sf::Color::White);

			BackGround->getSprite().setScale(2, 2);

		//Text::makeHint();
		//Text::hint->setWindow(window);
		//std::cout << "MENU" << std::endl;
		////std::cout << Text::hint->getTextObject().getCharacterSize() << std::endl;
		//Text::hint->getTextObject().setCharacterSize(24);
		//Text::hint->getTextObject().setPosition(100, 500);
		//std::cout << "load menu" << std::endl;
	}

	void Scene::hardCode()
	{
		if (_name.find("Level") != std::string::npos)
		{
			const std::string name = _name.substr(0, _name.find('.'));
			//std::cout << name << std::endl;
			switch (name.back())
			{
			default: std::cout << "Default case" << std::endl;
			case '1':
			case '2':
			case '3':
			case '4':
			{
				Game* game = getGame();

				game->background->load("Assets/Audio/Howling wind.wav");
				game->background->setVolume(0.4f);
				game->background->setLooping(true);

				game->music->load("Assets/Audio/Background music.wav");
				game->music->setVolume(0.15f);
				game->music->setLooping(true);

				game->background->play();
				game->music->play();
				break;
			}

			case '5':
			case '6':
			{
				Game* game = getGame();

				game->background->load("Assets/Audio/Village Ambience.wav");
				game->background->setVolume(0.4f);
				game->background->setLooping(true);

				game->music->load("Assets/Audio/Background music.wav");
				game->music->setVolume(0.15f);
				game->music->setLooping(true);

				game->background->play();
				game->music->play();
				break;
			}
			}
		}

		GameObject_* p1_1 = this->findGameObject("Pressure plate 1");
		if (p1_1)
		{
			//std::cout << "KJHLKJ" << std::endl;
			p1_1->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate1_1")->getComponent<GateBehaviour>()->AddPlate(p1_1->getComponent<PressurePlateBehaviour>());
		}

		std::string suffix = "2_1";
		GameObject_* p2_1 = this->findGameObject("Pressure plate " + suffix);
		if (p2_1)
		{
			p2_1->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate" + suffix)->getComponent<GateBehaviour>()->AddPlate(p2_1->getComponent<PressurePlateBehaviour>());
		}

		suffix = "2_2";
		GameObject_* p2_2 = this->findGameObject("Pressure plate " + suffix);
		if (p2_2)
		{
			p2_2->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate" + suffix)->getComponent<GateBehaviour>()->AddPlate(p2_2->getComponent<PressurePlateBehaviour>());
		}

		suffix = "3_1";
		GameObject_* p3_1 = this->findGameObject("Pressure plate " + suffix);
		if (p3_1)
		{
			p3_1->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Door 1")->getComponent<GateBehaviour>()->AddPlate(p3_1->getComponent<PressurePlateBehaviour>());
		}

		suffix = "3_3";
		GameObject_* p3_3 = this->findGameObject("Pressure plate " + suffix);
		if (p3_3)
		{
			p3_3->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Door 5")->getComponent<GateBehaviour>()->AddPlate(p3_3->getComponent<PressurePlateBehaviour>());
		}

		suffix = "3_4";
		GameObject_* p3_4 = this->findGameObject("Pressure plate " + suffix);
		if (p3_4)
		{
			p3_4->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Door 5")->getComponent<GateBehaviour>()->AddPlate(p3_4->getComponent<PressurePlateBehaviour>());
		}

		suffix = "5_1";
		GameObject_* p5_1 = this->findGameObject("Pressure plate " + suffix);
		if (p5_1)
		{
			p5_1->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate" + suffix)->getComponent<GateBehaviour>()->AddPlate(p5_1->getComponent<PressurePlateBehaviour>());
		}

		suffix = "5_2";
		GameObject_* p5_2 = this->findGameObject("Pressure plate " + suffix);
		if (p5_2)
		{
			p5_2->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate5_1")->getComponent<GateBehaviour>()->AddPlate(p5_2->getComponent<PressurePlateBehaviour>());
		}

		suffix = "6_1";
		GameObject_* p6_1 = this->findGameObject("Pressure plate " + suffix);
		if (p6_1)
		{
			p6_1->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate" + suffix)->getComponent<GateBehaviour>()->AddPlate(p6_1->getComponent<PressurePlateBehaviour>());
		}

		suffix = "6_2";
		GameObject_* p6_2 = this->findGameObject("Pressure plate " + suffix);
		if (p6_2)
		{
			p6_2->getComponent<collider>()->SetSphereRadius(1.5f);
			auto go = this->findGameObject("Gate" + suffix)->getComponent<GateBehaviour>();
			go->AddPlate(p6_2->getComponent<PressurePlateBehaviour>());
			go->AddPlate(p6_1->getComponent<PressurePlateBehaviour>());
		}

		suffix = "6_3";
		GameObject_* p6_3 = this->findGameObject("Pressure plate " + suffix);
		if (p6_3)
		{
			p6_3->getComponent<collider>()->SetSphereRadius(1.5f);
			this->findGameObject("Gate" + suffix)->getComponent<GateBehaviour>()->AddPlate(p6_3->getComponent<PressurePlateBehaviour>());
			this->findGameObject("Gate6_1")->getComponent<GateBehaviour>()->AddPlate(p6_1->getComponent<PressurePlateBehaviour>());
		}

	}

	void Scene::neededHardCode()
	{
		Core::GameObject_* camera = new Core::GameObject_("Cam", "", glm::vec3(0, 100, 3000));

		Core::Camera_* cameraComp = new Core::Camera_();
		camera->addComponent(cameraComp);
		Core::Camera_::setMainCamera(cameraComp);
		camera->addComponent<AudioListener>();

		//Core::GameObject_* lightgo = new Core::GameObject_("Light", "", glm::vec3(100, 0, 1));
		//Rendering::Light_* light = new Rendering::Light_();
		//lightgo->addComponent(light);
		//light->setColor(glm::vec3(1));
		//light->setLightType(Rendering::LightType::Directional);
		//lightgo->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		//lightgo->getTransform()->rotate(lightgo->getTransform()->right(), -glm::radians(30.0f));
		//light->setRange(35000);

		//Core::GameObject_* lightgo1 = newCore::GameObject_("Light", "", glm::vec3(-500, -500, 2000));

		//Core::GameObject_* lightgo1 = //new Core::GameObject_("point");
		//	Model::loadModel("mge/models/cube_smooth.obj");
		//lightgo1->getTransform()->translate(glm::vec3(-300, 300, 500));
		//Rendering::Light_* light1 = new Rendering::Light_();
		//lightgo1->addComponent(light1);
		//light1->setLightType(Rendering::LightType::Point);
		//lightgo1->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		//light1->setColor(glm::vec3(0, 1, 0));
		//light1->setRange(350000);

		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientLightColor(glm::vec3(1));
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientStrength(0.3f);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAttenuation(1.0f, 0.07f, 0.017f);
	}

	Game* Scene::getGame()
	{
		if (!_game) _game = ServiceLocator::instance()->getService<Game>();
		return _game;
	}
}
