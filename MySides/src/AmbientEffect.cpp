#include "stdafx.h"
#include "AmbientEffect.hpp"

AmbientEffect::AmbientEffect(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance) : 
	nearFactor_(nearFactor),
	farFactor_(farFactor),
	nearDistance_(nearDistance),
	farDistance_(farDistance),
	volume_(100)
{
	ambience_.openFromFile(path);
	ambience_.setLoop(true);
	ambience_.setVolume(volume_);
}

void AmbientEffect::updateFactor()
{
	//Pull distance from listener
	sf::Vector3f position(sf::Listener::getPosition());
	float distance = std::sqrt(pow(position.x, 2) + pow(position.y, 2));

	float factor = 0;

	if (distance <= nearDistance_) { factor = nearFactor_; }
	else if (distance >= farDistance_) { factor = farFactor_; }

	else
	{
		float n2f = (distance - nearDistance_) / (farDistance_ - nearDistance_);
		float diff = farFactor_ - nearFactor_;

		factor = nearFactor_ + (diff * n2f);
	}

	//std::cout << factor << std::endl;
	ambience_.setVolume(volume_ * factor);
}

void AmbientEffect::setVolume(float volume) 
{
	volume_ = fmaxf(0, fminf(volume, 100)); 
	ambience_.setVolume(volume_); 
}

void AmbientEffect::setPosition(sf::Vector2f position) { ambience_.setPosition(sf::Vector3f(position.x, position.y, 0.f)); }

void AmbientEffect::play() { 
	ambience_.stop();
	ambience_.play(); 
}

void AmbientEffect::pause()
{
	ambience_.pause();
}

void AmbientEffect::resume()
{
	if (ambience_.getStatus() == sf::SoundSource::Status::Paused)
	{
		ambience_.play();
	}
}

void AmbientEffect::stop()
{
	ambience_.stop();
}