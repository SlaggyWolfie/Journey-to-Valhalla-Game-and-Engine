#include "Scene.h"



Scene::Scene()
{
}

void Scene::AddGameObject(GameObject_* obj)
{
	_SceneObjects.push_back(obj);
	
}
Scene::~Scene()
{
}
