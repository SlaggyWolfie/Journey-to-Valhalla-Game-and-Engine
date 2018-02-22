#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include "Component.hpp"
#include "GL/glew.h"
#include <string>

namespace Engine
{
	namespace Core { class Transform; }
	namespace Rendering
	{
		enum RenderQueue { Opaque, Transparent };
		class Material_;
		class Mesh_;
		class Shader;
		class LightManager;

		class Renderer_ : public Core::Component
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
			static GLint _uniformModelMatrix;
			static GLint _uniformViewMatrix;
			static GLint _uniformProjectionMatrix;

			static GLint _uniformModelViewProjectionMatrix;
			static GLint _uniformModelViewMatrix;
			static GLint _uniformViewProjectionMatrix;

			static GLint _uniformNormalMatrix;

			static GLint _attributeVertexPositions;
			static GLint _attributeVertexNormals;
			static GLint _attributeVertexUVs;

			static GLint _bufferVertexPositions;
			static GLint _bufferVertexNormals;
			static GLint _bufferVertexUVs;
			static GLint _bufferIndex;

			static GLint _uniformMaterialDiffuseColor;
			static GLint _uniformMaterialSpecularColor;
			static GLint _uniformMaterialEmissionColor;

			static GLint _uniformMaterialDiffuseMap;
			static GLint _uniformMaterialSpecularMap;
			static GLint _uniformMaterialEmissionMap;

			static GLint _uniformMaterialUseDiffuseMap;
			static GLint _uniformMaterialUseSpecularMap;
			static GLint _uniformMaterialUseEmissionMap;
			static GLint _uniformMaterialUseEmission;

			static GLint _uniformMaterialShininess;

			static GLint _uniformCameraPosition;

			static GLint _uniformAmbientLightColor;
			static GLint _uniformAmbientLightStrength;
			static GLint _uniformAmbientLightTerm;
			static GLint _uniformAttenuationConstants;

			static GLint _uniformDirectionalLightsAmount;
			static GLint _uniformPointLightsAmount;
			static GLint _uniformSpotLightsAmount;

			//Everything else
			void initialize();
			void initializeShader(const std::string& path);
			void setup() const;
			void pushMatrices() const;
			void pushLights() const;
			void pushMaterial() const;
			void pushCameraPosition() const;
			void pushMesh() const;

			void render() const;

			LightManager* _lightManager;

			Core::Transform* _transform;
			Material_ * _material;
			Mesh_ * _mesh;
			Shader* _shader;

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