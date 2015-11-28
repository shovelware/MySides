#include "Bounds.hpp"

Bounds::Bounds(b2Body* body, float radius) : Entity(body), radius_(radius)
{
	b2ChainShape chain;
	fillChain(chain, radius, circlePoints);
	fixtureDef_.shape = &chain;

	//Add material properties to the fixture
	fixtureDef_.density = 1.0f;
	fixtureDef_.friction = 0.0f;
	fixtureDef_.restitution = 0.0f;

	//Add userdata to fixture for contacts
	fixtureDef_.userData = "bounds";

	//Create and add fixture using body's factory
	body_->CreateFixture(&fixtureDef_);
}

//Adds a generated shape to the passed chain, with radius and number of points
void Bounds::fillChain(b2ChainShape &c, float radius, int points)
{
	b2Vec2* verts = new b2Vec2[points];

	for (int i = 0; i < points; ++i)
	{
		verts[i] = radius * getCirclePoint(i, points);
	}

	c.CreateLoop(verts, points);

	delete[] verts;
}

void Bounds::resize(float radius)
{
	//Check if there's a fixture there already
	b2Fixture* fix = body_->GetFixtureList();
	if (fix != nullptr)
	{
		//if so, destroy it
		body_->DestroyFixture(fix);
	}

	//Remake the chain and replace in fixture definition
	b2ChainShape chain;
	fillChain(chain, radius, circlePoints);
	fixtureDef_.shape = &chain;

	//Rereate fixture using body's factory
	body_->CreateFixture(&fixtureDef_);
}

//Gets radius of bounds
float Bounds::getRadius()
{
	return radius_;
}

//Returns a point number index on a circle divided into maxpoints
//Begins at far left and runs cw, how to get to top and CCW?
b2Vec2 Bounds::getCirclePoint(int index, int maxpoints)
{
	b2Vec2 point;

	point.x = -(cos((2 * M_PI) / maxpoints * index));
	point.y = -(sin((2 * M_PI) / maxpoints * index));

	return point;
}
