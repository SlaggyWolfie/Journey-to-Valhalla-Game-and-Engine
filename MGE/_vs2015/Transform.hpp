#pragma once
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

//#include "GameObject.hpp"
#include "glm.hpp"
#include "Component.hpp"
#include <vector>

namespace Engine
{
	namespace Core
	{
		class Transform : public Component
		{
		public:
			friend class GameObject_;

			Transform();
			~Transform();

			Transform(const Transform& other);
			Transform& operator=(const Transform& other);


			/**
			 * \brief Sets the transform's world position.
			 * \param position The target world position.
			 */
			void setPosition(const glm::vec3& position);
			/**
			 * \brief Gets the transform's world position.
			 * \return The transform's world position.
			 */
			glm::vec3 getPosition();

			/**
			* \brief Sets the transform's world rotation.
			* \return The target world rotation.
			*/
			void setRotation(const glm::quat& rotation);
			/**
			* \brief Gets the transform's world rotation.
			* \return The transform's world rotation.
			*/
			glm::quat getRotation();

			/**
			* \brief Sets the transform's world scale.
			* \return The target world scale.
			*/
			void setScale(const glm::vec3& scale);
			/**
			* \brief Gets the transform's position.
			* \return The transform's position.
			*/
			glm::vec3 getScale();

			/**
			* \brief Gets the transform's position.
			* \return The transform's position.
			*/
			void setWorldMatrix4X4(const glm::mat4& matrix);
			/**
			* \brief Gets the transform's position.
			* \return The transform's position.
			*/
			glm::mat4 getMatrix4X4();

			//Local

			void setLocalPosition(const glm::vec3& position);
			glm::vec3 getLocalPosition() const;

			void setLocalRotation(const glm::quat& rotation);
			glm::quat getLocalRotation() const;

			void setLocalScale(const glm::vec3& scale);
			glm::vec3 getLocalScale() const;

			void setLocalMatrix4X4(const glm::mat4& matrix);
			glm::mat4 getLocalMatrix4X4();

			glm::mat3 getNormalMatrix();

			glm::vec3 forward();
			glm::vec3 up();
			glm::vec3 right();

			//Operations

			glm::vec3 transformPoint(const glm::vec3& point);
			glm::vec3 inverseTransformPoint(const glm::vec3& point);

			void lookAt(Transform* lookAtTarget, const glm::vec3& up = glm::vec3(0, 1, 0));
			void translate(const glm::vec3& translation);
			void rotate(const glm::vec3& axis, float angleRotation);
			void scale(const glm::vec3& scaler);

			//Parent/Children
			void setParent(Transform* parent, bool keepWorldTransform = false);
			Transform* getParent() const;

			//Transform* addChild(Transform* child, bool keepWorldTransform = false);
			Transform* removeChild(const int& index);
			Transform* removeChild(Transform* child);
			int getChildCount() const;
			Transform* getChild(int index) const;
			Transform** getChildren() const;
		private:
			Transform * _parent;
			std::vector<Transform*> _children;

			glm::vec3 _localPosition;
			glm::quat _localRotation;
			glm::vec3 _localScale;
			glm::mat4 _localMatrix; //unused

			glm::vec3 _worldPosition; //unused
			glm::quat _worldRotation; //unused
			glm::vec3 _worldScale; //unused
			glm::mat4 _worldMatrix; //unused
			glm::mat3 _normalMatrix;

			bool _isDirty; //unused

			void _determineCaching();
			void _cacheLocal();
			void _cacheWorld();

			glm::mat4 _calculateLocalMatrix();

			//good
			//Recurse through parents to construct World Matrix
			//Use said matrix to construct TRS

			glm::vec3 _calculateWorldPosition();
			glm::quat _calculateWorldRotation();
			glm::vec3 _calculateWorldScale();
			glm::mat4 _calculateWorldMatrix();

			//bad
			//Recurse through parents to construct components (TRS)
			//Use said components to construct World Matrix

			glm::vec3 _calculateWorldPosition2();
			glm::quat _calculateWorldRotation2();
			glm::vec3 _calculateWorldScale2();
			glm::mat4 _calculateWorldMatrix2();

			//Convenience
			static glm::vec3 _getTranslation(const glm::mat4& matrix);
			static glm::quat _getOrientation(const glm::mat4& matrix);
			static glm::vec3 _getScale(const glm::mat4& matrix);
			static glm::vec3 _getSkew(const glm::mat4& matrix);
			static glm::vec4 _getPerspective(const glm::mat4& matrix);

		protected:
			void start() override {};
			void awake() override {};
			void update() override {};
			void fixedUpdate() override {};
			void lateUpdate() override {};
			void onValidate() override {};
			bool isUniquePerGameObject() override;
			void destroy() override;
		};
	}
}
#endif //TRANSFORM_HPP