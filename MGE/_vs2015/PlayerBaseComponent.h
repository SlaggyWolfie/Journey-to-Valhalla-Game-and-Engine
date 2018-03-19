#pragma once
#include "Component.hpp"
#include "glm.hpp"
#include "collider.h"
class PlayerBaseComponent : public Engine::Core::Component
{
public:
	enum _playerState { jumpingToObject, usingObject, idle, jumpingFromObject };
	PlayerBaseComponent();
	virtual ~PlayerBaseComponent() = default;
	PlayerBaseComponent(const PlayerBaseComponent& other) = default;
	PlayerBaseComponent& operator=(const PlayerBaseComponent& other) = default;

	void update() override;
	void start() override;
	//void awake() override;
	//void prewake() override;
	//void update() override;
	//void fixedUpdate() override;
	//void lateUpdate() override;
	//void onValidate() override;
	void MoveInsideObj(GameObject_* obj);
	void RayCast();
	bool IsInside();
	//void Movement();
	glm::vec3 moveTowards(glm::vec3 current, glm::vec3 target, float maxDelta);
	void SetInside(bool n);
	void destroy() override;
	bool isUniquePerGameObject() override;
	glm::vec3 lastPos;
private:
	bool _insideObj;
	glm::vec3 _targetScale;
	glm::vec3 _originalScale;
	GameObject_* _objectToMove;
	glm::vec3 _targetPosition;
	_playerState _playerS = idle;

};

