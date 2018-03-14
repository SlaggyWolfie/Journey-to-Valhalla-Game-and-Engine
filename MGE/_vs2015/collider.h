#pragma once
#include "Component.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include "glm.hpp"
using namespace Engine::Core;

class ColliderManager;

class collider :public Component
{
public:
	enum colliderType { box, sphere, point };
	collider();
	virtual ~collider();
	collider(const collider& other);
	collider& operator= (const collider& other);
	void SetType(int i);
	void SetBoxSize(float width, float height, float length);
	void SetSphereRadius(float r);
	void update() override;
	void fixedUpdate() override;
	void PushBackObj(std::string n, std::string n1);
	bool Enabled();
	void SetEnable(bool n);

	glm::vec3 GetPos();
	float GetWidth();
	float GetHeight();
	float GetLength();
	float GetRadius();
	void  jumpToObj();
	glm::vec3 lastPos = glm::vec3(0);
	int index = 1;
	void setI(int i);
	void SetTrans(Transform* t);
	//void start() override;
	//void awake() override;
	void prewake() override;
	//void update() override;
	//void fixedUpdate() override;
	//void lateUpdate() override;
	//void onValidate() override;
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
	glm::vec3 point4;
	glm::vec3 point5;
	glm::vec3 point6;
	glm::vec3 normal1;
	glm::vec3 normal2;
	glm::vec3 normal3;
	glm::vec3 normal4;
	glm::vec3 normal5;
	glm::vec3 normal6;
private:
	ColliderManager * _colliderManager;
	glm::vec3 _pos;
	glm::vec3 _rot;
	Transform* _t;
	colliderType _type = box;
	float _width = 1;
	float _length = 1;
	float  _height = 1;
	float _radius = 1;
	bool _enabled;
};

