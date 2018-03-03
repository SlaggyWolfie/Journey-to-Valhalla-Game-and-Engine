#pragma once
#include "Component.hpp"
#include "GameObject_.hpp"
#include "Transform.hpp"
#include "glm.hpp"
#include "RenderManager.hpp"
#include"ServiceLocator.hpp"
using namespace Engine::Core;
class collider:public Component
{
public:
	enum colliderType { box, sphere, point };
	collider(colliderType pType);
	~collider();
	void SetBoxSize(int width,int height);
	void SetSphereRadius(int r);
	void update() override;
	glm::vec3 GetPos();
	int GetWidth();
	int GetHeight();
	int GetRadius();
	void SetTrans(Transform* t);
	 //void start() override;
	 //void awake() override;
	 //void prewake() override;
	 //void update() override;
	 //void fixedUpdate() override;
	 //void lateUpdate() override;
	 //void onValidate() override;
private:
	glm::vec3 _pos;
	glm::vec3 _rot;
	Transform* _t;
	colliderType _type;
	int _width = 0;
	int _height = 0;
	int _r = 0;
};

