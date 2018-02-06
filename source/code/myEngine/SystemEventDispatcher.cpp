#include <iostream>
#include <string>
#include <algorithm>
//#include <vector>
//#include <map>
#include "myEngine\SystemEvent.hpp"
#include "myEngine\SystemEventListener.hpp"
#include "myEngine\SystemEventDispatcher.hpp"

namespace MyEngine
{
	//void SystemEventDispatcher::initialize()
	//{
	//	_eventTypeListeners = std::map<SystemEvent, std::vector<SystemEventListener*>>();
	//}
	//void SystemEventDispatcher::dispatchEvent(const SystemEvent & event)
	//{
	//}
	//void SystemEventDispatcher::registerForEvent(const SystemEvent& eventType, SystemEventListener* listener)
	//{
	//	std::vector<SystemEventListener*> listeners = _eventTypeListeners[eventType];
	//	//checks if it's not already present
	//	if (std::find(listeners.begin(), listeners.end(), listener) == listeners.end())
	//		//adds it
	//		_eventTypeListeners[eventType].push_back(listener);
	//}
	//void SystemEventDispatcher::deregisterFromEvent(const SystemEvent & eventType, SystemEventListener * listener)
	//{
	//	std::vector<SystemEventListener*> listeners = _eventTypeListeners[eventType];
	//	//checks if it's present
	//	if (std::find(listeners.begin(), listeners.end(), listener) != listeners.end())
	//		//removes it
	//		listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
	//}
	//void SystemEventDispatcher::deregisterFromAll(SystemEventListener* listener)
	//{
	//}
	//SystemEventDispatcher::~SystemEventDispatcher()
	//{
	//	//delete _eventTypeListeners;
	//}
}