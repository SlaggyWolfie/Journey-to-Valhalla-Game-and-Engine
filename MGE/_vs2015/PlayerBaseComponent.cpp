#include "PlayerBaseComponent.h"



PlayerBaseComponent::PlayerBaseComponent()
{
}


PlayerBaseComponent::~PlayerBaseComponent()
{
}

void PlayerBaseComponent::update()
{
	GameObject_* p = getGameObject();
	p->getTransform()->rotate(glm::vec3(0, 1, 0), glm::radians(0.03f));
}
