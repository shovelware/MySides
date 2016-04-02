#ifndef MS_SPREADBAT_HPP
#define MS_SPREADBAT_HPP

#include "Weapon.hpp"
#include "Battery.hpp"

namespace Weapon {
	class SpreadBat : public WeaponI {
	public:
		SpreadBat(fireFunc& callback, ProjectileDef const &ammo, std::string id);
		SpreadBat(fireFunc& callback, ProjectileDef const &ammo, std::string id, int batterySize, int refireTime, int rechargeTime, int fireCharge, int amount, float amountScale, float spread, float spreadScale);

		bool isUpping() const;
		bool canFire() const;
		bool canTrigger() const;

		void reup(bool instant = false);
		void update(int dt);

		void setBatterySize(int size, bool recharge = false);

		void setRefireTime(int ms);
		void setRechargeTime(int ms);
		void setFireCharge(int charge);
		void setAmount(int amount);
		void setAmountScale(float amountScale);
		void setSpread(float spread);
		void setSpreadScale(float spreadScale);

		int getBar() const;
		int getBarMAX() const;

	private:
		void fire(b2Vec2 &heading);

		Battery battery_;

		int refireTime_;
		int refireTimeMAX_;

		int rechargeTime_;
		int rechargeTimeMAX_;

		int fireCharge_;

		
		float amountMAX_;
		float amountScale_;

		float spread_;
		float spreadScale_;

		bool cooled_;
	};
}

#endif