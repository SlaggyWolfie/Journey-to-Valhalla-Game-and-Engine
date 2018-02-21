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
			static glm::uint _uniformModelMatrix;
			static glm::uint _uniformViewMatrix;
			static glm::uint _uniformProjectionMatrix;

			static glm::uint _uniformMVP_Matrix;
			static glm::uint _uniformMV_Matrix;
			static glm::uint _uniformVP_Matrix;

			static glm::uint _uniformNormalMatrix;

			static glm::uint _uniformDiffuseColor;
			static glm::uint _uniformSpecularColor;
			static glm::uint _uniformEmissionColor;

			static glm::uint _uniformDiffuseMap;
			static glm::uint _uniformSpecularMap;
			static glm::uint _uniformEmissionMap;

			static glm::uint _attributePositions;
			static glm::uint _attributeNormals;
			static glm::uint _attributeUVs;

			static glm::uint _attributeIndices;

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