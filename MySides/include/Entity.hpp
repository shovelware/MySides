// Entity.hpp
// Base entity class, includes movement functions, body, active and alive status
// Children: Shape, Projectile, Bounds Side, //Obstacle

#ifndef MS_ENTITY_HPP
#define MS_ENTITY_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

//#include "GameDrawer.hpp"

//Forward declaration for collision

//Using pi to convert deg <-> rad //DO I NEED THIS?
#define _USE_MATH_DEFINES
#include "math.h"
const double RD = 180 / M_PI;
const double DR = M_PI / 180;

class Entity {
public:
	Entity(b2Body * body);

	virtual void setPosition(b2Vec2 position);
	virtual void setRotation(float amount);

	b2Vec2 getPosition() const;
	float getRotation() const;

	b2Body * getBody();

	bool getAlive() const;
	bool getActive() const;
	void setAlive(bool a);
	void setActive(bool a);

	virtual void kill();

	virtual bool collide(Entity* other, b2Contact& contact);
	
	//virtual void draw(GameDrawer d) = 0;

	/*b2Shape& getVertices();*/

protected:
	b2Body * body_;

	bool alive_;
	bool active_;
private:
};

//Entities know about children for collision handling
//Ask Alex how to do this better

#endif