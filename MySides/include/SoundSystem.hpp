#ifndef MS_SOUNDSYSTEM
#define	MS_SOUNDSYSTEM

#include "stdafx.h"

#include <map>

#include "SoundEffect.hpp"
#include "AmbientEffect.hpp"

class SoundSystem {
public:
	SoundSystem();
	~SoundSystem();
	
	void setListener(sf::Vector2f position);

	void playSFX(std::string name, sf::Vector2f position);
	void playBGM(std::string name);
	void playAFX(std::string name);
	
	void pauseBGM();
	void resumeBGM(); //TEST THIS

	void pauseAFX(std::string name);
	void resumeAFX(std::string name);
	void stopAFX(std::string name);
	void stopAllAFX();

	void addSFX(std::string name, std::string path, unsigned int voices);
	void addBGM(std::string name, std::string path);
	void addAFX(std::string name, std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance);

	void removeSFX(std::string name);
	void removeBGM(std::string name);
	void removeAFX(std::string name);

	float getMasterVolume() const;
	float getSFXVolume() const;
	float getBGMVolume() const;
	float getAFXVolume() const;

	std::string getBGMNowPlaying() const;

	bool getBGMPlaying() const;
	bool getAFXPlaying() const; //Doubtful we need these

	void setMasterVolume(float volume);
	void setSFXVolume(float volume);
	void setMusicVolume(float volume);
private:
	std::map<std::string, SoundEffect*> sfx_;
	std::map<std::string, AmbientEffect*> afx_;
	std::map<std::string, std::string> bgm_;

	float volMaster_;
	float volSFX_;
	float volBGM_;
	float volAFX_;

	bool checkBGM(std::string name) const;
	bool checkSFX(std::string name) const;
	bool checkAFX(std::string name) const;

	sf::Music playingBGM_;
	AmbientEffect* playingAFX_;

	float bgmMaxDist_;
	float bgmMinDist_;

	float afxMaxDist_;
	float afxMinDist_;

	std::string currentBGMName_;
	std::string currentAFXName_;
};

#endif