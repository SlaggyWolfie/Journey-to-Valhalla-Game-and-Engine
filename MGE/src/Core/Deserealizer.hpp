#ifndef DESEREALIZER_HPP
#define DESEREALIZER_HPP

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "../_vs2015/Scene.hpp"

class Game;
/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
class Deserealizer
{
    public:

		Deserealizer(Game* game);
		Engine::Scene ConstructGameObj();
};

#endif // DESEREALIZER_HPP
