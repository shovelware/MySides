#include "Attractor.hpp"
#include "Projectile.hpp"
#include "Bounds.hpp"
#include "Pickup.hpp"

Pickup::Attractor::Attractor(b2Body* body, const PickupDef& def) :
	Pickup::PickupI(body, def.time),
	radius_(def.size),
	strength_(def.strength)
{
	//Body is initially made by pickup base class
}

void Pickup::Attractor::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());

	b2FixtureDef def;
	def.userData = "pickup";

	def.density = 0.f;
	def.friction = 0.f;
	def.restitution = 0.f;
	def.isSensor = true;

	b2CircleShape shape;
	shape.m_radius = radius_;

	def.shape = &shape;

	body_->CreateFixture(&def);

	collected_ = true;
}

bool Pickup::Attractor::collide(Entity* other, b2Contact& contact)
{

	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		if (!collected_)
		{
			setOwner(shape);
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

	else if (Pickup::PickupI* pickup = dynamic_cast<Pickup::PickupI*>(other))
	{
		handled = true;
	}

	else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	{
		handled = true;
	}

	return handled;
}

void Pickup::Attractor::update(int milliseconds)
{
	Pickup::PickupI::update(milliseconds);

	//Do our collection stuff outside b2 step
	if (owner_ != nullptr && collected_ == false)
	{
		onCollect();
	}

	if (collected_ == true)
	{
		if (owner_ != nullptr)
		{
			setPosition(owner_->getPosition());

			for (b2ContactEdge* ed = body_->GetContactList(); ed != nullptr; ed = ed->next)
			{
				Side* s = nullptr;

				if (ed->contact->GetFixtureA()->GetUserData() == "side")
					s = static_cast<Side*>(ed->contact->GetFixtureA()->GetBody()->GetUserData());


				else if (ed->contact->GetFixtureB()->GetUserData() == "side")
					s = static_cast<Side*>(ed->contact->GetFixtureB()->GetBody()->GetUserData());

				if (s != nullptr)
					s->attract(owner_->getPosition() - s->getPosition());
			}
		}

		else time_ == 0;
	}

	//Deletion flag
	if (time_ == 0)
	{
		owner_ = nullptr;
	}
}

float Pickup::Attractor::getRadius()
{
	return radius_;
}
