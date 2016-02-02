#include "Side.hpp"

#include "Bounds.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"

Side::Side(b2Body * body, b2Vec2 normal, float length) : Entity(body), length_(length)
{
	setShape(length);
}

Side::Side(b2Body * body, SideDef def) : Entity(body), length_(def.length)
{
	setShape(length_);

	//Align side to normal here--------------v
	body_->SetTransform(def.position, body_->GetAngle());

	//Shoot off, while spinning
	//body_->SetLinearVelocity(def.normal);
	body_->ApplyAngularImpulse(randFloat(0, 1), true);
}

void Side::setShape(float size)
{
	b2EdgeShape line;
	line.Set(b2Vec2(0, -size / 2), b2Vec2(0, size / 2));
	b2FixtureDef lineDef;
	lineDef.shape = &line;
	lineDef.userData = "side";
	lineDef.isSensor = true;

	body_->CreateFixture(&lineDef);

	//Border box, slightly bigger than side
	b2PolygonShape box;
	box.SetAsBox(size * 0.05f, size * 0.55f);
	b2FixtureDef boxDef;
	boxDef.shape = &box;
	boxDef.userData = "sidebox";
	boxDef.isSensor = true;

	//Create and add fixture using body's factory
	body_->CreateFixture(&boxDef);
}

void Side::collect()
{
	setActive(false);
}

float Side::getValue()
{
	return length_;
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

		//Make sure collision is with side
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