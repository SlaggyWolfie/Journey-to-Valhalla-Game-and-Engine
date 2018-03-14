#pragma once
#ifndef TIME_HPP
#define TIME_HPP
#include <memory>
#include <SFML/System/Clock.hpp>
#include <set>
#include <functional>
#include "Core/GameLoop.hpp"

namespace Engine
{
	namespace Utility
	{
		class Time
		{
			friend class Engine::Core::GameLoop;

			class TimeoutEvent
			{
			public:
				TimeoutEvent(float interval, std::function<void()> event);
				~TimeoutEvent() = default;
				TimeoutEvent(const TimeoutEvent& other) = default;
				TimeoutEvent& operator= (const TimeoutEvent& other) = default;
				bool operator <(const TimeoutEvent& other) const;
				bool operator >(const TimeoutEvent& other) const;
				bool raise() const;
			private:
				float _interval;
				float _timeout;
				std::function<void()> _event;
			};

		public:
			static bool isPaused();
			static void pause();
			static void unpause();
			static void setPause(bool paused = true);
			static float now();
			static float deltaTime();
			static float fixedDeltaTime();
			static void setTimeStep(float timeStep);
			static float getTimeStep();
			static void timeout(float interval, const std::function<void()>& timeoutEvent);
		private:
			static void start();
			static void start(float timeStep);
			static void update();
			static void stop();
			static std::unique_ptr<sf::Clock> _clock;
			static bool _paused;
			static float _now_seconds;
			static float _timeStep_seconds;
			static float _variableTimeStep_seconds;
			static std::set<TimeoutEvent> _timeouts;
		};
	};
}

#endif //TIME_HPP