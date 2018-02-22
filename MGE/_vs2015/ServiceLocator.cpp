#include "ServiceLocator.hpp"
#include "Service.hpp"
#include "GeneralHelpers.hpp"
#include <algorithm>
#include <iostream>
#include "Core/Game.hpp"

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
		_instance->removeService<Game>();
		delete _instance;
	}

	ServiceLocator::ServiceLocator()
		= default;

	ServiceLocator::~ServiceLocator()
	{
		//Search for a component that can be cast to T
		//for (auto& service : _services)
		//{
		//	if (dynamic_cast<Engine::Game*>(service) != nullptr) continue;
		//	delete service;
		//}
		//removeService<Game>();
		_services.clear();

		/*for(size_t i = 1; i < _services.size(); i++)
		{
			delete _services[i];
		}*/
		//_services.clear();
	}

	void ServiceLocator::removeServiceDirect(Service* service)
	{
		const auto check = std::find(_services.begin(), _services.end(), service);

		if (check != _services.end()) List::removeFrom(_services, service);
	}
}
