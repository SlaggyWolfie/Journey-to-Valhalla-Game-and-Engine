#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "Component.hpp"

namespace Engine
{
	namespace Rendering
	{
		class Material;
		class Mesh;

		class Renderer_ : public Core::Component
		{
			friend class RenderManager;
		public:
			Renderer_();
			virtual ~Renderer_();
			Renderer_(Material* material, Mesh* mesh);
			void destroy() override;
		protected:
			Material * _material;
			Mesh *_mesh;

			void render(float deltaTime);

			void start() override;
			void awake() override;
			void update() override;
			void fixedUpdate() override;
			void lateUpdate() override;
			void onValidate() override;
			bool isUniquePerGameObject() override;
		};
	}
}

#endif //RENDERER_HPP