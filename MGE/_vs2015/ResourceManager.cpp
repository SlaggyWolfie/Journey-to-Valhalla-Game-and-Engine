#include "ResourceManager.hpp"

void Engine::ResourceManager::cache(const std::string& path, Rendering::Texture_* texture)
{
	_textureCache[path] = std::unique_ptr<Rendering::Texture_>(texture);
}

void Engine::ResourceManager::cache(const std::string& path, sf::SoundBuffer* soundBuffer)
{
	_soundBufferCache[path] = std::unique_ptr<sf::SoundBuffer>(soundBuffer);
}

bool Engine::ResourceManager::isCached(const std::string& path)
{
	return
		_textureCache.find(path) != _textureCache.end() ||
		_soundBufferCache.find(path) != _soundBufferCache.end();
}

Engine::Rendering::Texture_* Engine::ResourceManager::getTexture(const std::string& path)
{
	Rendering::Texture_* texture = nullptr;
	if (_textureCache.find(path) != _textureCache.end()) texture = retrieveTexture(path);
	return texture;
}

sf::SoundBuffer* Engine::ResourceManager::getSoundBuffer(const std::string& path)
{
	sf::SoundBuffer* soundBuffer = nullptr;
	if (_soundBufferCache.find(path) != _soundBufferCache.end()) soundBuffer = retrieveSoundBuffer(path);
	return soundBuffer;
}

Engine::Rendering::Texture_* Engine::ResourceManager::retrieveTexture(const std::string& path)
{
	return _textureCache[path].get();
}

sf::SoundBuffer* Engine::ResourceManager::retrieveSoundBuffer(const std::string& path)
{
	return _soundBufferCache[path].get();
}

void Engine::ResourceManager::initialize()
{
}

void Engine::ResourceManager::reset()
{
	if (isGoingToReset())
		forceReset();
}

void Engine::ResourceManager::shouldBeReset(const bool reset)
{
	_shouldBeReset = reset;
}

bool Engine::ResourceManager::isGoingToReset() const
{
	return _shouldBeReset;
}

void Engine::ResourceManager::forceReset()
{
	_textureCache.clear();
	_soundBufferCache.clear();
}
