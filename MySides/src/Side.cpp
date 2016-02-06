#include "Side.hpp"

#include "Bounds.hpp"
#include "Shape.hpp"
#include "Projectile.hpp"

Side::Side(b2Body * body, SideDef def) : 
	Entity(body), 
	length_(def.length),
	heading_(def.normal)
{
	setShape(length_);

	//Align side to normal here--------------v
	float bodyAngle = body_->GetAngle();
	float desiredAngle = atan2f(-def.normal.x, def.normal.y);

	body_->SetTransform(def.position, desiredAngle);

	//Shoot off, while spinning
	b2Vec2 impulse = heading_;
	impulse *= 0.085f;
	body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);
	body_->ApplyAngularImpulse(randFloat(-100, 100), true);

	//Color data
	colPrim_ = def.colPrim;
	colSecn_ = def.colSecn;
	colTert_ = def.colTert;
}

void Side::setShape(float size)
{
	b2EdgeShape line;
	line.Set(b2Vec2(-size / 2, 0), b2Vec2(size / 2, 0));
	b2FixtureDef lineDef;
	lineDef.shape = &line;
	lineDef.userData = "side";
	lineDef.isSensor = true;

	body_->CreateFixture(&lineDef);

	lineDef.density = 1.0f;
	lineDef.friction = 0.0f;
	lineDef.restitution = 1.0f;

	//Border box, slightly bigger than side
	b2PolygonShape box;
	box.SetAsBox(size * 0.55f, size * 0.05f);
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

b2Vec2 Side::getHeading()
{
	return heading_;
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