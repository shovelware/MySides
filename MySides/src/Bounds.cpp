#include "Bounds.hpp"

Bounds::Bounds(b2Body* body, float radius) : Entity(body), radius_(radius)
{
	b2ChainShape shap;
	
	int maxPoints = 32;
	int i = 0;

	b2Vec2* verts = new b2Vec2[maxPoints];

	for (; i <= maxPoints; ++i)
	{
		verts[i] = radius_ * getCirclePoint(i, maxPoints);
	}

	shap.CreateLoop(verts, maxPoints);


	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shap;

	//Add material properties to the fixture
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.9f;
	fixtureDef.restitution = 0.f;

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);
}

//Gets radius of bounds
float Bounds::getRadius()
{
	return radius_;
}

//Though we are an entity, we don't move or rotate
void Bounds::move(b2Vec2) {}
void Bounds::rotate(float) {}

b2Vec2 Bounds::getCirclePoint(int index, int maxpoints)
{
	b2Vec2 point;

	point.x = (cos((2 * M_PI) / maxpoints * index));
	point.y = (sin((2 * M_PI) / maxpoints * index));

	return point;
}
