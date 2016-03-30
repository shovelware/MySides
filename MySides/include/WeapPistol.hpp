#ifndef MS_WPISTOL_HPP
#define MS_WPISTOL_HPP

#include "Weapon.hpp"
#include "Magazine.hpp"

namespace Weapon {
	class Pistol : public WeaponI {
	public:
		Pistol(std::function<void(std::vector<ProjectileDef>& defs, std::string id)>& callback, ProjectileDef const &ammo);
		
		bool canFire() const;

		void reup();
		void update(int dt);

		void setResetTime(int ms);
		void setReloadTime(int ms);
		void setMagSize(int size, bool reload = false);

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