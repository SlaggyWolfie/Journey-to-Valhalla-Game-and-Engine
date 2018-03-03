#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP
#include <vector>
#include <memory>
#include "Core/Deserealizer.hpp"

struct GameObject_s;

namespace Engine
{
	namespace Core
	{
		class GameObject_;
	}
	class Scene
	{
	public:
		Scene();
		explicit Scene(std::string name);
		~Scene();
		void initialize(bool hard = false, bool fromFile = true);
		Core::GameObject_* findGameObject(const std::string& name) const;
		Core::GameObject_** getGameObjectsArray() const;
		std::vector<Core::GameObject_*> getGameObjects()const;
		std::string getName() const;
		void addGameObject(Core::GameObject_* gameObject);
		void deserializeStructs(std::vector<GameObject_s> structs);
	private:
		void hardCode();
		std::string _name = nullptr;
		std::vector<std::unique_ptr<Core::GameObject_>> _gameObjects;
	};
}

#endif