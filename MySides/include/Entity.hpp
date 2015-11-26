// Entity.hpp
// Base entity class, includes movement functions
// Children: Shape, Projectile, Side, Obstacle

#ifndef MS_ENTITY_HPP
#define MS_ENTITY_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

//#include "GameDrawer.hpp"

//Using pi to convert deg <-> rad
#define _USE_MATH_DEFINES
#include "math.h"
const double RD = 180 / M_PI;
const double DR = M_PI / 180;

class Entity {
private:
protected:
	b2Body * body_;
	bool alive_;
	bool active_;
public:
	Entity(b2Body * body);

	virtual void move(b2Vec2) = 0;
	virtual void setPosition(b2Vec2 position);

	virtual void rotate(float) = 0;
	virtual void setRotation(float amount);

	bool getAlive() const;
	bool getActive() const;

	virtual void kill();
	void deactivate();

	b2Vec2 getPosition() const;
	float getRotation() const;
	b2Body * getBody();
	/*
	virtual void draw(GameDrawer d) = 0;

	b2Shape* getVertices();*/
};

#endif