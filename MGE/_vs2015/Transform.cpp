#include "Transform.hpp"
#include "GameObject_.hpp"
#include <algorithm>
//#include "glm/gtx/decomposition.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace Engine
{
	//Public
	//Getters and Setters
	void Transform::setPosition(const glm::vec3& position)
	{
		if (_parent == nullptr) _localPosition = position;
		else _localPosition = _parent->inverseTransformPoint(position);
	}

	glm::vec3 Transform::getPosition() const
	{
		if (_parent == nullptr) return _localPosition;
		else return _calculateWorldPosition();
	}

	void Transform::setRotation(const glm::quat& rotation)
	{
		if (_parent == nullptr) _localRotation = rotation;
		else
		{
			glm::mat4 const difference = glm::mat4_cast(rotation) / glm::mat4_cast(_parent->getRotation());
			glm::quat const extractedRotation = _getOrientation(difference);

			_localRotation = extractedRotation;
		}
	}

	glm::quat Transform::getRotation() const
	{
		if (_parent == nullptr) return _localScale;
		return _calculateWorldRotation();
	}

	void Transform::setScale(const glm::vec3& scale)
	{
		if (_parent == nullptr) _localScale = scale;
		else _localScale = scale / _getScale(_parent->getMatrix4X4());
	}

	glm::vec3 Transform::getScale() const
	{
		if (_parent == nullptr) return _localScale;
		else return _calculateWorldScale();
	}

	void Transform::setWorldMatrix4X4(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		setPosition(translation);
		setRotation(rotation);
		setScale(scale);
	}

	glm::mat4 Transform::getMatrix4X4() const
	{
		if (_parent == nullptr) return getLocalMatrix4X4();
		else return _calculateWorldMatrix();
	}

	void Transform::setLocalPosition(const glm::vec3& position)
	{
		_localPosition = position;
	}

	glm::vec3 Transform::getLocalPosition() const
	{
		return _localPosition;
	}

	void Transform::setLocalRotation(const glm::quat& rotation)
	{
		_localRotation = rotation;
	}

	glm::quat Transform::getLocalRotation() const
	{
		return _localRotation;
	}

	void Transform::setLocalScale(const glm::vec3& scale)
	{
		_localScale = scale;
	}

	glm::vec3 Transform::getLocalScale() const
	{
		return _localScale;
	}

	void Transform::setLocalMatrix4X4(const glm::mat4& matrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(matrix, scale, rotation, translation, skew, perspective);

		setLocalPosition(translation);
		setLocalRotation(rotation);
		setLocalScale(scale);
	}

	glm::mat4 Transform::getLocalMatrix4X4() const
	{
		return _calculateLocalMatrix();
	}

	glm::vec3 Transform::transformPoint(const glm::vec3& point) const
	{
		glm::vec4 const transformedPointer = glm::vec4(point, 1) * getMatrix4X4();
		return glm::vec3(transformedPointer);
	}

	glm::vec3 Transform::inverseTransformPoint(const glm::vec3& point) const
	{
		glm::vec4 const transformedPoint = glm::vec4(point, 1) * glm::inverse(getMatrix4X4());
		return glm::vec3(transformedPoint);
	}

	void Transform::lookAt(Transform* lookAtTarget, const glm::vec3& up)
	{
		const glm::vec3 eye = _localPosition;
		const glm::vec3 target = lookAtTarget->getPosition();

		const glm::vec3 forward = -glm::normalize(target - eye);
		const glm::vec3 side = glm::normalize(glm::cross(up, forward));
		const glm::vec3 upward = glm::normalize(glm::cross(forward, side));

		const glm::mat4 lookAtMatrix = glm::mat4(
			glm::vec4(side, 0),
			glm::vec4(upward, 0),
			glm::vec4(forward, 0),
			glm::vec4(_localPosition, 1));

		setRotation(_getOrientation(lookAtMatrix));
	}

	void Transform::translate(const glm::vec3& translation)
	{
		_localPosition += translation;
	}

	void Transform::rotate(const glm::vec3& axis, float angleRotation)
	{
		//Angle is in Radians.

		//_localRotation *= glm::mat4_cast(rotation);
		//_localRotation *= glm::mat4();
		_localRotation = glm::rotate(_localRotation, angleRotation, axis);
	}

	void Transform::scale(const glm::vec3& scaler)
	{
		_localScale *= scaler;
	}

	glm::vec3 Transform::forward() const
	{
		return transformPoint(glm::vec3(0, 0, -1));
	}

	glm::vec3 Transform::up() const
	{
		return transformPoint(glm::vec3(0, 1, 0));
	}

	glm::vec3 Transform::right() const
	{
		return transformPoint(glm::vec3(1, 0, 0));
	}

	//Parenting

	void Transform::setParent(Transform* parent, const bool keepWorldTransform)
	{
		//Don't reattach to ourselves or our parent
		if (parent == this || (parent != nullptr && parent->_parent == this))
			return;

		//Detach ourselves from our old parent
		if (_parent != nullptr)
			_parent->removeChild(this);

		//Attach ourselves to the new parent
		if (parent != nullptr)
			parent->_children.push_back(this);

		//Store old transform if needed
		glm::vec3 oldWorldPosition;
		glm::quat oldWorldRotation;
		glm::vec3 oldWorldScale;

		if (keepWorldTransform)
		{
			oldWorldPosition = getPosition();
			oldWorldRotation = getRotation();
			oldWorldScale = getScale();
		}

		//Apply Parent
		_parent = parent;

		if (keepWorldTransform)
		{
			//Reset transform
			setPosition(oldWorldPosition);
			setRotation(oldWorldRotation);
			setScale(oldWorldScale);
		}
	}

	Transform* Transform::getParent() const
	{
		return _parent;
	}

	/*Transform* Transform::addChild(Transform* child, bool keepWorldTransforms)
	{
	}*/

	Transform* Transform::removeChild(const int& index)
	{
		Transform* child = _children[index];

		_children.erase(
			std::remove(
				_children.begin(), _children.end(), child),
			_children.end());

		return child;
	}

	Transform* Transform::removeChild(Transform* child)
	{
		_children.erase(
			std::remove(
				_children.begin(), _children.end(), child),
			_children.end());

		return child;
	}

	int Transform::getChildCount() const
	{
		return _children.size();
	}

	Transform* Transform::getChild(const int index) const
	{
		return _children[index];
	}

	Transform** Transform::getChildren() const
	{
		const int size = static_cast<unsigned int>(_children.size());
		Transform** children = new Transform*[size];
		std::copy(_children.begin(), _children.end(), children);
		return children;
	}

	//Private

	void Transform::_determineCaching()
	{
		//if (getGameObject()->isStatic() && !_isDirty)
	}

	void Transform::_cacheLocal()
	{
		_localMatrix = _calculateLocalMatrix();
	}

	void Transform::_cacheWorld()
	{
		_worldPosition = _calculateWorldPosition();
		_worldRotation = _calculateWorldRotation();
		_worldScale = _calculateWorldScale();
		_worldMatrix = _calculateWorldMatrix();
	}

	glm::mat4 Transform::_calculateLocalMatrix() const
	{
		const glm::vec3 p = glm::eulerAngles(_localRotation);
		return
			glm::translate(_localPosition) *
			glm::eulerAngleXYZ(p.x, p.y, p.z) *
			glm::scale(_localScale) *
			glm::mat4(1);
	}

	glm::vec3 Transform::_calculateWorldPosition() const
	{
		return _getTranslation(_calculateWorldMatrix());
	}

	glm::quat Transform::_calculateWorldRotation() const
	{
		return _getOrientation(_calculateWorldMatrix());
	}

	glm::vec3 Transform::_calculateWorldScale() const
	{
		return _getScale(_calculateWorldMatrix());
	}

	glm::mat4 Transform::_calculateWorldMatrix() const
	{
		glm::mat4 matrix = getLocalMatrix4X4();
		for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
			matrix *= parent->getLocalMatrix4X4();
		return matrix;
	}

	glm::vec3 Transform::_calculateWorldPosition2() const
	{
		glm::vec3 position = _localPosition;
		for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
			position += parent->_localPosition;
		return position;
	}

	glm::quat Transform::_calculateWorldRotation2() const
	{
		glm::quat rotation = _localRotation;
		for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
			rotation = glm::cross(rotation, parent->_localRotation);
		return rotation;
	}

	glm::vec3 Transform::_calculateWorldScale2() const
	{
		glm::vec3 scale = _localScale;
		for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
			scale += parent->_localScale;
		return scale;
	}

	glm::mat4 Transform::_calculateWorldMatrix2() const
	{
		const glm::vec3 p = glm::eulerAngles(getRotation());
		return
			glm::translate(getPosition()) *
			glm::eulerAngleXYZ(p.x, p.y, p.z) *
			glm::scale(getScale()) *
			glm::mat4(1);
	}

	glm::vec3 Transform::_getTranslation(const glm::mat4& modelMatrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

		return translation;
	}

	glm::quat Transform::_getOrientation(const glm::mat4& modelMatrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

		return rotation;
	}

	glm::vec3 Transform::_getScale(const glm::mat4& modelMatrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

		return scale;
	}

	glm::vec3 Transform::_getSkew(const glm::mat4& modelMatrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

		return skew;
	}

	glm::vec4 Transform::_getPerspective(const glm::mat4& modelMatrix)
	{
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(modelMatrix, scale, rotation, translation, skew, perspective);

		return perspective;
	}

	bool Transform::isUniquePerGameObject()
	{
		return true;
	}

	Transform::Transform() :
		_parent(nullptr),
		_localPosition(glm::vec3(0, 0, 0)),
		_localRotation(glm::quat(glm::vec3(0, 0, 0))),
		_localScale(glm::vec3(1, 1, 1)),
		_isDirty(true)
	{
		_children = std::vector<Transform*>();
		_children.push_back(this);
	}

	Transform::~Transform()
	{
		//Get rid of all relationships
		for (auto & i : _children)
			i->setParent(nullptr);
		//for (unsigned int i = 0; i < _children.size(); i++)
		//	_children[i]->setParent(nullptr);
		_children.clear();

		if (_parent != nullptr)
			_parent->removeChild(this);
		_parent = nullptr;
	}

	Transform::Transform(const Transform& other) :
		_parent(other._parent),
		_localPosition(other._localPosition),
		_localRotation(other._localRotation),
		_localScale(other._localScale),
		_isDirty(other._isDirty)
	{
	}

	Transform& Transform::operator=(const Transform& other)
	{
		_localPosition = other._localPosition;
		_localRotation = other._localRotation;
		_localScale = other._localScale;
		_parent = other._parent;
		_isDirty = other._isDirty;

		return *this;
	}
}
