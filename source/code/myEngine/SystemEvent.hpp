#ifndef SYSTEM_EVENT_HPP
#define SYSTEM_EVENT_HPP

namespace MyEngine
{
	class SystemEvent
	{
	public:
		SystemEvent();
		~SystemEvent();
		SystemEvent(const SystemEvent&);
		SystemEvent& operator=(const SystemEvent&);
	private:
	};
}
#endif // SYSTEM_EVENT_HPP
