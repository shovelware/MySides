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
	
	//Movement
	void move(b2Vec2 direction); //override
	void stopMove();

	//Orientation
	void orient(b2Vec2 direction);
	void snapOrient(b2Vec2 direction);
	void stopRotate();
	b2Vec2 getOrientation() const;

	//State changers
	void heal(int health);
	bool syncHP();
	void syncPoly();
	void takeDamage(int damage, b2Vec2 direction);
	void collect(int value);
	void setPrimary(b2Color col);
	void setSecondary(b2Color col);
	void setTertiary(b2Color col);

	//HP values
	int getHP() const;
	unsigned int getHPMax() const;
	int getUHP() const;
	unsigned int getUHPMax() const;

	//Accessors
	int getSidesCollected() const;
	float getSize() const;
	
	//Death
	void explode();

	//New weapons system
	void arm(Weapon::WeaponI* weapon);
	void disarm();
	bool getArmed();
	Weapon::WeaponI* getWeapon() const;//???
	bool getWeaponReady() const;
	bool getWeaponLoading() const;
	int getWeaponLevel() const;
	void trigger(b2Vec2& direction);
	void release();
	void reup();
	int getWeaponBar();
	int getWeaponBarMAX();

	bool collide(Entity* other, b2Contact& contact, std::string tag);
	void update(int milliseconds);
protected:
	/*const*/ float maxVel_;//maximum velocity
	/*const*/ float maxRot_;//maximum rotation


	//Add physics and shapes
	void setPoly(int vertices, float radius);
	void clearb2();
	b2FixtureDef shapeFixDef_;


	//Side dropping
	std::function<void(SideDef&)> sideCallback_;
	void dropSide(b2Vec2 dir);
	b2Vec2 lastDamage_;

	//HP
	int hp_;
	unsigned int hpMAX_;
	int hpScale_;

	int uhp_;
	unsigned int uhpMAX_;
	int uhpScale_;

	//Weapon
	Weapon::WeaponI* weapon_;

	//Body tracking
	int shapeVertices_;
	int vertices_;
	const unsigned int verticesMIN_ = 2;
	const unsigned int verticesMAX_ = 8;
	float size_;

	int sides_; //!< Currency
	bool collector_;
	bool hasCollected_;
};

#endif