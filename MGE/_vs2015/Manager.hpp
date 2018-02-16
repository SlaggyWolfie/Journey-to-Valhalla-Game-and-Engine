#pragma once
#ifndef MANAGER_HPP
#define MANAGER_HPP
#include "Service.hpp"

namespace Engine
{
	class Manager: public Service
	{
	public:
		Manager() = default;
		virtual ~Manager() = default;
	};
}

#endif //MANAGER_HPP