// Bounds.hpp
// Level bounds in a concave circle approximation using edge chains

#ifndef MS_BOUNDS_HPP
#define MS_BOUNDS_HPP

#include "Entity.hpp"

class Bounds : public Entity {
public:
	Bounds(b2Body* body, float radius);
	void resize(float radius);
	b2CircleShape* getShape();
	float getRadius();
	float getSideLength();

	bool collide(Entity* other, bool& physicsCollision); //Override
private:
	float radius_;

	b2FixtureDef boundsDef_;
	b2FixtureDef frictionDef_;

	b2Vec2 getCirclePoint(int index, int maxpoints);
	const int circlePoints = 32;

	void fillChain(b2ChainShape& c, float radius, int points);
};
#endif