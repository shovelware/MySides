#include "WeapRifle.hpp"

Weapon::Rifle::Rifle(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	magazine_(30)
{
	refireTimeMAX_ = 100;
	refireTime_ = 0;

	reloadTimeMAX_ = 5000;
	reloadTime_ = 0;

	id_ = "rifle";
}

void Weapon::Rifle::reup()
{
	if (reloadTime_ <= 0)
	{
		reloadTime_ = reloadTimeMAX_;
	}
}

void Weapon::Rifle::update(int dt)
{
	if (pin_)
	{
		fire(barrel_);
	}

	//If we're reloading, time down
	if (reloadTime_ > 0)
	{
		reloadTime_ = (reloadTime_ - dt >= 0 ? reloadTime_ - dt : 0);

		if (reloadTime_ <= 0)
		{
			magazine_.reload();
		}
	}

	// Else if we're cycling, time down
	else if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}
}

void Weapon::Rifle::setRefireTime(int ms)
{
	refireTimeMAX_ = (ms > 0 ? ms : refireTimeMAX_);
}

void Weapon::Rifle::setReloadTime(int ms)
{
	reloadTimeMAX_ = (ms > 0 ? ms : reloadTimeMAX_);
}

void Weapon::Rifle::setMagSize(int size, bool reload)
{
	magazine_.resize(size, reload);
}

int Weapon::Rifle::getBar() const { return magazine_.getCount(); }
int Weapon::Rifle::getBarMAX() const { return magazine_.getCountMAX(); }

void Weapon::Rifle::fire(b2Vec2 &heading)
{
	pin_ = false;

	//Set up vector
	std::vector<ProjectileDef> pv;
	pv.emplace_back(output_);
	std::vector<ProjectileDef>::iterator newProj = pv.begin();

	//Set up projectile
	newProj->origin = owner_->getPosition() + heading;
	newProj->heading = heading;
	newProj->owner = owner_;

	//Fire projectile
	fireCallback_(pv, id_);
	
	//Reactions:
	magazine_.remove();
	refireTime_ = refireTimeMAX_;

	//If we're empty, reload
	if (magazine_.checkEmpty())
	{
		refireTime_ = 0;
		reloadTime_ = reloadTimeMAX_;
	}
}

bool Weapon::Rifle::canFire() const
{
	bool ready = false;

	//If we're not cycling AND we have ammo AND aren't reloading
	if (refireTime_ <= 0 && !magazine_.checkEmpty() && reloadTime_ <= 0)
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}

	return ready;
}
