#ifndef DESERIALIZER2_HPP
#define DESERIALIZER2_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "json.hpp"
#include "Component.hpp"

using namespace Engine::Core;

struct TransformStruct;
struct ComponentStruct;

struct GameStruct
{
public:
	bool hasMesh = false;

	std::string name = "Default Name";
	std::string tag = "Default Tag";
	bool active = true;

	TransformStruct* transform = nullptr;
	std::vector<ComponentStruct*> components;

	GameStruct();
	~GameStruct();
	GameStruct(const GameStruct& other) = default;
	GameStruct& operator=(const GameStruct& other) = default;

	void deserialize(nlohmann::json& jsonObject);

	template <typename T>
	T* getComponent();
};

template <typename T>
T* GameStruct::getComponent()
{
	if (!std::is_base_of<ComponentStruct, T>())
	{
		std::cout << "Type T is not of type ComponentStruct" << std::endl;
		return nullptr;
	}

	for (auto & component : components)
	{
		T* cast_component = dynamic_cast<T*>(component);

		//std::cout << "I am copying: " + std::string(cast_component != component.get() ? "true." : "false.") << std::endl;
		if (cast_component != nullptr)
			return cast_component;
	}
	//std::cout << "boi_______" << std::endl;

	return nullptr;
}

struct ComponentStruct
{
public:
	virtual ~ComponentStruct() = default;
	virtual void deserialize(nlohmann::json& jsonObject) = 0;

	virtual Component* makeObject();
};

struct TransformStruct : public ComponentStruct
{
	void deserialize(nlohmann::json& jsonObject) override;
	Component* makeObject() override;
	glm::vec3 position = glm::vec3();
	glm::quat rotation = glm::quat();
	glm::vec3 scale = glm::vec3();
	int parentID = 0;
	int selfID = 0;
};

struct LightStruct : public ComponentStruct
{
	void deserialize(nlohmann::json& jsonObject) override;
	Component* makeObject() override;
	float intensity = 0;
	int type = 0;
	glm::vec4 color = glm::vec4();
};

struct BoxColliderStruct : public ComponentStruct
{
	void deserialize(nlohmann::json& jsonObject) override;
	Component* makeObject() override;
	glm::vec3 size = glm::vec3();
	glm::vec3 center = glm::vec3();
};

struct MeshStruct : public ComponentStruct
{
	void deserialize(nlohmann::json& jsonObject) override;
	Component* makeObject() override;
	std::string path = "";
};

class Deserializer2
{
public:
	static std::map<std::type_info, std::type_info> structToObject;
	std::vector<GameStruct*> gameStructs;

	Deserializer2() = default;
	~Deserializer2();

	std::vector<GameStruct*> deserializeIntoStructs(const std::string& fileName);

	static glm::vec3 jsonToVec3(nlohmann::json& jsonObject);
	static glm::vec4 jsonToVec4(nlohmann::json& jsonObject);
	static glm::quat jsonToQuat(nlohmann::json& jsonObject);

	static void jsonToVec3(glm::vec3& vector, nlohmann::json& jsonObject);
	static void jsonToVec4(glm::vec4& vector, nlohmann::json& jsonObject);
	static void jsonToQuat(glm::quat& quaternion, nlohmann::json& jsonObject);
};

#endif // DESERIALIZER2_HPP
