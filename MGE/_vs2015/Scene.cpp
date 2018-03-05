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
		for (const GameObject_s& gameStruct : structs)
		{
			GameObject_* gameObject;

			if (!gameStruct.meshName.empty() && gameStruct.meshName != std::string("")
				&& gameStruct.meshName.find('.') != std::string::npos)
			{
				gameObject = Model::loadModel(gameStruct.meshName);
				gameObject->setName(gameStruct.name);
			}
			else
			{
				gameObject = new GameObject_(gameStruct.name, "");
			}
			

			Transform* transform = gameObject->getTransform();
			transform->setLocalPosition(gameStruct.position);
			std::cout << gameStruct.name + " Position: " + glm::to_string(gameStruct.position) << std::endl;
			transform->setLocalRotation(glm::eulerAngleXYZ(gameStruct.rotation.x, gameStruct.rotation.y, gameStruct.rotation.z));
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
			transform->setParent(id_to_go[parentID]->getTransform(), true);
		}
	}

	void Scene::hardCode()
	{
		LuaScript* luaS = new LuaScript();

		Deserealizer d;
		Core::GameObject_* camera = new Core::GameObject_("Cam", "", glm::vec3(0, 0, 2000));
		Core::GameObject_* lightgo = new Core::GameObject_("Light", "", glm::vec3(500, 0, 2000));
		Rendering::Light_* light = new Rendering::Light_();
		lightgo->addComponent(light);
		light->setLightType(Rendering::LightType::Directional);
		lightgo->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		lightgo->getTransform()->rotate(lightgo->getTransform()->right(), -glm::radians(30.0f));
		//Core::GameObject_* lightgo1 = new Core::GameObject_("Light", "", glm::vec3(-500, -500, 2000));
		Core::GameObject_* lightgo1 = Model::loadModel("mge/models/cube_smooth.obj");
		lightgo1->getTransform()->translate(glm::vec3(-500, -599, 2000));
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
		Core::GameObject_* playerModel = Model::loadModel(d.structs[0].meshName);
		//playerModel->getTransform()->setPosition(playerModel->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//playerModel->addComponent(new PlayerBaseComponent());
		Core::GameObject_* obj1 = Model::loadModel("Player.obj");
		obj1->getTransform()->translate(glm::vec3(-300, -599, 300));
		obj1->setName("Player");
		obj1->addComponent(new collider());
		obj1->addComponent(new PlayerBaseComponent());

		
	

		Core::GameObject_* obj2 = Model::loadModel("Player.obj");
		obj2->getTransform()->translate(glm::vec3(-100, -599, 300));
		obj2->setName("obj2");
		obj2->addComponent(new collider());
		
		obj1->addComponent(luaS);


		//Core::GameObject_* playerModel = Model::loadModel(d.structs[0].meshName);
		//playerModel->getTransform()->setPosition(playerModel->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//playerModel->addComponent(new PlayerBaseComponent());
		Core::GameObject_* plane = Model::loadModel("mge/models/plane.obj");
		plane->getTransform()->scale(glm::vec3(5000));
		plane->getTransform()->translate(glm::vec3(glm::vec3(0, -600, 0)));

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
