#pragma once
#ifndef SERVICE_LOCATOR_HPP
#define SERVICE_LOCATOR_HPP
#include <memory>
#include <vector>
#include <iostream>
#include <../_vs2015/GeneralHelpers.hpp>
//#include <../_vs2015/Service.hpp>
#include <string>

namespace Engine
{
	//class Service;
	namespace Rendering { class RenderManager; }
	namespace Core { class GameLoop; }
	class Service;

	class ServiceLocator
	{
	public:
		static ServiceLocator* instance();
		static void destroyInstance();
		void removeServiceDirect(Service* service);

		template<typename T>
		void addService(T* service);
		template<typename T>
		void removeService();
		template<typename T>
		T* getService();
		template<typename T>
		T* findService();
	private:
		ServiceLocator();
		~ServiceLocator();
		static ServiceLocator* _instance;
		std::vector<Service*> _services;
	};

	template <typename T>
	void ServiceLocator::addService(T* service)
	{
		auto check = findService<T>();

		if (check == nullptr)
			_services.push_back(static_cast<Service*>(service));
	}

	template <typename T>
	void ServiceLocator::removeService()
	{
		T* check = findService<T>();

		if (check != nullptr) List::removeFrom(_services, &*static_cast<Service*>(check));
	}

	template <typename T>
	T* ServiceLocator::getService()
	{
		auto check = findService<T>();
		if (check != nullptr) return check;
		return nullptr;
	}

	template <typename T>
	T* ServiceLocator::findService()
	{
		if (!std::is_base_of<Service, T>())
		{
			std::cout << "Wrong Type T: getService()" << std::endl;
			return nullptr;
		}

		for (auto & service : _services)
		{
			T* cast_service = dynamic_cast<T*>(service);
			if (cast_service != nullptr) return cast_service;
			//std::cout << "Saltier" + std::to_string(typeid(T)) << std::endl;
		}
		 //std::cout << "Salty" << std::endl;


		return nullptr;
	}
}

#endif //SERVICE_LOCATOR_HPP