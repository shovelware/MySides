#include "WeapAutoMag.hpp"

Weapon::AutoMag::AutoMag(fireFunc& callback, ProjectileDef const &ammo, std::string id) :
	WeaponI(callback, ammo, id),
	magazine_(30),
	refireTime_(0),
	refireTimeMAX_(100),
	reloadTime_(0),
	reloadTimeMAX_(5000),
	spread_(0.005f)
{
}

Weapon::AutoMag::AutoMag(fireFunc & callback, ProjectileDef const & ammo, std::string id,
	int magSize, int refireTime, int reloadTime, int spread) :
	WeaponI(callback, ammo, id),
	magazine_(magSize),
	refireTime_(0),
	refireTimeMAX_(refireTime),
	reloadTime_(0),
	reloadTimeMAX_(reloadTime),
	spread_(spread)
{
}

void Weapon::AutoMag::reup(bool instant)
{
	if (instant == false && reloadTime_ <= 0)
	{
		reloadTime_ = reloadTimeMAX_;
	}

	else
	{
		magazine_.reload();
		reloadTime_ = 0;
		refireTime_ = 0;
	}
}

void Weapon::AutoMag::update(int dt)
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

	// Else if we're cycling, time down
	else if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}

	//Fire if we can
	if (pin_)
	{
		fire(barrel_);

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
}

void Weapon::AutoMag::setMagSize(int size, bool reload)
{
	magazine_.resize(size, reload);
}

void Weapon::AutoMag::setRefireTime(int ms)
{
	refireTimeMAX_ = (ms > 0 ? ms : refireTimeMAX_);
}

void Weapon::AutoMag::setReloadTime(int ms)
{
	reloadTimeMAX_ = (ms > 0 ? ms : reloadTimeMAX_);
}

void Weapon::AutoMag::setSpread(float spread)
{
	spread_ = (spread >= 0 ? spread : spread_);
}

int Weapon::AutoMag::getBar() const { return magazine_.getCount(); }
int Weapon::AutoMag::getBarMAX() const { return magazine_.getCountMAX(); }

void Weapon::AutoMag::fire(b2Vec2 &heading)
{
	pin_ = false;

	//Set up vector
	std::vector<ProjectileDef> pv;
	pv.emplace_back(output_);
	std::vector<ProjectileDef>::iterator newProj = pv.begin();

	//Set up projectile
	newProj->origin = owner_->getPosition() + heading;


	float rotation = atan2f(heading.y, heading.x);
	float adjust = randFloat(-spread_, spread_);
	b2Vec2 newDir(cosf(rotation + adjust), sinf(rotation + adjust));

	newProj->heading = newDir;
	newProj->owner = owner_;

	//Fire projectile
	fireCallback_(pv, id_);
}

bool Weapon::AutoMag::isUpping() const
{
	return (reloadTime_ != 0);
}

bool Weapon::AutoMag::canFire() const
{
	return (output_.isValid() && reloadTime_ <= 0);
}

bool Weapon::AutoMag::canTrigger() const
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
