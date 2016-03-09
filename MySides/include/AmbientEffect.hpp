#ifndef MS_AMBIENTEFFECT
#define MS_AMBIENTEFFECT

#include "stdafx.h"

#include "SFML/Audio.hpp"


//!	@class	AmbientEffect
//!	@brief	An ambient sound based on sf::Music.
//!			Scales volume between two factors based on distance to listener.
//!	@see	SoundEffect

class AmbientEffect
{
public:
	AmbientEffect(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance);

	void updateFactor();						//!< Updates the volume factor based on listener's position

	void setVolume(float volume);				//!< Sets the base volume of the sound. This will be affected by nearFactor_ and farFactor_.
	void setPosition(sf::Vector2f position);	//!< Sets the position of the underlying sf::Music

	void play();			//!< Plays the sound from the beginning
	void pause();			//!< Pauses the sound
	void resume();			//!< Resumes the sound if it was paused, plays if not
	void stop();			//!< Stops the sound

	float nearFactor_;		//!< The volume factor for the near distance
	float nearDistance_;	//!< Near distance, in SFML units

	float farFactor_;		//!< Volume factor for far distance
	float farDistance_;		//!< Far distance, in SFML units

private:
	float volume_;			//!< The base volume of the sound
	sf::Music ambience_;	//!< The actual music object
};

#endif