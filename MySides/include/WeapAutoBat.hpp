#ifndef MS_WAUTOBAT_HPP
#define MS_WAUTOBAT_HPP

#include "Weapon.hpp"
#include "Battery.hpp"

namespace Weapon {
	class AutoBat : public WeaponI {
	public:
		AutoBat(fireFunc& callback, ProjectileDef const &ammo);
		AutoBat(fireFunc& callback, ProjectileDef const & ammo, int batterySize, int refireTime, int rechargeTime, int fireCharge);

		bool canFire() const;

		void reup();
		void update(int dt);

		void setBatterySize(int size, bool recharge = false);

		void setRefireTime(int ms);
		void setRechargeTime(int ms);
		void setFireCharge(int charge);

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
	};
}
#endif