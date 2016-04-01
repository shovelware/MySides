#include "..\include\WeapSemiBat.hpp"

Weapon::SemiBat::SemiBat(fireFunc & callback, ProjectileDef const & ammo) :
	WeaponI(callback, ammo),
	battery_(2000),
	chargeAmount_(16),
	dischargeAmount_(32),
	primed_(true)
{
	id_ = "railgun";
	battery_.empty();
}

Weapon::SemiBat::SemiBat(fireFunc & callback, ProjectileDef const & ammo, 
	int batterySize, int chargeAmount, int dischargeAmount) :
	WeaponI(callback, ammo),
	battery_(batterySize),
	chargeAmount_(dischargeAmount),
	dischargeAmount_(dischargeAmount),
	primed_(true)
{
	id_ = "railgun";
	battery_.empty();
}

void Weapon::SemiBat::reup()
{
	//No reload
}

void Weapon::SemiBat::update(int dt)
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

void Weapon::SemiBat::setdisChargeAmount(int amount)
{
	dischargeAmount_ = (amount > 0 ? amount : dischargeAmount_);
}

int Weapon::SemiBat::getBar() const
{
	return battery_.getCharge();
}

int Weapon::SemiBat::getBarMAX() const
{
	return battery_.getChargeMAX();
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

	//Fire projectile
	fireCallback_(pv, id_);
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