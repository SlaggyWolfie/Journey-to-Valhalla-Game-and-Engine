#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "Component.hpp"
#include "glm/glm.hpp"
#include "NeedsGLSetup.hpp"

namespace Engine
{
	namespace Rendering
	{
		enum RenderQueue { Opaque, Transparent };
		class Material_;
		class Mesh_;

		class Renderer_ : public Core::Component, public NeedsGLSetup
		{
			friend class RenderManager;
		public:
			Renderer_();
			Renderer_(Material_* material, Mesh_* mesh, RenderQueue renderQueue = Opaque, bool castShadows = true);
			virtual ~Renderer_() = default;
			Renderer_(const Renderer_& other) = default;
			Renderer_& operator=(const Renderer_& other) = default;

			void destroy() override;

			bool shouldCastShadows(bool castShadows = false);
			bool castsShadows() const;
		protected:
			//GL cache location
			static glm::uint _uModelMatrix;
			static glm::uint _uViewMatrix;
			static glm::uint _uProjectionMatrix;

			static glm::uint _uMVP_Matrix;
			static glm::uint _uMV_Matrix;
			static glm::uint _uVP_Matrix;

			static glm::uint _uNormalMatrix;

			static glm::uint _uDiffuseColor;
			static glm::uint _uSpecularColor;
			static glm::uint _uEmissionColor;

			static glm::uint _uDiffuseMap;
			static glm::uint _uSpecularMap;
			static glm::uint _uEmissionMap;

			static glm::uint _aPositions;
			static glm::uint _aNormals;
			static glm::uint _aUVs;

			static glm::uint _aIndices;

			//Everything else
			void setupGL() override;
			void bind();
			void pushMatrix();
			void unbind();
			void draw();

			void render();

			Material_ * _material;
			Mesh_ * _mesh;

			RenderQueue _renderQueue;
			bool _castsShadows;

			void findMesh();
			void findMaterial();
			void prewake() override;
			bool isUniquePerGameObject() override;
		};
	}
}

#endif //RENDERER_HPP