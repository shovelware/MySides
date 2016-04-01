#ifndef MS_WSEMIBAT_HPP
#define MS_WSEMIBAT_HPP

#include "Weapon.hpp"
#include "Battery.hpp"

namespace Weapon {
	class SemiBat : public WeaponI {
	public:
		SemiBat(fireFunc& callback, ProjectileDef const& ammo);
		SemiBat(fireFunc& callback, ProjectileDef const& ammo, int batterySize, int chargeAmount, int dischargeAmount);

		bool canTrigger() const;

		void reup();
		void update(int dt);

		void setBatterySize(int size, bool recharge = false);

		void setChargeAmount(int amount);
		void setdisChargeAmount(int amount);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Battery battery_;

		int chargeAmount_;
		int dischargeAmount_;

		bool primed_;
	};
}
#endif