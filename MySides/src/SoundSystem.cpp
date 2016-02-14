#include "SoundSystem.hpp"

SoundSystem::SoundSystem() :
	volMaster_(50.f),
	volSFX_(100.f),
	volBGM_ (50.f),
	currentBGMName_("")
{
	sf::Listener::setGlobalVolume(volMaster_);
	playingBGM_.setVolume(volBGM_);
	playingBGM_.setLoop(true);
}

SoundSystem::~SoundSystem()
{
}

bool SoundSystem::checkBGM(std::string name)
{
	return (bgm_.count(name) > 0);
}

bool SoundSystem::checkSFX(std::string name)
{
	return (sfx_.count(name) > 0);
}

void SoundSystem::setListener(sf::Vector2f position)
{
	sf::Listener::setPosition(sf::Vector3f(position.x, position.y, 0));
}

void SoundSystem::playSFX(std::string name, sf::Vector2f position)
{
	if (checkSFX(name))
	{
		sfx_.at(name).setVolume(volSFX_);
		sfx_.at(name).play(position);
	}
}

void SoundSystem::playBGM(std::string name)
{
	if (checkBGM(name))
	{
		//If we match current, restart
		if (currentBGMName_ == name)
		{
			playingBGM_.play();
		}

		//else update
		currentBGMName_ = name;
		playingBGM_.openFromFile(bgm_.at(name));
		playingBGM_.play();
	}
}

void SoundSystem::pauseBGM()
{
	playingBGM_.pause();
}

void SoundSystem::unpauseBGM()
{
	if (playingBGM_.getStatus() == sf::SoundSource::Status::Paused)
	{
		playingBGM_.play();
	}
}

void SoundSystem::addSFX(std::string name, std::string path, unsigned int voices)
{
	if (!checkSFX(name))
	{
		sfx_.emplace(name, SoundEffect(path, voices));
		sfx_.at(name).setMinDistance(100);
	}
}

void SoundSystem::addBGM(std::string name, std::string path)
{
	if (!checkSFX(name))
	{
		bgm_.emplace(name, path);
	}
}

void SoundSystem::removeSFX(std::string name)
{
	if (checkSFX(name))
	{
		sfx_.erase(name);
	}
}

void SoundSystem::removeBGM(std::string name)
{
	if (checkBGM(name))
	{
		bgm_.erase(name);
	}
}

float SoundSystem::getMasterVolume() { return volMaster_; }
float SoundSystem::getSFXVolume() { return volSFX_; }
float SoundSystem::getMusicVolume() { return volBGM_; }
std::string SoundSystem::getNowPlaying() { return currentBGMName_; }

void SoundSystem::setMasterVolume(float volume)
{
	if (0 <= volume && volume <= 100.f)
	{
		volMaster_ = volume;
		sf::Listener::setGlobalVolume(volMaster_);
	}
}

void SoundSystem::setSFXVolume(float volume)
{
	if (0 <= volume && volume <= 100.f)
	{
		volSFX_ = volume;
	}
}

void SoundSystem::setMusicVolume(float volume)
{
	if (0 <= volume && volume <= 100.f)
	{
		volBGM_ = volume;
		playingBGM_.setVolume(volume);
	}
}