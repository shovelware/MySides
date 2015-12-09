#include "Side.hpp"

Side::Side(b2Body * body, b2Vec2 normal, float size) : Entity(body), size_(size)
{
	//Create a shape, the outline
	b2PolygonShape box;
	
	box.SetAsBox(size_, 0.01f);

	//Create a fixture, the link for body -> shape
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;

	//Add material properties to the fixture
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;

	//Add userdata to fixture for contacts
	fixtureDef.userData = "side";

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef);

	//End box2d setup
}

void Side::collect()
{
	setActive(false);
}

float Side::getValue()
{
	return size_;
}