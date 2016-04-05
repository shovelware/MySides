#include "Weapon.hpp"

Weapon::WeaponI::WeaponI(FireFunc& callback, ProjectileDef const &ammo, std::string id) :
	owner_(nullptr),
	fireCallback_(callback),
	output_(ProjectileDef(ammo)),
	id_(id),
	pin_(false),
	barrel_(b2Vec2_zero)
	{

	}

	void Weapon::WeaponI::trigger(b2Vec2 & direction)
	{
		if (canTrigger())
		{
			//fire(direction);
			barrel_ = direction;
			pin_ = true;
		}
	}

	void Weapon::WeaponI::release()
	{
		pin_ = false;
	}

	void Weapon::WeaponI::setProjectile(ProjectileDef const &pd)
	{
		ProjectileDef output = ProjectileDef(pd);
		output.colPrim = output_.colPrim;
		output.colSecn = output_.colSecn;
		output.colTert = output_.colTert;
		output_ = output;
	}

	void Weapon::WeaponI::setLevel(int level)
	{
		level_ = level;
	}

	int Weapon::WeaponI::getLevel() const
	{
		return level_;
	}

	void Weapon::WeaponI::setOwner(Shape* owner)
	{
		owner_ = owner;
	}

	Shape* Weapon::WeaponI::getOwner() const { return owner_; }

	b2Color Weapon::WeaponI::getPrimary() const { return output_.colPrim; }
	b2Color Weapon::WeaponI::getSecondary() const { return output_.colSecn; }
	b2Color Weapon::WeaponI::getTertiary() const { return output_.colTert; }

	void Weapon::WeaponI::setPrimary(b2Color col) { output_.colPrim = col; }
	void Weapon::WeaponI::setSecondary(b2Color col) { output_.colSecn = col; }
	void Weapon::WeaponI::setTertiary(b2Color col) { output_.colTert = col; }

	void Weapon::WeaponI::setID(std::string id)	{ id_ = id;	}

	std::string Weapon::WeaponI::getID() { return id_; }