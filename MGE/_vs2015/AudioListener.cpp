#include "AudioListener.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include <SFML/Audio.hpp>

namespace Engine
{
	namespace Audio
	{
		AudioListener::AudioListener(): _volume(1)
		{
			setVolume(_volume);
		}

		void AudioListener::setVolume(float volume)
		{
			volume = glm::clamp(volume, 0.0f, 1.0f);
			_volume = volume;
			//sf::Listener::
			sf::Listener::setGlobalVolume(volume * 100);
		}

		float AudioListener::getVolume() const
		{
			return _volume;
		}

		void AudioListener::destroy()
		{
			delete this;
		}

		void AudioListener::update()
		{
			Core::Transform* transform = getGameObject()->getTransform();
			const glm::vec3 position = transform->getPosition();
			const glm::vec3 forward = transform->forward();
			const glm::vec3 up = transform->up();

			sf::Listener::setPosition(position.x, position.y, position.z);
			sf::Listener::setDirection(forward.x, forward.y, forward.z);
			sf::Listener::setUpVector(up.x, up.y, up.z);
		}

		bool AudioListener::isUniquePerGameObject()
		{
			return true;
		}
	}
}