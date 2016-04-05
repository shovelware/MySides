#include "WeapSemiBat.hpp"

Weapon::SemiBat::SemiBat(FireFunc& callback, ProjectileDef const& ammo, std::string id) :
	WeaponI(callback, ammo, id),
	battery_(2000, false),
	chargeAmount_(16),
	dischargeAmount_(32),
	primed_(true)
{
}

Weapon::SemiBat::SemiBat(FireFunc& callback, ProjectileDef const& ammo, std::string id,
	int batterySize, int chargeAmount, int dischargeAmount) :
	WeaponI(callback, ammo, id),
	battery_(batterySize, false),
	chargeAmount_(dischargeAmount),
	dischargeAmount_(dischargeAmount),
	primed_(true)
{
}

void Weapon::SemiBat::reup(bool instant)
{
	//No reload
	if (instant && primed_ == false)
	{
		battery_.empty();
		primed_ = true;
	}

	else;
}

void Weapon::SemiBat::update(int ms)
{
	//Idle/charging up
	if (primed_)
	{
		if (pin_)
		{
			battery_.recharge(chargeAmount_);
		}

		else
		{
			battery_.discharge(dischargeAmount_);
		}

		if (battery_.getPercent() == 100)
		{
			fire(barrel_);
			pin_ = false;
			primed_ = false;
		}
	}

	//Cooldown
	else
	{
		if (pin_)
		{
			battery_.discharge((dischargeAmount_ / 4 > 0 ? dischargeAmount_ / 4 : 1));
			//Slow cooling
		}

		else
		{
			battery_.discharge(dischargeAmount_);
		}

		if (!pin_ && battery_.getPercent() == 0)
		{
			primed_ = true;
		}
	}
}

void Weapon::SemiBat::setBatterySize(int size, bool recharge)
{
	battery_.resize(size, recharge);
}

void Weapon::SemiBat::setChargeAmount(int amount)
{
	chargeAmount_ = (amount > 0 ? amount : chargeAmount_);
}

void Weapon::SemiBat::setDischargeAmount(int amount)
{
	dischargeAmount_ = (amount > 0 ? amount : dischargeAmount_);
}

int Weapon::SemiBat::getBar() const
{
	return battery_.getPercent();
}

int Weapon::SemiBat::getBarMAX() const
{
	return 100;
}

void Weapon::SemiBat::fire(b2Vec2 & heading)
{
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

bool Weapon::SemiBat::isUpping() const
{
	return (!primed_);
}

bool Weapon::SemiBat::canFire() const
{
	return (output_.isValid() && primed_);
}

bool Weapon::SemiBat::canTrigger() const
{
	bool ready = false;

	if (output_.isValid())
	{
		ready = true;
	}

	return ready;
}