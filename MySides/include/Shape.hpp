// Shape.hpp [BASE: Entity.hpp]
// Game entity with most complex funtion, can move, shoot, die, spawn, decide, etc.

#ifndef MS_SHAPE_HPP
#define MS_SHAPE_HPP

#include "Entity.hpp"

namespace traits{
	namespace speed 
	{
		/*
	SLOWEST
	SLOWER
	SLOW
	NORMAL
	FAST
	FASTER
	FASTEST
	*/
	}
}

class Shape : public Entity {
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

	void update(int milliseconds);

//	void draw(GameDrawer d); // override
};

#endif