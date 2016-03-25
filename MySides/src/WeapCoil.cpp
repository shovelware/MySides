#include "WeapCoil.hpp"
			
Weapon::Coilgun::Coilgun(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	battery_(2000)
{
	refireTimeMAX_ = 100;
	refireTime_ = 0;

	fireCharge_ = 30;

	chargeTime_ = 0;
	chargeTimeMAX_ = 500;

	id_ = "coilgun";
}

void Weapon::Coilgun::update(int dt)
{
	//If we're shooting, refire cool
	if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}

	//Otherwise start charging
	else
	{
		if (chargeTime_ > 0)
		{
			chargeTime_ = (chargeTime_ - dt >= 0 ? chargeTime_ - dt : 0);
		}

		else battery_.recharge(fireCharge_ / 2);
	}
}

void Weapon::Coilgun::fire(b2Vec2 & heading)
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
	refireTime_ = refireTimeMAX_ * (2 - (battery_.getPercent() / 100.f)) ;
	chargeTime_ = chargeTimeMAX_;
	battery_.discharge(fireCharge_);
}

bool Weapon::Coilgun::canFire()
{
	bool ready = false;

	if (refireTime_ <= 0 && fireCharge_ <= battery_.getCharge())
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}
	return ready;
}