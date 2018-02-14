#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>

class Game;
class Renderer;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
class GameLoop
{
    public:

        GameLoop(Renderer* renderer);
        virtual ~GameLoop();

		virtual void update();
		virtual void fixedUpdate();
		virtual void lateUpdate();
        //run the actual process of updating all objects, rendering them and processing events
		virtual void renderUpdate();

    protected:
        //call update on all game objects in the display root
        virtual void _update(float pStep);
        //render all game objects in the display root
        virtual void _render();
        //process any sfml window events (see SystemEventDispatcher/Listener)
        virtual void _processEvents();

		sf::RenderWindow* _window;  //sfml window to render into
		Renderer* _renderer;        //the renderer class to render the world
		Game* _game;              //the root game object that represents our scene
		float _fps;                 //stores the real fps
		//std::vector<GameObject*> _gameObjectUpdateList;

    private:
        GameLoop(const GameLoop&);
        GameLoop& operator=(const GameLoop&);


};

#endif // GAMELOOP_HPP
