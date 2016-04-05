#include "WeapAutoBat.hpp"
			
Weapon::AutoBat::AutoBat(FireFunc& callback, ProjectileDef const &ammo, std::string id) :
	WeaponI(callback, ammo, id),
	battery_(1600),
	refireTime_(0),
	refireTimeMAX_(150),
	rechargeTime_(0),
	rechargeTimeMAX_(1000),
	fireCharge_(40)
{
}

Weapon::AutoBat::AutoBat(FireFunc & callback, ProjectileDef const & ammo, std::string id,
	int batterySize, int refireTime, int rechargeTime, int fireCharge) :
	WeaponI(callback, ammo, id),
	battery_(batterySize),
	refireTime_(0),
	refireTimeMAX_(refireTime),
	rechargeTime_(0),
	rechargeTimeMAX_(rechargeTime),
	fireCharge_(fireCharge)
{
}

void Weapon::AutoBat::reup(bool instant)
{
	if (instant)
	{
		battery_.refill();
		rechargeTime_ = 0;
		refireTime_ = 0;
	}

	//No manual reload
	else;
}

void Weapon::AutoBat::update(int ms)
{
	//If we're shooting, cycle
	if (refireTime_ > 0)
	{
		refireTime_ = (refireTime_ - ms >= 0 ? refireTime_ - ms : 0);
	}

	//Otherwise start charging
	else
	{
		if (rechargeTime_ > 0)
		{
			rechargeTime_ = (rechargeTime_ - ms >= 0 ? rechargeTime_ - ms : 0);
		}

		else battery_.recharge(fireCharge_ / 2);
	}

	//Fire if we can
	if (pin_ && fireCharge_ <= battery_.getCharge())
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
	newProj->inVelocity = owner_->getBody()->GetLinearVelocity();

	//Fire projectile
	fireCallback_(pv, id_);
}

bool Weapon::AutoBat::isUpping() const
{
	return (battery_.checkFull() == false && rechargeTime_ <=0);
}

bool Weapon::AutoBat::canFire() const
{
	return (output_.isValid() && battery_.getCharge() >= fireCharge_);
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