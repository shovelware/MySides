// Bounds.hpp
// Level bounds in a concave circle approximation using edge chains

#ifndef MS_BOUNDS_HPP
#define MS_BOUNDS_HPP

#include <stdafx.h>
#include "Entity.hpp"

class Bounds : public Entity {
private:
	float radius_;
	b2FixtureDef fixtureDef_;
	b2Vec2 getCirclePoint(int index, int maxpoints);
	const int circlePoints = 32;

public:
	Bounds(b2Body* body, float radius);
	void fillChain(b2ChainShape& c, float radius, int points);
	void resize(float radius);
	float getRadius();
};
#endif