#include "WeapSpreadMag.hpp"

Weapon::SpreadMag::SpreadMag(fireFunc& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	magazine_(8),
	resetTime_(0),
	reloadTime_(0),
	cocked_(false)
{
	resetTimeMAX_ = 300;

	reloadTimeMAX_ = 1250;

	pellets_ = 8;
	spread_ = 0.15f;

	id_ = "shotgun";
}

Weapon::SpreadMag::SpreadMag(fireFunc & callback, ProjectileDef const & ammo,
	int magSize, int resetTime, int reloadTime, int pellets, float spread) :
	WeaponI(callback, ammo),
	magazine_(magSize),
	resetTime_(0),
	resetTimeMAX_(resetTime),
	reloadTime_(0),
	reloadTimeMAX_(reloadTime),
	pellets_(pellets),
	spread_(spread),
	cocked_(false)
{
}

void Weapon::SpreadMag::reup()
{
	if (reloadTime_ <= 0)
	{
		reloadTime_ = reloadTimeMAX_;
	}
}

void Weapon::SpreadMag::update(int dt)
{
	//Else If we're not cycling, reload
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
	pv.emplace_back(output_);
	std::vector<ProjectileDef>::iterator newProj = pv.begin();
	b2Vec2 origin = owner_->getPosition();

	//Set up projectile (centre)
	newProj->origin = origin + heading;
	newProj->heading = heading;
	newProj->owner = owner_;
	
	//Set up other projectiles
	b2Vec2 newDir = heading;
	float rotation = atan2f(heading.y, heading.x);
	float adjust = 0.f;

	//Thor distributions later
	for (int i = 1; i < pellets_; ++i)
	{
		pv.emplace_back(output_);
		newProj = --pv.end();

		adjust = randFloat(-spread_, spread_);
		newDir.x = cosf(rotation +adjust);
		newDir.y = sinf(rotation +adjust);

		newProj->origin = origin + newDir;
		newProj->heading = newDir;
		newProj->owner = owner_;
	}

	//Fire projectiles
	fireCallback_(pv, id_);
	
	//Reactions
	magazine_.remove();
	cocked_ = false;
	resetTime_ = resetTimeMAX_;
	reloadTime_ = 0;

	if (magazine_.checkEmpty())
	{
		cocked_ = true;
		resetTime_ = 0;
		reloadTime_ = reloadTimeMAX_;
	}
}

bool Weapon::SpreadMag::canFire() const
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