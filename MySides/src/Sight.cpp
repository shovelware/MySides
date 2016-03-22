#include "Sight.hpp"

Pickup::Sight::Sight(b2Body* body, const PickupDef& def) :
	Pickup::PickupI(body, def.time),
	size_(def.size),
	strength_(def.strength)
{
	//Body is initially made by pickup base class
}

void Pickup::Sight::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());

	collected_ = true;
}

bool Pickup::Sight::collide(Entity* other, b2Contact& contact)
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

	// else if (Projectile* proj = dynamic_cast<Projectile*>(other))
	//{
	//	if (proj->getOwner() != this)
	//	{
	//		takeDamage(proj->getDamage(), proj->getDirection());
	//	}
	//
	//	else contact.SetEnabled(false);
	//
	//	handled = true;
	//}
	//
	//else if (Side* side = dynamic_cast<Side*>(other))
	//{
	//	//char* tagA = static_cast<char*>(contact.GetFixtureA()->GetUserData());
	//	//char* tagB = static_cast<char*>(contact.GetFixtureB()->GetUserData());
	//	//
	//	//if (tagA == "side" || tagB == "side")
	//	//{
	//	//	collect(side->getValue());
	//	//}
	//
	//	handled = true;
	//}
	//
	//else if (Bounds* bounds = dynamic_cast<Bounds*>(other))
	//{
	//	handled = true;
	//}
	//
	return handled;
}

void Pickup::Sight::update(int milliseconds)
{
	//Do our collection stuff outside b2 step
	if (owner_ != nullptr && collected_ == false)
	{
		onCollect();
	}

	if (collected_ == true)
	{
		setPosition(owner_->getPosition());

		float angle = owner_->getBody()->GetAngle();

		end_ = body_->GetPosition() + b2Vec2(sin(angle) * -size_, -cos(angle) * -size_);
	}

	Pickup::PickupI::update(milliseconds);
}

b2Vec2 Pickup::Sight::getEnd()
{
	return end_;
}
