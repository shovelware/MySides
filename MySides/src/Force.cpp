#include "Force.hpp"

Force::Force(b2Body * body, float force, float radius, int lifeTime) :
	Entity(body),
	force_(force),
	radius_(radius),
	lifeTimeMAX_(lifeTime),
	lifeTime_(lifeTime)
{
	createBody(radius_);
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

	for (b2ContactEdge* ed = body_->GetContactList(); ed != nullptr; ed = ed->next)
	{
		b2Body* e = nullptr;
		char* tagA;
		char* tagB;

		tagA = static_cast<char*>(ed->contact->GetFixtureA()->GetUserData());
		tagB = static_cast<char*>(ed->contact->GetFixtureB()->GetUserData());
		if ((tagA == "enemy" || tagA == "player" || tagA == "pickup" || tagA == "side"))
			e = ed->contact->GetFixtureA()->GetBody();


		else if ((tagB == "enemy" || tagB == "player" || tagB == "pickup" || tagB == "side"))
			e = ed->contact->GetFixtureB()->GetBody();

		if (e != nullptr)
		{
			b2Vec2 dir =  e->GetPosition()- body_->GetPosition();
			float dist = dir.Length();
			if (dist < radius_)
			{

				dir.Normalize();
				dir *= force_ * ((dist / radius_));
				e->ApplyForceToCenter(dir, true);
			}
		}
	}
}

float Force::getForce() const { return force_; }
float Force::getRadius() const { return radius_; }
float Force::getLifeTimePercent() const { return (lifeTime_  * 1.f/ lifeTimeMAX_ * 1.f) * 100.f; }

bool Force::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = true;

	if (tag == "enemy" || tag == "player" || tag == "pickup" || tag == "side")
	{
		b2Vec2 dir = other->getPosition() - body_->GetPosition();
		dir.Normalize();
		dir *= 10.f * force_;

		other->getBody()->ApplyForceToCenter(dir, true);
	}

	return handled;
}