//#pragma once
#ifndef RENDERER__HPP
#define RENDERER__HPP
#include "Component.hpp"
#include "GL/glew.h"
#include <string>

namespace Engine
{
	namespace Core { class Transform; }
	namespace Rendering
	{
		enum class RenderQueue { Opaque, Transparent };
		class Material_;
		class Mesh_;
		class Shader;
		class LightManager;
		class RenderManager;

		class Renderer_ : public Core::Component
		{
			friend class RenderManager;
		public:
			Renderer_();
			Renderer_(Material_* material, Mesh_* mesh, RenderQueue renderQueue = RenderQueue::Opaque, bool castShadows = true);
			virtual ~Renderer_() = default;
			Renderer_(const Renderer_& other) = default;
			Renderer_& operator=(const Renderer_& other) = default;

			//void destroy() override;

			bool shouldCastShadows(bool castShadows = false);
			bool castsShadows() const;

			void debug(bool debug);
			bool isDebugMode() const;
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

			//static GLint _bufferVertexPositions;
			//static GLint _bufferVertexNormals;
			//static GLint _bufferVertexUVs;
			//static GLint _bufferIndex;

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
			void drawDebug() const;

			void warning() const;

			void render();

			LightManager* _lightManager = nullptr;
			RenderManager* _renderManager = nullptr;

			Core::Transform* _transform = nullptr;
			Material_ * _material = nullptr;
			Mesh_ * _mesh = nullptr;
			Shader* _shader = nullptr;

			RenderQueue _renderQueue = RenderQueue::Opaque;
			bool _castsShadows = true;
			bool _debug = false;

			void findMesh();
			void findMaterial();
			void prewake() override;
			bool isUniquePerGameObject() override;
		};
	}
}

#endif //RENDERER_HPP