#include "Weapon.hpp"
	Weapon::WeaponI::WeaponI(Shape* owner, std::function<void(ProjectileDef&)>& callback, ProjectileDef const &ammo) :
		owner_(owner),
		fireCallback_(callback),
		output_(ProjectileDef(ammo))
	{

	}

	void Weapon::WeaponI::setProjectile(ProjectileDef const &pd)
	{
		output_ = ProjectileDef(pd);
	}

	void Weapon::WeaponI::setOwner(Shape* owner)
	{
		owner_ = owner;
	}

	void Weapon::WeaponI::trigger(b2Vec2 & direction)
	{
		if (canFire())
		{
			fire(direction);
		}
	}