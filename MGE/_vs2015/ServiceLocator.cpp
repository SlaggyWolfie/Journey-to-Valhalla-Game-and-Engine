#include "ServiceLocator.hpp"
#include "Service.hpp"
#include "GeneralHelpers.hpp"
#include <algorithm>
#include <iostream>

namespace Engine
{
	ServiceLocator* ServiceLocator::_instance = nullptr;

	ServiceLocator* ServiceLocator::instance()
	{
		if (_instance == nullptr) _instance = new ServiceLocator();
		return _instance;
	}

	void ServiceLocator::destroyInstance()
	{
		delete _instance;
	}

	ServiceLocator::ServiceLocator()
		= default;

	ServiceLocator::~ServiceLocator()
	{
		//Search for a component that can be cast to T
		for (auto& service : _services)
			service.reset();
		_services.clear();
	}
}
