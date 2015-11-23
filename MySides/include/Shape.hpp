// Shape.hpp
// Game entity with most complex funtion, can move, shoot, die, spawn, decide, etc.

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Entity.hpp"

class Shape : protected Entity {
private:
	/*const*/ float maxVel_;//maximum velocity
	/*const*/ float maxRot_;//maximum rotation

	b2Vec2 pole_;//Orientation pole
	int hp_;
	int maxHP_;

	bool alive_;
	bool active_;
public:
	Shape(b2Body * body);
	Shape(b2Body * body, int vertices);

	void move(b2Vec2 direction); //override
	void orientedMove(b2Vec2 direction); //Thrusts from back and rotates towards direction
	void stopMove();

	void orient(b2Vec2 direction);
	void rotate(float amount); // override
	void stopRotate();

	void applyForces(int milliseconds);

	void draw(GameDrawer d); // override
};

#endif