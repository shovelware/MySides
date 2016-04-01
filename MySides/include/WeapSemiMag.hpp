#ifndef MS_WSEMIMAG_HPP
#define MS_WSEMIMAG_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class SemiMag : public WeaponI {
	public:
		SemiMag(fireFunc& callback, ProjectileDef const &ammo);
		SemiMag(fireFunc& callback, ProjectileDef const &ammo, int magSize, int resetTime, int reloadTime);

		bool canFire() const;

		void reup();
		void update(int dt);

		void setMagSize(int size, bool reload = false);

		void setResetTime(int ms);
		void setReloadTime(int ms);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int resetTime_;
		int resetTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;

		bool cocked_;
	};
}

#endif