#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Shape : protected Entity {
private:
	float accel_;
	float topSpeed_;

public:
	Shape(b2Body * body);

	void move(b2Vec2 direction);
	void rotate(float amount);
};

#endif