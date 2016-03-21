#include "Weapon.hpp"

Weapon::WeaponI::WeaponI(Shape* owner, std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo) :
		owner_(owner),
		fireCallback_(callback),
		output_(ProjectileDef(ammo)),
		id_("weapon")
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


	b2Color Weapon::WeaponI::getPrimary() const { return output_.colPrim; }
	b2Color Weapon::WeaponI::getSecondary() const { return output_.colSecn; }
	b2Color Weapon::WeaponI::getTertiary() const { return output_.colTert; }

	void Weapon::WeaponI::setPrimary(b2Color col) { output_.colPrim = col; }
	void Weapon::WeaponI::setSecondary(b2Color col) { output_.colSecn = col; }
	void Weapon::WeaponI::setTertiary(b2Color col) { output_.colTert = col; }

	std::string Weapon::WeaponI::getID() { return id_; }