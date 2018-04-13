#include "Text.hpp"
#include <iostream>
#include "Core/Game.hpp"
#include "Time.hpp"

namespace Engine
{
	namespace UI
	{
		//bool Text::drawHint = false;
		Hint* Text::_hint = nullptr;
		//sf::Vector2f Text::hintTargetPosition;

		void Hint::showHint(const std::string& hintText, const sf::Vector2f target, const float duration)
		{
			//hint()->setTextInformation(hintText);
			draw = true;
			move = true;
			durationSet = false;
			showing = true;
			targetPosition = target;
			this->duration = duration;
		}

		void Hint::startCountdown()
		{
			bool* move_p = &move;
			sf::Vector2f* hintTargetPosition_p = &targetPosition;
			sf::Vector2f* startPosition_p = &startPosition;

			const std::function<void()> moveDown = [move_p, hintTargetPosition_p, startPosition_p]() -> void
			{
				*move_p = true;
				*hintTargetPosition_p = *startPosition_p;
			};

			Engine::Utility::Time::timeout(duration, moveDown);
			durationSet = true;
		}

		void Hint::shouldDraw(const bool show)
		{
			draw = show;
		}

		void Hint::setPosition(const float x, const float y) const
		{
			text->getTextObject().setPosition(x, y);
		}

		void Hint::setPosition(const sf::Vector2f position) const
		{
			text->getTextObject().setPosition(position);
		}

		Hint::Hint(const bool rendering)
		{
			text = new Text(rendering);

			const sf::Vector2f windowSize = static_cast<sf::Vector2f>(text->getWindow()->getSize());

			startPosition = sf::Vector2f(windowSize.x * 0.5f, windowSize.y * 1.1f);
			text->getTextObject().setPosition(startPosition);
			text->setTextInformation("HINT!");
			text->getTextObject().setCharacterSize(30);
			text->getTextObject().setFillColor(sf::Color::White);
			text->setFont(File::findPath("celtic.ttf"));
		}

		void Hint::update()
		{
			if (showing && move)
			{
				setPosition(Text::moveTowards(text->getTextObject().getPosition(), targetPosition, 1000 * Engine::Utility::Time::deltaTime()));
				if (text->getTextObject().getPosition() == targetPosition)
				{
					move = false;
					if (!durationSet)
					{
						durationSet = true;
						startCountdown();
						return;
					}

					showing = false;
				}
			}
		}

		Hint* Text::hint()
		{
			if (!_hint) _hint = new Hint(false);
			return _hint;
		}

		//Text* Text::hint()
		//{
		//	if (!_hint)
		//	{
		//		_hint = new Text(false);
		//		_hint->getTextObject().setPosition(600, 600);
		//		_hint->setTextInformation("HINT!");
		//		_hint->getTextObject().setCharacterSize(100);
		//		_hint->getTextObject().setFillColor(sf::Color::White);
		//		_hint->setFont("mge/fonts/arial.ttf");
		//	}

		//	return _hint;
		//}

		//void Text::showHint(const std::string& hintText, const float startAfterTime, const float duration)
		//{
		//	hint()->setTextInformation(hintText);

		//	std::function<void()> hide = []
		//	{
		//		Text::drawHint = false;
		//	};
		//	std::function<void()> unhide = [duration, hide]
		//	{
		//		Text::drawHint = true;
		//		Engine::Utility::Time::timeout(duration, hide);
		//	};

		//	Engine::Utility::Time::timeout(startAfterTime, unhide);
		//}

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


		sf::Vector2f Text::moveTowards(const sf::Vector2f current, const sf::Vector2f target, const float delta)
		{
			const sf::Vector2f deltaLocal = target - current;
			const float length = std::hypot(deltaLocal.x, deltaLocal.y);
			const sf::Vector2f normalDelta = deltaLocal / length;
			const sf::Vector2f movementDelta = length > delta ? normalDelta * delta : deltaLocal;
			return current + movementDelta;
		}

		sf::Vector2f Text::lerp(const sf::Vector2f current, const sf::Vector2f target, float delta)
		{
			if (delta > 1) delta = 1;
			if (delta < 0) delta = 0;
			return (1 - delta) * current + target * delta;
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
