#pragma once
#ifndef TEXTURE__HPP
#define TEXTURE__HPP
#include <string>
#include <GL/glew.h>
#include <unordered_map>
#include <memory>
#include <SFML/Graphics/Image.hpp>

namespace Engine
{
	class ResourceManager;
	namespace Rendering
	{
		enum class TextureType : int { Diffuse, Specular, Emission, Normal };

		class Texture_
		{
		public:
			static Texture_* load(const std::string& texturePath, TextureType type = TextureType::Diffuse, bool flipVertically = false, bool flipHorizontally = false);
			//static Texture_* load(const std::string& texturePath, TextureType type = TextureType::Diffuse);
			static Texture_* loadDefault(float r, float g, float b, TextureType type = TextureType::Diffuse);

			~Texture_();

			GLuint getId() const;
			std::string getPath() const;
			sf::Image* getImage() const;
			TextureType getType() const;
			void bind() const;
			static void unbind();

			//static std::unordered_map<std::string, std::unique_ptr<Texture_>> textureMap;
		private:
			Texture_(TextureType type = TextureType::Diffuse);

			static int rgb_to_hex(float r, float g, float b);

			std::string _path;
			std::unique_ptr<sf::Image> _image;
			TextureType _type;

			GLuint _id;

			static bool _debug;

			static ResourceManager* _resourceManager;
			static ResourceManager* getResourceManager();
		};
	}
}

#endif