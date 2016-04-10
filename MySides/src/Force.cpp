#include "Force.hpp"

Force::Force(b2Body * body, float force, float radius, int lifeTime, int faction) :
	Entity(body),
	force_(force),
	radius_(radius),
	lifeTimeMAX_(lifeTime),
	lifeTime_(lifeTime)
{
	faction_ = faction;
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
		Entity* e = nullptr;
		char* tagA;
		char* tagB;

		tagA = static_cast<char*>(ed->contact->GetFixtureA()->GetUserData());
		tagB = static_cast<char*>(ed->contact->GetFixtureB()->GetUserData());
		if ((tagA == "enemy" || tagA == "player" || tagA == "pickup" || tagA == "side"))
			e = static_cast<Entity*>(ed->contact->GetFixtureA()->GetBody()->GetUserData());


		else if ((tagB == "enemy" || tagB == "player" || tagB == "pickup" || tagB == "side"))
			e = static_cast<Entity*>(ed->contact->GetFixtureB()->GetBody()->GetUserData());

		if (e != nullptr)
		{
			if (e->getFaction() != faction_ && e->getFaction() != GOD)
			{
				b2Vec2 dir = e->getPosition() - body_->GetPosition();
				float dist = dir.Length();

				if (dist < radius_)
				{

					dir.Normalize();
					dir *= force_ * ((dist / radius_));
					e->getBody()->ApplyForceToCenter(dir, true);
				}

			}//end faction check
		}//end null check
	}//end edge loop
}

float Force::getForce() const { return force_; }
float Force::getRadius() const { return radius_; }
float Force::getLifeTimePercent() const { return (lifeTime_  * 1.f/ lifeTimeMAX_ * 1.f) * 100.f; }

bool Force::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = true;

	return handled;
}