#include "pch.h"

#include "AssetManager.h"

void AssetManager::LoadErrorResponses()
{
	AddTexture("error", "../../../res/error.jpg");
	AddSound("error", "../../../res/error.wav");
}

bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath, const sf::IntRect& area)
{
	if (mTextureList.find(textureName) != mTextureList.end())
	{
		std::cout << "Texture " << textureName << " already exists" << std::endl;
		return false;
	}

	sf::Texture* texture = new sf::Texture();

	if (area != sf::IntRect())
	{
		if (!texture->loadFromFile(texturePath, area))
		{
			std::cout << "Texture " << textureName << " (Rect) failed to load" << std::endl;
			delete texture;
			return false;
		}
		else
			std::cout << "Texture "<< textureName <<" (Rect) loaded succesfully" << std::endl;
	}
	else
	{
		if (!texture->loadFromFile(texturePath))
		{
			std::cout << "Texture " << textureName << " (All) failed to load" << std::endl;
			delete texture;
			return false;
		}
		else
			std::cout << "Texture " << textureName << " (All) loaded succesfully" << std::endl;
	}

	mTextureList.emplace(textureName, texture);
	return true;
}

const sf::Texture& AssetManager::GetTexture(const std::string& textureName, const sf::IntRect& area)
{
	auto textureItr = mTextureList.find(textureName);
	if (textureItr == mTextureList.end())
	{
		std::cout << "Texture '" << textureName << "' cannot be found" << std::endl;
		return *mTextureList["error"];
	}

	return *textureItr->second;
}



bool AssetManager::AddSound(const std::string& soundName, const std::string& soundPath)
{
	if (mSoundBufferList.find(soundName) != mSoundBufferList.end())
	{
		std::cout << "Sound " << soundName << " already exists" << std::endl;
		return false;
	}

	sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
	if (!soundBuffer->loadFromFile(soundPath))
	{
		std::cout << "Sound " << soundName << " failed to load" << std::endl;
		delete soundBuffer;
		return false;
	}
	std::cout << "Sound " << soundName << " loaded successfully" << std::endl;

	mSoundBufferList.emplace(soundName, std::move(soundBuffer));

	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(*mSoundBufferList[soundName]); 
	mSoundList.emplace(soundName, sound);
	return true;
}

const sf::Sound& AssetManager::GetSound(const std::string& soundName)
{
	auto soundItr = mSoundList.find(soundName);
	if (soundItr == mSoundList.end())
	{
		std::cout << "Sound '" << soundName << "' cannot be found" << std::endl;
		return *mSoundList["error"];
	}
	return *soundItr->second;
}

void AssetManager::PlayLocalSound(const std::string& soundName)
{
	mSoundList[soundName]->play();
}



bool AssetManager::AddMusic(const std::string& musicName, const std::string& musicPath)
{
	if (mMusicList.find(musicName) != mMusicList.end())
	{
		std::cout << "Music " << musicName << " already exists" << std::endl;
		return false;
	}

	sf::Music* music = new sf::Music();
	if (!music->openFromFile(musicPath))
	{
		std::cout << "Music " << musicName << " failed to load" << std::endl;
		delete music;
		return false;
	}
	std::cout << "Music " << musicName << " loaded successfully" << std::endl;

	mMusicList.emplace(musicName, music);

	return true;
}


sf::Music& AssetManager::GetMusic(const std::string& musicName)
{
	return *mMusicList.at(musicName);
}

void AssetManager::ReleaseAll()
{
	for (auto& pair : mTextureList) {
		delete pair.second;
	}
	mTextureList.clear();

	for (auto& pair : mSoundBufferList) {
		delete pair.second;
	}
	mSoundBufferList.clear();

	for (auto& pair : mSoundList) {
		delete pair.second;
	}
	mSoundList.clear();

	for (auto& pair : mMusicList) {
		delete pair.second;
	}
	mMusicList.clear();

	std::cout << "All assets have been released." << std::endl;
}