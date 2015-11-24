// Bounds.hpp
// Level bounds in a concave circle approximation

#ifndef MS_BOUNDS_HPP
#define MS_BOUNDS_HPP

#include <stdafx.h>
#include "Entity.hpp"

class Bounds : public Entity {
private:
	void move(b2Vec2);
	void rotate(float);
	float radius_;
	
public:
	Bounds(b2Body* body, float radius);
	float getRadius();
};
#endif