#pragma once
#include "Component.hpp"
#include "glm.hpp"
#include "collider.h"
#include "PressurePlateBehaviour.h"
class GateBehaviour : public Engine::Core::Component
{
public:
	GateBehaviour();
	virtual ~GateBehaviour() = default;
	GateBehaviour(const GateBehaviour& other) = default;
	GateBehaviour& operator=(const GateBehaviour& other) = default;

	void update() override;

	bool IsActivated();
	void TogglePositions();
	void CheckCollision();
	void start() override;
	bool AllPressed();
	void AddPlate(PressurePlateBehaviour* plate);
	 //void awake() override;
	 //void prewake() override;
	 //void update() override;
	 //void fixedUpdate() override;
	 //void lateUpdate() override;
	 //void onValidate() override;
	void destroy() override;
	bool isUniquePerGameObject() override;
private:
	bool _open = false;
	glm::vec3 _activatedPos;
	glm::vec3 _notActivatedPos;
	std::vector<PressurePlateBehaviour*> _plates;
};

