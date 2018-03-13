#include "TextHUD.hpp"
#include <iostream>
#include <GL/glew.h>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"

namespace Engine
{
	namespace Rendering
	{
		TextHUD::TextHUD() : _window(nullptr)
		{
		}

		TextHUD::TextHUD(sf::RenderWindow* window) : _window(nullptr)
		{
		}

		void TextHUD::setWindow(sf::RenderWindow* window)
		{
			_window = window;
		}

		void TextHUD::setTextAlignment(const TextAlignment alignment)
		{
			_alignment = alignment;
			switch (alignment)
			{
			default:
				std::cout << "Undefined Text Alignment. Defaulting to Left." << std::endl;
			case Left_Justified:
				_text.setOrigin(0, 0); break;
			case Right_Justified:
				_text.setOrigin(0, 0); break;
			case Middle_Justified:
				_text.setOrigin(0, 0); break;
			}
		}

		TextAlignment TextHUD::getTextAlignment() const
		{
			return _alignment;
		}

		void TextHUD::draw() 
		{
			glActiveTexture(GL_TEXTURE0);
			_window->pushGLStates();
			_window->draw(_text);
			_window->popGLStates();
		}

		//TextHUD::~TextHUD()
		//{
		//}

		//TextHUD::TextHUD(const TextHUD& other) : _window(other._window), _info(other._info), _font(other._font), _text(other._text)
		//{
		//}

		//TextHUD& TextHUD::operator=(const TextHUD& other)
		//{
		//	_window = other._window;
		//	_info = other._info;
		//	_font = other._font;
		//	_text = other._text;
		//}

		void TextHUD::setTextInformation(const std::string& text)
		{
			_info = text;
			_text.setString(text);
		}

		std::string TextHUD::getTextInformation() const
		{
			return _info;
		}

		sf::Text& TextHUD::getTextObject()
		{
			return _text;
		}

		sf::Font& TextHUD::getFontObject()
		{
			return _font;
		}

		void TextHUD::setFont(const std::string& fontPath)
		{
			sf::Font* font = new sf::Font();;
			if (!font->loadFromFile(fontPath))
			{
				std::cout << "Could not load font, returning..." << std::endl;
				return;
			}

			setFont(*font);
		}

		void TextHUD::setFont(const sf::Font& font)
		{
			_font = font;
			_text.setFont(font);
		}
	}
}
