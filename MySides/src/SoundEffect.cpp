#include "SoundEffect.hpp"

SoundEffect::SoundEffect(std::string path, unsigned int voices) :
	voices_(voices),
	lastPlayed_(-1)
{
	buffer_ = new sf::SoundBuffer();

	if (!buffer_->loadFromFile(path))
	{
		std::cout << "Error loading sound" << endl;
	}
	
	sounds_.reserve(voices_);
	
	for (int i = 0; i < voices_; ++i)
	{
		sounds_.push_back(sf::Sound(*buffer_));
	}
}

SoundEffect::~SoundEffect()
{
	delete buffer_;
}

void SoundEffect::play(sf::Vector2f position)
{
	//Increment last played sound
	lastPlayed_ = (lastPlayed_ + 1) % voices_;
	
	std::cout << "playing" << std::to_string(lastPlayed_ + 1) <<  "/" << std::to_string(voices_) << std::endl;

	//Play the next free one
	sounds_[lastPlayed_].setPosition(sf::Vector3f(position.x, position.y, 0));
	sounds_[lastPlayed_].play();
}

void SoundEffect::stopAll()
{
	for (int i = 0; i < voices_; ++i)
	{
		sounds_[i].stop();
	}
}

void SoundEffect::setVolume(float volume)
{
	for (int i = 0; i < voices_; ++i)
	{
		sounds_[i].setVolume(volume);
	}
}

void SoundEffect::setAttenuation(float attenuation)
{
	for (int i = 0; i < voices_; ++i)
	{
		sounds_[i].setAttenuation(attenuation);
	}
}

void SoundEffect::setMinDistance(float minDistance)
{
	for (int i = 0; i < voices_; ++i)
	{
		sounds_[i].setMinDistance(minDistance);
	}
}

