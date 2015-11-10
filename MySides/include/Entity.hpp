// Entity.hpp
// Base entity class, includes movement functions
// Children: Projectile, Shape, Side, Obstacle

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <stdafx.h>
#include <Box2D\Box2D.h>

//Using pi to convert deg <-> rad
#define _USE_MATH_DEFINES
#include "math.h"
const double RD = 180 / M_PI;
const double DR = M_PI / 180;

class Entity {
private:

protected:
	b2Body * body_;
public:

	Entity(b2Body * body);

	virtual void move(b2Vec2) = 0;
	virtual void rotate(float) = 0;

	void setPosition(b2Vec2 position);
	void setRotation(float amount);
};

#endif