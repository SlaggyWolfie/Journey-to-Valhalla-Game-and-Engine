#pragma once
#ifndef MUSIC_HPP
#define MUSIC_HPP
#include "Component.hpp"
#include <SFML/Audio.hpp>
#include <memory>

namespace Engine
{
	namespace Audio
	{
		enum AudioStatusMusic { NoMusic, Playing, Paused, Stopped };

		class Music : public Engine::Core::Component
		{
		public:
			Music();
			explicit Music(const std::string filename);
			virtual ~Music();

			void load(const std::string filename);

			void play();
			void stop();
			void pause();
			bool isPlaying() const;
			bool isStopped() const;
			bool isPaused() const;
			AudioStatusMusic getStatus() const;

			void atTime(float time) const;
			float getTime() const;
			float getDuration() const;
			void setLooping(bool loop = true);
			bool getLooping() const;
			void setVolume(float volume);
			float getVolume() const;
			void setPitch(float pitch);
			float getPitch() const;

			void destroy() override;
		protected:
			bool isUniquePerGameObject() override;
		private:
			std::unique_ptr<sf::Music> _music = nullptr;

			std::string _filename = "";

			AudioStatusMusic _status = AudioStatusMusic::NoMusic;

			float _volume = 0;
			float _pitch = 0;
			bool _looping = false;
		};
	}
}

#endif //AUDIO_HPP