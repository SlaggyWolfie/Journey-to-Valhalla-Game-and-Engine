#include "Transform.hpp"
#include "GameObject_.hpp"
#include <algorithm>
//#include "glm/gtx/decomposition.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "Core/Game.hpp"

namespace Engine
{
	namespace Core
	{
		//Public
		//Getters and Setters
		void Transform::setPosition(const glm::vec3& position)
		{
			if (_parent == nullptr) setLocalPosition(position);
			else setLocalPosition(_parent->inverseTransformPoint(position));
		}

		glm::vec3 Transform::getPosition()
		{
			if (_parent == nullptr) return getLocalPosition();

			return _getTranslation(getMatrix4X4());
		}

		void Transform::setRotation(const glm::quat& rotation)
		{
			if (_parent == nullptr) setLocalRotation(rotation);
			else
			{
				glm::mat4 const difference = glm::mat4_cast(rotation) / glm::mat4_cast(_parent->getRotation());
				glm::quat const extractedRotation = _getOrientation(difference);

				setLocalRotation(extractedRotation);
			}
		}

		glm::quat Transform::getRotation()
		{
			if (_parent == nullptr) return getLocalRotation();
			return _getOrientation(getMatrix4X4());
		}

		void Transform::setScale(const glm::vec3& scale)
		{
			if (_parent == nullptr) setLocalScale(scale);
			else setLocalScale(scale / _getScale(_parent->getMatrix4X4()));
		}

		glm::vec3 Transform::getScale()
		{
			if (_parent == nullptr) return getLocalScale();
			return _getScale(getMatrix4X4());
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

		glm::mat4 Transform::getMatrix4X4()
		{
			if (_parent == nullptr) return getLocalMatrix4X4();

			_determineCaching(true);
			return _worldMatrix;
		}

		void Transform::setLocalPosition(const glm::vec3& position)
		{
			if (getGameObject()->isStatic() && _game->isRunning()) return;

			_localPosition = position;
			_isLocalMatrixDirty = true;
			_isWorldMatrixDirty = true;
		}

		glm::vec3 Transform::getLocalPosition() const
		{
			return _localPosition;
		}

		void Transform::setLocalRotation(const glm::quat& rotation)
		{
			if (getGameObject()->isStatic() && _game->isRunning()) return;

			_localRotation = rotation;
			_isLocalMatrixDirty = true;
			_isWorldMatrixDirty = true;
		}

		glm::quat Transform::getLocalRotation() const
		{
			return _localRotation;
		}

		void Transform::setLocalScale(const glm::vec3& scale)
		{
			if (getGameObject()->isStatic() && _game->isRunning()) return;

			_localScale = scale;
			_isLocalMatrixDirty = true;
			_isWorldMatrixDirty = true;
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

		glm::mat4 Transform::getLocalMatrix4X4()
		{
			_determineCaching(false);
			return _localMatrix;
			//return _calculateLocalMatrix();
		}

		glm::mat3 Transform::getNormalMatrix()
		{
			_determineCaching(true);
			return _normalMatrix;
		}

		glm::vec3 Transform::transformPoint(const glm::vec3& point)
		{
			//glm::vec4 const transformedPoint = getMatrix4X4() * glm::vec4(point, 1);
			glm::vec4 const transformedPoint = glm::vec4(point, 1) * getMatrix4X4();
			return glm::vec3(transformedPoint);
		}

		glm::vec3 Transform::inverseTransformPoint(const glm::vec3& point)
		{
			//glm::vec4 const transformedPoint = glm::inverse(getMatrix4X4()) * glm::vec4(point, 1);
			glm::vec4 const transformedPoint = glm::vec4(point, 1) * glm::inverse(getMatrix4X4());
			return glm::vec3(transformedPoint);
		}

		void Transform::lookAt(Transform* lookAtTarget, const glm::vec3& up)
		{
			if (getGameObject()->isStatic()) return;

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
			if (getGameObject()->isStatic() && _game->isRunning()) return;
			//_localPosition += translation;
			//setLocalPosition()
			setLocalPosition(_localPosition + translation);
		}

		void Transform::rotate(const glm::vec3& axis, const float angleRotation)
		{
			if (getGameObject()->isStatic() && _game->isRunning()) return;
			//Angle is in Radians.

			//_localRotation *= glm::mat4_cast(rotation);
			//_localRotation *= glm::mat4();
			//_localRotation = glm::rotate(_localRotation, angleRotation, axis);
			setLocalRotation(glm::rotate(_localRotation, angleRotation, axis));
		}

		void Transform::scale(const glm::vec3& scaler)
		{
			if (getGameObject()->isStatic() && _game->isRunning()) return;
			//_localScale *= scaler;
			setLocalScale(_localScale * scaler);
		}

		glm::vec3 Transform::forward()
		{
			return glm::normalize(inverseTransformPoint(glm::vec3(0, 0, -1)));
			//return glm::normalize(transformPoint(glm::vec3(0, 0, -1)));
		}

		glm::vec3 Transform::up()
		{
			return glm::normalize(inverseTransformPoint(glm::vec3(0, 1, 0)));
			//return glm::normalize(transformPoint(glm::vec3(0, 1, 0)));
		}

		glm::vec3 Transform::right()
		{
			return glm::normalize(inverseTransformPoint(glm::vec3(1, 0, 0)));
			//return glm::normalize(transformPoint(glm::vec3(1, 0, 0)));
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

		void Transform::makeStatic()
		{
			for (auto& child : _children)
				child->getGameObject()->setStatic(true);

		}

		void Transform::unmakeStatic(bool children)
		{
			for (auto& child : _children)
				child->getGameObject()->setStatic(false);
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

		void Transform::_determineCaching(const bool forWorldMatrix)
		{
			//for world
			if (forWorldMatrix)
			{
				if (_isWorldMatrixDirty)
				{
					_worldMatrix = _calculateWorldMatrix();
					_normalMatrix = glm::mat3(glm::transpose(glm::inverse(_worldMatrix)));
					_isWorldMatrixDirty = true;
				}
			}
			//for local
			else
			{
				if (_isLocalMatrixDirty)
				{
					_localMatrix = _calculateLocalMatrix();
					_isLocalMatrixDirty = true;
				}
			}
		}

		glm::mat4 Transform::_calculateLocalMatrix() const
		{
			const glm::vec3 p = glm::eulerAngles(_localRotation);
			const glm::mat4 local =
				glm::translate(_localPosition) *
				glm::eulerAngleXYZ(p.x, p.y, p.z) *
				glm::scale(_localScale) *
				glm::mat4(1);


			return local;
		}

		glm::vec3 Transform::_calculateWorldPosition()
		{
			return _getTranslation(_calculateWorldMatrix());
		}

		glm::quat Transform::_calculateWorldRotation()
		{
			return _getOrientation(_calculateWorldMatrix());
		}

		glm::vec3 Transform::_calculateWorldScale()
		{
			return _getScale(_calculateWorldMatrix());
		}

		glm::mat4 Transform::_calculateWorldMatrix()
		{
			glm::mat4 matrix = getLocalMatrix4X4();
			for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
				matrix *= parent->getLocalMatrix4X4();
			return matrix;
		}

		glm::vec3 Transform::_calculateWorldPosition2()
		{
			glm::vec3 position = _localPosition;
			for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
				position += parent->_localPosition;
			return position;
		}

		glm::quat Transform::_calculateWorldRotation2()
		{
			glm::quat rotation = _localRotation;
			for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
				rotation = glm::cross(rotation, parent->_localRotation);
			return rotation;
		}

		glm::vec3 Transform::_calculateWorldScale2()
		{
			glm::vec3 scale = _localScale;
			for (Transform* parent = _parent; parent != nullptr; parent = parent->_parent)
				scale += parent->_localScale;
			return scale;
		}

		glm::mat4 Transform::_calculateWorldMatrix2()
		{
			glm::vec3 p = glm::eulerAngles(getRotation());
			return
				glm::translate(getPosition()) *
				glm::eulerAngleXYZ(p.x, p.y, p.z) *
				glm::scale(getScale()) *
				glm::mat4(1);
		}

		glm::vec3 Transform::_getTranslation(const glm::mat4& matrix)
		{
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 scale;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(matrix, scale, rotation, translation, skew, perspective);

			return translation;
		}

		glm::quat Transform::_getOrientation(const glm::mat4& matrix)
		{
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 scale;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(matrix, scale, rotation, translation, skew, perspective);

			return rotation;
		}

		glm::vec3 Transform::_getScale(const glm::mat4& matrix)
		{
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 scale;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(matrix, scale, rotation, translation, skew, perspective);

			return scale;
		}

		glm::vec3 Transform::_getSkew(const glm::mat4& matrix)
		{
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 scale;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(matrix, scale, rotation, translation, skew, perspective);

			return skew;
		}

		glm::vec4 Transform::_getPerspective(const glm::mat4& matrix)
		{
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 scale;
			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(matrix, scale, rotation, translation, skew, perspective);

			return perspective;
		}

		bool Transform::isUniquePerGameObject()
		{
			return true;
		}

		void Transform::destroy()
		{
			for (auto & i : _children)
				i->getGameObject()->destroy();
			_children.clear();
			delete this;
		}

		void Transform::prewake()
		{
			_game = ServiceLocator::instance()->getService<Game>();
		}

		Transform::Transform() :
			_parent(nullptr),
			_localPosition(glm::vec3(0, 0, 0)),
			_localRotation(glm::quat(glm::vec3(0, 0, 0))),
			_localScale(glm::vec3(1, 1, 1)),
			_isLocalMatrixDirty(false), _isWorldMatrixDirty(false),
			_game(nullptr)
		{
			_children = std::vector<Transform*>();
			//_children.push_back(this);
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
			_isLocalMatrixDirty(other._isLocalMatrixDirty),
			_isWorldMatrixDirty(other._isWorldMatrixDirty),
			_game(other._game)
		{
		}

		Transform& Transform::operator=(const Transform& other)
		{
			_localPosition = other._localPosition;
			_localRotation = other._localRotation;
			_localScale = other._localScale;
			_parent = other._parent;
			_isLocalMatrixDirty = other._isLocalMatrixDirty;
			_isWorldMatrixDirty = other._isWorldMatrixDirty;
			_game = other._game;

			return *this;
		}
	}
}