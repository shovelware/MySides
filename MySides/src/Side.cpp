#include "Side.hpp"

Side::Side(b2Body * body, const SideDef& def) : 
	Entity(body), 
	length_(def.length),
	heading_(def.normal)
{
	setShape(length_);

	float desiredAngle = atan2f(-def.normal.x, def.normal.y);
	body_->SetTransform(def.position, desiredAngle);

	//Shoot off, while spinning
	b2Vec2 impulse = heading_;

	float rotation = atan2f(heading_.y, heading_.x);
	float adjust = randFloat(-0.2f, 0.2f);
	impulse.x = cosf(rotation + adjust);
	impulse.y = sinf(rotation + adjust);

	impulse.Normalize();
	impulse *= 0.03f;

	body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);

	bool dir = coinFlip();
	body_->SetAngularVelocity(randFloat(0.035f, 0.037f) * (dir ? 1 : -1));
	body_->SetAngularDamping(randFloat(0.0001f, 0.0003f));

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
	return std::ceil(length_); //Round up as some shapes can drop sides of <1 length
}

b2Vec2 Side::getHeading()
{
	return heading_;
}

bool Side::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (tag == "projectile")
	{
		body_->ApplyAngularImpulse(other->getBody()->GetLinearVelocity().Length(), true);
	}

	return handled;
}