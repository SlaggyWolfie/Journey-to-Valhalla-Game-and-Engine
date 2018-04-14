#include "Texture_.hpp"
#include <iostream>
#include "ResourceManager.hpp"

namespace Engine
{
	namespace Rendering
	{
		bool Texture_::_debug = false;

		//std::unordered_map<std::string, std::unique_ptr<Texture_>> Texture_::textureMap
		//	= std::unordered_map<std::string, std::unique_ptr<Texture_>>();

		ResourceManager* Texture_::_resourceManager = nullptr;

		Texture_::Texture_(const TextureType type) : _type(type), _id()
		{
			glGenTextures(1, &_id);
		}

		Engine::ResourceManager* Texture_::getResourceManager()
		{
			if (!_resourceManager) _resourceManager = ServiceLocator::instance()->getService<Engine::ResourceManager>();
			return _resourceManager;
		}

		Texture_::~Texture_()
		{
			glDeleteTextures(1, &_id);
			//_image = nullptr;
			_image.release();
		}

		Texture_* Texture_::loadDefault(const float r, const float g, const float b, const TextureType type)
		{
			if (_debug)
				std::cout << "Warning, creating default texture with color values: (" + std::to_string(r) + ", "
				+ std::to_string(r) + ", " + std::to_string(r) + ") of type "
				+ std::to_string(static_cast<int>(type)) + "." << std::endl;

			Texture_ * texture = new Texture_(type);
			std::vector<GLubyte> emptyData(4, 0);
			const int color = rgb_to_hex(r, g, b);
			for (unsigned char & i : emptyData)
				i = color;

			glBindTexture(GL_TEXTURE_2D, texture->getId());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			return texture;
			//return nullptr;
		}

		Texture_* Texture_::load(const std::string& texturePath, const TextureType type, const bool flipVertically, const bool flipHorizontally)
		{
			if (getResourceManager()->isCached(texturePath)) return getResourceManager()->retrieveTexture(texturePath);

			// load from file and store in cache
			sf::Image* image = new sf::Image();;
			if (image->loadFromFile(texturePath))
			{
				//normal image 0,0 is top left, but opengl considers 0,0 to be bottom left, so we flip the image internally
				image->flipVertically();
				Texture_ * texture = new Texture_(type);

				//Assign internals
				texture->_image = std::unique_ptr<sf::Image>(image);
				if (flipVertically) texture->_image->flipVertically();
				if (flipHorizontally) texture->_image->flipHorizontally();
				texture->_path = texturePath;

				//Generate Texture
				glBindTexture(GL_TEXTURE_2D, texture->getId());
				switch (type)
				{
				default:
				case TextureType::Diffuse:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA,
						image->getSize().x, image->getSize().y, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelsPtr());
					break;
				case TextureType::Emission:
				case TextureType::Specular:
				case TextureType::Normal:
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
						image->getSize().x, image->getSize().y, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelsPtr());
					break;
				}
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				//	image->getSize().x, image->getSize().y, 0,
				//	GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelsPtr());
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				getResourceManager()->cache(texturePath, texture);

				return texture;
			}

			return nullptr;
		}

		void Texture_::bind() const
		{
			glBindTexture(GL_TEXTURE_2D, _id);
		}

		void Texture_::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		GLuint Texture_::getId() const
		{
			return _id;
		}

		std::string Texture_::getPath() const
		{
			return _path;
		}

		sf::Image* Texture_::getImage() const
		{
			return _image.get();
		}

		TextureType Texture_::getType() const
		{
			return _type;
		}

		int Texture_::rgb_to_hex(float r, float g, float b)
		{
			return
				((static_cast<int>(r * 255) & 0xff) << 16) +
				((static_cast<int>(g * 255) & 0xff) << 8) +
				(static_cast<int>(b * 255) & 0xff);
		}
	}
}
