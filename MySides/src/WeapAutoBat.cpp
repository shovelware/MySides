#include "WeapAutoBat.hpp"
			
Weapon::AutoBat::AutoBat(fireFunc& callback, ProjectileDef const &ammo) :
	WeaponI(callback, ammo),
	battery_(2000),
	refireTime_(0),
	refireTimeMAX_(100),
	rechargeTime_(0),
	rechargeTimeMAX_(500)
{

	fireCharge_ = 30;

	id_ = "coilgun";
}

Weapon::AutoBat::AutoBat(fireFunc & callback, ProjectileDef const & ammo, 
	int batterySize, int refireTime, int rechargeTime, int fireCharge) :
	WeaponI(callback, ammo),
	battery_(batterySize),
	refireTime_(0),
	refireTimeMAX_(refireTime),
	rechargeTime_(0),
	rechargeTimeMAX_(rechargeTime),
	fireCharge_(fireCharge)
{
	id_ = "coilgun";
}

void Weapon::AutoBat::reup()
{
	//No manual reload
}

void Weapon::AutoBat::update(int dt)
{
	//If we're shooting, cycle
	if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - dt >= 0 ? refireTime_ - dt : 0);
	}

	//Otherwise start charging
	else
	{
		if (rechargeTime_ > 0)
		{
			rechargeTime_ = (rechargeTime_ - dt >= 0 ? rechargeTime_ - dt : 0);
		}

		else battery_.recharge(fireCharge_ / 2);
	}

	//Fire if we can
	if (pin_)
	{
		fire(barrel_);

		//Reactions:
		pin_ = false;
		refireTime_ = refireTimeMAX_ * (2 - (battery_.getPercent() / 100.f));
		rechargeTime_ = rechargeTimeMAX_;
		battery_.discharge(fireCharge_);
	}
}

void Weapon::AutoBat::setBatterySize(int size, bool reload)
{
	battery_.resize(size, reload);
}

void Weapon::AutoBat::setRefireTime(int ms)
{
	refireTimeMAX_ = (ms > 0 ? ms : refireTimeMAX_);
}

void Weapon::AutoBat::setRechargeTime(int ms)
{
	rechargeTimeMAX_ = (ms > 0 ? ms : rechargeTimeMAX_);
}

void Weapon::AutoBat::setFireCharge(int charge)
{
	fireCharge_ = (charge >= 0 ? charge : fireCharge_);
}

int Weapon::AutoBat::getBar() const { return battery_.getCharge(); }
int Weapon::AutoBat::getBarMAX() const { return battery_.getChargeMAX(); }

void Weapon::AutoBat::fire(b2Vec2 & heading)
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
}

bool Weapon::AutoBat::canTrigger() const
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