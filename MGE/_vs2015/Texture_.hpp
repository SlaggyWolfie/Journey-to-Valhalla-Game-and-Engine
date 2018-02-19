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
	namespace Rendering
	{
		enum TextureType { Diffuse, Specular, Emission, Normal };

		class Texture_
		{
		public:
			static Texture_* load(const std::string& texturePath, TextureType type = Diffuse);

			GLuint getId() const;
			std::string getPath() const;
			sf::Image* getImage() const;
			TextureType getType() const;
			void bind() const;
			static void unbind();

			static std::unordered_map<std::string, std::shared_ptr<Texture_>> textureMap;
		private:
			Texture_(TextureType type = Diffuse);
			~Texture_();

			std::string _path;
			std::unique_ptr<sf::Image> _image;
			TextureType _type;

			GLuint _id;
		};
	}
}

#endif