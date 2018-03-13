#pragma once
#ifndef SERVICE_HPP
#define SERVICE_HPP
#include "ServiceLocator.hpp"

namespace Engine
{
	class Service
	{
	public:
		Service() = default;
		virtual ~Service() = default;
		virtual void initialize() = 0;
		virtual void reset() = 0;
	};

	//inline Service::~Service()
	//{
	//	ServiceLocator::instance()->removeServiceDirect(this);
	//}
}

#endif //SERVICE_HPP