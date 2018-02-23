#include "Time.hpp"
#include <utility>
#include <glm/gtc/epsilon.hpp>

namespace Engine
{
	namespace Utility
	{

		std::unique_ptr<sf::Clock> Time::_clock = nullptr;
		 float Time::_now_seconds = 0;
		 float Time::_timeStep_seconds = 0;
		 float Time::_variableTimeStep_seconds = 0;
		 std::set<Time::TimeoutEvent> Time::_timeouts;

		//TimeoutEvent

		Time::TimeoutEvent::TimeoutEvent(const float interval, std::function<void()>   event) :
			_interval(interval), _timeout(Time::now() + _interval), _event(std::move(event))
		{
		}

		bool Time::TimeoutEvent::operator<(const TimeoutEvent& other) const
		{
			if (abs(_timeout - other._timeout) < glm::epsilon<float>())
				return false;
			return _timeout < other._timeout;
		}

		bool Time::TimeoutEvent::operator>(const TimeoutEvent& other) const
		{
			return other < *this;
		}

		bool Time::TimeoutEvent::raise() const
		{
			if (_timeout <= Time::now())
			{
				_event();
				return true;
			}
			return false;
		}

		//Time

		void Time::start()
		{
			_clock = std::make_unique < sf::Clock >();
			_clock->restart();

			_now_seconds = 0;
			_timeStep_seconds = 0;
			_variableTimeStep_seconds = 0;

			_timeouts = std::set<TimeoutEvent>();
		}

		void Time::start(const float timeStep)
		{
			start();
			setTimeStep(timeStep);
		}

		void Time::update()
		{
			const float lastTime = _now_seconds;
			_now_seconds = _clock->getElapsedTime().asSeconds();
			_variableTimeStep_seconds = _now_seconds - lastTime;

			// check for timeouts and deliver for all needed
			while (!_timeouts.empty() && _timeouts.begin()->raise())
			{
				_timeouts.erase(_timeouts.begin());
			}
		}

		void Time::stop()
		{
			_clock.release();

			_now_seconds = 0;
			_timeStep_seconds = 0;
			_variableTimeStep_seconds = 0;

			_timeouts.clear();
		}

		float Time::now()
		{
			return _now_seconds;
		}

		float Time::deltaTime()
		{
			return _variableTimeStep_seconds;
		}

		float Time::fixedDeltaTime()
		{
			return getTimeStep();
		}

		void Time::setTimeStep(const float timeStep)
		{
			_timeStep_seconds = timeStep;
		}

		float Time::getTimeStep()
		{
			return _timeStep_seconds;
		}

		void Time::timeout(const float interval, const std::function<void()>& timeoutEvent)
		{
			_timeouts.emplace_hint(_timeouts.begin(), TimeoutEvent(interval, timeoutEvent));
		}
	}
}