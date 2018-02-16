#pragma once
#ifndef GENERAL_HELPERS_HPP
#define GENERAL_HELPERS_HPP

#include <vector>
#include <ctime>

namespace Engine
{
	class Random
	{
	public:
		static float range(float min, float max, int decimalAccuracy = 6);
		static int range(int min, int max);
		static float value();
	private:
		static void setup();
		static bool _setup;
	};

	class List
	{
	public:
		template<typename T>
		static void removeFrom(std::vector<T> vector, T item);
		template<typename T>
		static bool contains(std::vector<T> vector, T item);
	};
}

#endif //GENERAL_HELPERS_HPP