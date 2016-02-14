#ifndef MS_SOUNDSYSTEM
#define	MS_SOUNDSYSTEM

#include "stdafx.h"

#include <map>

#include "SoundEffect.hpp"

class SoundSystem {
public:
	SoundSystem();
	~SoundSystem();
	
	void setListener(sf::Vector2f position);

	void playSFX(std::string name, sf::Vector2f position);
	void playBGM(std::string name);
	
	void pauseBGM();
	void unpauseBGM();

	void addSFX(std::string name, std::string path, unsigned int voices);
	void addBGM(std::string name, std::string path);

	void removeSFX(std::string name);
	void removeBGM(std::string name);

	float getMasterVolume();
	float getSFXVolume();
	float getMusicVolume();

	std::string getNowPlaying();

	void setMasterVolume(float volume);
	void setSFXVolume(float volume);
	void setMusicVolume(float volume);


private:
	std::map<std::string, SoundEffect> sfx_;
	std::map<std::string, std::string> bgm_;

	float volMaster_;
	float volSFX_;
	float volBGM_;

	bool checkBGM(std::string name);
	bool checkSFX(std::string name);

	sf::Music playingBGM_;
	std::string currentBGMName_;
};

#endif