#ifndef MS_AMBIENTEFFECT
#define MS_AMBIENTEFFECT

#include "stdafx.h"

#include "SFML/Audio.hpp"

//!	@class	AmbientEffect
//!	@brief	Scales volume between two factors based on distance to listener.
//!	@see	SoundEffect

//!		An ambient sound based on sf::Music.
//!		The sound is played at nearFactor when distance =< nearDistance,
//!		farFactor when distance >= farDistance,
//!		and scales between the two in between.

class AmbientEffect
{
public:
	//! Test
	AmbientEffect(std::string path, float nearFactor, float farFactor, float nearDistance, float farDistance);

	//! Updates the volume factor based on listener's position
	//! Pulls the listener from SFML audio system, then calculates blending between both factors
	//!
	void updateFactor();

	//! Sets the base volume of the sound. This will be affected by nearFactor_ and farFactor_.
	void setVolume(float volume);				
	
	//! Sets the position of the underlying sf::Music
	void setPosition(sf::Vector2f position);	

	void play();			//!< Plays the sound from the beginning
	void pause();			//!< Pauses the sound
	void resume();			//!< Resumes the sound if it was paused, plays if not
	void stop();			//!< Stops the sound

	float nearFactor_;		//!< The volume factor for the near distance
	float nearDistance_;	//!< Near distance, in SFML units

	float farFactor_;		//!< Volume factor for far distance
	float farDistance_;		//!< Far distance, in SFML units

private:
	float volume_;
	sf::Music ambience_;
};

#endif