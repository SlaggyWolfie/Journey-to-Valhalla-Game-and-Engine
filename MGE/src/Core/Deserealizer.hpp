#ifndef DESEREALIZER_HPP
#define DESEREALIZER_HPP

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include "../_vs2015/Scene.hpp"

using namespace std;
class Game;
/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
class Deserealizer
{
    public:
<<<<<<< HEAD
		Deserealizer();
		void Deserealizer::ConstructGameObj();
		vector<GameObject_*> gms;
		string oneMesh;
=======

		Deserealizer(Game* game);
		Engine::Scene ConstructGameObj();
>>>>>>> f5e6312b9895fddb1fc553656729c4784828f5f2
};

#endif // DESEREALIZER_HPP
