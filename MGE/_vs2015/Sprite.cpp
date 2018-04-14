#include "Sprite.hpp"
#include <iostream>
#include <GL/glew.h>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"

namespace Engine
{
	namespace UI
	{
		Sprite::Sprite(const bool rendering) : ComponentUI(rendering)
		{
		}

		Sprite::Sprite(sf::RenderWindow* window, const bool rendering) : ComponentUI(window, rendering)
		{
		}

		//SpriteHUD::~SpriteHUD()
		//{
		//}

		//SpriteHUD::SpriteHUD(const SpriteHUD& other) : _window(other._window), _info(other._info), _font(other._font), _text(other._text)
		//{
		//}

		//SpriteHUD& SpriteHUD::operator=(const SpriteHUD& other)
		//{
		//	_window = other._window;
		//}

		sf::Sprite& Sprite::getSprite()
		{
			return _sprite;
		}

		sf::Texture & Sprite::getTexture()
		{
			return _texture;
		}

		void Sprite::loadSprite(const sf::Texture & texture)
		{
			//sf::Sprite sprite;
			_sprite.setTexture(texture, true);
			setDrawable(&_sprite);
		}

		void Sprite::loadSprite(const std::string & texturePath)
		{
			//sf::Texture texture;
			if (!_texture.loadFromFile(texturePath))
			{
				std::cout << "Texture didn't load in Sprite." << std::endl;
				return;
			}

			loadSprite(_texture);
		}

		void Sprite::changeTexture(const sf::Texture & texture)
		{
			_texture = texture;
			_sprite.setTexture(texture, true);
		}

		void Sprite::changeTexture(const std::string & texturePath)
		{
			sf::Texture texture;
			if (!texture.loadFromFile(texturePath))
			{
				std::cout << "Texture didn't load in Sprite." << std::endl;
				return;
			}

			changeTexture(texture);
		}

		void Sprite::draw()
		{
			getWindow()->draw(_sprite);
		}
	}
}
