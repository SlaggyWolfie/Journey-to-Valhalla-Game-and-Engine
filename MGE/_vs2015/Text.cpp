#include "Text.hpp"
#include <iostream>
#include <GL/glew.h>
#include "ServiceLocator.hpp"
#include "Core/Game.hpp"
#include "Time.hpp"

namespace Engine
{
	namespace UI
	{
		bool Text::drawHint = false;
		Text* Text::hint = nullptr;

		void Text::makeHint()
		{
			hint = new Text(false);
		}
		void Text::showHint(std::string hintText, float startAfterTime, float duration)
		{
			if (!hint) makeHint();
			std::cout << "\t\t Call show hint" << std::endl;
			std::function<void()> hide = [] {

				std::cout << "\t\t hide hint" << std::endl;
				drawHint = false; };
			std::function<void()> unhide = [duration, &hide] 
			{ 
				std::cout << "\t\t unhide hint" << std::endl;
				drawHint = true;
				Engine::Utility::Time::timeout(duration, hide);
			};

			Engine::Utility::Time::timeout(startAfterTime, unhide);
		}
		Text::Text(const bool rendering) : ComponentUI(rendering)
		{
			setTextInformation("");
		}

		Text::Text(sf::RenderWindow* window, const bool rendering) : ComponentUI(window, rendering)
		{
			setTextInformation("");
		}

		void Text::setTextAlignment(const TextAlignment alignment)
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

		TextAlignment Text::getTextAlignment() const
		{
			return _alignment;
		}

		void Text::draw()
		{
			getWindow()->draw(_text);
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

		void Text::setTextInformation(const std::string& text)
		{
			_info = text;
			_text.setString(text);
		}

		std::string Text::getTextInformation() const
		{
			return _info;
		}

		sf::Text& Text::getTextObject()
		{
			return _text;
		}
		sf::Font& Text::getFontObject()
		{
			return _font;
		}

		void Text::setFont(const std::string& fontPath)
		{
			sf::Font* font = new sf::Font();;
			if (!font->loadFromFile(fontPath))
			{
				std::cout << "Could not load font, returning..." << std::endl;
				return;
			}

			setFont(*font);
		}

		void Text::setFont(const sf::Font& font)
		{
			_font = font;
			_text.setFont(font);
			setDrawable(&_text);
		}
	}
}
