#pragma once
#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "../_vs2015/ServiceLocator.hpp"

namespace Engine
{
	class Service
	{
	public:
		Service() = default;
		virtual ~Service() = default;
	};
}

#endif //SERVICE_HPP