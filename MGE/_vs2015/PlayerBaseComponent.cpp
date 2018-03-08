#include "PlayerBaseComponent.h"
#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
#include "LastposStasher.h"

PlayerBaseComponent::PlayerBaseComponent()
{
}

void PlayerBaseComponent::update()
{
	
}

void PlayerBaseComponent::destroy()
{
	delete this;
}

bool PlayerBaseComponent::isUniquePerGameObject()
{
	return true;
}
