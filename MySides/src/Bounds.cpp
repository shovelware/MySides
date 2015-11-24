#include "Bounds.hpp"

Bounds::Bounds(b2Body* body, float radius) : Entity(body)
{
	b2PolygonShape shap;
	shap.SetAsBox(10, 10);

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