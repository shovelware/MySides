#include "WeapPistol.hpp"

Weapon::Pistol::Pistol(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	magazine_(18),
	cocked_(false)
{
	idleTimeMAX_ = 50;
	idleTime_ = 0;

	reloadTimeMAX_ = 2500;
	reloadTime_ = 0;

	id_ = "pistol";
}

void Weapon::Pistol::update(int dt)
{
	if (pin_ && cocked_)
	{
		fire(barrel_);
		pin_ = false;
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
	else if (!pin_ && idleTime_ > 0)
	{
		idleTime_ = (idleTime_ - dt >= 0 ? idleTime_ - dt : 0);
	}

	if (idleTime_ == 0)
	{
		cocked_ = true;
	}
}

void Weapon::Pistol::fire(b2Vec2 &heading)
{
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
	cocked_ = false;
	idleTime_ = idleTimeMAX_;

	//If we're empty, reload
	if (magazine_.checkEmpty())
	{
		cocked_ = true;
		reloadTime_ = reloadTimeMAX_;
	}
}

bool Weapon::Pistol::canFire()
{
	bool ready = false;

	//If we're not cycling AND we have ammo AND aren't reloading
	if (!magazine_.checkEmpty() && reloadTime_ <= 0)
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}

	return ready;
}