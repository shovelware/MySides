//Rifle
//Concrete weapon, fires a set number of rounds and reloads

#ifndef MS_WAUTOMAG_HPP
#define MS_WAUTOMAG_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class AutoMag : public WeaponI {
	public:
		AutoMag(fireFunc& callback, ProjectileDef const &ammo);
		AutoMag(fireFunc& callback, ProjectileDef const &ammo, int magSize, int refireTime, int reloadTime);
		
		bool canFire() const;

		void reup();
		void update(int dt);

		void setMagSize(int size, bool reload = false);

		void setRefireTime(int ms);
		void setReloadTime(int ms);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int refireTime_;
		int refireTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;
	};
}
#endif