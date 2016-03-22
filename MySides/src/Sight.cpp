#include "Sight.hpp"

Pickup::Sight::Sight(b2Body* body, const PickupDef& def) :
	Pickup::PickupI(body, def.time),
	size_(def.size),
	strength_(def.strength)
{
	//Body is initially made by pickup
}

void Pickup::Sight::onCollect()
{
	//remake box2d
	collected_ = true;

}

bool Pickup::Sight::collide(Entity* other, b2Contact& contact)
{
	//if !collected_ && shape:  owner = shape, oncollect;
	bool handled = false;

	if (Shape* shape = dynamic_cast<Shape*>(other))
	{
		if (!collected_)
		{
			setOwner(shape);
			onCollect();
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
	if (collected_ == true)
	{
		setPosition(owner_->getPosition());
		end_ = b2Vec2(0, 1);// *owner_->getRotation();
	}

	Pickup::PickupI::update(milliseconds);
}

b2Vec2 Pickup::Sight::getEnd()
{
	return end_;
}
