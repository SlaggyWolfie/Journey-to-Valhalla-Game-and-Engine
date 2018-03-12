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
#include "Component.hpp"
#include "PlayerBaseComponent.h"
#include "LastposStasher.h"
#include "collider.h"
#include <lua\lua.hpp>
#include "Core\LuaScript.h"
#include "RotatingComponent.hpp"
#include "SFML\Graphics\RenderWindow.hpp"
#include "../src/Core/Game.hpp"
#include "PressurePlateBehaviour.h"
#include "GateBehaviour.h"


namespace Engine
{
	Scene::Scene() : _name(nullptr), _gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
	{
	}

	Scene::Scene(std::string name) : _name(std::move(name)), _gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
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
		if (hard)
			hardCode();
		if (fromFile)
		{
			//Model::debug(true);
			Deserealizer deserealizer;
			deserealizer.deserializeIntoStructs();
			deserializeStructs(deserealizer.structs);
		}
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

	void Scene::deserializeStructs(std::vector<GameObject_s> structs)
	{
		std::unordered_map<int, GameObject_*> id_to_go;
		//std::unordered_map<GameObject_*, int> go_to_id;

		//first pass - initialize GOs
		int index = 0;
		std::cout << "Deserealizing " + std::to_string(structs.size()) + " objects." << std::endl;
		for (GameObject_s& gameStruct : structs)
		{
			std::cout << "\tObject " + std::to_string(++index) + "." << std::endl;
			GameObject_* gameObject = nullptr;

			if (!gameStruct.meshName.empty() && gameStruct.meshName != std::string("")
				&& gameStruct.meshName.find('.') != std::string::npos
				)
			{
				while (gameStruct.meshName.find('/') != std::string::npos)
				{
					gameStruct.meshName = gameStruct.meshName.substr(gameStruct.meshName.find('/') + 1, std::string::npos);
					std::cout << gameStruct.meshName << std::endl;
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
			//gameStruct.position.z *= -1;
			//gameStruct.position.y *= -1;
			transform->setLocalPosition(gameStruct.position);
			std::cout << gameStruct.name + " Position: " + glm::to_string(gameStruct.position) << std::endl;
			//const glm::vec3 rotation = glm::vec3(glm::radians(gameStruct.rotation.x), glm::radians(gameStruct.rotation.y), glm::radians(gameStruct.rotation.z));
			//transform->setLocalRotation(glm::quat(glm::eulerAngleYXZ(rotation.x, -rotation.y, -rotation.z)));
			std::cout << glm::to_string(gameStruct.rotation) << std::endl;
			//gameStruct.rotation.x *= -1;
			gameStruct.rotation.y *= -1;
			gameStruct.rotation.z *= -1;
			transform->setLocalRotation(gameStruct.rotation);
			std::cout << glm::to_string(gameStruct.rotation) << std::endl;
			//transform->setLocalRotation(glm::quat(glm::eulerAngleXYZ(gameStruct.rotation.x, gameStruct.rotation.y, gameStruct.rotation.z)));
			transform->setLocalScale(gameStruct.scale);

			//if (!gameStruct.meshName.empty() && gameStruct.meshName != std::string("")
			//	&& gameStruct.meshName.find('.') != std::string::npos)
			//{
			//	ShallowMesh* shallow = Model::loadModelShallow(gameStruct.meshName);
			//	std::cout << "Why are you running: " + std::to_string(shallow != nullptr) << std::endl;
			//	gameObject->addComponent(shallow->mesh);
			//	gameObject->addComponent(shallow->material);
			//	gameObject->addComponent(shallow->renderer);
			//}

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

		//for (auto& go: _gameObjects)
		//{
		//	go->addComponent(new RotatingComponent());
		//}

		id_to_go.clear();
	}

	void Scene::hardCode()
	{
		//Core::GameObject_ * crate = Model::loadModel("Crate.fbx");
		//crate->addComponent(new RotatingComponent());
		//Core::GameObject_ *shipOBJ = Model::loadModel("Fence_centered.obj");
		//shipOBJ->addComponent(new RotatingComponent());
		//shipOBJ->getTransform()->translate(glm::vec3(250, 0, 0));
		//Core::GameObject_ *shipFBX = Model::loadModel("Fence_centered.fbx");
		//shipFBX->getTransform()->translate(glm::vec3(25, 0, 0));
		//shipFBX->addComponent(new RotatingComponent());
		//LuaScript* luaS = new LuaScript();

		//Deserealizer d;
		Core::GameObject_* camera = new Core::GameObject_("Cam", "", glm::vec3(0, 100, 3000));

		Core::GameObject_* lightgo = new Core::GameObject_("Light", "", glm::vec3(100, 0, 1));
		Rendering::Light_* light = new Rendering::Light_();
		lightgo->addComponent(light);
		light->setColor(glm::vec3(1));
		light->setLightType(Rendering::LightType::Directional);
		lightgo->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		lightgo->getTransform()->rotate(lightgo->getTransform()->right(), -glm::radians(30.0f));
		//Core::GameObject_* lightgo1 = new Core::GameObject_("Light", "", glm::vec3(-500, -500, 2000));
		Core::GameObject_* lightgo1 = Model::loadModel("mge/models/cube_smooth.obj");
		lightgo1->getTransform()->translate(glm::vec3(-300, 300, 500));
		Rendering::Light_* light1 = new Rendering::Light_();
		lightgo1->addComponent(light1);
		light1->setLightType(Rendering::LightType::Point);
		lightgo1->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		light1->setColor(glm::vec3(0, 1, 0));
		light1->setRange(350000);
		light->setRange(35000);
		Core::Camera_* cameraComp = new Core::Camera_();
		camera->addComponent(cameraComp);
		Core::Camera_::setMainCamera(cameraComp);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientLightColor(glm::vec3(1));
		//ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientStrength(0.3f);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAttenuation(1.0f, 0.07f, 0.017f);

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
		crate->setName("crate");
		crate->addComponent(new collider());
		crate->getComponent<collider>()->SetBoxSize(60, 150, 60);






		//obj1->addComponent(new RotatingComponent());

		//Core::GameObject_* obj2 = Model::loadModel("Player.obj");

		//obj2->getTransform()->translate(glm::vec3(-0, -199, 0));





		Core::GameObject_* obj3 = Model::loadModel("Crate.fbx");
		obj3->getTransform()->setScale(glm::vec3(2, 2, 2));
		obj3->getTransform()->translate(glm::vec3(-500, 60, 100));
		obj3->setName("crate");
		obj3->addComponent(new collider());
		obj3->getComponent<collider>()->SetBoxSize(60, 150, 60);

		Core::GameObject_* obj4 = Model::loadModel("Crate.fbx");
		obj4->getTransform()->setScale(glm::vec3(2, 2, 2));
		obj4->getTransform()->translate(glm::vec3(-700, 60, 600));
		obj4->setName("crate");
		obj4->addComponent(new collider());
		obj4->getComponent<collider>()->SetBoxSize(60, 150, 60);
	

		Core::GameObject_* obj6 = Model::loadModel("Crate.fbx");
		obj6->getTransform()->setScale(glm::vec3(2, 2, 2));
		obj6->getTransform()->translate(glm::vec3(-400, 60, 100));
		obj6->setName("crate");
		obj6->addComponent(new collider());
		obj6->getComponent<collider>()->SetBoxSize(60, 150,60);
		//obj2->addComponent(new RotatingComponent());

		Core::GameObject_* plateBorder = Model::loadModel("Pressure plate Border.fbx");
		plateBorder->getTransform()->setScale(glm::vec3(2, 2, 2));

		Core::GameObject_* plate = Model::loadModel("Pressure plate 1.fbx");
		plate->getTransform()->setScale(glm::vec3(2, 2, 2));
		plate->setName("plate");
		//not pressed position glm::vec3(0,3,0)
		//pressed position glm::vec3(0,-8,0) (or -5(sth like that))
		plate->getTransform()->setPosition(plateBorder->getTransform()->getPosition() + glm::vec3(0, 3, 0));
		plate->addComponent(new collider());
		plate->addComponent(new PressurePlateBehaviour());
		
		Core::GameObject_* gate = Model::loadModel("Door 1.fbx");
		gate->getTransform()->setScale(glm::vec3(0.9, 0.9, 0.9));
		gate->getTransform()->translate(glm::vec3(500, 200, 700));
		gate->setName("gate");
		gate->addComponent(new collider());
		gate->addComponent(new GateBehaviour());
		gate->getComponent<collider>()->SetBoxSize(70, 1500,600);
		gate->getComponent<GateBehaviour>()->AddPlate(plate->getComponent<PressurePlateBehaviour>());

		obj1->addComponent(luaS);

		Core::GameObject_* testModel = Model::loadModel("Dungeon_Wall_Corner_001.fbx");
		Core::GameObject_* testModel1 = Model::loadModel("Forge.fbx");
		//testModel1->addComponent(new RotatingComponent());
		//testModel1->addComponent<RotatingComponent>();
		std::cout << "0: " + glm::to_string(testModel->getTransform()->getPosition()) << std::endl;
		std::cout << "0: " + std::to_string(testModel->getTransform()->getChildCount()) << std::endl;
		std::cout << "0: " + glm::to_string(testModel->getTransform()->getRotation()) << std::endl;
		std::cout << "0: " + glm::to_string(testModel->getTransform()->getScale()) << std::endl;
		std::cout << "1: " + glm::to_string(testModel1->getTransform()->getPosition()) << std::endl;
		std::cout << "1: " + glm::to_string(testModel1->getTransform()->getRotation()) << std::endl;
		std::cout << "1: " + glm::to_string(testModel1->getTransform()->getScale()) << std::endl;
		std::cout << "1: " + std::to_string(testModel1->getTransform()->getChildCount()) << std::endl;
		//std::cout << "1: " + testModel1->getTransform()->getChild(0)->getGameObject()->getName() << std::endl;
		std::cout << "1: " + testModel1->getName() << std::endl;
		//Core::GameObject_* tm_ = Model::loadModel("ketabxane.fbx");
		//tm_->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(180.0f));
		//std::cout << tm_->getComponentsCount() << std::endl;
		//tm_->addComponent(new RotatingComponent());


		//Core::GameObject_* playerModel = Model::loadModel(d.structs[0].meshName);
		//playerModel->getTransform()->setPosition(playerModel->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//playerModel->addComponent(new PlayerBaseComponent());
		Core::GameObject_* plane = Model::loadModel("mge/models/plane.obj");
		plane->getTransform()->scale(glm::vec3(5000));
		plane->getTransform()->translate(glm::vec3(glm::vec3(0, 0, 0)));
		////Core::GameObject_* playerModel = Model::loadModel(d.structs[0].meshName);
		////playerModel->getTransform()->setPosition(playerModel->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		////playerModel->addComponent(new PlayerBaseComponent());

		////Core::GameObject_* obj1 = Model::loadModel("MainCharacter_.fbx");
		//Core::GameObject_* obj1 = Model::loadModel("test-for-Slavi.obj");
		////Core::GameObject_* obj1 = Model::loadModel("Player.obj");
		//
		//obj1->getTransform()->translate(glm::vec3(-0, -199, 0));
		//obj1->setName("Player");
		//obj1->addComponent(new collider());
		//obj1->addComponent(new PlayerBaseComponent());
		//obj1->addComponent(new RotatingComponent());

		//Core::GameObject_* obj2 = Model::loadModel("Player.obj");
		//obj2->getTransform()->translate(glm::vec3(-0, -199, 0));
		//Core::GameObject_* obj5 = Model::loadModel("house plant.obj");
		//obj5->getTransform()->translate(glm::vec3(-300, -599, 700));
		//obj5->setName("Player");
		//obj5->addComponent(new collider());


		//Core::GameObject_* obj3 = Model::loadModel("house plant.obj");
		//obj3->getTransform()->translate(glm::vec3(-500, -599, 1000));
		//obj3->setName("enemy");
		//obj3->addComponent(new collider());

		//Core::GameObject_* obj4 = Model::loadModel("house plant.obj");
		//obj4->getTransform()->translate(glm::vec3(-700, -599, 600));
		//obj4->setName("npc");
		//obj4->addComponent(new collider());
		////obj1->addComponent(new PlayerBaseComponent());
	

		//Core::GameObject_* obj6 = Model::loadModel("Player.obj");
		//obj6->getTransform()->translate(glm::vec3(-100, -599, 100));
		//obj6->setName("obj2");
		//obj6->addComponent(new collider());
		//obj2->addComponent(new RotatingComponent());
		//
		//obj1->addComponent(luaS);

		//Core::GameObject_* testModel = Model::loadModel("Dungeon_Wall_Corner_001.fbx");
		//Core::GameObject_* testModel1 = Model::loadModel("Forge.fbx");
		//testModel1->addComponent(new RotatingComponent());
		////testModel1->addComponent<RotatingComponent>();
		//std::cout << "0: " + glm::to_string(testModel->getTransform()->getPosition()) << std::endl;
		//std::cout << "0: " + std::to_string(testModel->getTransform()->getChildCount()) << std::endl;
		//std::cout << "0: " + glm::to_string(testModel->getTransform()->getRotation()) << std::endl;
		//std::cout << "0: " + glm::to_string(testModel->getTransform()->getScale()) << std::endl;
		//std::cout << "1: " + glm::to_string(testModel1->getTransform()->getPosition()) << std::endl;
		//std::cout << "1: " + glm::to_string(testModel1->getTransform()->getRotation()) << std::endl;
		//std::cout << "1: " + glm::to_string(testModel1->getTransform()->getScale()) << std::endl;
		//std::cout << "1: " + std::to_string(testModel1->getTransform()->getChildCount()) << std::endl;
		////std::cout << "1: " + testModel1->getTransform()->getChild(0)->getGameObject()->getName() << std::endl;
		//std::cout << "1: " + testModel1->getName() << std::endl;
		////Core::GameObject_* tm_ = Model::loadModel("ketabxane.fbx");
		////tm_->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(180.0f));
		////std::cout << tm_->getComponentsCount() << std::endl;
		////tm_->addComponent(new RotatingComponent());
		//Core::GameObject_ *shipFBX = Model::loadModel("Ship_test.fbx");
		//shipFBX->getTransform()->translate(glm::vec3(1000, 0, 0));
		////Core::GameObject_ *shipObject = Model::loadModel("Ship_test.obj");
		////shipObject->getTransform()->translate(glm::vec3(1000, 200, 0));


		////Core::GameObject_* playerModel = Model::loadModel(d.structs[0].meshName);
		////playerModel->getTransform()->setPosition(playerModel->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		////playerModel->addComponent(new PlayerBaseComponent());
		//Core::GameObject_* plane = Model::loadModel("mge/models/plane.obj");
		//plane->getTransform()->scale(glm::vec3(5000));
		//plane->getTransform()->translate(glm::vec3(glm::vec3(0, -600, 0)));

		//Core::GameObject_* mainCharacter = Model::loadModel("MainCharacter.fbx");
		//Collisions
		//btCollisionObject playerCO;
		//btScalar radius = 6;
		//btSphereShape playershape = btSphereShape(radius);


		/*Core::GameObject_* tower = Model::loadModel("Tower.fbx");
		tower->getTransform()->setPosition(tower->getTransform()->getPosition() + glm::vec3(0, -600, 0));*/
		//std::cout << glm::to_string(go->getTransform()->getScale()) << std::endl;
		//lightgo->getTransform()->setPosition(go->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//Core::GameObject_* go = Model::loadModel("cube_smooth.obj");

		//std::cout << go->getComponent<Rendering::Renderer_>()->getGameObject()->getName() << std::endl;
		//go->destroy();
	}
}
