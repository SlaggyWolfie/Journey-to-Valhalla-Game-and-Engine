#include <iostream>
#include "GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"

GameObject::GameObject(const std::string& pName, const glm::vec3& pPosition )
:	_name( pName ), _transform( glm::translate( pPosition ) ), _scalar(glm::vec3(1)), _parent(nullptr), _children(),
    _mesh( nullptr ),_behaviour( nullptr ), _material(nullptr), _world(nullptr)

{
}

GameObject::~GameObject()
{
    //detach all children
    std::cout << "GC running on:" << _name << std::endl;

    while (_children.size() > 0) {
        GameObject* child = _children[0];
        remove (child);
        delete child;
    }

    //do not forget to delete behaviour, material, mesh, collider manually if required!
}

void GameObject::setName (const std::string& pName)
{
    _name = pName;
}

std::string GameObject::getName() const
{
	return _name;
}

void GameObject::setTransform (const glm::mat4& pTransform)
{
    _transform = pTransform;
}

const glm::mat4& GameObject::getTransform() const
{
    return _transform;
}

void GameObject::setLocalPosition (glm::vec3 pPosition)
{
    _transform[3] = glm::vec4 (pPosition,1);
}

glm::vec3 GameObject::getLocalPosition() const
{
	return glm::vec3(_transform[3]);
}

void GameObject::setMaterial(AbstractMaterial* pMaterial)
{
    _material = pMaterial;
	_material->_owner = this;
}

AbstractMaterial * GameObject::getMaterial() const
{
    return _material;
}

void GameObject::setMesh(Mesh* pMesh)
{
	_mesh = pMesh;
}

Mesh * GameObject::getMesh() const
{
    return _mesh;
}

void GameObject::setBehaviour(AbstractBehaviour* pBehaviour)
{
	_behaviour = pBehaviour;
	_behaviour->setOwner(this);
}

AbstractBehaviour* GameObject::getBehaviour() const
{
    return _behaviour;
}

void GameObject::setParent (GameObject* pParent) {
    //remove from previous parent
    if (_parent != nullptr) {
        _parent->_innerRemove(this);
    }

    //set new parent
    if (pParent != nullptr) {
        pParent->_innerAdd(this);
    }

    //if we have been detached from our parent, make sure
    //the world reference for us and all our children is set to null
    //if we have been attached to a parent, make sure
    //the world reference for us and all our children is set to our parent world reference
    //(this could still be null if the parent or parent's parent is not attached to the world)
    if (_parent == nullptr) {
        _setWorldRecursively(nullptr);
    } else {
        //might still not be available if our parent is not part of the world
        _setWorldRecursively(_parent->_world);
    }
}

void GameObject::_innerRemove (GameObject* pChild) {
    for (auto i = _children.begin(); i != _children.end(); ++i) {
        if (*i == pChild) {
            _children.erase(i);
            pChild->_parent = nullptr;
            return;
        }
    }
}

void GameObject::_innerAdd(GameObject* pChild)
{
	_children.push_back(pChild);
   pChild->_parent = this;
}

void GameObject::add (GameObject* pChild) {
    pChild->setParent(this);
}

void GameObject::remove (GameObject* pChild) {
    pChild->setParent(nullptr);
}

GameObject* GameObject::getParent() const {
    return _parent;
}

////////////

//costly operation, use with care
glm::vec3 GameObject::getWorldPosition() const
{
	return glm::vec3(getWorldTransform()[3]);
}

//costly operation, use with care
glm::mat4 GameObject::getWorldTransform() const
{
	if (_parent == nullptr) return _transform;
	else return _parent->getWorldTransform() * _transform;
}

////////////

void GameObject::translate(glm::vec3 pTranslation)
{
	setTransform(glm::translate(_transform, pTranslation));
}

void GameObject::scale(glm::vec3 pScale)
{
	_scalar *= pScale;
	setTransform(glm::scale(_transform, pScale));
}

glm::vec3 GameObject::getScale()
{
	return _scalar;
}

void GameObject::rotate(float pAngle, glm::vec3 pAxis)
{
	setTransform(glm::rotate(_transform, pAngle, pAxis));
}

void GameObject::update(float pStep)
{
    //make sure behaviour is updated after worldtransform is set
	if (_behaviour) {
		_behaviour->update(pStep);
	}

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->update(pStep);
    }
}

void GameObject::_setWorldRecursively (World* pWorld) {
    _world = pWorld;

    for (int i = _children.size()-1; i >= 0; --i ) {
        _children[i]->_setWorldRecursively (pWorld);
    }
}

int GameObject::getChildCount() const {
    return _children.size();
}

GameObject* GameObject::getChildAt(int pIndex) const {
    return _children[pIndex];
}

