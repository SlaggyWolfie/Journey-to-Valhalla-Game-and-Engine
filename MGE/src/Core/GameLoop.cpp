#include <iostream>

#include "GameLoop.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/World.hpp"

GameLoop::GameLoop(Renderer* pRenderer) :_renderer(pRenderer), _fps(0)
{
	

	
	//ctor
}

GameLoop::~GameLoop()
{
	//dtor
	delete _renderer;
	delete _game;
}








///MAIN GAME LOOP
void GameLoop::update()
{
	//setting to calculate fps

	while (_window->isOpen()) {

			_window->display();


		//empty the event queue
		//_processEvents();
	}
}

void GameLoop::renderUpdate()
{

}

void GameLoop::fixedUpdate()
{
	sf::Clock renderClock;
	int frameCount = 0;
	float timeSinceLastFPSCalculation = 0;

	//settings to make sure the update loop runs at 60 fps
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Clock updateClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window->isOpen()) {
		timeSinceLastUpdate += updateClock.restart();

		if (timeSinceLastUpdate > timePerFrame)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			while (timeSinceLastUpdate > timePerFrame) {
				timeSinceLastUpdate -= timePerFrame;
				_update(timePerFrame.asSeconds());
			}

			renderUpdate();
			_window->display();

			//fps count is updated once every 1 second
			frameCount++;
			timeSinceLastFPSCalculation += renderClock.restart().asSeconds();
			if (timeSinceLastFPSCalculation > 1) {
				_fps = frameCount / timeSinceLastFPSCalculation;
				timeSinceLastFPSCalculation -= 1;
				frameCount = 0;
			}

		}

		//empty the event queue
		//_processEvents();
	}

}

void GameLoop::lateUpdate()
{
}


void GameLoop::_update(float pStep) {
	/*or (int i = _gameobjectupdatelist.size(); i>0; i--)
	{
		_gameobjectupdatelist[i]->update();
		(*_gameobjectupdatelist[i]).update();
	}*/
}

void GameLoop::_render() {
	//_renderer->render(_game);
}

void GameLoop::_processEvents()
{
	sf::Event event;
	bool exit = false;

	//we must empty the event queue
	while (_window->pollEvent(event)) {
		//give all system event listeners a chance to handle events
		//optionally to be implemented by you for example you could implement a
		//SystemEventDispatcher / SystemEventListener pair which allows Listeners to
		//register with the dispatcher and then do something like:
		//SystemEventDispatcher::dispatchEvent(event);

		switch (event.type) {

		case sf::Event::MouseWheelMoved:
		case sf::Event::MouseWheelScrolled:
		{
			//mouseScrollWheel += (event.mouseWheel.delta * 0.01f);
			//if (mouseScrollWheel < 0) mouseScrollWheel = 0;
			break;
		}
		case sf::Event::Closed:
			exit = true;
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				exit = true;
			}
			break;
		case sf::Event::Resized:
			//would be better to move this to the renderer
			//this version implements nonconstrained match viewport scaling
			glViewport(0, 0, event.size.width, event.size.height);
			break;

		default:
			break;
		}
	}

	if (exit) {
		_window->close();
	}
}



