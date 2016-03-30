// Shape.hpp [BASE: Entity.hpp]
// Game entity with most complex funtion, can move, shoot, die, spawn, decide, etc.

#ifndef MS_SHAPE_HPP
#define MS_SHAPE_HPP

#include <functional>

#include <Thor\Vectors\VectorAlgebra2D.hpp>

#include "Entity.hpp"
#include "ShapeDef.hpp"

#include "Weapon.hpp"
#include "SideDef.hpp"

#define _USE_MATH_DEFINES
#include "math.h"
const double RD = 180 / M_PI;
const double DR = M_PI / 180;

class Shape : public Entity {
public:
	Shape(b2Body* body, const ShapeDef &def, std::function<void(SideDef&)>& callback);
	~Shape();

	void move(b2Vec2 direction); //override
	void orientedMove(b2Vec2 direction); //Thrusts and rotates towards direction
	void stopMove();

	void orient(b2Vec2 direction);
	void snapOrient(b2Vec2 direction);
	void stopRotate();
	b2Vec2 getOrientation() const;

	void takeDamage(int damage, b2Vec2 direction);
	void collect(int value);

	//HP values
	int getHP() const;
	unsigned int getHPMax() const;

	bool canCollect() const;
	int getSidesCollected() const;
	float getSize() const;
		
	void explode();

	//New weapons system
	void arm(Weapon::WeaponI* weapon);
	void disarm();
	bool getArmed();
	Weapon::WeaponI* getWeapon() const;//???
	bool getWeaponReady();
	void trigger(b2Vec2& direction);
	void release();
	void reup();
	int getWeaponBar();
	int getWeaponBarMAX();
	
	void update(int milliseconds);

	void testBed();

	//void draw(GameDrawer d); // override//override
protected:
	/*const*/ float maxVel_;//maximum velocity
	/*const*/ float maxRot_;//maximum rotation

	b2FixtureDef shapeFixDef_;

	//Add physics and shapes
	void addMaterial(b2FixtureDef & def);
	void setPoly(int vertices, float radius);
	void clearb2();

	b2Vec2 lastDamage_;

	//Side dropping
	std::function<void(SideDef&)> sideCallback_;
	void dropSide(b2Vec2 dir, float size);

	//Side collecting
	bool collector_;

	//HP
	int hp_;
	unsigned int hpMAX_;
	int hpScale_;
	
	//New firing logic
	Weapon::WeaponI* weapon_;

	int shapeVertices_;
	int vertices_;
	float size_;

	int sides_; //Currency
};

#endif