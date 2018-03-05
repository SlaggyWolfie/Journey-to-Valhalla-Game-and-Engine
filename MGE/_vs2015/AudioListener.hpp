#pragma once
#ifndef AUDIO_LISTENER_HPP
#define AUDIO_LISTENER_HPP
#include "Component.hpp"

namespace Engine
{
	namespace Audio
	{
		class AudioListener : public Engine::Core::Component
		{
		public:
			AudioListener();
			virtual ~AudioListener() = default;
			void setVolume(float volume);
			float getVolume() const;

			void destroy() override;
		protected:
			void update() override;
			bool isUniquePerGameObject() override;
		private:
			float _volume = 0;
		};
	}
}

#endif //AUDIO_LISTENER_HPP