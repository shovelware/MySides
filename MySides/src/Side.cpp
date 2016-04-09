#include "Side.hpp"

Side::Side(b2Body * body, const SideDef& def) : 
	Entity(body), 
	heading_(def.normal),
	length_(def.length),
	lifeTimeMAX_(def.lifeTime),
	lifeTime_(def.lifeTime)

{
	setShape(length_);

	float desiredAngle = atan2f(-def.normal.x, def.normal.y);
	body_->SetTransform(def.position, desiredAngle);

	//Shoot off, while spinning
	b2Vec2 impulse = heading_;

	bool dir = coinFlip();
	//float rotation = atan2f(heading_.y, heading_.x);
	//float adjust = randFloat(-0.25f, 0.25f);
	//impulse.x = cosf(rotation + adjust);
	//impulse.y = sinf(rotation + adjust);

	impulse.Normalize();
	impulse *= 0.25f;

	float spin = randFloat(0.03f) * (dir ? 1 : -1);

	body_->SetLinearDamping(0.02f);
	body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);

	body_->SetAngularDamping(0.004f);
	body_->SetAngularVelocity(spin);
	//std::cout << spin << "||" << body_->GetAngularVelocity() << std::endl;
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

	lineDef.density = 5.0f;
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

float Side::getTimer() const
{
	return (lifeTime_ *1.f / lifeTimeMAX_ * 1.f);
}

void Side::update(int milliseconds)
{
	lifeTime_ -= milliseconds;
	if (alive_)
	{
		if (lifeTime_ <= 0)
		{
			alive_ = false;
		}
	}

	else active_ = false;
}

bool Side::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (tag == "projectile")
	{
		//Spin when hit
		body_->ApplyAngularImpulse(500.f, true);
	}

	return handled;
}