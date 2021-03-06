#include "Bounds.hpp"

Bounds::Bounds(b2Body* body, float radius) : 
	Entity(body), 
	radius_(radius),
	points_(32)
{
	//Add userdata to fixture for contacts
	boundsDef_.userData = "bounds";

	b2ChainShape chain;
	fillChain(chain, radius, points_);
	boundsDef_.shape = &chain;
	boundsDef_.isSensor = true;

	//Add material properties to the fixture
	boundsDef_.density = 0.0f;
	boundsDef_.friction = 0.5f;
	boundsDef_.restitution = 1.0f;

	//Ground friction circle
	frictionDef_.userData = "friction";
	frictionDef_.isSensor = true;

	b2CircleShape circ;
	circ.m_radius = radius;
	frictionDef_.shape = &circ;

	colPrim_ = b2Color(0.f, 0.f, 0.5f);
	colSecn_ = b2Color(.4f, .4f, .4f);
	colTert_ = b2Color(0.7f, 0.5f, 0.f);

	//Create and add fixture using body's factory
	body_->CreateFixture(&boundsDef_);
	body_->CreateFixture(&frictionDef_);
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

void Bounds::resize(float radius, int points)
{
	//Set new radius
	if (radius_ != radius || points_ != points)
	{
		radius_ = radius;
		points_ = points;

		//Clear the fixture list
		for (b2Fixture* fix = body_->GetFixtureList(); fix; fix = body_->GetFixtureList())
		{
			body_->DestroyFixture(fix);
		}

		//Remake the chain and replace in fixture definition
		b2ChainShape chain;
		fillChain(chain, radius_, points);
		boundsDef_.shape = &chain;

		//Resize circle and replace in fixture definition
		b2CircleShape circ;
		circ.m_radius = radius_;
		frictionDef_.shape = &circ;

		//Recreate fixtures using body's factory
		body_->CreateFixture(&boundsDef_);
		body_->CreateFixture(&frictionDef_);
	}
}

//Returns the friction circle rather than bounds for testPoint
b2CircleShape* Bounds::getCircleShape() const
{
	for (b2Fixture* fix = body_->GetFixtureList(); fix; fix = fix->GetNext())
	{
		if (fix->GetType() == b2Shape::Type::e_circle)
		{
			return static_cast<b2CircleShape*>(fix->GetShape());
		}
	}

	return nullptr;
}

b2ChainShape* Bounds::getPoly() const
{
	for (b2Fixture* fix = body_->GetFixtureList(); fix; fix = fix->GetNext())
	{
		if (fix->GetType() == b2Shape::Type::e_chain)
		{
			return static_cast<b2ChainShape*>(fix->GetShape());
		}
	}

	return nullptr;
}

//Gets radius of bounds
float Bounds::getRadius() const
{
	return radius_;
}

float Bounds::getSideLength() const
{
	//a^2 = b^2 + c^2 -2bc cosA
	float rhs = (radius_ * radius_) + (radius_ * radius_) - (2 * radius_ * radius_) * cos(2 * M_PI / points_);

	return sqrt(rhs);
}

bool Bounds::collide(Entity * other, b2Contact& contact, std::string tag)
{
	return true;
}

//Returns a point number index on a circle divided into maxpoints
//Begins at far left and runs cw, how to get to top and CCW?
b2Vec2 Bounds::getCirclePoint(int index, int maxpoints)
{
	b2Vec2 point;

	point.y = -(cos((2 * M_PI) / maxpoints * index));
	point.x = -(sin((2 * M_PI) / maxpoints * index));

	return point;
}

