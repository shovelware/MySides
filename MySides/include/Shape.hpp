#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"

class Shape : protected Entity {
private:
	/*const*/ float maxVel_;//maximum velocity
	/*const*/ float maxRot_;//maximum rotation

	b2Vec2 pole_;//Orientation pole

public:
	Shape(b2Body * body);
	Shape(b2Body * body, int vertices);

	void move(b2Vec2 direction);
	void orientedMove(b2Vec2 direction); //Thrusts from back and rotates towards direction
	void stopMove();

	void orient(b2Vec2 direction);
	void rotate(float amount);
	void stopRotate();

	void applyForces(int milliseconds);
};

#endif