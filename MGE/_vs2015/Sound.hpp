#pragma once
#ifndef SOUND_HPP
#define SOUND_HPP
#include "Component.hpp"
#include <SFML/Audio.hpp>
#include <memory>

namespace Engine
{
	namespace Audio
	{
		enum class AudioStatus { NoSound, Playing, Paused, Stopped };

		class Sound : public Engine::Core::Component
		{
		public:
			Sound();
			explicit Sound(const std::string& filename);
			virtual ~Sound();

			void load(const std::string& filename);

			void play();
			void stop();
			void pause();
			bool isPlaying() const;
			bool isStopped() const;
			bool isPaused() const;
			AudioStatus getStatus() const;

			void atTime(float time) const;
			float getTime() const;
			float getDuration() const;
			void setLooping(bool loop = true);
			bool getLooping() const;
			void setVolume(float volume);
			float getVolume() const;
			void setPitch(float pitch);
			float getPitch() const;
			void setAttenuationValue(float attenuation);
			float getAttenuationValue() const;
			void setMinimumDistance(float minDistance);
			float getMinimumDistance() const;

			void destroy() override;
			static void playOneShot(const std::string& path);
		protected:
			void update() override;
			bool isUniquePerGameObject() override;
		private:
			std::unique_ptr<sf::SoundBuffer> _buffer = nullptr;
			std::unique_ptr<sf::Sound> _instance = nullptr;

			std::string _filename = "";

			AudioStatus _status = AudioStatus::NoSound;

			float _volume = 0;
			float _pitch = 0;
			bool _looping = false;
			float _attenuation = 0;
			float _minimumDistance = 0;

		};
	}
}

#endif //AUDIO_HPP