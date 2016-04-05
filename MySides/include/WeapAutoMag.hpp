//Rifle
//Concrete weapon, fires a set number of rounds and reloads

#ifndef MS_WAUTOMAG_HPP
#define MS_WAUTOMAG_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"
namespace Weapon {
	class AutoMag : public WeaponI {
	public:
		AutoMag(FireFunc& callback, ProjectileDef const &ammo, std::string id);
		AutoMag(FireFunc& callback, ProjectileDef const &ammo, std::string id, int magSize, int refireTime, int reloadTime, float spread);

		bool isUpping() const;
		bool canFire() const;
		bool canTrigger() const;

		void reup(bool instant = false);
		void update(int dt);

		void setMagSize(int size, bool reload = false);

		void setRefireTime(int ms);
		void setReloadTime(int ms);
		void setSpread(float spread);

		int getBar() const;
		int getBarMAX() const;
	private:
		void fire(b2Vec2 &heading);

		Magazine magazine_;

		int refireTime_;
		int refireTimeMAX_;

		int reloadTime_;
		int reloadTimeMAX_;

		float spread_;
	};
}
#endif