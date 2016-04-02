#ifndef MS_WSEMIMAG_HPP
#define MS_WSEMIMAG_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class SemiMag : public WeaponI {
	public:
		SemiMag(fireFunc& callback, ProjectileDef const &ammo, std::string id);
		SemiMag(fireFunc& callback, ProjectileDef const &ammo, std::string id, int magSize, int resetTime, int reloadTime);

		bool isUpping() const;
		bool canFire() const;
		bool canTrigger() const;

		void reup(bool instant = false);
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