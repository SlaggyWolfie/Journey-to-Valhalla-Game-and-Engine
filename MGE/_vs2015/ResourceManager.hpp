#pragma once
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP
#include "Manager.hpp"
#include <unordered_map>
#include "Texture_.hpp"
#include <SFML/Audio.hpp>
#include <memory>

namespace Engine
{
	class ResourceManager : public Manager
	{
	private:
		bool _shouldBeReset = false;

		//std::unordered_map<std::string, std::unique_ptr<Rendering::Texture_>> _textureCache;
		std::unordered_map<std::string, std::unique_ptr<Rendering::Texture_>> _textureCache;
		std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> _soundBufferCache;
		//mesh cache

	public:
		void cache(const std::string& path, Rendering::Texture_* texture);
		void cache(const std::string& path, sf::SoundBuffer* soundBuffer);

		bool isCached(const std::string& path);
		//bool isCached(Rendering::Texture_* texture);
		//bool isCached(sf::SoundBuffer* soundBuffer);

		Rendering::Texture_* retrieveTexture(const std::string& path);
		sf::SoundBuffer* retrieveSoundBuffer(const std::string& path);

		Rendering::Texture_* getTexture(const std::string& path);
		sf::SoundBuffer* getSoundBuffer(const std::string& path);

		void initialize() override;
		void reset() override;

		void shouldBeReset(bool reset);
		bool isGoingToReset() const;
		void forceReset();

		//explicit ResourceManager() = default;
		virtual ~ResourceManager() = default;
	};
}

#endif