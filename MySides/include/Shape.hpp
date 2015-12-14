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

	enum type {
		ANGLE,

		TRI_ISO,
		TRI_EQU,

		SQU_,
		SQU_RHOMBUS,
		SQU_RECT,
		SQU_TRAP,

		PEN_
	};
}

class Shape : public Entity {
public:
	Shape(b2Body* body);
	Shape(b2Body* body, int vertices, float radius);

	void move(b2Vec2 direction); //override
	void orientedMove(b2Vec2 direction); //Thrusts from back and rotates towards direction
	void stopMove();

	void orient(b2Vec2 direction);
	void rotate(float amount); // override
	void stopRotate();

	void takeDamage(int damage);
	void collect(int value);

	bool getControlled() const;
	bool getAI() const;

	void setControlled(bool con);
	void setAI(bool ai);

	int getHP() const;
	unsigned int getMaxHP() const;

	////Temp for weapon refire
	b2Vec2 getFirePoint(float x, float y);
	bool getArmed(); 
	
	void update(int milliseconds);

//	void draw(GameDrawer d); // override

	bool collide(Entity* other, b2Contact& contact); //override

private:
	/*const*/ float maxVel_;//maximum velocity
	/*const*/ float maxRot_;//maximum rotation


	//Set: sides = 1 * scale
	void setTriangleEqu(b2PolygonShape& s, float scale);
	void setTriangleIso(b2PolygonShape& s, float scale);

	//Altset: Area = 1 * scale ^2
	void altSetTriangleEq(b2PolygonShape& s, float scale);
	void altSetTriangleIs(b2PolygonShape& s, float scale);

	b2Vec2 pole_;//Orientation pole
	int hp_;
	unsigned int maxHP_;

	//controlled and ai bools
	bool controlled_;
	bool ai_;

	////Temp for weapon refire
	unsigned int refireTime_;
	int coolDown_;

	int sides_;
};

#endif