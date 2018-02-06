#ifndef SYSTEM_EVENT_DISPATCHER_HPP
#define SYSTEM_EVENT_DISPATCHER_HPP

#include <vector>
#include <map>

namespace MyEngine
{
	class SystemEvent;
	class SystemEventListener;

	class SystemEventDispatcher
	{
		//PUBLIC FUNCTIONS

	//public:
	//	static void initialize();

	//	static void dispatchEvent(const SystemEvent& event);
	//	static void registerForEvent(const SystemEvent& eventType, SystemEventListener* listener);
	//	static void deregisterFromEvent(const SystemEvent& eventType, SystemEventListener* listener);
	//	static void deregisterFromAll(SystemEventListener* listener);
	//private:
	//	static std::map<SystemEvent, std::vector<SystemEventListener*>> _eventTypeListeners;

	//	SystemEventDispatcher();
	//	~SystemEventDispatcher();
	//	SystemEventDispatcher(const SystemEventDispatcher&);
	//	SystemEventDispatcher& operator=(const SystemEventDispatcher&);
	};
}
#endif // SYSTEM_EVENT_DISPATCHER_HPP
