#include "Sight.hpp"
#include "Projectile.hpp"
#include "Side.hpp"
#include "Bounds.hpp"
#include "Pickup.hpp"

Pickup::Sight::Sight(b2Body* body, int time) :
	Pickup::PickupI(body, time),
	size_(10)
{
	//Body is initially made by pickup base class
}

void Pickup::Sight::onCollect()
{
	body_->DestroyFixture(body_->GetFixtureList());

	collected_ = true;
}

bool Pickup::Sight::collide(Entity* other, b2Contact& contact, std::string tag)
{
	bool handled = false;

	if (tag == "shape")
	{
		Shape* shape = dynamic_cast<Shape*>(other);

		if (!collected_)
		{
			setOwner(shape);
		}

		else
		{
			if (shape != owner_)
			{
				//We've hit an enemy
				//There's no body here for this though
				//hyuk
			}
		}

		handled = true;
	}

	return handled;
}

void Pickup::Sight::update(int milliseconds)
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

			float angle = owner_->getBody()->GetAngle();

			end_ = body_->GetPosition() + b2Vec2(sin(angle) * -size_, -cos(angle) * -size_);
		}

		else
		{
			time_ == 0;
		}
	}

	//Deletion flag
	if (time_ == 0)
	{
		owner_ = nullptr;
	}

}

b2Vec2 Pickup::Sight::getEnd()
{
	return end_;
}
