#include "Weapon.hpp"

Weapon::Weapon(Shape* owner, std::function<void(ProjectileDef&)>& callback) :
	owner_(owner),
	fireCallback_(callback) 
{

}

void Weapon::setProjectile(ProjectileDef const &pd)
{
	output_ = ProjectileDef(pd);
}

void Weapon::setOwner(Shape* owner)
{
	owner_ = owner;
}

void Weapon::trigger(b2Vec2 & heading)
{
	if (canFire())
	{
		fire(heading);
	}
}