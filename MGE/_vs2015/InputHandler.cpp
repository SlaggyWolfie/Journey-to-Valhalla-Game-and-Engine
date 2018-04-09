#include "InputHandler.hpp"

std::map<sf::Keyboard::Key, KeyState> InputHandler::_keyboardKeys;
std::map<sf::Mouse::Button, KeyState> InputHandler::_mouseButtons;
std::unordered_set<sf::Mouse::Button> InputHandler::_mouseButtonsConsumed;
std::unordered_set<sf::Keyboard::Key> InputHandler::_keyboardKeysConsumed;

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
	_mouseButtonsConsumed.insert(key);
	return _mouseButtons[key] == PRESSED;
}

bool InputHandler::keyUp(const sf::Mouse::Button key)
{
	_mouseButtonsConsumed.insert(key);
	return _mouseButtons[key] == RELEASED;
}

bool InputHandler::keyPressed(const sf::Mouse::Button key)
{
	return _mouseButtons[key] == PRESSED;
}

bool InputHandler::keyDown(const sf::Keyboard::Key key)
{
	_keyboardKeysConsumed.insert(key);
	return _keyboardKeys[key] == PRESSED;
}

bool InputHandler::keyUp(const sf::Keyboard::Key key)
{
	_keyboardKeysConsumed.insert(key);
	return _keyboardKeys[key] == RELEASED;
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

void InputHandler::update()
{
	for (auto& key : _keyboardKeysConsumed)
		_keyboardKeys[key] = NOT_PRESSED;

	for (auto& key : _mouseButtonsConsumed)
		_mouseButtons[key] = NOT_PRESSED;

	_keyboardKeysConsumed.clear();
	_mouseButtonsConsumed.clear();
}

void InputHandler::updateEvent(const sf::Event event)
{
	//const bool keyPressed = event.type == sf::Event::KeyPressed;
	//const bool keyReleased = event.type == sf::Event::KeyReleased;

	//if (keyPressed || keyReleased) 

	switch (event.type)
	{
	case sf::Event::KeyPressed:
	{
		const sf::Keyboard::Key key = event.key.code;
		testKey(key);

		_keyboardKeys[key] = PRESSED;
		break;
	}
	case sf::Event::KeyReleased:
	{
		_keyboardKeys[event.key.code] = RELEASED;
		break;
	}
	case sf::Event::MouseButtonPressed:
	{
		const sf::Mouse::Button key = event.mouseButton.button;
		testKey(key);

		_mouseButtons[event.mouseButton.button] = PRESSED;
		break;

	}
	default:
	case sf::Event::MouseButtonReleased:
	{
		_mouseButtons[event.mouseButton.button] = RELEASED;
		break;
	}
	}
}
