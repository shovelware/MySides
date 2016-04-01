#include "WeapSemiMag.hpp"

Weapon::SemiMag::SemiMag(fireFunc& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	magazine_(18),
	resetTime_(0),
	reloadTime_(0),
	cocked_(false)
{
	resetTimeMAX_ = 50;

	reloadTimeMAX_ = 2500;

	id_ = "pistol";
}

Weapon::SemiMag::SemiMag(fireFunc& callback, ProjectileDef const &ammo,
	int magSize, int resetTime, int reloadTime) :
	WeaponI(callback, ammo),
	magazine_(magSize),
	resetTime_(0),
	resetTimeMAX_(resetTime),
	reloadTime_(0),
	reloadTimeMAX_(reloadTime),
	cocked_(false)
{
	id_ = "pistol";
}

void Weapon::SemiMag::reup()
{
	if (reloadTime_ <= 0)
	{
		reloadTime_ = reloadTimeMAX_;
	}
}

void Weapon::SemiMag::update(int dt)
{
	//If we're reloading, time down
	if (reloadTime_ > 0)
	{
		reloadTime_ = (reloadTime_ - dt >= 0 ? reloadTime_ - dt : 0);

		if (reloadTime_ <= 0)
		{
			magazine_.reload();
		}
	}

	// If we're cycling, time down
	if (!pin_ && resetTime_ > 0)
	{
		resetTime_ = (resetTime_ - dt >= 0 ? resetTime_ - dt : 0);
	}

	if (resetTime_ == 0)
	{
		cocked_ = true;
	}

	if (pin_ && cocked_)
	{
		fire(barrel_);
		pin_ = false;
	}
}

void Weapon::SemiMag::setMagSize(int size, bool reload)
{
	magazine_.resize(size, reload);
}

void Weapon::SemiMag::setResetTime(int ms)
{
	resetTimeMAX_ = (ms > 0 ? ms : resetTimeMAX_);
}

void Weapon::SemiMag::setReloadTime(int ms)
{
	reloadTimeMAX_ = (ms > 0 ? ms : reloadTimeMAX_);
}

int Weapon::SemiMag::getBar() const { return magazine_.getCount(); }
int Weapon::SemiMag::getBarMAX() const { return magazine_.getCountMAX(); }

void Weapon::SemiMag::fire(b2Vec2 &heading)
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
	resetTime_ = resetTimeMAX_;

	//If we're empty, reload
	if (magazine_.checkEmpty())
	{
		cocked_ = true;
		resetTime_ = 0;
		reloadTime_ = reloadTimeMAX_;
	}
}

bool Weapon::SemiMag::canFire() const
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