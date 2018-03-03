#pragma once
#include "Manager.hpp"
#include <vector>
#include "collider.h"
#include "glm.hpp"
using namespace Engine;
class ColliderManager:public Manager
{
public:
	void initialize() override;
	ColliderManager();
	~ColliderManager();
	std::vector<collider> CheckCollision(collider object);
private:
	std::vector<collider> allColliders;

};

