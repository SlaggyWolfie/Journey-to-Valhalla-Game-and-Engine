#pragma once
#ifndef GENERAL_HELPERS_HPP

#include <vector>
#include <ctime>

namespace Engine
{
	class Random
	{
	public:
		static float range(float min = 0, float max = 1, int decimalAccuracy = 6);
		static int range(int min = 0, int max = 2);
	private:
		static void setup();
		static bool _setup;
	};

	class List
	{
	public:
		template<typename T>
		static void removeFrom(std::vector<T> vector, T item);
	};
}

#endif //GENERAL_HELPERS_HPP