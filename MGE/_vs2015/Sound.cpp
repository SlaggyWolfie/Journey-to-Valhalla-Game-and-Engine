#include "Sound.hpp"
#include <glm.hpp>
#include <SFML/System/Time.hpp>
#include "GameObject_.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "ResourceManager.hpp"

namespace Engine
{
	namespace Audio
	{
		ResourceManager* Sound::_resourceManager = nullptr;

		Sound::Sound() : _buffer(nullptr), _instance(nullptr), _filename(""), _status(AudioStatus::NoSound), _volume(1),
			_pitch(1), _looping(false), _attenuation(1), _minimumDistance(1)
		{
		}

		Sound::Sound(const std::string& filename) : Sound()
		{
			load(filename);
		}

		Sound::~Sound()
		{
			stop();

			_buffer = nullptr;
			_instance = nullptr;
		}

		void Sound::load(const std::string& filename)
		{
			_filename = filename;

			sf::SoundBuffer* buffer = getResourceManager()->getSoundBuffer(filename);

			if (!buffer)
			{
				if (!buffer->loadFromFile(filename))
				{
					std::cout << "Failed to load " + filename << std::endl;
					delete buffer;
					return;
				}

				getResourceManager()->cache(filename, buffer);
			}

			_buffer = buffer;

			_status = AudioStatus::Stopped;
		}

		void Sound::play()
		{
			if (_instance == nullptr)
			{
				_instance = std::make_unique<sf::Sound>();
				_instance->setBuffer(*_buffer);

				_instance->setVolume(_volume * 100);
				_instance->setPitch(_pitch);
				_instance->setLoop(_looping);

				_instance->setAttenuation(_attenuation);
				_instance->setMinDistance(_minimumDistance);
				_instance->setRelativeToListener(false);
			}

			if (isPlaying()) return;

			_instance->play();
			_status = AudioStatus::Playing;
		}

		void Sound::stop()
		{
			if (_instance == nullptr) return;
			_instance->stop();
			_status = AudioStatus::Stopped;
		}

		void Sound::pause()
		{
			if (_instance == nullptr) return;
			_instance->pause();
			_status = AudioStatus::Paused;
		}

		bool Sound::isPlaying() const
		{
			return getStatus() == AudioStatus::Playing;
		}

		bool Sound::isStopped() const
		{
			return getStatus() == AudioStatus::Stopped;
		}

		bool Sound::isPaused() const
		{
			return getStatus() == AudioStatus::Paused;
		}

		AudioStatus Sound::getStatus() const
		{
			return _status;
		}

		void Sound::atTime(const float time) const
		{
			if (_instance == nullptr) return;

			_instance->setPlayingOffset(sf::seconds(time));
		}

		float Sound::getTime() const
		{
			if (_instance == nullptr) return -1;

			return _instance->getPlayingOffset().asSeconds();
		}

		float Sound::getDuration() const
		{
			if (_buffer == nullptr) return -1;
			return _buffer->getDuration().asSeconds();
		}

		void Sound::setLooping(const bool loop)
		{
			_looping = loop;

			if (_instance == nullptr) return;
			_instance->setLoop(loop);
		}

		bool Sound::getLooping() const
		{
			return _looping;
		}

		void Sound::setVolume(float volume)
		{
			volume = glm::clamp(volume, 0.0f, 1.0f);

			_volume = volume;

			if (_instance == nullptr) return;
			_instance->setVolume(volume * 100);
		}

		float Sound::getVolume() const
		{
			return _volume;
		}

		void Sound::setPitch(const float pitch)
		{
			_pitch = pitch;

			if (_instance == nullptr) return;
			_instance->setPitch(pitch);
		}

		float Sound::getPitch() const
		{
			return _pitch;
		}

		void Sound::setAttenuationValue(const float attenuation)
		{
			_attenuation = attenuation;

			if (_instance == nullptr) return;
			_instance->setAttenuation(attenuation);
		}

		float Sound::getAttenuationValue() const
		{
			return _attenuation;
		}

		void Sound::setMinimumDistance(const float minDistance)
		{
			_minimumDistance = minDistance;

			if (_instance == nullptr) return;
			_instance->setMinDistance(minDistance);
		}

		float Sound::getMinimumDistance() const
		{
			return _minimumDistance;
		}

		void Sound::destroy()
		{
			delete this;
		}

		void Sound::update()
		{
			if (_instance == nullptr) return;
			const glm::vec3 position = getGameObject()->getTransform()->getPosition();
			_instance->setPosition(position.x, position.y, position.z);
		}

		bool Sound::isUniquePerGameObject()
		{
			return false;
		}

		ResourceManager* Sound::getResourceManager()
		{
			if (!_resourceManager) _resourceManager = ServiceLocator::instance()->getService<ResourceManager>();
			return _resourceManager;
		}

		void Sound::playOneShot(const std::string& path)
		{
			//sf::SoundBuffer* buffer = getResourceManager()->getSoundBuffer(path);
			sf::SoundBuffer* buffer = nullptr;

			if (!getResourceManager()->isCached(path))
			{
				buffer = new sf::SoundBuffer();
				if (!buffer->loadFromFile(path))
				{
					std::cout << "Failed to load " + path << std::endl;
					delete buffer;
					return;
				}

				getResourceManager()->cache(path, buffer);
			}
			else
			{
				buffer = getResourceManager()->retrieveSoundBuffer(path);
			}

			sf::Sound* sound = new sf::Sound();
			sound->setBuffer(*buffer);
			sound->play();
		}
		
		void Sound::playOneShotOld(const std::string& path)
		{
			sf::SoundBuffer* buffer = new sf::SoundBuffer();
			if (!buffer->loadFromFile(path))
			{
				std::cout << "Failed to load and play " + path << std::endl;
				delete buffer;
				return;
			}

			sf::Sound* sound = new sf::Sound();
			sound->setBuffer(*buffer);
			sound->play();

			const std::function<void()> deleter = [sound, buffer]
			{
				delete sound;
				delete buffer;
			};

			Engine::Utility::Time::timeout(buffer->getDuration().asSeconds() + 1, deleter);
		}
	}
}
