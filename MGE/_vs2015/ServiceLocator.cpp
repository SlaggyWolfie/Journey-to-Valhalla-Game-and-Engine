#include "ServiceLocator.hpp"
#include "Service.hpp"
#include "GeneralHelpers.hpp"
#include <algorithm>

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

	void ServiceLocator::addService(Service* service)
	{
		if (std::find(_services.begin(), _services.end(),
			std::shared_ptr<Service>(service)) == _services.end())
			return;

		_services.push_back(std::shared_ptr<Service>(service));
	}


	template <typename T>
	void ServiceLocator::removeService()
	{
		const std::vector<std::shared_ptr<Service>>::iterator iter;
		iter = std::find_if(_services.begin(), _services.end(), std::is_base_of<Service, T>());
		if (iter == _services.end()) return;

		List::removeFrom(_services, *iter);
	}

	template <typename T>
	T* ServiceLocator::getService()
	{
		//std::cout << "Hi";
		const std::vector<std::shared_ptr<Service>>::const_iterator iter
			= std::find_if(_services.begin(), _services.end(), std::is_base_of<Service, T>());
		if (iter == _services.end()) return nullptr;

		return static_cast<T*>(iter->get());
	}
}
