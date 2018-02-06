#ifndef SYSTEM_EVENT_LISTENER_HPP
#define SYSTEM_EVENT_LISTENER_HPP

namespace MyEngine
{
	class SystemEventListener
	{
	public:
		SystemEventListener();
		~SystemEventListener();
		SystemEventListener(const SystemEventListener& copy);
		SystemEventListener& operator=(const SystemEventListener& copy);
		//bool operator ==(const SystemEventListener& LHS);
	private:
	};
}
#endif // SYSTEM_EVENT_LISTENER_HPP
