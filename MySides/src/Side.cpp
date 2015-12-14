#include "Side.hpp"

Side::Side(b2Body * body, b2Vec2 normal, float size) : Entity(body), size_(size)
{
	//Create a shape, the outline
	b2EdgeShape line;
	
	line.Set(b2Vec2(0, -size / 2), b2Vec2(0, size / 2));

	//Create a fixture, the link for body -> shape
	b2FixtureDef lineDef;
	lineDef.shape = &line;

	//Add material properties to the fixture
	lineDef.density = 0.5f;
	lineDef.friction = 0.0f;
	lineDef.restitution = 0.0f;

	//Add userdata to fixture for contacts
	lineDef.userData = "side";

	lineDef.isSensor = true;

	body_->CreateFixture(&lineDef);

	b2PolygonShape box;
	
	box.SetAsBox(size * 0.05f, size * 0.55f);
	b2FixtureDef boxDef;
	boxDef.shape = &box;
	boxDef.userData = "sidebox";
	boxDef.isSensor = true;

	//Create and add fixture using body's factory
	body_->CreateFixture(&boxDef);

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

bool Side::collide(Entity* other, b2Contact& contact)
{
	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		//If we're a player
		//Or maybe other things can collect stuff?

		char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
		char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());

		if (tagA == "side" || tagB == "side")
		{
			collect();
		}
		handled = true;
	}

	else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	{
		handled = true;
	}

	else if (Side* side = dynamic_cast<Side*>(other))
	{
		handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		handled = true;
	}

	return handled;
}