#include "InputHandler.hpp"

std::map<sf::Keyboard::Key, KeyState> InputHandler::_keyboardKeys;
std::map<sf::Mouse::Button, KeyState> InputHandler::_mouseButtons;

void InputHandler::testKey(const sf::Keyboard::Key key)
{
	if (_keyboardKeys.find(key) != _keyboardKeys.end()) _keyboardKeys[key] = NOT_PRESSED;
}

void InputHandler::testKey(const sf::Mouse::Button key)
{
	if (_mouseButtons.find(key) != _mouseButtons.end()) _mouseButtons[key] = NOT_PRESSED;
}

bool InputHandler::keyDown(const sf::Mouse::Button key)
{
	const bool check = _mouseButtons[key] == PRESSED;

	if (check)
	{
		_mouseButtons[key] = NOT_PRESSED;
		return true;
	}

	return false;
}

bool InputHandler::keyUp(const sf::Mouse::Button key)
{
	const bool check = _mouseButtons[key] == RELEASED;

	if (check)
	{
		_mouseButtons[key] = NOT_PRESSED;
		return true;
	}

	return false;
}

bool InputHandler::keyPressed(const sf::Mouse::Button key)
{
	return _mouseButtons[key] == PRESSED;
}

bool InputHandler::keyDown(const sf::Keyboard::Key key)
{
	const bool check = _keyboardKeys[key] == PRESSED;

	if (check)
	{
		_keyboardKeys[key] = NOT_PRESSED;
		return true;
	}

	return false;
}

bool InputHandler::keyUp(const sf::Keyboard::Key key)
{
	const bool check = _keyboardKeys[key] == RELEASED;

	if (check)
	{
		_keyboardKeys[key] = NOT_PRESSED;
		return true;
	}

	return false;
}

bool InputHandler::keyPressed(const sf::Keyboard::Key key)
{
	return _keyboardKeys[key] == PRESSED;
}

void InputHandler::reset()
{
	_keyboardKeys.clear();
	_mouseButtons.clear();
}

void InputHandler::updateEvent(const sf::Event event)
{
	//const bool keyPressed = event.type == sf::Event::KeyPressed;
	//const bool keyReleased = event.type == sf::Event::KeyReleased;

	//if (keyPressed || keyReleased) 

	if (event.type == sf::Event::KeyPressed)
	{
		const sf::Keyboard::Key key = event.key.code;
		testKey(key);

		_keyboardKeys[key] = PRESSED;

		return;
	}

	if (event.type == sf::Event::KeyReleased)
	{
		_keyboardKeys[event.key.code] = RELEASED;
		return;
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		const sf::Mouse::Button key = event.mouseButton.button;
		testKey(key);

		_mouseButtons[event.mouseButton.button] = PRESSED;

		return;
	}

	if (event.type == sf::Event::MouseButtonReleased)
	{
		_mouseButtons[event.mouseButton.button] = RELEASED;
	}
}
