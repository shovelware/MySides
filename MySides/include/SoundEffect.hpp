// SoundEffect.hpp
// Wrapper for sf::Sound and sf::SoundBuffer, provides one sfx with multiple voices

#ifndef MS_SOUNDEFFECT
#define MS_SOUNDEFFECT

#include "stdafx.h"

#include <vector>

#include <SFML/Audio.hpp>

class SoundEffect
{
public:
	SoundEffect(std::string path, unsigned int voices);
	~SoundEffect();

	void play(sf::Vector2f position);
	void stopAll();

	void setVolume(float volume);
	void setAttenuation(float attenuation);
	void setMinDistance(float minDistance);

private:
	sf::SoundBuffer buffer_;

	unsigned int lastPlayed_;
	const unsigned int voices_;

	std::vector<sf::Sound> sounds_;
};

#endif