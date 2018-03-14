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

namespace Engine
{
	using namespace Rendering;
	Scene::Scene() : _name(""), _path(""), _gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
	{
	}

	Scene::Scene(std::string name, std::string path) : _name(std::move(name)), _path(std::move(path)),
		_gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
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
		std::cout << "Loading Scene..." << std::endl;

		if (hard)
			hardCode();
		//return;
		if (fromFile)
		{
			//Model::debug(true);
			Model::clipPaths = false;
			Deserealizer deserealizer;
			// deserealizer.deserializeIntoStructs("level1.json");
			deserealizer.deserializeIntoStructs(_path);
			deserializeStructs(deserealizer.structs, false);
		}
		if (hard && fromFile)
			neededHardCode();

		std::cout << "Loaded Scene." << std::endl;
	}

	Core::GameObject_* Scene::findGameObject(const std::string& name) const
	{
		for (const auto& gameObject : _gameObjects)
			if (gameObject->getName() == name)
				return gameObject.get();

		return nullptr;
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
		//return;
		std::unordered_map<int, GameObject_*> id_to_go;
		//std::unordered_map<GameObject_*, int> go_to_id;

		//first pass - initialize GOs
		//int index = 0;
		std::cout << "Deserealizing " + std::to_string(structs.size()) + " objects." << std::endl;
		for (GameObject_s& gameStruct : structs)
		{
			//std::cout << "\tObject " + std::to_string(++index) + "." << std::endl;
			GameObject_* gameObject = nullptr;

			if (!gameStruct.meshName.empty() && gameStruct.meshName != std::string("")
				&& gameStruct.meshName.find('.') != std::string::npos
				)
			{
				if (clipPaths)
				{
					while (gameStruct.meshName.find('/') != std::string::npos)
					{
						gameStruct.meshName = gameStruct.meshName.substr(gameStruct.meshName.find('/') + 1, std::string::npos);
						//std::cout << gameStruct.meshName << std::endl;
					}
				}

				if (gameStruct.meshName.find("default") != std::string::npos)
				{
					//continue;
					if (gameStruct.name.find("Cylinder") != std::string::npos)
						gameStruct.meshName = "Cylinder.fbx";
					else
						//if (gameStruct.name.find("Cube") != std::string::npos)
						gameStruct.meshName = "Cube.fbx";
				}

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

			if (gameObject == nullptr) return;
			gameObject->setName(gameStruct.name);

			Transform* transform = gameObject->getTransform();
			gameStruct.position.x *= -1;
			transform->setLocalPosition(gameStruct.position);
			std::cout << gameStruct.name + " Position: " + glm::to_string(gameStruct.position) << std::endl << std::endl;
			gameStruct.rotation.y *= -1;
			gameStruct.rotation.z *= -1;
			transform->setLocalRotation(gameStruct.rotation);

			//const auto check = glm::equal(transform->getScale(), glm::vec3(1));
			//if (!check.x) transform->scale(glm::vec3(1/gameStruct.scale.x, 1, 1));
			//if (!check.y) transform->scale(glm::vec3(1, 1/gameStruct.scale.y, 1));
			//if (!check.z) transform->scale(glm::vec3(1, 1, 1/gameStruct.scale.z));
			//transform->scale(gameStruct.scale);
			//transform->setLocalScale(glm::vec3(1, 1, 1));

			addGameObject(gameObject);
			id_to_go[gameStruct.selfID] = gameObject;
			//go_to_id[gameObject] = gameStruct.selfID;
		}

		//second pass - set parents
		for (size_t i = 0; i < structs.size(); i++)
		{
			const GameObject_s gameStruct = structs[i];
			GameObject_* gameObject = _gameObjects[i].get();

			const int parentID = gameStruct.parentID;

			if (parentID == 0) continue;

			Transform* transform = gameObject->getTransform();
			transform->setParent(id_to_go[parentID]->getTransform(), false);
		}

		//for (auto& go : _gameObjects)
		//	go->addComponent(new RotatingComponent());

		id_to_go.clear();
	}

	void Scene::loadMenu()
	{

		using namespace Engine::UI;
		sf::RenderWindow* window = ServiceLocator::instance()->getService<Engine::Game>()->getWindow();
		const sf::Vector2u windowSize = window->getSize();
		float width = windowSize.x;
		float height = windowSize.y;

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


		//BackGround->getSprite().setScale(2, 2);
		Text::makeHint();
		Text::hint->setWindow(window);
		std::cout << "MENUU" << std::endl;
		std::cout << Text::hint->getTextObject().getCharacterSize() << std::endl;
		Text::hint->getTextObject().setPosition(100, 100);
		Text::showHint("CABBAGE", 30, 5);

		std::cout << "load menu" << std::endl;
	}

	void Scene::hardCode()
	{
		//loadMenu();
		std::function<void()> pr = ([] {std::cout << "Print" << std::endl; });
		Engine::Utility::Time::timeout(1, pr);
		Engine::Utility::Time::timeout(2, pr);
		Engine::Utility::Time::timeout(3, pr);
		Engine::Utility::Time::timeout(4, pr);
		Model::clipPaths = true;
		Core::GameObject_* door_1 = Model::loadModel("Assets/Props/Door_1.fbx");
		door_1->getTransform()->translate(glm::vec3(0, 5, 15));
		door_1->getComponentInChildren<Material_>()->setSpecularStrength(0.001f);
		//std::cout << "Find path check " + Engine::File::findPath("Viking_House_2_coloured.fbx") << std::endl;
		//Core::GameObject_ * house = Model::loadModel("Viking_House_2_coloured.fbx");
		//house->getTransform()->getChild(0)->getGameObject()->getComponent<Rendering::Material_>()->
		//	setDiffuseMap(Rendering::Texture_::load(
		//		"Viking_House_2_Viking_House_Texture_2_AlbedoTransparency.png"));
		//anvil->addComponent(new RotatingComponent());

		//Core::GameObject_ * table = Model::loadModel("Table.fbx");
		//table->addComponent(new RotatingComponent());
		//Core::GameObject_ * table_C = Model::loadModel("Table_centered.fbx");
		//table_C->addComponent(new RotatingComponent()); 
		//table_C->getTransform()->translate(glm::vec3(10, 0, 0));
		//Core::GameObject_ * crate = Model::loadModel("Crate.fbx");
		//crate->addComponent(new RotatingComponent());
		//Core::GameObject_ *shipOBJ = Model::loadModel("Fence_centered.obj");
		//shipOBJ->addComponent(new RotatingComponent());
		//shipOBJ->getTransform()->translate(glm::vec3(250, 0, 0));
		//Core::GameObject_ *shipFBX = Model::loadModel("Fence_centered.fbx");
		//shipFBX->getTransform()->translate(glm::vec3(25, 0, 0));
		//shipFBX->addComponent(new RotatingComponent());
		LuaScript* luaS = new LuaScript();
		//return;

		//Core::GameObject_* playerModel = Model::loadModel(d.structs[0].meshName);
		//playerModel->getTransform()->setPosition(playerModel->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//playerModel->addComponent(new PlayerBaseComponent());

		//Core::GameObject_* obj1 = Model::loadModel("MainCharacter_.fbx");
		Core::GameObject_* obj1 = Model::loadModel("Player.obj");
		//obj1->getTransform()->setScale(glm::vec3(2, 2, 2));
		//Core::GameObject_* obj1 = Model::loadModel("Player.obj");

		obj1->getTransform()->translate(glm::vec3(0, 10, -600));
		obj1->addComponent(new collider());
		obj1->addComponent(new PlayerBaseComponent());
		obj1->setName("Player");
		obj1->getComponent<collider>()->SetBoxSize(50, 50, 50);

		Core::GameObject_* crate = Model::loadModel("Crate.fbx");
		crate->getTransform()->setScale(glm::vec3(2, 2, 2));
		crate->getTransform()->translate(glm::vec3(700, -100, 0));
		crate->setName("crate1");
		crate->addComponent(new collider());
		crate->getComponent<collider>()->SetBoxSize(60, 150, 60);

		//obj1->addComponent(new RotatingComponent());

		//Core::GameObject_* obj2 = Model::loadModel("Player.obj");

		//obj2->getTransform()->translate(glm::vec3(-0, -199, 0));

		Core::GameObject_* obj3 = Model::loadModel("Crate.fbx");
		obj3->getTransform()->setScale(glm::vec3(2, 2, 2));
		obj3->getTransform()->translate(glm::vec3(-500, 60, 100));
		obj3->setName("crate2");
		obj3->addComponent(new collider());
		obj3->getComponent<collider>()->SetBoxSize(60, 150, 60);

		Core::GameObject_* obj4 = Model::loadModel("Crate.fbx");
		obj4->getTransform()->setScale(glm::vec3(2, 2, 2));
		obj4->getTransform()->translate(glm::vec3(-700, 60, 600));
		obj4->setName("crate3");
		obj4->addComponent(new collider());
		obj4->getComponent<collider>()->SetBoxSize(60, 150, 60);
		Texture_* text;
		text = Texture_::load("white.jpg", TextureType::Specular);
		obj4->getComponentInChildren<Material_>()->setEmissionMap(text);
		obj4->getComponentInChildren<Material_>()->setDiffuseMap(Texture_::load("MementoMori.jpg"));
		obj4->getComponentInChildren<Material_>()->useSpecularMap();
		obj4->getComponentInChildren<Material_>()->useEmissionMap();
		obj4->getComponentInChildren<Material_>()->useEmission();
		Core::GameObject_* obj6 = Model::loadModel("Crate.fbx");
		obj6->getTransform()->setScale(glm::vec3(2, 2, 2));
		obj6->getTransform()->translate(glm::vec3(-400, 60, 100));
		obj6->setName("crate4");
		obj6->addComponent(new collider());
		obj6->getComponent<collider>()->SetBoxSize(60, 150, 60);
		//obj2->addComponent(new RotatingComponent());

		//Core::GameObject_* plateBorder = Model::loadModel("Pressure plate Border.fbx");
		//plateBorder->getTransform()->setScale(glm::vec3(2, 2, 2));

		//Core::GameObject_* plate = Model::loadModel("Pressure plate 1.fbx");
		//plate->getTransform()->setScale(glm::vec3(2, 2, 2));
		//plate->setName("plate");
		////not pressed position glm::vec3(0,3,0)
		////pressed position glm::vec3(0,-8,0) (or -5(sth like that))
		//plate->getTransform()->setPosition(plateBorder->getTransform()->getPosition() + glm::vec3(0, 3, 0));
		//plate->addComponent(new collider());
		//plate->addComponent(new PressurePlateBehaviour());

		//Core::GameObject_* gate = Model::loadModel("Door 1.fbx");
		//gate->getTransform()->setScale(glm::vec3(0.9, 0.9, 0.9));
		//gate->getTransform()->translate(glm::vec3(500, 200, 700));
		//gate->setName("gate");
		//gate->addComponent(new collider());
		//gate->addComponent(new GateBehaviour());
		//gate->getComponent<collider>()->SetBoxSize(70, 1500, 600);
		//gate->getComponent<GateBehaviour>()->AddPlate(plate->getComponent<PressurePlateBehaviour>());

		obj1->addComponent(luaS);
	}

	void Scene::neededHardCode()
	{
		Core::GameObject_* camera = new Core::GameObject_("Cam", "", glm::vec3(0, 100, 3000));

		Core::Camera_* cameraComp = new Core::Camera_();
		camera->addComponent(cameraComp);
		Core::Camera_::setMainCamera(cameraComp);

		Core::GameObject_* lightgo = new Core::GameObject_("Light", "", glm::vec3(100, 0, 1));
		Rendering::Light_* light = new Rendering::Light_();


		lightgo->addComponent(light);
		light->setColor(glm::vec3(1));
		light->setLightType(Rendering::LightType::Directional);
		lightgo->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		lightgo->getTransform()->rotate(lightgo->getTransform()->right(), -glm::radians(30.0f));
		//Core::GameObject_* lightgo1 = new Core::GameObject_("Light", "", glm::vec3(-500, -500, 2000));
		Core::GameObject_* lightgo1 = //new Core::GameObject_("point");
			Model::loadModel("mge/models/cube_smooth.obj");
		lightgo1->getTransform()->translate(glm::vec3(-300, 300, 500));
		Rendering::Light_* light1 = new Rendering::Light_();
		lightgo1->addComponent(light1);
		light1->setLightType(Rendering::LightType::Point);
		lightgo1->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		light1->setColor(glm::vec3(0, 1, 0));
		light1->setRange(350000);
		light->setRange(35000);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientLightColor(glm::vec3(1));
		//ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientStrength(0.3f);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAttenuation(1.0f, 0.07f, 0.017f);
	}
}
