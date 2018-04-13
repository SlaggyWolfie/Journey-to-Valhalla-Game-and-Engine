#include "Music.hpp"
#include <glm.hpp>
#include <SFML/System/Time.hpp>

namespace Engine
{
	namespace Audio
	{
		Music::Music() :_music(nullptr), _filename(""), _status(AudioStatusMusic::NoMusic), _volume(1), _pitch(1), _looping(false)
		{
		}

		Music::Music(const std::string filename) : Music()
		{
			load(filename);
		}

		Music::~Music()
		{
			stop();

			_music = nullptr;
		}

		void Music::load(const std::string filename)
		{
			_filename = filename;

			sf::Music* music = new sf::Music;
			if (!music->openFromFile(filename))
			{
				std::cout << "Failed to load " + filename << std::endl;
				delete music;
				return;
			}

			_music = std::unique_ptr<sf::Music>(music);
			_status = AudioStatusMusic::Stopped;
		}

		void Music::play()
		{
			if (_music == nullptr || isPlaying()) return;

			_music->setVolume(_volume * 100);
			_music->setPitch(_pitch);
			_music->setLoop(_looping);

			_music->play();
			_status = AudioStatusMusic::Playing;
		}

		void Music::stop()
		{
			if (_music == nullptr) return;
			_music->stop();
			_status = AudioStatusMusic::Stopped;
		}

		void Music::pause()
		{
			if (_music == nullptr) return;
			_music->pause();
			_status = AudioStatusMusic::Paused;
		}

		bool Music::isPlaying() const
		{
			return getStatus() == AudioStatusMusic::Playing;
		}

		bool Music::isStopped() const
		{
			return getStatus() == AudioStatusMusic::Stopped;
		}

		bool Music::isPaused() const
		{
			return getStatus() == AudioStatusMusic::Paused;
		}

		AudioStatusMusic Music::getStatus() const
		{
			return _status;
		}

		void Music::atTime(const float time) const
		{
			if (_music == nullptr) return;

			_music->setPlayingOffset(sf::seconds(time));
		}

		float Music::getTime() const
		{
			if (_music == nullptr) return -1;

			return _music->getPlayingOffset().asSeconds();
		}

		float Music::getDuration() const
		{
			if (_music == nullptr) return -1;
			return _music->getDuration().asSeconds();
		}

		void Music::setLooping(const bool loop)
		{
			_looping = loop;

			if (_music == nullptr) return;
			_music->setLoop(loop);
		}

		bool Music::getLooping() const
		{
			return _looping;
		}

		void Music::setVolume(float volume)
		{
			volume = glm::clamp(volume, 0.0f, 1.0f);

			_volume = volume;

			if (_music == nullptr) return;
			_music->setVolume(volume * 100);
		}

		float Music::getVolume() const
		{
			return _volume;
		}

		void Music::setPitch(const float pitch)
		{
			_pitch = pitch;

			if (_music == nullptr) return;
			_music->setPitch(pitch);
		}

		float Music::getPitch() const
		{
			return _pitch;
		}

		void Music::destroy()
		{
			delete this;
		}

		bool Music::isUniquePerGameObject()
		{
			return false;
		}
	}
}