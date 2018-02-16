#pragma once
#ifndef SERVICE_LOCATOR_HPP
#define SERVICE_LOCATOR_HPP
#include <memory>
#include <vector>
#include <iostream>
#include <../_vs2015/GeneralHelpers.hpp>
#include <../_vs2015/Service.hpp>

namespace Engine
{
	//class Service;
	namespace Rendering { class RenderManager; }
	namespace Core { class GameLoop; }
	class ServiceLocator
	{
	public:
		static ServiceLocator* instance();
		static void destroyInstance();

		template<typename T>
		void addService(T* service);
		template<typename T>
		void removeService();
		template<typename T>
		T* getService();
		template<typename T>
		std::shared_ptr<T> findService();
	private:
		ServiceLocator();
		~ServiceLocator();
		static ServiceLocator* _instance;
		std::vector<std::shared_ptr<Engine::Service>> _services;
	};

	template <typename T>
	void ServiceLocator::addService(T* service)
	{
		auto check = findService<T>();

		if (check != nullptr && check.get() != nullptr)
			_services.push_back(std::shared_ptr<Service>(static_cast<Service*>(service)));
	}

	template <typename T>
	void ServiceLocator::removeService()
	{
		auto check = findService<T>();

		if (check != nullptr) List::removeFrom(_services, check);
	}

	template <typename T>
	T* ServiceLocator::getService()
	{
		auto check = findService<T>();
		if (check != nullptr && check.get() != nullptr) return check.get();
		return nullptr;
	}

	template <typename T>
	std::shared_ptr<T> ServiceLocator::findService()
	{
		if (!std::is_base_of<Service, T>())
		{
			std::cout << "Wrong Type T: getService()" << std::endl;
			return nullptr;
		}

		for (auto & service : _services)
		{
			T* cast_service = dynamic_cast<T*>(service.get());
			if (cast_service != nullptr) return std::shared_ptr<T>(cast_service);
		}

		return nullptr;
	}
}

#endif //SERVICE_LOCATOR_HPP