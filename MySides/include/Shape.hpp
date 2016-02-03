// Shape.hpp [BASE: Entity.hpp]
// Game entity with most complex funtion, can move, shoot, die, spawn, decide, etc.

#ifndef MS_SHAPE_HPP
#define MS_SHAPE_HPP

#include <Thor\Vectors\VectorAlgebra2D.hpp>

#include "Entity.hpp"
#include "ShapeDef.hpp"

#include <functional>
#include "Weapon.hpp"
#include "SideDef.hpp"

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

		SQU_EQU,
		SQU_RHO,
		SQU_REC,
		SQU_TRA,

		PEN_EQU
	};
}

class Shape : public Entity {
public:
	Shape(b2Body* body, ShapeDef &def, std::function<void(SideDef&)>& callback);
	~Shape();

	void move(b2Vec2 direction); //override
	void orientedMove(b2Vec2 direction); //Thrusts from back and rotates towards direction
	void stopMove();

	void orient(b2Vec2 direction);
	void rotate(float amount); // override
	void stopRotate();

	void takeDamage(int damage);
	void collect(int value);

	//Setting of ai and controlled, should be inheritance
	bool getControlled() const;
	bool getAI() const;
	void setControlled(bool con);
	void setAI(bool ai);

	//HP values
	int getHP() const;
	unsigned int getHPMax() const;

	int getSidesCollected() const;
		
	void explode();

	//New weapons system
	void arm(Weapon::WeaponI* weapon);
	void disarm();
	bool getArmed();
	void fire(b2Vec2 direction);
	
	void update(int milliseconds);

	//void draw(GameDrawer d); // override

	bool collide(Entity* other, b2Contact& contact); //override
private:
	/*const*/ float maxVel_;//maximum velocity
	/*const*/ float maxRot_;//maximum rotation

	//Set: sides = 1 * scale
	void setTriangleEqu(b2PolygonShape& s, float scale);
	void setSquare(b2PolygonShape& s, float scale);

	//These three repeat themselves, I'd like to make this neater. KISS,DRY!
	void setAsTriangle(float size);
	void setAsSquare(float size);
	void setAsPentagon(float size);

	//Add physics and shapes
	void addMaterial(b2FixtureDef & def);
	void setPoly(b2PolygonShape& s, int vertices, float radius);
	void clearb2();

	b2Vec2 pole_;//Orientation pole

	//Side dropping
	std::function<void(SideDef&)> sideCallback_;
	void dropSide(b2Vec2 dir, float size);

	//HP
	int hp_;
	unsigned int hpMAX_;

	//controlled and ai bools
	bool controlled_;
	bool ai_;

	//New firing logic
	Weapon::WeaponI* weapon_;

	int shapeVertices_;
	int vertices_;
	float size_;

	int sides_; //Currency
	//int type_; //Possibly type enum
};

#endif