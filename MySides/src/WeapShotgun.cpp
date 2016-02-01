#include "WeapShotgun.hpp"

Weapon::Shotgun::Shotgun(Shape* owner, std::function<void(ProjectileDef&)>& callback, ProjectileDef const &ammo) :
	WeaponI(owner, callback, ammo)
{
	refireTimeMax_ = 600;
	refireTime_ = 0;
}

void Weapon::Shotgun::update(int dt)
{
	// Else if we're cycling, time down
	if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}
}

void Weapon::Shotgun::fire(b2Vec2 &heading)
{
	//Set up projectile
	ProjectileDef newProj = ProjectileDef(output_);
	newProj.origin = owner_->getPosition();
	newProj.heading = heading;
	newProj.owner = owner_;

	//Fire centre
	fireCallback_(newProj);

	b2Vec2 newDir = heading;
	float rotation = atan2f(heading.y, heading.x);

	//Thor distributions

	////Calculate left
	newDir.x = cosf(rotation - 0.2f);
	newDir.y = sinf(rotation - 0.2f);

	newProj.heading = newDir;
	fireCallback_(newProj);

	newDir.x = cosf(rotation - 0.1f);
	newDir.y = sinf(rotation - 0.1f);

	newProj.heading = newDir;
	fireCallback_(newProj);

	////Calculate right
	newDir.x = cosf(rotation + 0.2f);
	newDir.y = sinf(rotation + 0.2f);

	newProj.heading = newDir;
	fireCallback_(newProj);


	newDir.x = cosf(rotation + 0.1f);
	newDir.y = sinf(rotation + 0.1f);

	newProj.heading = newDir;
	fireCallback_(newProj);

	//Abstract this into weapon later

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