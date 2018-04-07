#include "PressurePlateBehaviour.h"
#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"
using namespace Engine::Core;
using namespace Engine;


PressurePlateBehaviour::PressurePlateBehaviour()
{


}

void PressurePlateBehaviour::update()
{
	TogglePositions();
	CheckCollision();
	//std::cout << "PP: " + glm::to_string(getGameObject()->getTransform()->getPosition() )<< std::endl;
	//std::cout << "PPCP1: " + glm::to_string(getGameObject()->getComponent<collider>()->point1) << std::endl;
}

bool PressurePlateBehaviour::IsPressed()
{
	return _isPressed;
}

void PressurePlateBehaviour::SetPressed(bool value)
{
	_isPressed = value;
}

void PressurePlateBehaviour::CheckCollision()
{

	std::vector<collider*> colList = ServiceLocator::instance()->getService<ColliderManager>()->
	CheckCollision(getGameObject()->getComponent<collider>());

	//std::cout << colList.size()<<" size " << std::endl;
	/*std::cout << this->getGameObject()->getComponent<collider>()->GetPos() << " pos " << std::endl;*/

	for (unsigned i = 0; i < colList.size(); i++)
	{
		//std::cout << colList.size() << " "<<"size" << std::endl;
		std::string name = colList[i]->getGameObject()->getName();
		if (name.find("Runestone") != std::string::npos)
		{
			std::cout << "I should be pressed" << std::endl;
			SetPressed(true);
			return;
		}
	}

	SetPressed(false);
}

void PressurePlateBehaviour::start()
{
	Transform* transform = getGameObject()->getTransform();
	_notActivatedPos = transform->getPosition();
	_activatedPos = _notActivatedPos//transform->getPosition();
		//;
		+ glm::vec3(0, -0.3f, 0);
}

void PressurePlateBehaviour::TogglePositions()
{
	const glm::vec3 targetPosition = _isPressed ? _activatedPos : _notActivatedPos;

	const auto check = glm::equal(getGameObject()->getTransform()->getPosition(), targetPosition);
	if (!check.y)
		getGameObject()->getTransform()->setPosition(lerp(this->getGameObject()->getTransform()->getPosition(), targetPosition, 0.1f));
}
void PressurePlateBehaviour::destroy()
{
	delete this;
}

bool PressurePlateBehaviour::isUniquePerGameObject()
{
	return true;
}
