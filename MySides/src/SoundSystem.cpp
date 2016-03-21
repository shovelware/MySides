#include "SoundSystem.hpp"

SoundSystem::SoundSystem() :
	volMaster_(50.f),
	volSFX_(75.f),
	volAFX_(50.f),
	volBGM_ (0.f),
	bgmMinDist_(420.f),
	bgmMaxDist_(1024.f),
	currentBGMName_("")
{
	sf::Listener::setGlobalVolume(volMaster_);
	sf::Listener::setDirection(sf::Vector3f(0, 0, -1));

	playingBGM_.setLoop(true);
	playingBGM_.setVolume(volBGM_);
}

SoundSystem::~SoundSystem() {
	for (std::map<std::string, AmbientEffect*>::iterator iter = afx_.begin(), end = afx_.end(); iter != end; ++iter)
	{
		delete iter->second;
	}

	for (std::map<std::string, SoundEffect*>::iterator iter = sfx_.begin(), end = sfx_.end(); iter != end; ++iter)
	{
		delete iter->second;
	}
}

bool SoundSystem::checkBGM(std::string name) const { return (bgm_.count(name) > 0); }
bool SoundSystem::checkSFX(std::string name) const { return (sfx_.count(name) > 0); }
bool SoundSystem::checkAFX(std::string name) const { return (afx_.count(name) > 0); }

void SoundSystem::setListener(sf::Vector2f position)
{
	sf::Listener::setPosition(sf::Vector3f(position.x, position.y, 0));
	
	for (std::map<std::string, AmbientEffect*>::iterator iter = afx_.begin(), end = afx_.end(); iter != end; ++iter)
	{
		iter->second->updateFactor();
	}

	//Dampen Music sound with distance
	float distance = std::sqrt(pow(position.x, 2) + pow(position.y, 2));
	float bgmfactor = 1;

	float bgmouterfac = 1.2 - (distance - bgmMinDist_) / (bgmMaxDist_ - bgmMinDist_);
	bgmfactor = (bgmouterfac <= bgmfactor ? bgmouterfac : bgmfactor);

	//Factor should be 1 at dist < internal
	//should be 0.25 at edge of radius
	//should scale between

	//std::cout << bgmfactor << std::endl;
	playingBGM_.setVolume(volBGM_ * bgmfactor);

	float afxfactor = 1;
}

void SoundSystem::playSFX(std::string name, sf::Vector2f position)
{
	if (volSFX_ != 0 && checkSFX(name))
	{
		sfx_.at(name)->setVolume(volSFX_);
		sfx_.at(name)->play(position);
	}
}

void SoundSystem::playBGM(std::string name)
{
	if (volBGM_ != 0 && checkBGM(name))
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

void SoundSystem::playAFX(std::string name)
{
	if (volAFX_ != 0 && checkAFX(name))
	{
		afx_.at(name)->setVolume(volAFX_);
		afx_.at(name)->play();
	}
}

void SoundSystem::pauseAFX(std::string name)
{
	if (checkAFX(name))
	{
		afx_.at(name)->pause();
	}
}

void SoundSystem::resumeAFX(std::string name)
{
	if (checkAFX(name))
	{
		afx_.at(name)->resume();
	}
}

void SoundSystem::stopAFX(std::string name)
{
	if (checkAFX(name))
	{
		afx_.at(name)->stop();
	}
}

void SoundSystem::stopAllAFX()
{
	//Loop through ambient effects and stop them
	for (std::map<std::string, AmbientEffect*>::iterator iter = afx_.begin(), end = afx_.end(); iter != end; ++iter)
	{
		iter->second->stop();
	}
}

void SoundSystem::pauseBGM()
{
	playingBGM_.pause();
}

void SoundSystem::resumeBGM()
{
	if (volBGM_ != 0 &&playingBGM_.getStatus() == sf::SoundSource::Status::Paused)
	{
		playingBGM_.play();
	}
}

void SoundSystem::addSFX(std::string name, std::string path, unsigned int voices)
{
	if (!checkSFX(name))
	{
		sfx_.emplace(name, new SoundEffect(path, voices));
		sfx_.at(name)->setMinDistance(64);
	}
}

void SoundSystem::addBGM(std::string name, std::string path)
{
	if (!checkSFX(name))
	{
		bgm_.emplace(name, path);
	}
}

void SoundSystem::addAFX(std::string name, std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance)
{
	if (!checkAFX(name))
	{
		afx_.emplace(name, new AmbientEffect(path, nearFactor, farFactor, nearDistance, farDistance));
	}
}

void SoundSystem::removeSFX(std::string name)
{
	if (checkSFX(name))
	{
		delete sfx_.at(name);
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

void SoundSystem::removeAFX(std::string name)
{
	if (checkAFX(name))
	{
		delete afx_.at(name);
		afx_.erase(name);
	}
}

float SoundSystem::getMasterVolume() const { return volMaster_; }
float SoundSystem::getSFXVolume() const { return volSFX_; }
float SoundSystem::getBGMVolume() const { return volBGM_; }
float SoundSystem::getAFXVolume() const { return volAFX_; }

std::string SoundSystem::getBGMNowPlaying() const { return currentBGMName_; }

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