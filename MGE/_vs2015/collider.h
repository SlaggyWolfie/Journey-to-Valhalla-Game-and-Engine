#pragma once
#include "Component.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include "glm.hpp"
using namespace Engine::Core;

class ColliderManager;

class collider:public Component
{
public:
	enum colliderType { box, sphere, point };
	collider();
	~collider();
	void SetType(int i);
	void SetBoxSize(int width,int height);
	void SetSphereRadius();
	void update() override;
	glm::vec3 GetPos();
	int GetWidth();
	int GetHeight();
	int GetRadius();
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
private:
	ColliderManager * _colliderManager;
	glm::vec3 _pos;
	glm::vec3 _rot;
	Transform* _t;
	colliderType _type=box;
	int _width = 0;
	int _height = 0;
	int _r=50;
};

