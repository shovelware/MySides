#ifndef MS_WSHOTGUN_HPP
#define MS_WSHOTGUN_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class SpreadMag : public WeaponI {
	public:
		SpreadMag(fireFunc& callback, ProjectileDef const &ammo);
		SpreadMag(fireFunc& callback, ProjectileDef const &ammo, int magSize, int resetTime, int reloadTime, int pellets, float spread);
		bool canFire() const;

		void reup();
		void update(int dt);

		void setMagSize(int size, bool reload = false);

		void setResetTime(int ms);
		void setReloadTime(int ms);
		void setPellets(int pellets);
		void setSpread(float spread);

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

		int pellets_;
		float spread_;
	};
}
#endif