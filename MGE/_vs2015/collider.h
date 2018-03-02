#pragma once
#include "Component.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include "glm.hpp"
using namespace Engine::Core;
class collider:public Component
{
public:

	collider();
	~collider();

	void update() override;
	 //void start() override;
	 //void awake() override;
	 //void prewake() override;
	 //void update() override;
	 //void fixedUpdate() override;
	 //void lateUpdate() override;
	 //void onValidate() override;
};

