#pragma once
#ifndef SERVICE_LOCATOR_HPP
#define SERVICE_LOCATOR_HPP
#include <memory>
#include <vector>

namespace Engine
{
	class Service;
	namespace Rendering { class RenderManager; }
	namespace Core { class GameLoop; }
	class ServiceLocator
	{
	public:
		static ServiceLocator* instance();
		static void destroyInstance();

		void addService(Service* service);
		template<typename T>
		void removeService();
		template<typename T>
		T* getService();
	private:
		ServiceLocator();
		~ServiceLocator();
		static ServiceLocator* _instance;
		std::vector<std::shared_ptr<Service>> _services;
	};
}

#endif //SERVICE_LOCATOR_HPP