#include "WeapSpreadMag.hpp"

Weapon::SpreadMag::SpreadMag(FireFunc& callback, ProjectileDef const &ammo, std::string id) :
	WeaponI(callback, ammo, id),
	magazine_(8),
	resetTime_(0),
	resetTimeMAX_(300),
	reloadTime_(0),
	reloadTimeMAX_(1250),
	pellets_(8),
	spread_(0.15f),
	pumped_(false)
{
}

Weapon::SpreadMag::SpreadMag(FireFunc & callback, ProjectileDef const & ammo, std::string id,
	int magSize, int resetTime, int reloadTime, int pellets, float spread) :
	WeaponI(callback, ammo, id),
	magazine_(magSize),
	resetTime_(0),
	resetTimeMAX_(resetTime),
	reloadTime_(0),
	reloadTimeMAX_(reloadTime),
	pellets_(pellets),
	spread_(spread),
	pumped_(false)
{
}

void Weapon::SpreadMag::reup(bool instant)
{
	if (instant)
	{
		magazine_.reload();
		reloadTime_ = 0;
		resetTime_ = 0;
		pumped_ = true;
	}

	else if (reloadTime_ <= 0)
	{
		reloadTime_ = reloadTimeMAX_;
	}
}


void Weapon::SpreadMag::update(int dt)
{
	//If we're reloading, keep doing it
	if (reloadTime_ > 0)
	{
		reloadTime_ = (reloadTime_ - dt >= 0 ? reloadTime_ - dt : 0);

		if (reloadTime_ <= 0)
		{
			magazine_.add();

			if (!magazine_.checkFull())
			{
				//Subsequent reloads take half the time
				reloadTime_ = reloadTimeMAX_ / 2;
			}
		}
	}

	//If we're cycling, time down
	if (!pin_ && resetTime_ > 0)
	{
		resetTime_ = (resetTime_ - dt >= 0 ? resetTime_ - dt : 0);
	}

	if (resetTime_ == 0)
	{
		pumped_ = true;
	}

	//Fire if we can
	if (pin_ && pumped_)
	{
		fire(barrel_);

		//Reactions
		pin_ = false;
		pumped_ = false;
		magazine_.remove();
		resetTime_ = resetTimeMAX_;
		reloadTime_ = 0;

		if (magazine_.checkEmpty())
		{
			reloadTime_ = reloadTimeMAX_;
		}
	}
}

void Weapon::SpreadMag::setMagSize(int size, bool reload)
{
	magazine_.resize(size, reload);
}

void Weapon::SpreadMag::setResetTime(int ms)
{
	resetTimeMAX_ = (ms > 0 ? ms : resetTimeMAX_);
}

void Weapon::SpreadMag::setReloadTime(int ms)
{
	reloadTimeMAX_ = (ms > 0 ? ms : reloadTimeMAX_);
}

void Weapon::SpreadMag::setPellets(int pellets)
{
	pellets_ = (pellets > 0 ? pellets : pellets_);
}

void Weapon::SpreadMag::setSpread(float spread)
{
	spread_ = (spread >= 0 ? spread : spread_);
}

int Weapon::SpreadMag::getBar() const { return magazine_.getCount(); }
int Weapon::SpreadMag::getBarMAX() const { return magazine_.getCountMAX(); }

void Weapon::SpreadMag::fire(b2Vec2 &heading)
{
	//Set up vector
	std::vector<ProjectileDef> pv;
	pv.reserve(pellets_);
	std::vector<ProjectileDef>::iterator newProj = pv.end();
	b2Vec2 origin = owner_->getPosition();
	
	//Set up other projectiles
	b2Vec2 newDir = heading;
	float rotation = atan2f(heading.y, heading.x);
	float adjust = 0.f;

	//Thor distributions later
	for (int i = 0; i < pellets_; ++i)
	{
		pv.emplace_back(output_);

		adjust = randFloat(-spread_, spread_);
		newDir.x = cosf(rotation +adjust);
		newDir.y = sinf(rotation +adjust);

		newProj = (--pv.end());
		newProj->origin = origin + newDir;
		newProj->heading = newDir;
		newProj->owner = owner_;
	}

	//Fire projectiles
	fireCallback_(pv, id_);
}

bool Weapon::SpreadMag::isUpping() const
{
	return (reloadTime_ != 0);
}

bool Weapon::SpreadMag::canFire() const
{
	return (output_.isValid() && pumped_ && magazine_.checkEmpty() == false);
}

bool Weapon::SpreadMag::canTrigger() const
{
	bool ready = false;

	if (!magazine_.checkEmpty())
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}

	return ready;
}