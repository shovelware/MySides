#include "Weapon.hpp"

Weapon::Weapon(Entity* owner) : owner_(owner)
{
	output_ = ProjectileDef();
	output_.owner = owner;
}

void Weapon::trigger(b2Vec2 &heading)
{
	if (canFire())
	{
		fire(heading);
	}
}

void Weapon::setProjectile(ProjectileDef &pd)
{
	output_ = ProjectileDef(pd);
}

void Weapon::setOwner(Entity * owner)
{
	owner_ = owner;
}

bool Weapon::canFire()
{
	return (coolDown_ <= 0);
}

void Weapon::fire(b2Vec2 &heading)
{
	ProjectileDef newProj(output_);

	newProj.origin = owner_->getPosition();
	newProj.heading = heading;
	newProj.inVelocity =  b2Vec2_zero;

	newProj.owner = owner_;
	newProj.target = nullptr;

	//Fire bullet with callback here

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
