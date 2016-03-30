#ifndef MS_WCOIL_HPP
#define MS_WCOIL_HPP

#include "Weapon.hpp"
#include "Battery.hpp"

namespace Weapon {
	class Coilgun : public WeaponI {
	public:
		Coilgun(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);
	
		bool canFire();

		void update(int dt);

		void setRefireTime(int ms);
		void setRechargeTime(int ms);
		void setFireCharge(int charge);
		void setBatterySize(int size, bool recharge = false);

		float getBar() const;
		float getBarMAX() const;
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