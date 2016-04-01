// Bounds.hpp
// Level bounds in a concave circle approximation using edge chains

#ifndef MS_BOUNDS_HPP
#define MS_BOUNDS_HPP

#include "Entity.hpp"

#define _USE_MATH_DEFINES
#include "math.h"]

class Bounds : public Entity {
public:
	Bounds(b2Body* body, float radius);
	void resize(float radius, int points = circlePoints_);
	b2CircleShape* getCircleShape();
	b2ChainShape* getPoly();
	float getRadius();
	float getSideLength();

	bool collide(Entity* other, b2Contact& contact, std::string tag); //Override
private:
	float radius_;
	int points_;

	b2FixtureDef boundsDef_;
	b2FixtureDef frictionDef_;

	b2Vec2 getCirclePoint(int index, int maxpoints);
	static const int circlePoints_ = 32;

	void fillChain(b2ChainShape& c, float radius, int points);
};
#endif