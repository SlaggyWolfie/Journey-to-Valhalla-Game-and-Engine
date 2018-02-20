#pragma once
#include<../_vs2015/GameObject_.hpp>
#include <vector>
using namespace Engine::Core;

class Scene
{

public:

	Scene();
	void AddGameObject(GameObject_* obj);
	~Scene();

private:	
	std::vector<GameObject_*> _SceneObjects;
};

