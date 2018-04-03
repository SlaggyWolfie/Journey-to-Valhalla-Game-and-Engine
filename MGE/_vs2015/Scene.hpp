#pragma once
#ifndef SCENE_HPP
#define SCENE_HPP
#include <vector>
#include <memory>
#include "Core/Deserealizer.hpp"
#include "Deserializer2.hpp"

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
		explicit Scene(std::string name, std::string path);
		~Scene();
		void initialize(bool hard = false, bool fromFile = true);
		Core::GameObject_* findGameObject(const std::string& name, bool exact =true) const;
		std::vector<Core::GameObject_*> findGameObjects(const std::string& name, bool exact = true) const;
		Core::GameObject_** getGameObjectsArray() const;
		std::vector<Core::GameObject_*> getGameObjects()const;
		std::string getName() const;
		void addGameObject(Core::GameObject_* gameObject);
		void deserializeStructs(std::vector<GameObject_s> structs, bool clipPaths = true);
		void deserializeStructs2(std::vector<GameStruct*> structs, bool clipPaths = true);
		void loadMenu();
	private:
		void hardCode();
		void neededHardCode();
		std::string _name = "";
		std::string _path = "";
		std::vector<std::unique_ptr<Core::GameObject_>> _gameObjects;
	};
}

#endif