#pragma once
#include "Component.hpp"
#include "glm.hpp"
#include "collider.h"
class PressurePlateBehaviour : public Engine::Core::Component
{
public:
	PressurePlateBehaviour();
	virtual ~PressurePlateBehaviour() = default;
	PressurePlateBehaviour(const PressurePlateBehaviour& other) = default;
	PressurePlateBehaviour& operator=(const PressurePlateBehaviour& other) = default;

	void update() override;

	bool IsPressed();
	void TogglePositions();
	void SetPressed(bool value);
	void CheckCollision();
	void start() override;
	 //void awake() override;
	 //void prewake() override;
	 //void update() override;
	 //void fixedUpdate() override;
	 //void lateUpdate() override;
	 //void onValidate() override;
	void destroy() override;
	bool isUniquePerGameObject() override;
private:
	bool _isPressed = false;
	glm::vec3 _activatedPos;
	glm::vec3 _notActivatedPos;
};

