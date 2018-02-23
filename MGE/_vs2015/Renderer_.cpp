#include "Renderer_.hpp"
#include "GameObject_.hpp"
#include "Mesh_.hpp"
#include "Material_.hpp"
#include "Transform.hpp"
#include "Camera_.hpp"
#include "Shader.hpp"
#include "Texture_.hpp"
#include "LightManager.hpp"
#include "Light_.hpp"
#include "ServiceLocator.hpp"
#include "RenderManager.hpp"

namespace Engine
{
	namespace Rendering
	{
		GLint Renderer_::_uniformModelMatrix = 0;
		GLint Renderer_::_uniformViewMatrix = 0;
		GLint Renderer_::_uniformProjectionMatrix = 0;

		GLint Renderer_::_uniformModelViewProjectionMatrix = 0;
		GLint Renderer_::_uniformModelViewMatrix = 0;
		GLint Renderer_::_uniformViewProjectionMatrix = 0;

		GLint Renderer_::_uniformNormalMatrix = 0;

		GLint Renderer_::_uniformMaterialDiffuseColor = 0;
		GLint Renderer_::_uniformMaterialSpecularColor = 0;
		GLint Renderer_::_uniformMaterialEmissionColor = 0;

		GLint Renderer_::_uniformMaterialDiffuseMap = 0;
		GLint Renderer_::_uniformMaterialSpecularMap = 0;
		GLint Renderer_::_uniformMaterialEmissionMap = 0;

		GLint Renderer_::_attributeVertexPositions = 0;
		GLint Renderer_::_attributeVertexNormals = 0;
		GLint Renderer_::_attributeVertexUVs = 0;

		GLint Renderer_::_uniformMaterialShininess = 0;

		GLint Renderer_::_uniformMaterialUseDiffuseMap = 0;
		GLint Renderer_::_uniformMaterialUseSpecularMap = 0;
		GLint Renderer_::_uniformMaterialUseEmissionMap = 0;
		GLint Renderer_::_uniformMaterialUseEmission = 0;

		GLint Renderer_::_uniformCameraPosition = 0;

		GLint Renderer_::_uniformAmbientLightColor = 0;
		GLint Renderer_::_uniformAmbientLightStrength = 0;
		GLint Renderer_::_uniformAmbientLightTerm = 0;
		GLint Renderer_::_uniformAttenuationConstants = 0;

		GLint Renderer_::_uniformDirectionalLightsAmount = 0;
		GLint Renderer_::_uniformPointLightsAmount = 0;
		GLint Renderer_::_uniformSpotLightsAmount = 0;

		Renderer_::Renderer_() : _lightManager(nullptr), _transform(nullptr),
			_material(nullptr), _mesh(nullptr), _shader(nullptr), _renderQueue(RenderQueue::Opaque),
			_castsShadows(true)
		{
		}

		Renderer_::Renderer_(Material_* material, Mesh_* mesh, const RenderQueue renderQueue, const bool castShadows) :
			_lightManager(nullptr), _transform(nullptr),
			_material(material), _mesh(mesh), _shader(_material->getShader()), _renderQueue(renderQueue),
			_castsShadows(castShadows)
		{
		}

		bool Renderer_::shouldCastShadows(const bool castShadows)
		{
			_castsShadows = castShadows;
			return castShadows;
		}

		bool Renderer_::castsShadows() const
		{
			return _castsShadows;
		}

		void Renderer_::initialize()
		{
			//initializeShader("default");
			_shader = _material->getShader();
			setup();
		}

		void Renderer_::initializeShader(const std::string& path)
		{
			_shader = new Shader();
			_shader->addShader(GL_VERTEX_SHADER, path + ".vs");
			_shader->addShader(GL_FRAGMENT_SHADER, path + ".fs");
			_shader->finalize();
		}

		void Renderer_::setup() const
		{
			_uniformModelMatrix = _shader->getUniformLocation("modelMatrix");
			_uniformViewMatrix = _shader->getUniformLocation("viewMatrix");
			_uniformProjectionMatrix = _shader->getUniformLocation("projectionMatrix");

			_uniformModelViewProjectionMatrix = _shader->getUniformLocation("mvpMatrix");
			_uniformModelViewMatrix = _shader->getUniformLocation("mvMatrix");
			_uniformViewProjectionMatrix = _shader->getUniformLocation("vpMatrix");

			_uniformNormalMatrix = _shader->getUniformLocation("normalMatrix");

			_attributeVertexPositions = _shader->getAttributeLocation("vertexPosition");
			_attributeVertexNormals = _shader->getAttributeLocation("vertexNormal");
			_attributeVertexUVs = _shader->getAttributeLocation("vertexUV");

			_uniformMaterialDiffuseColor = _shader->getUniformLocation("material.diffuseColor");
			_uniformMaterialSpecularColor = _shader->getUniformLocation("material.specularColor");
			_uniformMaterialEmissionColor = _shader->getUniformLocation("material.emissionColor");

			_uniformMaterialDiffuseMap = _shader->getUniformLocation("material.diffuseMap");
			_uniformMaterialSpecularMap = _shader->getUniformLocation("material.specularMap");
			_uniformMaterialEmissionMap = _shader->getUniformLocation("material.emissionMap");

			_uniformMaterialShininess = _shader->getUniformLocation("material.shininess");

			_uniformMaterialUseDiffuseMap = _shader->getUniformLocation("material.useDiffuseMap");
			_uniformMaterialUseSpecularMap = _shader->getUniformLocation("material.useSpecularMap");
			_uniformMaterialUseEmissionMap = _shader->getUniformLocation("material.useEmissionMap");
			_uniformMaterialUseEmission = _shader->getUniformLocation("material.useEmission");

			_uniformCameraPosition = _shader->getUniformLocation("cameraPosition");

			_uniformAmbientLightColor = _shader->getUniformLocation("light.ambientLightColor");
			_uniformAmbientLightStrength = _shader->getUniformLocation("light.ambientStrength");
			_uniformAmbientLightTerm = _shader->getUniformLocation("light.ambientTerm");

			_uniformAttenuationConstants = _shader->getUniformLocation("light.attenuationConstants");

			_uniformDirectionalLightsAmount = _shader->getUniformLocation("light.directionalLightsAmount");
			_uniformPointLightsAmount = _shader->getUniformLocation("light.pointLightsAmount");
			_uniformSpotLightsAmount = _shader->getUniformLocation("light.spotLightsAmount");
		}

		void Renderer_::pushMatrices() const
		{
			//get matrices
			const glm::mat4 model = getGameObject()->getTransform()->getMatrix4X4();
			const glm::mat4 view = Core::Camera_::getMainCamera()->getViewMatrix();
			const glm::mat4 projection = Core::Camera_::getMainCamera()->getProjectionMatrix();
			const glm::mat4 mvp = projection * view * model;
			const glm::mat4 mv = view * model;
			const glm::mat4 vp = projection * view;

			const glm::mat3 normal = getGameObject()->getTransform()->getNormalMatrix();

			//pass in all MVP matrices separately
			glUniformMatrix4fv(_uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(_uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(_uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(_uniformModelViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
			glUniformMatrix4fv(_uniformModelViewMatrix, 1, GL_FALSE, glm::value_ptr(mv));
			glUniformMatrix4fv(_uniformViewProjectionMatrix, 1, GL_FALSE, glm::value_ptr(vp));
			glUniformMatrix3fv(_uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normal));
		}

		void Renderer_::pushLights() const
		{
			const glm::vec3 ambientLightColor = _lightManager->getAmbientLightColor();
			const float ambientLightStrength = _lightManager->getAmbientStrength();
			glUniform3fv(_uniformAmbientLightColor, 1, glm::value_ptr(ambientLightColor));
			glUniform3fv(_uniformAmbientLightTerm, 1, glm::value_ptr(ambientLightColor * ambientLightStrength));
			glUniform1f(_uniformAmbientLightStrength, ambientLightStrength);

			glUniform3fv(_uniformAttenuationConstants, 1, glm::value_ptr(_lightManager->getAttenuation()));

			const int directionalLights = _lightManager->getDirectionalLightsAmount();
			const int pointLights = _lightManager->getPointLightsAmount();
			const int spotLights = _lightManager->getSpotLightsAmount();
			glUniform1i(_uniformDirectionalLightsAmount, directionalLights);
			glUniform1i(_uniformPointLightsAmount, pointLights);
			glUniform1i(_uniformSpotLightsAmount, spotLights);

			std::string prefix = "directionalLights";

			for (int i = 0; i < directionalLights; i++)
			{
				const std::string localPrefix = prefix + "[" + std::to_string(i) + "].";
				const Light_* light = _lightManager->getDirectionalLight(i);

				glUniform3fv(_shader->getUniformLocation(localPrefix + "color"), 1, glm::value_ptr(light->getColor()));
				glUniform3fv(_shader->getUniformLocation(localPrefix + "direction"), 1,
					glm::value_ptr(light->getGameObject()->getTransform()->forward()));
				glUniform1f(_shader->getUniformLocation(localPrefix + "intensity"), light->getLightIntensity());
				//glUniform1f(_shader->getUniformLocation(localPrefix + "range"), light->getRange()));
			}

			prefix = "pointLights";
			for (int i = 0; i < pointLights; i++)
			{
				const std::string localPrefix = prefix + "[" + std::to_string(i) + "].";
				const Light_* light = _lightManager->getPointLight(i);

				glUniform3fv(_shader->getUniformLocation(localPrefix + "color"), 1, glm::value_ptr(light->getColor()));
				glUniform3fv(_shader->getUniformLocation(localPrefix + "position"), 1,
					glm::value_ptr(light->getGameObject()->getTransform()->getPosition()));
				glUniform1f(_shader->getUniformLocation(localPrefix + "intensity"), light->getLightIntensity());
				glUniform1f(_shader->getUniformLocation(localPrefix + "range"), light->getRange());
			}

			prefix = "spotLights";
			for (int i = 0; i < pointLights; i++)
			{
				const std::string localPrefix = prefix + "[" + std::to_string(i) + "].";
				const Light_* light = _lightManager->getPointLight(i);
				Core::Transform* lightTransform = light->getGameObject()->getTransform();

				glUniform3fv(_shader->getUniformLocation(localPrefix + "color"), 1,
					glm::value_ptr(light->getColor()));
				glUniform3fv(_shader->getUniformLocation(localPrefix + "position"), 1,
					glm::value_ptr(lightTransform->getPosition()));
				glUniform3fv(_shader->getUniformLocation(localPrefix + "direction"), 1,
					glm::value_ptr(lightTransform->forward()));

				glUniform1f(_shader->getUniformLocation(localPrefix + "fallOff"), light->getFallOffRadians());
				glUniform1f(_shader->getUniformLocation(localPrefix + "fallOffOuter"), light->getFallOffOuterRadians());

				glUniform1f(_shader->getUniformLocation(localPrefix + "intensity"), light->getLightIntensity());
				glUniform1f(_shader->getUniformLocation(localPrefix + "range"), light->getRange());
			}
		}

		void Renderer_::pushMaterial() const
		{
			//Calculate total specular color and pass it in and the material shininess
			glUniform3fv(_uniformMaterialSpecularColor, 1,
				glm::value_ptr(_material->getSpecularStrength() * _material->getSpecularColor()));
			glUniform1f(_uniformMaterialShininess, _material->getShininess());

			//set the material color
			glUniform3fv(_uniformMaterialDiffuseColor, 1,
				glm::value_ptr(_material->getDiffuseStrength() * _material->getDiffuseColor()));

			//set the emission color
			glUniform3fv(_uniformMaterialEmissionColor, 1,
				glm::value_ptr(_material->getEmissionStrength() * _material->getEmissionColor()));

			glUniform1i(_uniformMaterialUseDiffuseMap, _material->isDiffuseMapUsed());
			glUniform1i(_uniformMaterialUseSpecularMap, _material->isSpecularMapUsed());
			glUniform1i(_uniformMaterialUseEmissionMap, _material->isEmissionMapUsed());
			glUniform1i(_uniformMaterialUseEmission, _material->isEmissionUsed());

			//setup texture slot
			glActiveTexture(GL_TEXTURE0);
			//bind the texture to the current active slot
			//glBindTexture(GL_TEXTURE_2D, _material->getDiffuseMap()->getId());
			_material->getDiffuseMap()->bind();
			//tell the shader the texture slot for the diffuse texture is slot 0
			glUniform1i(_uniformMaterialDiffuseMap, 0);

			//setup texture slot
			glActiveTexture(GL_TEXTURE1);
			//bind the texture to the current active slot
			_material->getSpecularMap()->bind();
			glUniform1i(_uniformMaterialSpecularMap, 1);

			//setup texture slot
			glActiveTexture(GL_TEXTURE2);
			_material->getEmissionMap()->bind();
			glUniform1i(_uniformMaterialEmissionMap, 2);
			Texture_::unbind();
		}

		void Renderer_::pushCameraPosition() const
		{
			//Create a fallback camera position (or get the actual one if possible) and pass it in
			glm::vec3 cameraPosition = glm::vec3(0);
			Core::Camera_* mainCamera = Core::Camera_::getMainCamera();
			if (mainCamera != nullptr) cameraPosition = mainCamera->getGameObject()->getTransform()->getPosition();
			glUniform3fv(_uniformCameraPosition, 1, glm::value_ptr(cameraPosition));
		}

		void Renderer_::pushMesh() const
		{
			_mesh->stream2(_attributeVertexPositions, _attributeVertexNormals, _attributeVertexUVs);
			//_mesh->stream(_attributeVertexPositions, _attributeVertexNormals, _attributeVertexUVs);
		}

		void Renderer_::render()
		{
			_shader->bind();

			pushLights();
			pushMatrices();
			pushMaterial();
			pushCameraPosition();
			pushMesh();

			Shader::unbind();
		}

		void Renderer_::findMesh()
		{
			_mesh = getGameObject()->getComponent<Mesh_>();
		}

		void Renderer_::findMaterial()
		{
			_material = getGameObject()->getComponent<Material_>();
			_shader = _material->getShader();
		}

		bool Renderer_::isUniquePerGameObject()
		{
			return true;
		}

		void Renderer_::prewake()
		{
			if (_mesh == nullptr || _material == nullptr)
			{
				findMesh();
				findMaterial();
			}

			_transform = getGameObject()->getTransform();
			_lightManager = ServiceLocator::instance()->getService<LightManager>();
			_renderManager = ServiceLocator::instance()->getService<RenderManager>();
			_renderManager->addRenderer(this);

			initialize();
		}
	}
}
