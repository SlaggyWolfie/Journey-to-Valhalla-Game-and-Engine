#pragma once
#include "Component.hpp"
#include "glm.hpp"
class LastposStasher : public Engine::Core::Component
{
public:

	LastposStasher();
	glm::vec3 GetLastPos();
	void SetLastPos(glm::vec3 p);

	 //void start() override;
	 //void awake() override;
	 //void prewake() override;
	 //void update() override;
	 //void fixedUpdate() override;
	 //void lateUpdate() override;
	 //void onValidate() override;
	glm::vec3 lastPos = glm::vec3(0);
};

