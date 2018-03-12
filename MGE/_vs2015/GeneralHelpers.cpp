#include "GeneralHelpers.hpp"
#include <iostream>
#include <valarray>
#include <vector>
#include <filesystem>

namespace Engine
{
	bool Random::_setup = false;

	float Random::range(const float min, const float max, int decimalAccuracy)
	{
		if (_setup) setup();

		while (std::pow(10, decimalAccuracy) >= RAND_MAX) decimalAccuracy /= 2;
		const float decimal = static_cast<float>(std::pow(10, decimalAccuracy));

		if (fabs(max - min) <= 1 / decimal)
		{
			std::cout << "Random Range Max and Min shouldn't be the same (or they're too close as float values)." << std::endl;
			return 0;
		}

		return fmod(rand() / decimal, max - min) + min;
	}

	int Random::range(const int min, const int max)
	{
		if (_setup) setup();

		if (max == min)
		{
			std::cout << "Random Range Max and Min shouldn't be the same." << std::endl;
			return 0;
		}

		return rand() % (max - min) + min;
	}

	float Random::value()
	{
		return range(0.0f, 1.0f);
	}

	void Random::setup()
	{
		//srand(0);
		srand(static_cast<unsigned>(time(nullptr)));
		_setup = true;
	}

	glm::vec3 Vector3::forward()
	{
		return glm::vec3(0, 0, -1);
	}

	glm::vec3 Vector3::up()
	{
		return glm::vec3(0, 1, 0);
	}

	glm::vec3 Vector3::right()
	{
		return glm::vec3(1, 0, 0);
	}

	glm::vec3 Vector3::backward()
	{
		return -forward();
	}

	glm::vec3 Vector3::down()
	{
		return -up();
	}

	glm::vec3 Vector3::left()
	{
		return -right();
	}

	std::string File::findPath(const std::string& nameOfFile, const std::string& rootDirectory)
	{
		namespace fs = std::experimental::filesystem;
		for (auto & p : fs::recursive_directory_iterator(rootDirectory.c_str()))
		{
			const std::string actualPath = p.path().generic_string();
			//std::cout << "Recursive Iterator " << actualPath << std::endl;
			if (clipPath(actualPath) == nameOfFile)
			{
				std::cout << "Found [" + nameOfFile + "] at [" + actualPath + "]!" << std::endl;
				return actualPath;
			}
		}

		std::cout << "Did not find [" + nameOfFile + "] at root[" + rootDirectory + "]!" << std::endl;
		return std::string();
	}

	std::string File::clipPath(std::string path)
	{
		while (path.find('\\') != std::string::npos)
			path = path.substr(path.find('\\') + 1, std::string::npos);

		while (path.find('/') != std::string::npos)
			path = path.substr(path.find('/') + 1, std::string::npos);

		return path;
	}
}
