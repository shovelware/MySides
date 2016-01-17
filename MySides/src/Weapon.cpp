#include "Weapon.hpp"

Weapon::Weapon(Entity* owner) : owner_(owner)
{
	output_ = ProjectileDef();
	output_.owner = owner;
}

void Weapon::trigger(b2Vec2 heading)
{
	if (canFire())
	{
		fire(heading);
	}
}

bool Weapon::pollFired(ProjectileDef & out)
{
	//If there's projectiles there, return true and it
	if (!fired_.empty())
	{
		out = fired_.front();
		fired_.pop();
		return true;
	}

	else return false;
}

void Weapon::clearFired()
{
	while (!fired_.empty())
	{
		fired_.pop();
	}
}

void Weapon::setProjectile(ProjectileDef pd)
{
	output_ = pd;
	output_.owner = owner_;
}

void Weapon::setOwner(Entity * owner)
{
	owner_ = owner;
	output_.owner = owner_;
}

bool Weapon::canFire()
{
	return (coolDown_ <= 0);
}

void Weapon::fire(b2Vec2 heading)
{
	ProjectileDef newProj(output_);

	newProj.origin = owner_->getPosition();
	newProj.heading = heading;
	newProj.owner = owner_;

	fired_.push(newProj);

	coolDown_ = refireTime_;
}

void Weapon::update(int milliseconds)
{
	//Cooldown
	if (coolDown_ > 0)
	{
		coolDown_ -= milliseconds;

		if (coolDown_ < 0)
		{
			coolDown_ = 0;
		}
	}
}
