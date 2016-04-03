#include "Force.hpp"

Force::Force(b2Body * body, float force, float radius, int lifeTime) :
	Entity(body),
	force_(force),
	lifeTime_(lifeTime)
{
	createBody(radius);
}

void Force::createBody(float radius)
{
	b2FixtureDef def;
	def.isSensor = true;
	def.userData = "force";
	
	b2CircleShape circle;
	circle.m_radius = radius;

	def.shape = &circle;

	body_->CreateFixture(&def);
}

void Force::update(int milliseconds)
{
	lifeTime_ -= milliseconds;

	if (lifeTime_ <= 0)
	{
		active_ = false;
		alive_ = false;
	}
}

bool Force::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = true;

	if (tag == "enemy"  || tag == "player" || tag == "pickup" || tag == "side")
	{
		b2Vec2 dir = other->getPosition() - body_->GetPosition();
		dir.Normalize();
		dir *= 10.f * force_ ;

		other->getBody()->ApplyForceToCenter(dir, true);
	}

	return handled;
}