#include "WeapRifle.hpp"

Rifle::Rifle(Shape* owner, std::function<void(ProjectileDef&)>& callback) : 
	Weapon(owner, callback),
	magazine_(30)
{
	refireTimeMAX_ = 300;
	refireTime_ = 0;

	reloadTimeMAX_ = 2000;
	reloadTime_ = 0;	
}

void Rifle::update(int dt)
{
	//If we're reloading, time down
	if (reloadTime_ >= 0)
	{
		reloadTime_ = (reloadTime_ - dt >= 0 ? reloadTime_ - dt : 0);
	}

	// Else if we're cycling, time down
	else if (refireTime_ >= 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}
}

void Rifle::fire(b2Vec2 &heading)
{
	//Set up projectile
	ProjectileDef newProj = ProjectileDef(output_);
	newProj.origin = owner_->getPosition();
	newProj.heading = heading;
	newProj.owner = owner_;

	//Fire projectile
	fireCallback_(newProj);

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

bool Rifle::canFire()
{
	bool ready = false;

	//If we're not cycling AND we have ammo OR aren't reloading
	if (refireTime_ <= 0 && !magazine_.checkEmpty() || reloadTime_ <= 0)
	{
		ready = true;
	}

	return ready;
}