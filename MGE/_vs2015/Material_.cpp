#include "Material_.hpp"

namespace Engine
{
	namespace Rendering
	{

		Material_::Material_() :
			_diffuseColor(glm::vec3(1)), _specularColor(glm::vec3(1)), _emissionColor(glm::vec3(0, 1, 0)),
			_diffuseMap(nullptr), _specularMap(nullptr), _emissionMap(nullptr)
		{
		}

		void Material_::setDiffuseColor(glm::vec3 color)
		{
		}
	}
}
