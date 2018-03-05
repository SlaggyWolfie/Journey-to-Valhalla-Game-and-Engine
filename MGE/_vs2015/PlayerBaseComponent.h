#pragma once
#include "Component.hpp"
#include "glm.hpp"
class PlayerBaseComponent : public Engine::Core::Component
{
public:

	PlayerBaseComponent();
	virtual ~PlayerBaseComponent() = default;
	PlayerBaseComponent(const PlayerBaseComponent& other) = default;
	PlayerBaseComponent& operator=(const PlayerBaseComponent& other) = default;

	void update() override;
	 //void start() override;
	 //void awake() override;
	 //void prewake() override;
	 //void update() override;
	 //void fixedUpdate() override;
	 //void lateUpdate() override;
	 //void onValidate() override;
	glm::vec3 lp;
	void destroy() override;
	bool isUniquePerGameObject() override;
};

