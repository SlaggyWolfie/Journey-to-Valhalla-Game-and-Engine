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


namespace Engine
{
	Scene::Scene() : _name(nullptr), _gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
	{
	}

	Scene::Scene(std::string  name) : _name(std::move(name)), _gameObjects(std::vector<std::unique_ptr<Core::GameObject_>>())
	{
	}

	Scene::~Scene()
	{
		for (auto& go : _gameObjects)
			go = nullptr;
		_gameObjects.clear();
	}

	void Scene::initialize(const bool hard)
	{
		if (hard)
		{
			hardCode();
			return;
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

	void Scene::hardCode()
	{
		Core::GameObject_* camera = new Core::GameObject_("Cam", "", glm::vec3(0, 0, 2000));
		Core::GameObject_* lightgo = new Core::GameObject_("Light", "", glm::vec3(500, 0, 2000));
		Rendering::Light_* light = new Rendering::Light_();
		lightgo->addComponent(light);
		light->setLightType(Rendering::LightType::Point);
		lightgo->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		Core::GameObject_* lightgo1 = new Core::GameObject_("Light", "", glm::vec3(-500, 0, 2000));
		Rendering::Light_* light1 = new Rendering::Light_();
		lightgo1->addComponent(light1);
		light1->setLightType(Rendering::LightType::Point);
		lightgo1->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(60.0f));
		light1->setColor(glm::vec3(0, 1, 0));
		light1->setRange(3500);
		light->setRange(3500);
		Core::Camera_* cameraComp = new Core::Camera_();
		camera->addComponent(cameraComp);
		Core::Camera_::setMainCamera(cameraComp);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientLightColor(glm::vec3(1));
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAmbientStrength(0.3f);
		ServiceLocator::instance()->getService<Rendering::LightManager>()->setAttenuation(1.0f, 0.07f, 0.017f);
		Core::GameObject_* go = Model::loadModel("Tower.fbx");
		go->getTransform()->setPosition(go->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		std::cout << glm::to_string(go->getTransform()->getScale()) << std::endl;
		//lightgo->getTransform()->setPosition(go->getTransform()->getPosition() + glm::vec3(0, -600, 0));
		//Core::GameObject_* go = Model::loadModel("cube_smooth.obj");

		//std::cout << go->getComponent<Rendering::Renderer_>()->getGameObject()->getName() << std::endl;
		//go->destroy();
	}
}