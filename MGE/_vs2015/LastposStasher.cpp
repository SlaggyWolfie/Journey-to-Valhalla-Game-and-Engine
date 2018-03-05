#include "LastposStasher.h"
#include "Camera_.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"

LastposStasher::LastposStasher()
{

}

glm::vec3 LastposStasher::GetLastPos()
{
	return lastPos;
}

void LastposStasher::SetLastPos(glm::vec3 p)
{
	lastPos = p;
}
