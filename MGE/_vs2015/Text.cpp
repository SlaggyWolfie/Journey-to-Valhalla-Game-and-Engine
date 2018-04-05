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
		Text* Text::_hint = nullptr;

		Text* Text::hint()
		{
			if (!_hint)
			{
				_hint = new Text(false);
				_hint->getTextObject().setPosition(600, 600);
				_hint->setTextInformation("HINT!");
				_hint->getTextObject().setCharacterSize(100);
				_hint->getTextObject().setFillColor(sf::Color::White);
				_hint->setFont("mge/fonts/arial.ttf");
			}

			return _hint;
		}

		void Text::showHint(const std::string& hintText, const float startAfterTime, const float duration)
		{
			hint()->setTextInformation(hintText);

			std::function<void()> hide = []
			{
				Text::drawHint = false;
			};
			std::function<void()> unhide = [duration, hide]
			{
				Text::drawHint = true;
				Engine::Utility::Time::timeout(duration, hide);
			};

			Engine::Utility::Time::timeout(startAfterTime, unhide);
		}

		Text::Text(const bool rendering) : ComponentUI(rendering)
		{

			//t =  sf::Text();
			//sf::Font f;
			//f.loadFromFile("mge/fonts/arial.ttf");
			//t.setFont(f);
			//t.setString("check");
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
				std::cout << "Undefined Text Alignment. Defaulting to Left Justified." << std::endl;
			case Left_Justified:
				_text.setOrigin(0, 0); break;
			case Right_Justified:
				_text.setOrigin(100, 0); break;
			case Middle_Justified:
				_text.setOrigin(50, 0); break;
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
