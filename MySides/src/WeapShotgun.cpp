#include "WeapShotgun.hpp"

Weapon::Shotgun::Shotgun(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo)
{
	refireTimeMax_ = 600;
	refireTime_ = 0;

	pellets_ = 8;
	id_ = "shotgun";
}

void Weapon::Shotgun::update(int dt)
{
	// Else if we're cycling, time down
	if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
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

		adjust = randFloat(-0.15f, 0.15f);
		newDir.x = cosf(rotation +adjust);
		newDir.y = sinf(rotation +adjust);

		newProj->origin = origin + newDir;
		newProj->heading = newDir;
		newProj->owner = owner_;
	}

	fireCallback_(pv, id_);

	refireTime_ = refireTimeMax_;
}

bool Weapon::Shotgun::canFire()
{
	bool ready = false;

	if (refireTime_ <= 0)
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}

	return ready;
}