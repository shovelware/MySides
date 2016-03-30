#include "WeapShotgun.hpp"

Weapon::Shotgun::Shotgun(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	magazine_(8)
{
	refireTimeMax_ = 600;
	refireTime_ = 0;

	reloadTimeMAX_ = 1000;
	reloadTime_ = 0;

	pellets_ = 8;
	spread_ = 0.15f;

	id_ = "shotgun";
}

void Weapon::Shotgun::update(int dt)
{
	if (pin_)
	{
		fire(barrel_);
		pin_ = false;
	}

	// Else if we're cycling, time down
	if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}

	//Else If we're not cycling, reload
	else if (reloadTime_ > 0)
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
}

void Weapon::Shotgun::setRefireTime(int ms)
{
	refireTimeMax_ = (ms > 0 ? ms : refireTimeMax_);
}

void Weapon::Shotgun::setReloadTime(int ms)
{
	reloadTimeMAX_ = (ms > 0 ? ms : reloadTimeMAX_);
}

void Weapon::Shotgun::setMagSize(int size, bool reload)
{
	magazine_.resize(size, reload);
}

void Weapon::Shotgun::setPellets(int pellets)
{
	pellets_ = (pellets > 0 ? pellets : pellets_);
}

void Weapon::Shotgun::setSpread(float spread)
{
	spread_ = (spread >= 0 ? spread : spread_);
}

float Weapon::Shotgun::getBar() const { return magazine_.getCount(); }
float Weapon::Shotgun::getBarMAX() const { return magazine_.getCountMAX(); }

void Weapon::Shotgun::fire(b2Vec2 &heading)
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
	refireTime_ = refireTimeMax_;
	reloadTime_ = reloadTimeMAX_;

	if (magazine_.checkEmpty())
	{
		refireTime_ = 0;
		reloadTime_ = reloadTimeMAX_;
	}
}

bool Weapon::Shotgun::canFire() const
{
	bool ready = false;

	if (!magazine_.checkEmpty() &&refireTime_ <= 0)
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}

	return ready;
}