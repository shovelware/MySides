#include "WeapSpreadBat.hpp"

Weapon::SpreadBat::SpreadBat(fireFunc& callback, ProjectileDef const& ammo, std::string id) :
	WeaponI(callback, ammo, id),
	battery_(1600),
	refireTime_(0),
	refireTimeMAX_(50),
	rechargeTime_(0),
	rechargeTimeMAX_(2000),
	fireCharge_(16),
	amountMAX_(8),
	amountScale_(0.5f),
	spread_(1.f),
	spreadScale_(0.5f),
	cooled_(true)
{
}

Weapon::SpreadBat::SpreadBat(fireFunc & callback, ProjectileDef const & ammo, std::string id,
	int batterySize, int refireTime, int rechargeTime, int fireCharge, int amount, float amountScale, float spread, float spreadScale) :
	WeaponI(callback, ammo, id),
	battery_(batterySize),
	refireTime_(0),
	refireTimeMAX_(refireTime),
	rechargeTime_(0),
	rechargeTimeMAX_(rechargeTime),
	fireCharge_(fireCharge),
	amountMAX_(amount),
	amountScale_(amountScale),
	spread_(spread),
	spreadScale_(spreadScale),
	cooled_(true)

{
}

bool Weapon::SpreadBat::isUpping() const
{
	return (!cooled_);
}

bool Weapon::SpreadBat::canFire() const
{
	return (output_.isValid() && cooled_ && battery_.checkEmpty() == false);
}

bool Weapon::SpreadBat::canTrigger() const
{
	bool ready = false;

	//If we're not cycling AND we have ammo AND aren't reloading
	if (refireTime_ <= 0)
	{
		if (output_.isValid())
		{
			ready = true;
		}
	}

	return ready;
}

void Weapon::SpreadBat::reup(bool instant)
{
	if (instant) {
		battery_.empty();
		rechargeTime_ = 0;
		cooled_ = true;
	}
	
	//Manually restart charging cycle
	else if (cooled_ == true)
	{
		cooled_ = false;
	}

}

void Weapon::SpreadBat::update(int ms)
{
	//If we haven't overheated
	if (cooled_)
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

			else battery_.recharge(fireCharge_ / 8);//adjust falloff here
		}

		//Fire if we can
		if (pin_)
		{
			fire(barrel_);

			//Reactions:
			pin_ = false;
			refireTime_ = refireTimeMAX_;
			battery_.discharge(fireCharge_);

			if (battery_.checkEmpty() == true)
			{
				rechargeTime_ = rechargeTimeMAX_;
				refireTime_ = 0;
				cooled_ = false;
			}
		}

	}

	//if we're not cooled
	else
	{
		if (rechargeTime_ > 0)
		{
			rechargeTime_ = (rechargeTime_ - ms >= 0 ? rechargeTime_ - ms : 0);
		}

		else battery_.recharge(fireCharge_);

		if (battery_.checkFull() == true)
		{
			cooled_ = true;
		}
	}

}

void Weapon::SpreadBat::setBatterySize(int size, bool recharge)
{
	battery_.resize(size, recharge);
}

void Weapon::SpreadBat::setRefireTime(int ms)
{
	refireTimeMAX_ = (ms >= 0 ? ms : refireTimeMAX_);
}

void Weapon::SpreadBat::setRechargeTime(int ms)
{
	rechargeTimeMAX_ = (ms >= 0 ? ms : rechargeTimeMAX_);
}

void Weapon::SpreadBat::setFireCharge(int charge)
{
	fireCharge_ = (charge >= 0 ? charge : fireCharge_);
}

void Weapon::SpreadBat::setAmount(int amount)
{
	amountMAX_ = (amount > 0 ? amount : amountMAX_);
}

void Weapon::SpreadBat::setAmountScale(float amountScale)
{
	amountScale_ = amountScale;
}

void Weapon::SpreadBat::setSpread(float spread)
{
	spread_ = (spread >= 0 ? spread : spread_);
}

void Weapon::SpreadBat::setSpreadScale(float spreadScale)
{
	spreadScale_ = spreadScale;
}

int Weapon::SpreadBat::getBar() const
{
	return 100 - battery_.getPercent();
}

int Weapon::SpreadBat::getBarMAX() const
{
	return 100;
}

void Weapon::SpreadBat::fire(b2Vec2 & heading)
{
	//Set up vector
	std::vector<ProjectileDef> pv;
	
	//Amount drains to amountMAX_ * amountScale_ over full charge cycle
	int amount = fmaxf(1.f,  amountMAX_ - amountMAX_ * (amountScale_ * (1.f - (battery_.getPercent() / 100.f))));

	pv.reserve(amount);
	std::vector<ProjectileDef>::iterator newProj = pv.end();
	b2Vec2 origin = owner_->getPosition();

	//Set up other projectiles
	b2Vec2 newOrig = origin;
	b2Vec2 newHead = heading;
	float rotation = atan2f(heading.y, heading.x);
	float adjust = 0.f;
	float spread = fmaxf(0.1f, spread_ - (spread_ * spreadScale_ * (1.f - (battery_.getPercent() / 100.f))));

	for (int i = 0; i < amount; ++i)
	{
		pv.emplace_back(output_);
		newProj = --pv.end();
	
		//New origin
		adjust = randFloat(-spread, spread);
		newOrig.x = cosf(rotation + adjust);
		newOrig.y = sinf(rotation + adjust);

		//New heading
		adjust = randFloat(-spread / 2.f, spread / 2.f);
		newHead.x = cosf(rotation + adjust);
		newHead.y = sinf(rotation + adjust);
	
		newProj->origin = origin + (newOrig - heading);
		newProj->heading = newHead;
		newProj->owner = owner_;
		newProj->inVelocity = owner_->getBody()->GetLinearVelocity();
	}

	//Fire projectiles
	fireCallback_(pv, id_);
}
