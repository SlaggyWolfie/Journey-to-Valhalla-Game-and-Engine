#pragma once
#ifndef GENERAL_HELPERS_HPP
#define GENERAL_HELPERS_HPP

#include <vector>
#include <ctime>
#include <glm/detail/type_vec3.hpp>

namespace Engine
{
	class Random
	{
	public:
		static float range(float min, float max, int decimalAccuracy = 6);
		static int range(int min, int max);
		static float value();
		static void setSeed(unsigned seed);
		static void defaultSeed();
	private:
		static void setup();
		static bool _setup;
	};

	class List
	{
	public:
		template<typename T>
		static void removeFrom(std::vector<T>& vector, T item);
		template<typename T>
		static bool contains(std::vector<T>& vector, T item);
	};

	template <typename T>
	void List::removeFrom(std::vector<T>& vector, T item)
	{
		vector.erase(
			std::remove(
				vector.begin(), vector.end(), item
			), vector.end());
	}

	template <typename T>
	bool List::contains(std::vector<T>& vector, T item)
	{
		return std::find(vector.begin(), vector.end(), item) != vector.end();
	}

	class Vector3
	{
	public:
		static glm::vec3 forward();
		static glm::vec3 up();
		static glm::vec3 right();
		static glm::vec3 backward();
		static glm::vec3 down();
		static glm::vec3 left();
	};

	class File
	{
	public:
		static std::string findPath(const std::string& nameOfFile, const std::string& rootDirectory = "Assets");
		static std::string clipPath(std::string path);
	};
}

#endif //GENERAL_HELPERS_HPP