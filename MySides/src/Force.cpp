#include "Force.hpp"

Force::Force(b2Body * body, float force, float radius, int lifeTime, int faction) :
	Entity(body),
	force_(force),
	radius_(radius),
	lifeTimeMAX_(lifeTime),
	lifeTime_(lifeTime),
	shapes_(true),
	sides_(true),
	projectiles_(false),
	pickups_(false)
{
	faction_ = faction;
	createBody(radius_);
}

Force::Force(b2Body * body, const ForceDef& def) :
	Entity(body),
	force_(def.force),
	radius_(def.radius),
	lifeTimeMAX_(def.lifeTime),
	lifeTime_(def.lifeTime),
	shapes_(def.shapes),
	sides_(def.sides),
	projectiles_(def.projectiles),
	pickups_(def.pickups)
{
	faction_ = def.faction;
	colPrim_ = def.primary;
	colSecn_ = def.secondary;
	colTert_ = def.tertiary;
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

		if ((shapes_ && (tagA == "enemy" || tagA == "player" )) || 
			(pickups_ && tagA == "pickup") ||
			(sides_ && tagA == "side") ||
			(projectiles_ && tagA == "projectile"))
			e = static_cast<Entity*>(ed->contact->GetFixtureA()->GetBody()->GetUserData());


		else if ((shapes_ && (tagB == "enemy" || tagB == "player")) ||
				(pickups_ && tagB == "pickup") ||
				(sides_ && tagB == "side") ||
				(projectiles_ && tagB == "projectile"))
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